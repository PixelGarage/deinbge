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


