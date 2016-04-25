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
    if ($amount <= 5) {
      $variables['label_title'] = t('One hour');
    }
    else if ($amount <= 15) {
      $variables['label_title'] = t('Three hour');
    }
    else if ($amount <= 50) {
      $variables['label_title'] = t('Half a day');
    }
    else if ($amount <= 100) {
      $variables['label_title'] = t('One day');
    }
    else if ($amount <= 300) {
      $variables['label_title'] = t('Three days');
    }
    else if ($amount <= 1000) {
      $variables['label_title'] = t('One week');
    }
    else if ($amount <= 2500) {
      $variables['label_title'] = t('One month');
    }
    $text = t($variables['label_text']);

    $output = '<div class="stripe-button-container">' .
      '<div class="stripe-button-title">' . $variables['label_title'] . '</div>' .
      '<div class="stripe-button-text">' . $text . '</div>' .
      '<button id="' . $variables['id'] . '" type="button" class="btn btn-normal stripe-button">' .
      $label .
      '</button></div>';
  }
  else if ($variables['amount'] == 0) {
    //
    // successful payment, create success button (disabled)
    $label = t("Yes");
    $output = '<div class="stripe-button-container">' .
      '<div class="stripe-button-title">' . $variables['label_title'] . '</div>' .
      '<div class="stripe-button-text">' . $variables['label_text'] . '</div>' .
      '<button id="' . $variables['id'] . '" type="button"' .
      ' class="btn btn-success stripe-button" disabled>' .
      '<span class="fa fa-check"></span>' . $label .
      '</button></div>';
  }
  else {
    //
    // failed payment, create fail button (disabled)
    $label = t("No");
    $output = '<div class="stripe-button-container">' .
      '<div class="stripe-button-title">' . $variables['label_title'] . '</div>' .
      '<div class="stripe-button-text">' . $variables['label_text'] . '</div>' .
      '<button id="' . $variables['id'] . '" type="button"' .
      ' class="btn btn-warning stripe-button" data-toggle="popover" data-trigger="hover" data-placement="auto bottom"'.
      ' title="Stripe Error" data-content="' . $variables['message']. '">' .
      '<span class="fa fa-times"></span>' . $label .
      '</button></div>';
  }

  return $output;
}

