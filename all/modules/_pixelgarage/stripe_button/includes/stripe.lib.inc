<?php
/**
 * Created by PhpStorm.
 * User: ralph
 * Date: 03.01.17
 * Time: 18:24
 */

/**
 * Returns true, if Stripe library is loaded and the API key is set. False otherwise.
 */
function stripe_button_library_loaded () {
  $library = libraries_load('stripe');
  if ($library['loaded']) {
    //
    // set the private API key of specific account
    \Stripe\Stripe::setApiKey(_stripe_api_secret_key());
    return true;
  }

  return false;
}

/**
 * Charges the credit card (token) with the given amount in the given currency
 * and sends an confirmation email when successful.
 *
 * @param $amount
 * @param $currency
 * @param $token
 * @param $email
 * @param string $desc
 */
function stripe_button_charge_card_once($amount, $currency, $token, $email, $desc = '') {
  //
  // create the Stripe charge
  $charge = \Stripe\Charge::create(array(
    "amount" => $amount,
    "currency" => $currency,
    "source" => $token,
    "description" => $desc,
    "receipt_email" => $email
  ));

}


/**
 * Return the registered user object, if available. Otherwise an Exception is thrown.
 *
 * @param $email  string
 *    The email address entered into Stripe Checkout dialog.
 * @return mixed|null
 *
 * @throws \Exception
 */
function stripe_button_registered_user($email) {
  $registered_user = null;
  if (user_is_anonymous()) {
    $registered_user = user_load_by_mail($email);
    if (!$registered_user) {
      // no recurring billing possible for anonymous user
      throw new Exception('No registration found. Only registered user can subscribe to recurring payments.');
    }
  }
  else {
    $registered_user = user_load($GLOBALS['user']->uid);
  }
  return $registered_user;
}

/**
 * Returns the ID of the Stripe subscription plan fitting the given parameter or of the newly created one.
 *
 * @param $amount
 * @param $currency
 * @param $recurring_billing
 * @return string   Stripe plan id.
 */
function stripe_button_subscription_plan($amount, $currency, $recurring_billing) {
  //
  // get subscription plan via plan-id, if available
  try {
    $plan_id = 'bge_' . $recurring_billing . '_' . $amount . '_' . $currency;
    $subscription_plan = \Stripe\Plan::retrieve($plan_id);
    return $subscription_plan->id;
  }
  catch (Exception $e) {
    // no plan with given id found, do nothing here
  }

  //
  // create a new subscription plan
  switch ($recurring_billing) {
    case 'daily':
      $interval = 'day';
      break;
    case 'weekly':
      $interval = 'week';
      break;
    case 'monthly':
    default:
      $interval = 'month';
      break;
    case 'yearly':
      $interval = 'year';
      break;
  }
  switch ($amount) {
    case 300:
      $plan_name = t('Basic Income Silver');
      break;
    case 600:
      $plan_name = t('Basic Income Gold');
      break;
    case 1000:
      $plan_name = t('Basic Income Platinum');
      break;
    default:
      $plan_name = t('Basic Income Personal-@amount', array('@amount' => $amount));
      break;
  }

  $plan = \Stripe\Plan::create(array(
    "id" => $plan_id,
    "amount" => $amount,
    "currency" => $currency,
    "interval" => $interval,
    "name" => $plan_name,
  ));

  return $plan->id;
}

/**
 * Subscribes the registered user to the Stripe subscription plan. Returns the created Stripe customer id.
 *
 * REMARK: If the registered user is already subscribed to a plan, the old plan
 * will be deleted before the user is subscribed to the new plan.
 *
 * @param $registered_user  object
 *    The registered user object.
 * @param $plan_id   object
 *    The Stripe subscription plan id
 * @return string
 *    Returns the created Stripe customer id.
 */
function stripe_button_subscribe_user_to_plan($registered_user, $plan_id, $email, $source) {
  //
  // check, if user is already subscribed to a plan and delete it, if any
  stripe_button_delete_subscription_for_user($registered_user);

  //
  // creating a Stripe customer with a plan and a source (credit card) performs the following:
  // - Creates a customer (unique)
  // - Saves the customer’s payment information
  // - Signs the customer up for a subscription
  // - By default, Stripe bills the customer immediately for their first week of the subscription
  $customer = \Stripe\Customer::create(array(
    'email' => $email,
    'source'  => $source,
    'plan' => $plan_id
  ));

  //
  // new subscription successful, store customer id and registered user id for later access
  // e.g. user wants to cancel subscription
  db_user_stripe_subscription_add($registered_user->uid, $customer->id, $plan_id);

  // return subscription (created customer id)
  return $customer->id;
}

/**
 * Deletes the Stripe subscription for the given registered user.
 *
 * @param $registered_user  object
 *    The registered user object.
 */
