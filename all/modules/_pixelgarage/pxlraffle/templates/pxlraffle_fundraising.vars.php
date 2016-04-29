<?php
/**
 * The preprocess function for the current raffle state theme.
 */

function template_preprocess_pxlraffle_fundraising(&$vars) {
  $raffle_nid = pxlraffle_get_current_raffle_nid();
  $current_raffle = node_load($raffle_nid);
  $raffle_id = strtoupper($current_raffle->field_raffle_id[LANGUAGE_NONE][0]['value']);

  //
  // get current raffle numbers to be displayed
  global $base_url, $language;

  $vars['title'] = t('Basic Income Pot');
  $vars['raffle_count'] = pxlraffle_get_raffle_count();
  $vars['total_amount'] = number_format(PXLRAFFLE_MAX_AMOUNT, 2, ".", "'");
  $current_amount = empty($current_raffle->field_raffle_donation) ? 0 : $current_raffle->field_raffle_donation[LANGUAGE_NONE][0]['value'];
  $vars['current_amount'] = number_format($current_amount, 2, ".", "'");;
  $fill_level = min($current_amount / PXLRAFFLE_MAX_AMOUNT * 100, 100);
  $vars['fill_level'] = $fill_level;
  $vars['current_fees'] = empty($current_raffle->field_club_fees) ? 0 : $current_raffle->field_club_fees[LANGUAGE_NONE][0]['value'];
  $vars['explanation'] = t("As soon as 30'000 CHF are collected, a raffle takes place.");
  $vars['label_participate'] = t('Participate');
  switch($language->language) {
    case 'en':
      $vars['url_participate'] = $base_url . '/en/participate';
      break;
    case 'de':
      $vars['url_participate'] = $base_url . '/de/participate';
      break;
    case 'fr':
      $vars['url_participate'] = $base_url . '/fr/participate';
      break;
    case 'it':
      $vars['url_participate'] = $base_url . '/it/participate';
      break;
    default:
      $vars['url_participate'] = $base_url . '/participate';
      break;
  }
  if ($fill_level < 75) {
    $trans_x = -50;
  }
  else if ($fill_level < 80) {
    $trans_x = -60;
  }
  else if ($fill_level < 85) {
    $trans_x = -70;
  }
  else if ($fill_level < 90) {
    $trans_x = -80;
  }
  else if ($fill_level < 95) {
    $trans_x = -90;
  }
  else {
    $trans_x = -100;
  }
  $vars['translate_x'] = sprintf('-webkit-transform: translateX(%d%%);-ms-transform: translateX(%d%%);transform: translateX(%d%%);', $trans_x,$trans_x,$trans_x);

  //
  // style odometer for user count
  _pxlraffle_odometer_theme_attachments($vars, 'pxlraffle_fundraising');
  $vars['user_count'] = pxlraffle_get_user_count_in_raffle($raffle_nid);
  $vars['odometer_label'] = t('participants are currently part of raffle "@number"', array('@number' => $raffle_id));
}
