<?php
/**
 * The preprocess function for the next raffle theme.
 */

function template_preprocess_pxlraffle_next_raffle(&$vars) {
  //
  // get next raffle data
  $raffle_nid = isset($vars['raffle_nid']) ? $vars['raffle_nid'] : pxlraffle_get_current_raffle_nid();
  $current_raffle = node_load($raffle_nid);
  $next_raffle = $current_raffle;
  $raffle_ids = '';
  $closed_raffles = pxlraffle_get_raffles_with_state('closed');

  if ($closed_raffles) {
    // get next raffle to be raffled
    $next_raffle_nid = $closed_raffles[0];
    $next_raffle = node_load($next_raffle_nid);

    // get the raffle numbers of all closed raffles
    foreach ($closed_raffles as $raffle) {
      $raffle_ids .= strtoupper($raffle->field_raffle_id[LANGUAGE_NONE][0]['value']) . ', ';
    }
    $numbers = rtrim($raffle_ids, ' ,');
  }
  $vars['next_raffle_slogan'] = t("During one year you get 2500 CHF per month.\nUnconditionally.");
  $vars['next_raffle_label'] = t('Next raffle:');
  $vars['next_raffle_date'] = pxlraffle_get_raffle_date($next_raffle);
  $vars['next_raffle_info'] = ($closed_raffles && (count($closed_raffles) > 1)) ?
    t('The raffles with the IDs @numbers are ready to be raffled.', array('@numbers' => $raffle_ids)) :
    t('At the moment no raffle is ready to be raffled.');
}
