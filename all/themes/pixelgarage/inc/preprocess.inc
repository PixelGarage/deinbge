<?php
/**
 * Created by PhpStorm.
 * User: ralph
 * Date: 06.02.15
 * Time: 17:23
 */

/**
 * Preprocess variables for the html template.
 */
function pixelgarage_preprocess_html(&$vars) {
  // make sure jQuery UI and effects is loaded for anonymous users
  drupal_add_library('system', 'ui');
  drupal_add_library('system', 'effects');

}

/**
 * Override or insert variables for the page templates.
 */
function pixelgarage_preprocess_page(&$vars) {
  // hide title for user registration / login
  switch (current_path()) {
    case 'user':
    case 'user/login':
    case 'user/register':
    case 'user/password':
      $vars['title'] = '';
  }

  // replace logo with .svg and make it language dependant
  global $language;

  switch ($language->language) {
    case 'en':
      $vars['logo'] = str_replace('_d.png', '_e.png', $vars['logo']);
      break;
    case 'fr':
      $vars['logo'] = str_replace('_d.png', '_f.png', $vars['logo']);
      break;
    case 'it':
      $vars['logo'] = str_replace('_d.png', '_i.png', $vars['logo']);
      break;
    case 'de':
    default:
      //$vars['logo'] = str_replace('.jpg', '.svg', $vars['logo']);
      break;
  }
}

/* =============================================================================
 *      Exposed search form
 * ========================================================================== */
/**
 * Adapts the exposed search form to show placeholders in search fields.
 *
 */
function pixelgarage_preprocess_views_exposed_form(&$vars) {
  //
  // add placeholders to search exposed form items
  $form = &$vars['form'];
  if ($form['#id'] != 'views-exposed-form-blog-panel-pane-1') return;

  foreach ($form['#info'] as $id => $info) {
    // add the description as placeholder to the widget
    $field = $form[$info['value']];
    $field['#attributes']['placeholder'] = $info['description'];
    $field['#printed'] = false;

    // render widget again
    $vars['widgets'][$id]->widget = drupal_render($field);
  }

}


/**
 * Themes a Stripe Checkout button with a predefined amount in a given currency.
 */
function pixelgarage_preprocess_stripe_button_fix_value(&$variables) {
  // Render Stripe Checkout button
  $amount = $variables['amount'];
  if ($amount > 0) {
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
    $variables['box_title'] = $box_title;

    // create box text depending on recurring billing
    $recurring_billing = $variables['recurring_billing'];
    switch ($recurring_billing) {
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
    $variables['box_text'] = $box_text;

  }
}

