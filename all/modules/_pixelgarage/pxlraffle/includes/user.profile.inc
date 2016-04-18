<?php
/**
 * Contains all enhancements of a user profile.
 */

/**
 * Alter the user registration form.
 *
 * Don't show the raffle fieldset.
 */
function pxlraffle_form_user_register_form_alter (&$form, &$form_state, $form_id) {
  //
  // hide the raffle fieldset on registration
  unset($form['#groups']['group_raffle_info']);
  unset($form['#fieldgroups']['group_raffle_info']);
}

/**
 * Alter the user profile form.
 *
 * Alter the user profile to prevent editing of raffle specifc data and to allow the user to renew its participation.
 */
function pxlraffle_form_user_profile_form_alter (&$form, &$form_state, $form_id) {
  //
  // disable or hide raffle fields
  $form['field_raffle_code']['#disabled'] = true;
  $form['field_current_raffle']['#access'] = false; // don't show current raffle field

  //
  // add additional form items, if a raffle is assigned to the user
  $user = $form['#user'];
  if (!empty($user->field_current_raffle)) {
    //
    // add raffle info
    $raffle_id = $user->field_current_raffle[LANGUAGE_NONE][0]['target_id'];
    $raffle = node_load($raffle_id);
    $raffle_id = strtoupper($raffle->field_raffle_id[LANGUAGE_NONE][0]['value']);
    $raffle_date = pxlraffle_get_raffle_date($raffle);
    $form['field_raffle_code']['#prefix'] = '<p>' . t('You participate in raffle "@number". The raffle date is @date.', array('@number' => $raffle_id, '@date' => $raffle_date)) . '</p>';
  }
  else {
    $form['field_raffle_code']['#prefix'] = '<p>' . t('You are not part of a raffle.') . '</p>';
  }

  //
  // add renew button
  $uid = $user->uid;
  $renew_url = 'user/' . $uid . '/raffle/renew';
  $remove_url = 'user/' . $uid . '/raffle/remove';
  $form['field_raffle_code']['#suffix'] = '<div class="raffle-action-container">' .
    l(t('Renew raffle'), $renew_url, array('attributes' => array('class' => array('btn btn-default refresh-raffle-button'), 'title' => t('Renew raffle')))) .
    l(t('Remove from raffle'), $remove_url, array('attributes' => array('class' => array('btn btn-default remove-raffle-button'), 'title' => t('Remove from raffle')))) .
    '</div>';
}

/**
 * Implements hook_user_presave().
 *
 * Called before a user is saved or updated. If the current raffle field is empty, the user is in the
 * registration process and the current raffle has to be added.
 */
function pxlraffle_user_presave(&$edit, $account, $category) {
  //
  // user is activated, add current raffle to user before save
  if (!$account->is_new && empty($account->field_current_raffle) && empty($account->field_raffle_code)) {
    pxlraffle_set_user_raffle($account);
  }
}
