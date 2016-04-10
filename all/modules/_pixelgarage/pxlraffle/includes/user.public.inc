<?php
/**
 * Contains all public functions of a user object.
 *
 * Created by ralph.
 */

/* --------------------------------------------------
 * User public functions
 * --------------------------------------------------*/
/**
 * Returns the count of participants in the raffle.
 */
function pxlraffle_get_user_count_in_raffle ($raffle_nid) {
  $recurring_nid = pxlraffle_get_recurring_raffle_nid();
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'user')
    ->entityCondition('bundle', 'user')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_current_raffle', 'target_id', array($raffle_nid, $recurring_nid), 'IN')
    ->count();
  $count = $query->execute();

  return $count;
}

/**
 * Links the current raffle to the given user, if the actual user raffle is in state 'raffled' or
 * the user is not part of a raffle yet. If the user raffle has not been raffled (state in {recurring, collecting, closed}),
 * no update is performed.
 *
 * @param $user
 */
function pxlraffle_renew_user_raffle ($user) {
  $user_raffle_nid = (empty($user->field_current_raffle)) ? -1 : $user->field_current_raffle[LANGUAGE_NONE][0]['target_id'];
  $current_raffle_nid = pxlraffle_get_current_raffle_nid();
  $recurring_raffle_nid = pxlraffle_get_recurring_raffle_nid();
  $current_raffle = node_load($current_raffle_nid);

  // check if user is not part of a raffle
  if ($user_raffle_nid == -1) {
    // add user to current raffle
    $user->field_current_raffle[LANGUAGE_NONE][0]['target_id'] = $current_raffle_nid;
    user_save($user);

    $msg = t('You have been successfully added to @raffle.', array('@raffle' => $current_raffle->title));
    drupal_set_message($msg);
    return;
  }
  else if ($user_raffle_nid == $recurring_raffle_nid) {
    // check if user has a subscription
    $msg = t('You do not have to switch to another raffle. You have a subscription and participate automatically in all raffles!');
    drupal_set_message($msg, 'warning');
    return;
  }

  // check user raffle state
  $user_raffle = node_load($user_raffle_nid);
  $user_raffle_state = $user_raffle->field_raffle_state[LANGUAGE_NONE][0]['value'];

  if (in_array($user_raffle_state, array(PXLRAFFLE_STATE_COLLECTING, PXLRAFFLE_STATE_CLOSED))) {
    $msg = t('You cannot switch to another raffle. You are part of an active raffle!');
    drupal_set_message($msg, 'warning');
  }
  else {
    //
    // the raffle is history so the user can change to the current raffle
    $user->field_current_raffle[LANGUAGE_NONE][0]['target_id'] = $current_raffle_nid;
    user_save($user);

    // send update message
    $msg = t('You have successfully renewed your raffle participation. You are now participant of @raffle.', array('@raffle' => $current_raffle->title));
    drupal_set_message($msg);
  }
}
