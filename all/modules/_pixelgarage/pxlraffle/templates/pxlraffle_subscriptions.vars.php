<?php
/**
 * The preprocess function for the next raffle theme.
 */

function template_preprocess_pxlraffle_subscriptions(&$vars) {
  //
  // get subscription raffle data
  global $base_url;
  $raffle_nid = pxlraffle_get_subscription_raffle_nid();
  $recurring_raffle = node_load($raffle_nid);
  $current_fees = empty($recurring_raffle->field_club_fees) ? 0 : $recurring_raffle->field_club_fees[LANGUAGE_NONE][0]['value'];

  $vars['title'] = t('Subscribe for all raffles');
  $vars['subscription_slogan'] = t("Become a Crowdhörnchen and automatically take part in all future raffles. No missed raffles anymore. <br>With only 5 CHF per month (minimum subscription) you become a Crowdhörnchen. A subscription can be cancelled at any time.");
  $vars['subscription_link_label'] = t('Become a Crowdhörnchen');
  $vars['subscription_link_url'] = $base_url . '/subscription';
  $vars['subscribtion_fee_statement'] = t('Crowdhörnchen decided to spend @amount CHF of this amount to support us in organizing these raffles. Thank you very much!',
      array('@amount' => $current_fees));

  //
  // style odometer for user count and amount
  _pxlraffle_odometer_theme_attachments($vars, 'pxlraffle_subscriptions');
  $vars['subscribed_users'] = 23678;//pxlraffle_get_user_count_in_raffle($raffle_nid);
  $vars['subscribed_users_label'] = t('Crowdhörnchen collect each month');
  $vars['subscribed_amount'] = empty($recurring_raffle->field_raffle_donation) ? 0 : $recurring_raffle->field_raffle_donation[LANGUAGE_NONE][0]['value'];
}