function stripe_button_delete_subscription_for_user($registered_user) {
  $stripe_subscriptions = db_user_stripe_subscription_get_subscriptions($registered_user->uid);
  foreach ($stripe_subscriptions as $stripe_subscription) {
    $stripe_cust_id = $stripe_subscription['stripe_cust_id'];
    $customer = \Stripe\Customer::retrieve($stripe_cust_id);
    if (!$customer->deleted) {
      $customer->delete();
    }
    db_user_stripe_subscription_delete($stripe_cust_id);
  }
}


/* --------------------------------------------------
 * DB UserID - Stripe subscription
 * --------------------------------------------------*/
/**
 * Adds a new record to the {pxl_user_stripe_subscription} db.
 *
 * @param $uid      int       User id.
 * @param $cust_id  string    Stripe customer id (subscription).
 * @param $plan_id string
 * @param $app_fee_percentage float   The chosen fee percentage for the subscription. Default = 0.0.
 */
function db_user_stripe_subscription_add($uid, $cust_id, $plan_id, $app_fee_percentage = 0.0) {
  $fields = array(
    'stripe_cust_id' => $cust_id,
    'user_id' => $uid,
    'stripe_plan_id' => $plan_id,
    'app_fee_percentage' => $app_fee_percentage,
  );
  db_insert('pxl_user_stripe_subscription')
    ->fields($fields)
    ->execute();

}

/**
 * Updates the subscription (Stripe customer) with the selected application fee percentage in the {pxl_user_stripe_subscription} db.
 *
 * @param $cust_id  string    Stripe customer id (subscription).
 * @param $app_fee_percentage float   The chosen fee percentage for the subscription. Default = 0.0.
 */
function db_user_stripe_subscription_update($cust_id, $app_fee_percentage = 0.0) {
  $fields = array(
    'app_fee_percentage' => $app_fee_percentage,
  );
  db_update('pxl_user_stripe_subscription')
    ->fields($fields)
    ->condition('stripe_cust_id', $cust_id)
    ->execute();

}

/**
 * Gets all Stripe subscriptions for the given user id.
 *
 * @param $uid      int
 *    User id.
 * @return mixed    array
 *    Array of Stripe subscriptions {'stripe_cust_id' => cust_id, 'app_fee_percentage' => app_fee, 'stripe_plan_id' => plan_id} for the given user or empty array.
 */
function db_user_stripe_subscription_get_subscriptions($uid) {
  // get related stripe customer for the given user id
  $results = db_select('pxl_user_stripe_subscription', 'p')
    ->fields('p', array('stripe_cust_id', 'app_fee_percentage', 'stripe_plan_id'))
    ->condition('p.user_id', $uid)
    ->execute()
    ->fetchAll(PDO::FETCH_ASSOC);

  return $results;
}

/**
 * Gets the Stripe subscription of the given stripe customer (only one subscription).
 * @param $cust_id
 * @return mixed
 */
function db_user_stripe_subscription_get_subscription($cust_id) {
  // get related stripe customer for the given user id
  $result = db_select('pxl_user_stripe_subscription', 'p')
    ->fields('p', array('stripe_cust_id', 'app_fee_percentage', 'stripe_plan_id'))
    ->condition('p.stripe_cust_id', $cust_id)
    ->execute()
    ->fetchAssoc();

  return $result;
}

/**
 * Deletes a given Stripe subscription (stripe_cust_id).
 *
 * @param $cust_id  string    Subscription to be deleted, e.g. the Stripe customer id.
 * @return bool   True, if deletion was successful, false otherwise.
 */
function db_user_stripe_subscription_delete($cust_id) {
  // delete stripe customer relation
  $deleted = db_delete('pxl_user_stripe_subscription')
    ->condition('stripe_cust_id', $cust_id)
    ->execute();
  return $deleted == 1;
}



/* --------------------------------------------------
 * User profile - extend profile with subscription information
 * --------------------------------------------------*/
/**
 * Alter the user profile form.
 *
 * Alter the user profile to make the subscription of a user manageable.
 */
function stripe_button_form_user_profile_form_alter (&$form, &$form_state, $form_id) {
  //
  // show subscription information in the user profile
  $user = $form['#user'];
  $stripe_subscriptions = db_user_stripe_subscription_get_subscriptions($user->uid);
  if (!empty($stripe_subscriptions) && stripe_button_library_loaded()) {
    // get subscription info for user
    try {
      $header = array(
        'subscription' => array('data' => t('Subscription')),
        'created' => array('data' => t('Created')),
        'ops' => array('data' => t('Operations')),
      );
      $rows = array();

      foreach ($stripe_subscriptions as $stripe_subscription) {
        $stripe_cust_id = $stripe_subscription['stripe_cust_id'];
        $customer = \Stripe\Customer::retrieve(array(
            "id" => $stripe_cust_id,
            "expand" => array("subscriptions")
          )
        );
        if ($customer->deleted) {
          // cleanup db and continue
          db_user_stripe_subscription_delete($stripe_cust_id);
          continue;
        }

        // create a row per subscription (should be only one)
        foreach($customer->subscriptions->data as $index => $subscription) {
          $plan = $subscription->plan;
          $subscription_text = t('@plan: @amount @curr per @interval', array(
              '@plan' => $plan->name,
              '@amount' => $plan->amount/100,
              '@curr' => strtoupper($plan->currency),
              '@interval' => $plan->interval,
            )
          );

          // create delete link
          $uid = $user->uid;
          $del_subscription = 'stripe/user/' . $uid . '/delete_subscription';
          $del_link = l(t('Delete'), $del_subscription, array(
            'attributes' => array('class' => array('btn btn-default stripe-user-subscription-delete'), 'title' => t('Delete user subscription'))
          ));

          // create row
          $rows[] = array(
            'subscription' => $subscription_text,
            'created' => format_date($plan->created, 'short'),
            'ops' => $del_link,
          );
        }
      }

      // add subscription table
      $form['subscription'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => t('No recurring payment subscription available.'),
      );

    }
    catch (Exception $e) {
      $msg = $e->getMessage();
    }
  }
}

