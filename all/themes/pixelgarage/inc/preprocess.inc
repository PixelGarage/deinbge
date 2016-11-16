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
  // hide titles on login forms
  pg_login_preprocess_page($vars);

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


