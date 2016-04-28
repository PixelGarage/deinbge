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
 *
 * @param $raffle_nid
 * @param bool $include_subscriptions
 * @return mixed
 */
function pxlraffle_get_user_count_in_raffle ($raffle_nid, $include_subscriptions = true) {
  $recurring_nid = pxlraffle_get_subscription_raffle_nid();
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'user')
    ->entityCondition('bundle', 'user')
    ->propertyCondition('status', 1);
  if ($include_subscriptions) {
    $query->fieldCondition('field_current_raffle', 'target_id', array($raffle_nid, $recurring_nid), 'IN');
  }
  else {
    $query->fieldCondition('field_current_raffle', 'target_id', $raffle_nid);
  }
  $count = $query->count()->execute();

  return $count;
}

/**
 * Adds the user to the current raffle and assigns a unique raffle code to the user.
 *
 * @param object $user    The user object
 * @param bool $do_save   If true, save the updated user object.
 * @return bool|mixed     The new assigned raffle object.
 * @throws Exception
 */
function pxlraffle_add_user_to_current_raffle(&$user, $do_save = false) {
  //
  // add user to current raffle (Start conditions: until 11. May 2016 to closed raffle)
  $raffle1_time = strtotime("12-may-16");
  if (time() < $raffle1_time) {
    // add users to raffle 1
    $nids = pxlraffle_get_raffles_with_state(PXLRAFFLE_STATE_CLOSED);
    if ($nids) {
      $current_raffle_nid = $nids[0];
    }
    else {
      $current_raffle_nid = pxlraffle_get_current_raffle_nid();
    }
  }
  else {
    // add user to current raffle
    $current_raffle_nid = pxlraffle_get_current_raffle_nid();
  }
  $user->field_current_raffle[LANGUAGE_NONE][0]['target_id'] = $current_raffle_nid;

  //
  // Set new raffle code for given user.
  // To guarantee, that each code is unique, the next available code is saved in the raffle object
  // and updated after usage. Count of raffle attached users is NOT applicable because user can delete their account!
  // Raffle code format: 'raffle_id-raffle_next_code'
  $user_raffle = node_load($current_raffle_nid);
  $raffle_id = strtoupper($user_raffle->field_raffle_id[LANGUAGE_NONE][0]['value']);
  $user_raffle_next_code = $user_raffle->field_next_code[LANGUAGE_NONE][0]['value'];
  $user->field_raffle_code[LANGUAGE_NONE][0]['value'] = $raffle_id . '-' . sprintf('%07d', $user_raffle_next_code);

  // save user if requested
  if ($do_save) {
    user_save($user);
  }

  // update user raffle with next code
  $user_raffle->field_next_code[LANGUAGE_NONE][0]['value'] = $user_raffle_next_code + 1;
  node_save($user_raffle);

  // return new user raffle
  return $user_raffle;
}

/**
 * Subscribes the given user (add it to the subscription raffle) and create a subscription code for the given user.
 *
 * @param object $user    The user object
 * @param bool $do_save   If true, save the updated user object.
 * @return bool|mixed     The new assigned raffle object.
 * @throws Exception
 */
function pxlraffle_subscribe_user(&$user, $do_save = false) {
  //
  // add user to subscription raffle
  $subscr_raffle_nid = pxlraffle_get_subscription_raffle_nid();
  $user->field_current_raffle[LANGUAGE_NONE][0]['target_id'] = $subscr_raffle_nid;

  //
  // Set new raffle code for given user.
  // To guarantee, that each code is unique, the next available code is saved in the raffle object
  // and updated after usage. Count of raffle attached users is NOT applicable because user can delete their account!
  // Raffle code format: 'raffle_id-raffle_next_code'
  $user_raffle = node_load($subscr_raffle_nid);
  $raffle_id = strtoupper($user_raffle->field_raffle_id[LANGUAGE_NONE][0]['value']);
  $user_raffle_next_code = $user_raffle->field_next_code[LANGUAGE_NONE][0]['value'];
  $user->field_raffle_code[LANGUAGE_NONE][0]['value'] = $raffle_id . '-' . sprintf('%07d', $user_raffle_next_code);

  // save user if requested
  if ($do_save) {
    user_save($user);
  }

  // update user raffle with next code
  $user_raffle->field_next_code[LANGUAGE_NONE][0]['value'] = $user_raffle_next_code + 1;
  node_save($user_raffle);

  // return new user raffle
  return $user_raffle;
}


/**
 * Returns an info html text about the user attached raffle.
 *
 * @param $user         The logged in user
 * @return null|string  User specific raffle info text.
 */
