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
  $raffle_date = $raffle->field_raffle_date[LANGUAGE_NONE][0]['value'];
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
 * Updates the current raffle with donation and fees.
 *
 * @return int      Returns the rest amount, if a new raffle has to be created.
 */
function pxlraffle_update_current_raffle_donation($amount, $stripe_fee, $app_fee) {
  //
  // open transaction to prevent doubled raffle codes
  $transaction = db_transaction(); // commit when $transaction goes out of scope

  //
  // get current raffle
  $current_raffle_nid = pxlraffle_get_current_raffle_nid();
  $raffle = node_load($current_raffle_nid);

  //
  // return amount higher than 30000 CHF
  $rest_amount = false;
  $new_amount = ($raffle->field_raffle_donation[LANGUAGE_NONE][0]['value'] + $amount - $stripe_fee - $app_fee)* 1.0; // casting to float

  // check if new raffle has to be opened
  if ($new_amount > PXLRAFFLE_MAX_AMOUNT) {
    $raffle->field_raffle_donation[LANGUAGE_NONE][0]['value'] = PXLRAFFLE_MAX_AMOUNT;
    $raffle->field_raffle_state[LANGUAGE_NONE][0]['value'] = PXLRAFFLE_STATE_CLOSED;
    $rest_amount = $new_amount - PXLRAFFLE_MAX_AMOUNT;
  }
  else {
    $raffle->field_raffle_donation[LANGUAGE_NONE][0]['value'] = $new_amount;
  }

  //
  // add application fees
  $raffle->field_club_fees[LANGUAGE_NONE][0]['value'] = $raffle->field_club_fees[LANGUAGE_NONE][0]['value'] + $app_fee;

  // save raffle
  node_save($raffle);

  // add donation to pxl_donations table
  $fields = array(
    'booked' => 0,
    'type' => 'one-time',
    'amount' => $amount,
    'stripe_fee' => $stripe_fee,
    'app_fee' => $app_fee,
    'timestamp' => time(),
  );
  db_insert('pxl_donations')
    ->fields($fields)
    ->execute();

  // return rest amount
  return $rest_amount;
}

