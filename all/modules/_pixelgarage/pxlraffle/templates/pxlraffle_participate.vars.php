<?php
/**
 * The preprocess function for the next raffle theme.
 */

function template_preprocess_pxlraffle_participate(&$vars) {
  //
  // create participate form
  global $base_url, $user;
  $raffle_nid = pxlraffle_get_current_raffle_nid();
  $current_raffle = node_load($raffle_nid);
  $raffle_id = strtoupper($current_raffle->field_raffle_id[LANGUAGE_NONE][0]['value']);

  //$vars['title'] = t('Participate');

  //
  // user login dependant variables
  if ($user->uid) {
    //
    // slogan for logged in users
    $vars['participation_slogan'] = t("Hi @user, you are a registered user.", array('@user' => $user->name));
    $vars['participation_acc_title'] = t('Hide user info');
    $vars['participation_acc_title_collapsed'] = t('Show user info');
    $vars['class_collapsed'] = '';
    $vars['class_in'] = 'in';

    //
    // add raffle and user information and edit buttons
    $uid = $user->uid;
    $full_user = user_load($uid);
    $recurring_raffle_nid = pxlraffle_get_subscription_raffle_nid();
    $user_raffle_nid = $full_user->field_current_raffle[LANGUAGE_NONE][0]['target_id'];
    $user_is_subscribed = ($recurring_raffle_nid == $user_raffle_nid);
    $markup = pxlraffle_get_user_raffle_info_html($full_user);

    if (!$user_is_subscribed) {
      // show raffle buttons only for user without subscription
      $renew_url = '/user/' . $uid . '/raffle/renew';
      $remove_url = '/user/' . $uid . '/raffle/remove';
      $markup .= '<div class="user-raffle-actions">' .
        l(t('Participate in raffle'), $renew_url, array('attributes' => array('class' => array('btn btn-default refresh-raffle-button'), 'title' => t('Renew raffle')))) .
        l(t('Remove from raffle'), $remove_url, array('attributes' => array('class' => array('btn btn-default remove-raffle-button'), 'title' => t('Remove from raffle')))) .
        '</div>';
    }
    $vars['participation_acc_body'] = array(
      '#markup' => $markup,
      '#prefix' => '<div class="jumbotron">',
      '#suffix' => '</div>'
    );
  }
  else {
    // user not logged in, show login/registration form
    $vars['participation_slogan'] = t("The current raffle is «@number». Register here now. Good luck!", array('@number' => $raffle_id));
    $vars['participation_acc_title'] = t('Participate');
    $vars['participation_acc_title_collapsed'] = t('Hide login');
    $vars['class_collapsed'] = 'collapsed';
    $vars['class_in'] = '';
    $vars['participation_acc_body'] = array(
      'form' => drupal_get_form('user_login'),
    );
  }

  // add js files
  $path = drupal_get_path('module', 'pxlraffle');
  $js_settings = array(
    'pxlraffle_participate' => array(
      'collapsed' => $vars['class_collapsed'] == 'collapsed',
      'hide_label' => empty($vars['class_collapsed']) ? $vars['participation_acc_title'] : $vars['participation_acc_title_collapsed'],
      'show_label' => empty($vars['class_collapsed']) ? $vars['participation_acc_title_collapsed'] : $vars['participation_acc_title'],
    ),
  );

  // add files
  drupal_add_js($path . '/js/participate.js');
  drupal_add_js($js_settings, 'setting');

}
