<?php
/**
 * The preprocess function for the next raffle theme.
 */

function template_preprocess_pxlraffle_subscriptions(&$vars) {
  //
  // get subscription raffle data
  global $base_url, $language;
  $raffle_nid = pxlraffle_get_subscription_raffle_nid();
  $recurring_raffle = node_load($raffle_nid);
  $current_fees = empty($recurring_raffle->field_club_fees) ? 0 : $recurring_raffle->field_club_fees[LANGUAGE_NONE][0]['value'];

  $vars['title'] = t('Still scrolling?');
  $vars['subscription_slogan'] = t("There's nothing to lose. Being part of it is everything!");
  $vars['subscription_link_label'] = t('Ok cool');
  switch($language->language) {
    case 'en':
      $vars['subscription_link_url'] = $base_url . '/en/user/register';
      break;
    case 'de':
    default:
      $vars['subscription_link_url'] = $base_url . '/user/register';
      break;
    case 'fr':
      $vars['subscription_link_url'] = $base_url . '/fr/user/register';
      break;
    case 'it':
      $vars['subscription_link_url'] = $base_url . '/it/user/register';
      break;
  }
  $vars['subscribtion_fee_statement'] = t('Subscribed users support us with @amount CHF each month. Thank you very much!',
      array('@amount' => $current_fees));

  //
  // style odometer for user count and amount
  _pxlraffle_odometer_theme_attachments($vars, 'pxlraffle_subscriptions');
  $subscribed_users = pxlraffle_get_user_count_in_raffle($raffle_nid, false);
  $vars['odometer_label'] = t('@num subscribed users donate each month', array('@num' => $subscribed_users));
  $vars['subscribed_amount'] = empty($recurring_raffle->field_raffle_donation) ? '0.00' : $recurring_raffle->field_raffle_donation[LANGUAGE_NONE][0]['value'];
}
