<?php
/**
 * The preprocess function for the current raffle state theme.
 */

function template_preprocess_pxlraffle_current_info(&$vars) {
  $raffle_nid = pxlraffle_get_current_raffle_nid();
  $current_raffle = node_load($raffle_nid);
  $number = $current_raffle->field_number[LANGUAGE_NONE][0]['value'];
  $total_amount = 30000;

  //
  // get current raffle numbers to be displayed
  global $base_url;

  $vars['title'] = t('Basic Income Pot');
  $vars['raffle_count'] = pxlraffle_get_raffle_count();
  $vars['total_amount'] = number_format($total_amount, 2, ".", "'");
  $current_amount = empty($current_raffle->field_raffle_donation) ? 0 : $current_raffle->field_raffle_donation[LANGUAGE_NONE][0]['value'];
  $vars['current_amount'] = number_format($current_amount, 2, ".", "'");;
  $vars['fill_level'] = $current_amount / $total_amount * 100;
  $vars['current_fees'] = empty($current_raffle->field_club_fees) ? 0 : $current_raffle->field_club_fees[LANGUAGE_NONE][0]['value'];
  $vars['label_participate'] = t('Participate');
  $vars['url_participate'] = $base_url . '/participate';

  //
  // style odometer for user count
  pxlraffle_odometer_theme_attachments($vars, 'pxlraffle_current_info');
  $vars['user_count'] = 23678;//pxlraffle_get_user_count_in_raffle($raffle_nid);
  $vars['odometer_label'] = t('participants are currently part of raffle number @number', array('@number' => $number));
}
