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
  $vars['title'] = t('Basic Income Pot');
  $vars['user_count'] = pxlraffle_get_user_count_in_raffle($raffle_nid);
  $vars['raffle_count'] = pxlraffle_get_raffle_count();
  $vars['summary'] = t('@users participants are currently part of raffle number @number', array('@users' => $vars['user_count'], '@number' => $number));
  $vars['total_amount'] = $total_amount;
  $current_amount = empty($current_raffle->field_raffle_donation) ? 0 : $current_raffle->field_raffle_donation[LANGUAGE_NONE][0]['value'];
  $vars['current_amount'] = $current_amount;
  $vars['fill_level'] = $current_amount / $total_amount * 100;
  $vars['current_fees'] = empty($current_raffle->field_club_fees) ? 0 : $current_raffle->field_club_fees[LANGUAGE_NONE][0]['value'];

}
