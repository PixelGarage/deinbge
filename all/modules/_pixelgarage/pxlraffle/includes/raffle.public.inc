<?php
/**
 * Contains all public functions of a raffle object.
 *
 * Created by ralph.
 */

/* --------------------------------------------------
 * Raffle states
 * --------------------------------------------------*/

/**
 * Defines the raffle state 'recurring'.
 */
define('PXLRAFFLE_STATE_RECURRING', 'recurring');

/**
 * Defines the raffle state 'collecting'.
 */
define('PXLRAFFLE_STATE_COLLECTING', 'collecting');

/**
 * Defines the raffle state 'closed'.
 */
define('PXLRAFFLE_STATE_CLOSED', 'closed');

/**
 * Defines the raffle state 'raffled'.
 */
define('PXLRAFFLE_STATE_RAFFLED', 'raffled');

/**
 * Defines the raffle state 'paying'.
 */
define('PXLRAFFLE_STATE_PAYING', 'paying');

/**
 * Defines the raffle state 'paid'.
 */
define('PXLRAFFLE_STATE_PAID', 'paid');

/**
 * Defines the raffle maximal amount.
 */
define('PXLRAFFLE_MAX_AMOUNT', 30000);



/* --------------------------------------------------
 * Raffle public functions
 * --------------------------------------------------*/

function pxlraffle_new_raffle($start_amount = 0.0) {
  // create raffle object
  $raffle = new stdClass();
  $raffle->type = 'pxl_raffle';
  $raffle->language = LANGUAGE_NONE;
  $raffle->uid = 1;
  $raffle->status = 1;
  node_object_prepare($raffle);

  // add required fields
  $raffle_count = pxlraffle_get_raffle_count() + 1;
  $raffle->title = 'Raffle ' . $raffle_count;
  $raffle->field_raffle_id[LANGUAGE_NONE][0]['value'] = 'R' . $raffle_count;
  $raffle->field_raffle_state[LANGUAGE_NONE][0]['value'] = PXLRAFFLE_STATE_COLLECTING;
  $raffle->field_next_code[LANGUAGE_NONE][0]['value'] = 1;
  $raffle->field_raffle_donation[LANGUAGE_NONE][0]['value'] = $start_amount;
  $raffle->field_club_fees[LANGUAGE_NONE][0]['value'] = 0.0;

    // save node
  $raffle = node_submit($raffle);
  node_save($raffle);
}

/**
 * Returns the count of all raffles (excluding the recurring raffle).
 */
function pxlraffle_get_raffle_count () {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'pxl_raffle')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_raffle_state', 'value', PXLRAFFLE_STATE_RECURRING, '<>')
    ->count();
  $count = $query->execute();

  return $count;
}


/**
 * Gets the raffle nodes with the given state.
 *
 * The returned array contains all found raffle nid's sorted by creation time in ascending order (oldest first).
 * If no raffle with the given state is found, false is returned.
 *
 * @return bool|mixed
 */
function pxlraffle_get_raffles_with_state($state) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'pxl_raffle')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->fieldCondition('field_raffle_state', 'value', $state)
    ->propertyOrderBy('created', 'ASC');
  $result = $query->execute();
  if (isset($result['node'])) {
    return array_keys($result['node']);
  }

  // no raffle found
  return false;
}

/**
 * Gets the current raffle node.
 *
 * There is only one active raffle, the one that is in state 'collecting'.
 *
 * @return bool|mixed
 */
function pxlraffle_get_current_raffle_nid() {
  // get the current raffle
  $nids = pxlraffle_get_raffles_with_state(PXLRAFFLE_STATE_COLLECTING);
  if ($nids) {
    return $nids[0];
  }
  return false;
}

/**
 * Gets the subscription raffle node id.
 *
 * There is only one subscription raffle (state = 'recurring'), used to hold all users with subscriptions.
 *
 * @return bool|mixed
 */
function pxlraffle_get_subscription_raffle_nid() {
  // get the recurring raffle
  $nids = pxlraffle_get_raffles_with_state(PXLRAFFLE_STATE_RECURRING);
  if ($nids) {
    return $nids[0];
  }
  return false;
}

/**
 * Returns the date of the given raffle, if any or a definable message for all others.
 */
