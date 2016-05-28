<?php
/**
 * Admin forms for the PxlRaffle module.
 */

/**
 * Implements hook_donation_operations().
 */
function pxlraffle_donation_operations() {
  $operations = array(
    'book' => array(
      'label' => t('Book selected donations'),
      'callback' => 'donation_mass_update',
      'callback arguments' => array('updates' => array('booked' => 1)),
    ),
    'unbook' => array(
      'label' => t('Unbook selected donations'),
      'callback' => 'donation_mass_update',
      'callback arguments' => array('updates' => array('booked' => 0)),
    ),
  );
  return $operations;
}

/* --------------------------------------------------
 * Donation mass updates
 * --------------------------------------------------*/

/**
 * Make mass update of donations, changing all donations in the $donations array
 * to update them with the field values in $updates.
 *
 * IMPORTANT NOTE: This function is intended to work when called from a form
 * submission handler. Calling it outside of the form submission process may not
 * work correctly.
 *
 * @param array $donations
 *   Array of donation dids to update.
 * @param array $updates
 *   Array of key/value pairs with node field names and the value to update that
 *   field to.
 */
function donation_mass_update($donations, $updates) {
  // We use batch processing to prevent timeout when updating a large number
  // of donations.
  if (count($donations) > 10) {
    $batch = array(
      'operations' => array(
        array('_donation_mass_update_batch_process', array($donations, $updates))
      ),
      'finished' => '_donation_mass_update_batch_finished',
      'title' => t('Processing'),
      // We use a single multi-pass operation, so the default
      // 'Remaining x of y operations' message will be confusing here.
      'progress_message' => '',
      'error_message' => t('The update has encountered an error.'),
      // The operations do not live in the .module file, so we need to
      // tell the batch engine which file to load before calling them.
      'file' => drupal_get_path('module', 'pxlraffle') . '/pxlraffle.admin.inc',
    );
    batch_set($batch);
  }
  else {
    foreach ($donations as $did) {
      _donation_mass_update_single($did, $updates);
    }
    drupal_set_message(t('The update has been performed.'));
  }
}

/**
 * Updates individual donations when fewer than 10 are queued.
 *
 * @param $did
 *   ID of donation to update.
 * @param $updates
 *   Associative array of updates.
 *
 * @return object
 *   An updated node object.
 *
 * @see node_mass_update()
 */
function _donation_mass_update_single($did, $updates) {
  // apply updates to donation
  $res = db_update('pxl_donations')
    ->fields($updates)
    ->condition('did', $did)
    ->execute();
  return $res;
}

/**
 * Implements callback_batch_operation().
 *
 * Executes a batch operation for donation_mass_update().
 *
 * @param array $donations
 *   An array of donation IDs.
 * @param array $updates
 *   Associative array of updates.
 * @param array $context
 *   An array of contextual key/values.
 */
function _donation_mass_update_batch_process($donations, $updates, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($donations);
    $context['sandbox']['donations'] = $donations;
  }

  // Process donations by groups of 5.
  $count = min(5, count($context['sandbox']['donations']));
  for ($i = 1; $i <= $count; $i++) {
    // For each did, load the donation, reset the values, and save it.
    $did = array_shift($context['sandbox']['donations']);
    _donation_mass_update_single($did, $updates);

    // Store result for post-processing in the finished callback.
    $context['results'][] = 'Donation ID = ' . $did;

    // Update our progress information.
    $context['sandbox']['progress']++;
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Implements callback_batch_finished().
 *
 * Reports the status of batch operation for node_mass_update().
 *
 * @param bool $success
 *   A boolean indicating whether the batch mass update operation successfully
 *   concluded.
 * @param int $results
 *   The number of nodes updated via the batch mode process.
 * @param array $operations
 *   An array of function calls (not used in this function).
 */
function _donation_mass_update_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('The update has been performed.'));
  }
  else {
    drupal_set_message(t('An error occurred and processing did not complete.'), 'error');
  }
  $message = format_plural(count($results), '1 item successfully processed:', '@count items successfully processed:');
  $message .= theme('item_list', array('items' => $results));
  drupal_set_message($message);
}

function donation_sum_column($donations, $updates) {
  $rows = &pxlraffle_session_data()['donation_rows'];

  foreach ($donations as $did) {
    ;
  }
  drupal_set_message(t('The columns sum has been updated.'));
}


/* --------------------------------------------------
 * PxlRaffle admin forms
 * --------------------------------------------------*/

/**
 * Page callback: Form constructor for the donation administration form.
 *
 * @see pxlraffle_admin_donations()
 * @see pxlraffle_admin_donations_submit()
 * @see pxlraffle_admin_donations_validate()
 * @see pxlraffle_menu()
 * @ingroup forms
 */
function pxlraffle_donation_overview($form, $form_state) {
  $form['admin'] = pxlraffle_admin_donations();

  return $form;
}

/**
 * Form builder: Builds the donation administration overview.
 *
 * @see pxlraffle_admin_donations()
 * @see pxlraffle_admin_donations_submit()
 * @see pxlraffle_admin_donations_validate()
 * @see pxlraffle_menu()
 *
 * @ingroup forms
 */