/**
 * Confirmation form for the subscription deletion.
 */
function stripe_button_user_subscription_delete_confirm($form, &$form_state, $user) {
  $confirm = null;
  $form_state['subscribed_user'] = $user;

  $message = t('Are you sure you want to delete the subscription?');
  $caption = '<p>' . t('This action cannot be undone.') . '</p>';
  return confirm_form($form, $message, "user/{$user->uid}/edit", $caption, t('Delete'));
}

/**
 * Submit handler from user_subscription_delete_confirm form.
 */
function stripe_button_user_subscription_delete_confirm_submit($form, &$form_state) {
  $user = $form_state['subscribed_user'];

  // delete subscription
  if (stripe_button_library_loaded ()) {
    stripe_button_delete_subscription_for_user($user);

    // inform other modules about the subscription deletion
    module_invoke_all('stripe_button_user_unsubscribed', $user);
  }

  // redirect to pixel structure main config page
  $form_state['redirect'] = "user/{$user->uid}/edit";
}



/* --------------------------------------------------
 * Stripe event - charge succeeded
 * --------------------------------------------------*/
function stripe_button_stripe_api_webhook_charge_succeeded($data, Stripe\Event $event = NULL) {
  $time = date('d-m-Y H:i:s.u');
  watchdog('stripe_event', 'Webhook charge.succeeded called: ' . $time, array(), WATCHDOG_DEBUG);

  $session_data = &stripe_button_session_data();
  if (isset($session_data['stripe_button_initialized_payment'])) {
    // stripe_button initialized the payment, don't process event
    unset($session_data['stripe_button_initialized_payment']);
    //return;

    $event_string = "Payment initialized on Client:\n\n";
  }
  else {
    $event_string = "Payment initialized on Stripe:\n\n";
  }

  // TODO: check correct event handling
  //
  // Process charge.succeeded event for recurring payments initialized on Stripe
  // create charge params form charge.succeeded event and inform other modules about charge
  if (stripe_button_library_loaded () && $event) {
    // get customer id of charge object
    try {
      $event_string .= $event->__toString();
      $destination = 'public://last_charge_succeeded_event.tmp';
      file_unmanaged_save_data($event_string, $destination, FILE_EXISTS_REPLACE);
      return;

      $charge = $event->data->object;
      if ($charge->object == 'charge' && $charge->customer) {
        //
        // get subscription plan via customer id
        $stripe_cust_id = $charge->customer;
        $subscription = db_user_stripe_subscription_get_subscription($stripe_cust_id);
        $plan_id = $subscription['stripe_plan_id'];
        $subscription_plan = \Stripe\Plan::retrieve($plan_id);

        // define charge parameters and invoke charge completed hook
        $amount = $subscription_plan->amount;
        $currency = $subscription_plan->currency;
        $interval = $subscription_plan->interval;
        $app_fee_percentage = $subscription['app_fee_percentage'];
        $app_fee = $amount * $app_fee_percentage;
        $stripe_fee = round($amount * 0.029) + 30;

        // get recurring billing type
        switch ($interval) {
          case 'day':
            $recurring_billing = 'daily';
            break;
          case 'week':
            $recurring_billing = 'weekly';
            break;
          case 'month':
          default:
          $recurring_billing = 'monthly';
            break;
          case 'year':
            $recurring_billing = 'yearly';
            break;
        }
        $params = array (
          'stripe_api_mode' => strtolower(variable_get('stripe_api_mode')),
          'recurring_billing' => $recurring_billing,
          'currency' => $currency,
          'amount' => ($amount / 100),
          'stripe_fee' => ($stripe_fee / 100),
          'app_fee' => ($app_fee / 100),
        );
        module_invoke_all('stripe_button_charge_completed', $params);

      }
    }
    catch (Exception $e) {
      watchdog('stripe_button', 'Webhook charge.succeeded event error: Charge not completed: @data', array(
        '@data' => $data,
      ), WATCHDOG_ERROR);
    }
  }
  else {
    watchdog('stripe_button', 'Webhook charge.succeeded event not delivered: Charge not completed: @data', array(
      '@data' => $data,
    ), WATCHDOG_ERROR);

  }
}
