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

  $vars['title'] = t('Participate in current raffle');
  $vars['participation_slogan'] = t("Do you want to be part of the current raffle '@number'.", array('@number' => $raffle_id));
  $vars['participation_acc_title'] = t('YES, I do');

  //
  // user login dependant variables
  if ($user->uid) {
    //
    // add raffle edit buttons
    $uid = $user->uid;
    $full_user = user_load($uid);
    $renew_url = '/user/' . $uid . '/raffle/renew';
    $remove_url = '/user/' . $uid . '/raffle/remove';
    $markup = pxlraffle_get_user_raffle_info_html($full_user);
    $markup .= '<div class="user-raffle-actions">' .
      l(t('Add to current raffle'), $renew_url, array('attributes' => array('class' => array('btn btn-default refresh-raffle-button'), 'title' => t('Add to current raffle')))) .
      l(t('Remove from raffle'), $remove_url, array('attributes' => array('class' => array('btn btn-default remove-raffle-button'), 'title' => t('Remove from raffle')))) .
      '</div>';
    $vars['participation_acc_body'] = array(
      '#markup' => $markup,
      '#prefix' => '<div class="jumbotron">',
      '#suffix' => '</div>'
    );
  }
  else {
    // user not logged in, show login/registration form
    $vars['participation_acc_body'] = array(
      'form' => drupal_get_form('user_login'),
    );
  }
}
