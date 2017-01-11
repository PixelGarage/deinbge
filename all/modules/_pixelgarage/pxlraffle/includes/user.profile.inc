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

  // add a checkbox with link to participation conditions
  $form['condition_checkbox'] = array(
    '#type' => 'checkbox',
    '#title' => t('I confirm that I have read the <a href="/node/126" target="_blank">Participation Conditions</a>'),
    '#return_value' => 1,
    '#default_value' => 0,
    '#required' => true,
    '#weight' => 3
  );
}

/**
 * Make sure that stripe_button_form_user_profile_form_alter (module stripe_button)
 * is called before the pxlraffle_form_user_profile_form_alter (this module).
 */
function pxlraffle_module_implements_alter(&$implementations, $hook){
  if ($hook == 'form_alter' && isset($implementations['pxlraffle'])) {
    // move pxlraffle module to the end of list
    $group = $implementations['pxlraffle'];
    unset($implementations['pxlraffle']);
    $implementations['pxlraffle'] = $group;
  }
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
  $user_has_subscription = false;
  if (!empty($user->field_current_raffle)) {
    //
    // add raffle info
    $raffle_id = $user->field_current_raffle[LANGUAGE_NONE][0]['target_id'];
    $raffle = node_load($raffle_id);
    $raffle_id = strtoupper($raffle->field_raffle_id[LANGUAGE_NONE][0]['value']);
    $raffle_date = pxlraffle_get_raffle_date($raffle);
    $user_has_subscription = $raffle_id == 'SUBSCR';
    $form['field_raffle_code']['#prefix'] = $user_has_subscription ?
      '<p>' . t('You are participating in all raffles.') . '</p>' :
      '<p>' . t('You are participating in raffle "@number". The raffle date is @date.', array('@number' => $raffle_id, '@date' => $raffle_date)) . '</p>';
  }
  else {
    $form['field_raffle_code']['#prefix'] = '<p>' . t('You are not part of a raffle.') . '</p>';
  }

  if ($user_has_subscription) {
    $subscr_table = $form['subscription'];
    unset($form['subscription']);
    $form['field_raffle_code']['#suffix'] = '<div class="subscription-container">' . drupal_render($subscr_table) . '</div>';
  }
  else {
    //
    // add action buttons
    $uid = $user->uid;
    $renew_url = 'user/' . $uid . '/raffle/renew-in-profile';
    $remove_url = 'user/' . $uid . '/raffle/remove-in-profile';
    $form['field_raffle_code']['#suffix'] = '<div class="raffle-action-container">' .
      l(t('Participate in raffle'), $renew_url, array('attributes' => array('class' => array('btn btn-default refresh-raffle-button'), 'title' => t('Renew raffle')))) .
      l(t('Remove from raffle'), $remove_url, array('attributes' => array('class' => array('btn btn-default remove-raffle-button'), 'title' => t('Remove from raffle')))) .
      '</div>';
  }
}

