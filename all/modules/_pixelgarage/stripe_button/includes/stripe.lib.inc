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
    if (!property_exists($registered_user, 'field_stripe_customer_id')) {
      // current user object misses property
      throw new Exception('Missing property on user. User object needs field to store ID of created Stripe customer.');
    }
  }
  return $registered_user;
}

/**
 * Returns the Stripe subscription plan that fits the given parameters.
 *
 * @param $amount
 * @param $currency
 * @param $recurring_billing
 * @return \Stripe\Plan
 */
function stripe_button_subscription_plan($amount, $currency, $recurring_billing) {
  //
  // get subscription plan via plan-id
  $plan_id = 'bge_' . $recurring_billing . '_' . $amount;
  $subscription_plan = \Stripe\Plan::retrieve($plan_id);
  if ($subscription_plan) return $subscription_plan;

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

  return $plan;
}

/**
 * Subscribes the registered user to the Stripe subscription plan.
 *
 * REMARK: If the registered user is already subscribed to a plan, the old plan
 * will be deleted before the user is subscribed to the new plan.
 *
 * @param $registered_user  object
 *    The registered user object.
 * @param $plan   object
 *    The Stripe subscription plan
 */
function stripe_button_subscribe_user_to_plan($registered_user, $plan, $email, $source) {
  //
  // check, if user is already subscribed to a plan and delete it, if any
  if (!empty($registered_user->field_stripe_customer_id)) {
    $stripe_cust_id = $registered_user->field_stripe_customer_id[LANGUAGE_NONE][0]['value'];
    $stripe_customer = \Stripe\Customer::retrieve($stripe_cust_id);
    if (!$stripe_customer->deleted) {
      $stripe_customer->delete();
    }
  }

  //
  // creating a Stripe customer with a plan and a source (credit card) performs the following:
  // - Creates a customer
  // - Saves the customer’s payment information
  // - Signs the customer up for a subscription
  // - By default, Stripe bills the customer immediately for their first week of the subscription
  $customer = \Stripe\Customer::create(array(
    'email' => $email,
    'source'  => $source,
    'plan' => $plan->id
  ));

  //
  // new subscription successful, store customer id in registered user for later access (user wants to cancel subscription)
  $registered_user->field_stripe_customer_id[LANGUAGE_NONE][0]['value'] = $customer->id;
  user_save($registered_user);
}


