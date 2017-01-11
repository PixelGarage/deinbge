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

  $vars['title'] = t('Subscribe for all raffles');
  $vars['subscription_slogan'] = t("Become a subscribed user and automatically take part in all future raffles. A subscription can be cancelled at any time.");
  $vars['subscription_link_label'] = t('Subscribe now!');
  switch($language->language) {
    case 'en':
      $vars['subscription_link_url'] = $base_url . '/en/support';
      break;
    case 'de':
      $vars['subscription_link_url'] = $base_url . '/de/support';
      break;
    case 'fr':
      $vars['subscription_link_url'] = $base_url . '/fr/support';
      break;
    case 'it':
      $vars['subscription_link_url'] = $base_url . '/it/support';
      break;
    default:
      $vars['subscription_link_url'] = $base_url . '/support';
      break;
  }
  $vars['subscribtion_fee_statement'] = t('Subscribed users assign @amount CHF of this amount to our association. Thank you very much!',
      array('@amount' => $current_fees));

  //
  // style odometer for user count and amount
  _pxlraffle_odometer_theme_attachments($vars, 'pxlraffle_subscriptions');
  $subscribed_users = pxlraffle_get_user_count_in_raffle($raffle_nid, false);
  $vars['odometer_label'] = t('@num subscribed user collect each month', array('@num' => $subscribed_users));
  $vars['subscribed_amount'] = empty($recurring_raffle->field_raffle_donation) ? '0.00' : $recurring_raffle->field_raffle_donation[LANGUAGE_NONE][0]['value'];
}