function pxlraffle_get_user_raffle_info_html($user) {
  //
  // show user's raffle info
  if (!empty($user->field_current_raffle)) {
    //
    // add raffle info
    $recurring_raffle_nid = pxlraffle_get_subscription_raffle_nid();
    $user_raffle_nid = $user->field_current_raffle[LANGUAGE_NONE][0]['target_id'];
    $user_raffle = node_load($user_raffle_nid);
    $user_raffle_state = $user_raffle->field_raffle_state[LANGUAGE_NONE][0]['value'];
    $user_raffle_code = (!empty($user->field_raffle_code)) ? $user->field_raffle_code[LANGUAGE_NONE][0]['value'] : t('undefined');

    if ($user_raffle_nid == $recurring_raffle_nid) {
      // user has a subscription
      $msg = '<div class="user-raffle-number">' . t('You have a subscription and will automatically participate in all raffles!') . '</div>';
      $msg .= '<div class="user-raffle-code">' . t('Your unique raffle code is: ') . '<span class="raffle-code">' . $user_raffle_code . '</span></div>';
    }
    else if (in_array($user_raffle_state, array(PXLRAFFLE_STATE_COLLECTING, PXLRAFFLE_STATE_CLOSED))) {
      // user is part of current raffle
      $raffle_id = strtoupper($user_raffle->field_raffle_id[LANGUAGE_NONE][0]['value']);
      $raffle_date = pxlraffle_get_raffle_date($user_raffle);
      $msg = '<div class="user-raffle-number">' . t('You are part of raffle "@number".', array('@number' => $raffle_id)) . '</div>';
      $msg .= '<div class="user-raffle-code">' . t('Your unique raffle code is: ') . '<span class="raffle-code">' . $user_raffle_code . '</span></div>';
      $msg .= '<div class="user-raffle-date">' . t('The raffle date is @date.', array('@date' => $raffle_date)) . '</div>';
    }
    else {
      // user is part of an old raffle
      $msg = t('You are part of a past raffle. Please press "Add to current raffle" to renew your participation in the current raffle.');
    }
  }
  else {
    // user is not part of a raffle
    $msg = t('You are currently not part of a raffle. Please press "Add to current raffle" to take part in the current raffle.');
  }
  return '<div class="user-raffle-info">' . $msg . '</div>';
}

/**
 * Links the current raffle to the given user, if the actual user raffle is in state 'raffled' or
 * the user is not part of a raffle yet. If the user raffle has not been raffled (state in {recurring, collecting, closed}),
 * no update is performed.
 *
 * @param $user
 */
function pxlraffle_renew_user_raffle(&$user) {
  if (!empty($user->field_current_raffle)) {
    //
    // check user attached raffle
    $recurring_raffle_nid = pxlraffle_get_subscription_raffle_nid();
    $user_raffle_nid = $user->field_current_raffle[LANGUAGE_NONE][0]['target_id'];
    $user_raffle = node_load($user_raffle_nid);
    $user_raffle_state = $user_raffle->field_raffle_state[LANGUAGE_NONE][0]['value'];

    if ($user_raffle_nid == $recurring_raffle_nid) {
      // user has a subscription
      $msg = t('You do not have to switch to another raffle. You have a subscription and participate automatically in all raffles!');
      drupal_set_message($msg, 'info');
    }
    else if (in_array($user_raffle_state, array(PXLRAFFLE_STATE_COLLECTING, PXLRAFFLE_STATE_CLOSED))) {
      // user is part of active raffle
      $msg = t('You cannot switch to another raffle. You are part of an active raffle!');
      drupal_set_message($msg, 'warning');
    }
    else {
      //
      // the user raffle has been raffled so the user can change to the current raffle
      $user_raffle = pxlraffle_add_user_to_current_raffle($user, true);
      $code = $user->field_raffle_code[LANGUAGE_NONE][0]['value'];

      // send update message
      $msg = t('You have been successfully switched to the new raffle "@raffle". Your new unique raffle code is @code.', array('@raffle' => $user_raffle->title, '@code' => $code));
      drupal_set_message($msg);
    }
  }
  else {
    // user is not part of a raffle, add user to current raffle
    $user_raffle = pxlraffle_add_user_to_current_raffle($user, true);
    $code = $user->field_raffle_code[LANGUAGE_NONE][0]['value'];

    // send update message
    $msg = t('You have been successfully added to raffle "@raffle". Your new unique raffle code is @code.', array('@raffle' => $user_raffle->title, '@code' => $code));
    drupal_set_message($msg);
  }
}
