<?php
/**
 * Created by PhpStorm.
 * User: ralph
 * Date: 06.02.15
 * Time: 17:22
 */
/* =============================================================================
 *      Language block theme
 * ========================================================================== */

function pixelgarage_links__locale_block(&$vars) {
  foreach($vars['links'] as $language => $langInfo) {
    $abbr = $langInfo['language']->language;
    $vars['links'][$language]['title'] = substr($abbr, 0, 1);
  }
  $content = theme_links($vars);
  return $content;
}

/**
 * Themes a Stripe Checkout button with a predefined amount in a given currency.
 */
function pixelgarage_stripe_button_fix_value($variables) {
  // Render Stripe Checkout button
  $field_id = $variables['field_id'];
  if ($variables['amount'] > 0) {
    //
    // Create initial button
    // Enforce strict content security policy
    if ($variables['csp'] == TRUE) {
      header("Content-Security-Policy: default-src 'self' https://checkout.stripe.com;");
    }

    // create HTML button
    $currency = strtoupper($variables['currency']);
    $amount = $variables['amount'] * 100 / 100; // no decimal value if not needed
    $label = t('@amount @currency', array('@amount' => $amount, '@currency' => $currency));

    $box_title = '';
    if ($amount <= 3) {
      $box_title = t('One hour');
    }
    else if ($amount <= 6) {
      $box_title = t('Two hours');
    }
    else if ($amount <= 10) {
      $box_title = t('Three hours');
    }
    else if ($amount <= 50) {
      $box_title = t('Half a day');
    }
    else if ($amount <= 100) {
      $box_title = t('One day');
    }
    else if ($amount <= 300) {
      $box_title = t('Three days');
    }
    else if ($amount <= 1000) {
      $box_title = t('One week');
    }
    else if ($amount <= 2500) {
      $box_title = t('One month');
    }

    // create box text depending on recurring billing
    switch ($variables['recurring_billing']) {
      case 'one-time':
      default:
        $box_text = t('of basic income');
        break;
      case 'daily':
        $box_text = t('of basic income each day');
        break;
      case 'weekly':
        $box_text = t('of basic income each week');
        break;
      case 'monthly':
        $box_text = t('of basic income each month');
        break;
      case 'yearly':
        $box_text = t('of basic income each year');
        break;
    }

    $output = '<div class="stripe-button-container">' .
      '<div class="stripe-button-title">' . $box_title . '</div>' .
      '<div class="stripe-button-text">' . $box_text . '</div>' .
      '<button id="' . $field_id . '" type="button" class="btn btn-normal stripe-button">' .
      $label .
      '</button></div>';
  }
  else if ($variables['amount'] == 0) {
    //
    // successful payment, create success button (disabled)
    $label = t("Yes");
    $output = '<div class="stripe-button-container">' .
      '<div class="stripe-button-title">' . t($variables['box_title']) . '</div>' .
      '<div class="stripe-button-text">' . t($variables['box_text']) . '</div>' .
      '<button id="' . $field_id . '" type="button" class="btn btn-success stripe-button" disabled>' .
      '<span class="fa fa-check"></span>' . $label .
      '</button></div>';
  }
  else {
    //
    // failed payment, create fail button (disabled)
    $label = t("No");
    $msg = t($variables['message']);
    $output = '<div class="stripe-button-container">' .
      '<div class="stripe-button-title">' . t($variables['box_title']) . '</div>' .
      '<div class="stripe-button-text">' . t($variables['box_text']) . '</div>' .
      '<button id="' . $field_id . '" type="button"' .
      ' class="btn btn-warning stripe-button" data-toggle="popover" data-trigger="hover" data-placement="auto bottom"'.
      ' title="Stripe Error" data-content="' . $msg . '">' .
      '<span class="fa fa-times"></span>' . $label .
      '</button></div>';
  }

  return $output;
}

