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



/* --------------------------------------------------
 * Raffle public functions
 * --------------------------------------------------*/
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
 * The returned array contains all found node id's sorted by creation time in ascending order (oldest first).
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
 * Gets the recurring raffle node id.
 *
 * There is only one recurring raffle (state = 'recurring'), used to hold all users with subscriptions.
 *
 * @return bool|mixed
 */
function pxlraffle_get_recurring_raffle_nid() {
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
  $default_msg = t('@msg', array('@msg' => $undefined));

  switch ($state) {
    case 'collecting':
    default:
      return $default_msg;
    case 'closed':
      if ($raffle_time > time()) {
        return date('j. F Y \u\m H:i', $raffle_time);
      }
      else {
        return $default_msg;
      }
      break;
  }
}


