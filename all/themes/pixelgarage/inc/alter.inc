<?php
/**
 * Created by PhpStorm.
 * User: ralph
 * Date: 06.02.15
 * Time: 17:22
 */

/* =============================================================================
 *    Proximity alter hooks implementation
 * ========================================================================== */

/**
 * Alters the proximity item load parameter array.
 *
 * The load parameter array defines a specific url parameter for each proximity item.
 * This parameter is added at the end of the request url and must be unique and URL conform.
 * The unique parameter defines, which item content has to be loaded from
 * the server (see next api function).
 *
 * @param $container_index      int     Index of proximity container (if more than one container exists in one page).
 * @param $view                 array   The view.
 * @param $ajax_load_params     array   Array of ajax load parameters to be altered, one for each proximity item (row)
 *                                      retrieved by the view. Default is the views row index.
 */
function pixelgarage_proximity_load_params_alter($container_index, $view, &$ajax_load_params) {
  // Return the node id as ajax parameter for each item.
  foreach ($view->result as $id => $item) {
    $ajax_load_params[$id] = $item->nid;
  }
}

/**
 * Implements the item specific content as render array or html string.
 * The input parameter $param contains the unique load parameter of the requested item.
 *
 * @param $container_index      int     Index of proximity container (if more than one container exists in one page).
 * @param $param                string  The item specific load parameter (see also hook_proximity_ajax_load_params_alter).
 * @param $render_item          mixed   The rendered content to be returned to the client. The $render_item should be
 *                                      replaced either by a string (rendered html content), a render array or an integer (error code).
 */
function pixelgarage_proximity_render_item_alter($container_index, $param, &$render_item) {
  // return the render array for the specific node, if available
  if ($node= node_load($param)) {
    $view_mode = 'full';
    if (property_exists($node, 'ds_switch') && !empty($node->ds_switch)) {
      // take an alternate view mode set by the ds switch
      $view_mode = $node->ds_switch;
    }
    $render_item = node_view($node, $view_mode);
  }
}


/* =============================================================================
 *    Stripe button alter hooks implementation
 * ========================================================================== */
/**
 * This hook allows to define all relations between a stripe button field and a fee button field.
 *
 * Use this hook to relate a stripe button field with a fee button field, so
 * that the correct fee percentage is used for the pressed stripe button.
 *
 * @param $stripe_button_relations  array
 *    Empty array to be filled with all stripe button - fee button relations.
 */
function pixelgarage_stripe_button_fee_button_relation_alter(&$stripe_button_relations) {
  $stripe_button_relations = array(
    'field-spenden' => 'field-spenden-fee-percentage',
    'field-spenden-predefined' => 'field-spenden-fee-percentage',
    'field-recurr-spenden' => 'field-recurr-spenden-fee-percent',
    'field-recurr-spenden-predefined' => 'field-recurr-spenden-fee-percent',
  );
}

/**
 * This hook alters the feedback associative array to provide a feedback for each selectable fee percentage button.
 * Use it to give a positive feedback to the user and explain, what the selected fee is used for.
 */
function pixelgarage_stripe_button_fee_select_feedback_alter(&$feedbacks, $fee_button_id) {
  if ($fee_button_id == 'field-spenden-fee-percentage') {
    $feedbacks += array (
      '0' => t('<strong>Too bad!</strong> We are entirely financed by voluntary commission. Your contribution would make a difference.'),
      '0.05' => t('<strong>Thank you!</strong> Your contribution shows us that you appreciate our work.'),
      '0.1' => t('<strong>Wow!</strong> Your contribution allows us to keep this platform up and running.'),
      '0.2' => t('<strong>Amazing!</strong> Your contribution enables us to enhance the functionality of this platform.'),
      '0.3' => t('<strong>Absolutely awesome!</strong> We are very grateful that you honor our work so generously.'),
    );
  }
  else if ($fee_button_id == 'field-recurr-spenden-fee-percent') {
    $feedbacks += array (
      '0.1' => t('<strong>Thank you!</strong> Your contribution shows us that you appreciate our work.'),
      '0.2' => t('<strong>Thank you!</strong> Your contribution shows us that you appreciate our work.'),
      '0.3' => t('<strong>Wow!</strong> Your contribution helps us to finance the administration tasks.'),
      '0.4' => t('<strong>Amazing!</strong> Your contribution enables us to enhance the functionality of this platform.'),
      '0.5' => t('<strong>Absolutely awesome!</strong> We are very grateful that you honor our work so generously.'),
    );
  }
}

