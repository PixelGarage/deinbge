<?php
/**
 * Contains all enhancements of the registration process and the user profile.
 */

/* --------------------------------------------------
 * User login form adaptation
 * --------------------------------------------------*/
/**
 * Alters the menu entries to hide the local task tabs.
 * @param $items
 */
function pxlraffle_menu_alter(&$items) {
  //
  // remove the tabs on the login/register form pages
  $items['user/login']['type'] = MENU_CALLBACK;
  $items['user/register']['type'] = MENU_CALLBACK;
  $items['user/password']['type'] = MENU_CALLBACK;
}

/**
 * Make sure that the pxlraffle_form_user_profile_form_alter (this module) is
 * called at the end of all form_alter hooks.
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
 * Alter the user login form.
 * Adapt the login form so that the user login part displays on the left side
 * and the user registration part on the right.
 *
 * @param $form
 * @param $form_state
 * @param $form_id
 */
function pxlraffle_form_user_login_alter(&$form, &$form_state, $form_id) {
  $form['name']['#prefix']  = '<div id="' . $form_id . '_form">';
  $form['name']['#prefix'] .= '<h1>' . t('Login') . '</h1>';
  $form['pass']['#suffix']  = '<div class="form-actions-wrapper">';
  $form['pass']['#suffix'] .= l(t('Forgot your password?'), 'user/password', array('attributes' => array('class' => array('login-password'), 'title' => t('Get a new password'))));
  $form['actions']['#suffix']  = '</div></div>';

  // add the registration form on the right side, if applicable
  if (variable_get('user_register', USER_REGISTER_VISITORS_ADMINISTRATIVE_APPROVAL) != USER_REGISTER_ADMINISTRATORS_ONLY) {
    $account_descr = t("Subscribe here and become part of the Basic Income movement.");
    $registration_descr = t('Once this is confirmed, you can manage your membership in your account at any time.');

    $form['actions']['#suffix'] .= '<div id="user-create-account" class="create-account clearfix">';
    $form['actions']['#suffix'] .= "\n<h2>" . t('I don\'t have an account') . "</h2>";
    $form['actions']['#suffix'] .= "\n<div class='create-account-description'><p>" . $account_descr . "</p>";
    $form['actions']['#suffix'] .= "\n<p>" . t("After successful subscription you will receive a verification email. ") . $registration_descr . "</p></div>";
    $form['actions']['#suffix'] .= "\n<div class='form-actions-wrapper'>" . l(t('Become a member'), 'user/register', array('attributes' => array('class' => array('login-register'), 'title' => t('Create new account'))));
    $form['actions']['#suffix'] .= '</div></div>';
  }
}


/**
 * Alter the user registration form.
 * @param $form
 * @param $form_state
 * @param $form_id
 */
function pxlraffle_form_user_register_form_alter (&$form, &$form_state, $form_id) {
  //
  // hide the raffle fieldset on registration
  unset($form['#groups']['group_raffle_info']);
  unset($form['#fieldgroups']['group_raffle_info']);

  //
  // add a checkbox with link to participation conditions
  $form['condition_checkbox'] = array(
    '#type' => 'checkbox',
    '#title' => t('I confirm that I have read the <a href="/node/126" target="_blank">Participation Conditions</a>'),
    '#return_value' => 1,
    '#default_value' => 0,
    '#required' => false,
    '#weight' => 3
  );

  //
  // create surrounding div for registration form and back link
  $form['account']['name']['#prefix'] = '<div id="' . $form_id . '">';
  $form['account']['name']['#prefix'] .= '<h1>' . t('Register') . '</h1>';
  $form['actions']['submit']['#suffix'] = '<div class="back-to-login clearfix">' . l(t('Back to login'), 'user/login', array('attributes' => array('class' => array('login-account'), 'title' => t('Sign in')))) . '</div>';
  $form['actions']['submit']['#suffix'] .= '</div>';
}

/**
 * Alter the user password form.
 * @param $form
 * @param $form_state
 * @param $form_id
 */
function pxlraffle_form_user_pass_alter (&$form, &$form_state, $form_id) {
  //
  // create surrounding div with back link
  $form['name']['#prefix'] = '<div id="' . $form_id . '_form">';
  $form['name']['#prefix'] .= '<h1>' . t('Request a new password') . '</h1>';
  $form['actions']['submit']['#suffix'] = '<div class="back-to-login clearfix">' . l(t('Back to login'), 'user/login', array('attributes' => array('class' => array('login-account'), 'title' => t('Sign in')))) . '</div>';
  $form['actions']['submit']['#suffix'] .= '</div>';
}


/* --------------------------------------------------
 * User profile adaptation
 * --------------------------------------------------*/
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
  $uid = $user->uid;
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

    $remove_url = 'user/' . $uid . '/raffle/remove-in-profile';
    $action_button = l(t('Remove from raffle'), $remove_url, array(
      'attributes' => array(
        'class' => array('btn btn-default remove-raffle-button'),
        'title' => t('Remove from raffle')
      )
    ));
  }
  else {
    $form['field_raffle_code']['#prefix'] = '<p>' . t('You are not part of a raffle.') . '</p>';
    $renew_url = 'user/' . $uid . '/raffle/renew-in-profile';
    $action_button = l(t('Participate in raffle'), $renew_url, array(
      'attributes' => array(
        'class' => array('btn btn-default refresh-raffle-button'),
        'title' => t('Renew raffle')
      )
    ));
  }

  if ($user_has_subscription) {
    $subscr_table = $form['subscription'];
    unset($form['subscription']);
    $form['account']['#prefix'] = '<div class="subscription-container">' . drupal_render($subscr_table) . '</div>';
  }
  else {
    //
    // add action buttons
    $form['field_raffle_code']['#suffix'] = '<div class="raffle-action-container">' . $action_button . '</div>';
  }
}