function pxlraffle_get_raffle_date ($raffle, $undefined = 'coming soon') {
  $state = $raffle->field_raffle_state[LANGUAGE_NONE][0]['value'];
  $raffle_date = !empty($raffle->field_raffle_date) ? $raffle->field_raffle_date[LANGUAGE_NONE][0]['value'] : 'now';
  $raffle_time = strtotime($raffle_date);
  $default_msg = t($undefined);

  // set locale
  global $language;

  switch ($language->language) {
    case 'de':
      setlocale(LC_ALL, 'de_CH');
      break;
    case 'fr':
      setlocale(LC_ALL, 'fr_FR');
      break;
    case 'it':
      setlocale(LC_ALL, 'it_IT');
      break;
    case 'en':
      setlocale(LC_ALL, 'en_US');
      break;
  }

  switch ($state) {
    case 'collecting':
    default:
      return $default_msg;
    case 'closed':
      if ($raffle_time > time()) {
        return strftime('%e. %B %Y', $raffle_time); // date('j. F Y', $raffle_time);
      }
      else {
        return $default_msg;
      }
      break;
  }
}

/**
 * Updates the current raffle amount with the given donation (amount excluding fees).
 * If the updated raffle amount exceeds the maximum amount, the current raffle is closed.
 *
 * The remaining amount of the closed raffle is returned.
 *
 * LIVE MODE: The given donation is permanently stored in the database.
 * TEST MODE: Neither the raffle object nor the donation table are updated.
 *            Only a watchdog message is created.
 *
 * @param $donation_params  array
 *    Associative array containing the values of the donation:
 *    - stripe_api_mode: "test" | "live"
 *    - recurring_billing: "one-time" | "daily" | "weekly" | "monthly" | "yearly"
 *    - currency: Currency of donation
 *    - amount: Amount of the donation
 *    - stripe_fee: transaction fee of Stripe (2.9% + 30)
 *    - app_fee: application fee (user defined percentage for the association)
 * @return int Returns the rest amount, if a new raffle has to be created, otherwise false.
 */
function pxlraffle_update_current_raffle_amount($donation_params) {
  //
  // open transaction
  $transaction = db_transaction(); // commit when $transaction goes out of scope

  //
  // get current raffle
  $current_raffle_nid = pxlraffle_get_current_raffle_nid();
  $raffle = node_load($current_raffle_nid);

  //
  // calculate new raffle donation amount
  $remaining_amount = false;
  $stripe_api_mode = $donation_params['stripe_api_mode'];
  $recurring_billing = $donation_params['recurring_billing'];
  $currency = $donation_params['currency'];
  $amount = $donation_params['amount'];
  $stripe_fee = $donation_params['stripe_fee'];
  $app_fee = $donation_params['app_fee'];
  $cust_id = $donation_params['stripe_cust_id'];

  $new_amount = ($raffle->field_raffle_donation[LANGUAGE_NONE][0]['value'] + $amount - $stripe_fee - $app_fee)* 1.0; // casting to float

  //
  // check if in test mode or the new amount exceeds the maximal raffle amount
  if ($stripe_api_mode === 'test') {
    watchdog(
      'PxlRaffle test',
      '@cust charged with @amount @curr (@recurring) including stripe fee = @stripe_fee @curr and application fee = @app_fee @curr. Old raffle amount: @old_amount @curr; New raffle amount: @new_amount @curr',
      array(
        '@cust' => $cust_id,
        '@curr' => $currency,
        '@amount' => $amount,
        '@recurring' => $recurring_billing,
        '@stripe_fee' => $stripe_fee,
        '@app_fee' => $app_fee,
        '@old_amount' => $raffle->field_raffle_donation[LANGUAGE_NONE][0]['value'],
        '@new_amount' => $new_amount,
      ), WATCHDOG_DEBUG);

    return false;
  }
  else if ($new_amount >= PXLRAFFLE_MAX_AMOUNT) {
    $raffle->field_raffle_donation[LANGUAGE_NONE][0]['value'] = PXLRAFFLE_MAX_AMOUNT;
    $raffle->field_raffle_state[LANGUAGE_NONE][0]['value'] = PXLRAFFLE_STATE_CLOSED;
    $remaining_amount = $new_amount - PXLRAFFLE_MAX_AMOUNT;
  }
  else {
    $raffle->field_raffle_donation[LANGUAGE_NONE][0]['value'] = $new_amount;
  }

  // add application fees and save raffle
  $raffle->field_club_fees[LANGUAGE_NONE][0]['value'] = $raffle->field_club_fees[LANGUAGE_NONE][0]['value'] + $app_fee;
  node_save($raffle);

  //
  // add current donation to pxl_donations table
  $fields = array(
    'booked' => 0,
    'type' => $recurring_billing,
    'currency' => $currency,
    'amount' => $amount,
    'stripe_fee' => $stripe_fee,
    'app_fee' => $app_fee,
    'stripe_cust_id' => $cust_id,
    'timestamp' => time(),
  );
  db_insert('pxl_donations')
    ->fields($fields)
    ->execute();

  // return rest amount
  return $remaining_amount;
}