function pxlraffle_admin_donations() {
  $admin_access = user_access('administer pixel raffle');

  //
  // Build the 'Update options' form.
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
    '#access' => $admin_access,
  );
  $options = array();
  foreach (module_invoke_all('donation_operations') as $operation => $array) {
    $options[$operation] = $array['label'];
  }
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'sum_all',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#validate' => array('pxlraffle_admin_donations_validate'),
    '#submit' => array('pxlraffle_admin_donations_submit'),
  );

  //
  // Build the sortable table header.
  $header = array(
    'did' => array('data' => t('ID'), 'field' => 'd.did'),
    'booked' => array('data' => t('Booked'), 'field' => 'd.booked'),
    'type' => array('data' => t('Type'), 'field' => 'd.type'),
    'amount' => array('data' => t('Donation amount'), 'field' => 'd.amount'),
    'stripe_fee' => array('data' => t('Stripe fee'), 'field' => 'd.stripe_fee'),
    'app_fee' => array('data' => t('Application fee'), 'field' => 'd.app_fee'),
    'changed' => array('data' => t('Updated'), 'field' => 'd.timestamp', 'sort' => 'desc')
  );

  //
  // select all donations ID's of a page
  $query = db_select('pxl_donations', 'd')->extend('PagerDefault')->extend('TableSort');
  $result = $query
    ->fields('d')
    ->limit(50)
    ->orderByHeader($header)
    ->execute();

  //
  // Prepare the list of donations.
  $session_data = &pxlraffle_session_data();
  $rows = array();
  $amount = 0;
  $stripe_fee = 0;
  $app_fee = 0;
  foreach ($result as $donation) {
    $rows[$donation->did] = array(
      'did' => $donation->did,
      'booked' => $donation->booked,
      'type' => check_plain($donation->type),
      'amount' => floatval($donation->amount),
      'stripe_fee' => floatval($donation->stripe_fee),
      'app_fee' => floatval($donation->app_fee),
      'changed' => format_date($donation->timestamp, 'short')
    );
    $amount += floatval($donation->amount);
    $stripe_fee += floatval($donation->stripe_fee);
    $app_fee += floatval($donation->app_fee);
  }
  $session_data['donation_rows'] = $rows;
  $session_data['donation_amount'] = $amount;
  $session_data['donation_stripe_fee'] = $stripe_fee;
  $session_data['donation_app_fee'] = $app_fee;

  //
  // add sums to header
  $header['amount'] = array('data' => t('Donation amount = @amount CHF', array('@amount' => $amount)), 'field' => 'd.amount');
  $header['stripe_fee'] = array('data' => t('Stripe fee = @stripe_fee CHF', array('@stripe_fee' => $stripe_fee)), 'field' => 'd.stripe_fee');
  $header['app_fee'] = array('data' => t('Application fee = @app_fee CHF', array('@app_fee' => $app_fee)), 'field' => 'd.app_fee');

  // Only use a tableselect when the current user is able to perform any
  // operations.
  if ($admin_access) {
    $form['donations'] = array(
      '#type' => 'tableselect',
      '#prefix' => '<div id="tableselect-wrapper">',
      '#suffix' => '</div>',
      '#header' => $header,
      '#options' => $rows,
      '#empty' => t('No content available.'),
      '#ajax' => array(
        'callback' => 'pxlraffle_tableselect_callback',
        'wrapper' => 'tableselect-wrapper',
      ),
    );
  }
  // Otherwise, use a simple table.
  else {
    $form['donations'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No content available.'),
    );
  }

  $form['pager'] = array('#markup' => theme('pager'));
  return $form;
}

/**
 * Validate pxlraffle_admin_donations form submissions.
 *
 * Checks whether any donations have been selected to perform the chosen 'Update
 * option' on.
 */
function pxlraffle_admin_donations_validate($form, &$form_state) {
  // Error if there are no items to select.
  if (!is_array($form_state['values']['donations']) || !count(array_filter($form_state['values']['donations']))) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * Process pxlraffle_admin_donations form submissions.
 *
 * Executes the chosen 'Update option' on the selected nodes.
 **/
function pxlraffle_admin_donations_submit($form, &$form_state) {
  $operations = module_invoke_all('donation_operations');
  $operation = $operations[$form_state['values']['operation']];
  // Filter out unchecked donations
  $donations = array_filter($form_state['values']['donations']);
  if ($function = $operation['callback']) {
    // Add in callback arguments if present.
    if (isset($operation['callback arguments'])) {
      $args = array_merge(array($donations), $operation['callback arguments']);
    }
    else {
      $args = array($donations);
    }
    call_user_func_array($function, $args);

    cache_clear_all();
  }
  else {
    // We need to rebuild the form to go to a second step. For example, to
    // show the confirmation form for the deletion of nodes.
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * AJAX callback for the tableselect.
 *
 * Calculates the sum of all selected donation amount, stripe and application fees.
 */
function pxlraffle_tableselect_callback($form, &$form_state) {
  // get donation rows
  $session_data = &pxlraffle_session_data();
  $rows = $session_data['donation_rows'];
  $amount = 0;
  $stripe_fee = 0;
  $app_fee = 0;

  // Filter out unchecked donations
  $donations = array_filter($form_state['values']['donations']);
  if (is_array($donations) && count($donations)) {
    // donations selected, calculate sum of selection
    foreach ($donations as $did) {
      $amount += $rows[$did]['amount'];
      $stripe_fee += $rows[$did]['stripe_fee'];
      $app_fee += $rows[$did]['app_fee'];
    }
  }
  else {
    // sum of all donations
    $amount = $session_data['donation_amount'];
    $stripe_fee = $session_data['donation_stripe_fee'];
    $app_fee = $session_data['donation_app_fee'];
  }

  //
  // add sums to header
  $table_donations = $form['admin']['donations'];
  $table_donations['#header']['amount']['data'] =  t('Donation amount = @amount CHF', array('@amount' => $amount));
  $table_donations['#header']['stripe_fee']['data'] = t('Stripe fee = @stripe_fee CHF', array('@stripe_fee' => $stripe_fee));
  $table_donations['#header']['app_fee']['data'] = t('Application fee = @app_fee CHF', array('@app_fee' => $app_fee));

  return $table_donations;
}
