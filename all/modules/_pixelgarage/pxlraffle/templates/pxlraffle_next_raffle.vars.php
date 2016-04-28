<?php
/**
 * The preprocess function for the next raffle theme.
 */

function template_preprocess_pxlraffle_next_raffle(&$vars) {
  //
  // get current and closed raffles
  $raffle_nid = pxlraffle_get_current_raffle_nid();
  $current_raffle = node_load($raffle_nid);
  $next_raffle = $current_raffle;
  $raffle_ids = '';
  $count = 0;
  $closed_raffles = pxlraffle_get_raffles_with_state('closed');

  if ($closed_raffles) {
    // get oldest closed raffle to be raffled next
    $next_raffle_nid = $closed_raffles[0];
    $next_raffle = node_load($next_raffle_nid);

    // get the raffle numbers of all closed raffles
    foreach ($closed_raffles as $closed_raffle_nid) {
      $closed_raffle = node_load($closed_raffle_nid);
      $raffle_ids .= strtoupper($closed_raffle->field_raffle_id[LANGUAGE_NONE][0]['value']) . ', ';
    }
    $raffle_ids = rtrim($raffle_ids, ' ,');
    $count = count($closed_raffles);
  }

  $vars['next_raffle_slogan'] = t("2500 CHF per month for one year.\nUnconditionally.");
  $vars['next_raffle_label'] = t('Next raffle:');
  $vars['next_raffle_date'] = pxlraffle_get_raffle_date($next_raffle);
  $vars['next_raffle_info'] = ($count > 1) ?
    t('The raffles with the IDs "@numbers" are ready to be raffled.', array('@numbers' => $raffle_ids)) :
    ($count == 1) ?
      t('The raffle with ID "@numbers" is ready to be raffled.', array('@numbers' => $raffle_ids)) :
    t('At the moment no raffle is ready to be raffled.');
}
