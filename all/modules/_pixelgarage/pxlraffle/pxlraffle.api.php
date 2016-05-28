<?php
/**
 * PxlRaffle API functionality.
 */

/**
 * Add mass donation operations.
 *
 * This hook enables modules to inject custom operations into the mass
 * raffle dropdown found at admin/pxlraffle/donations, by associating a callback
 * function with the operation, which is called when the form is submitted. The
 * callback function receives one initial argument, which is an array of the
 * checked donations.
 *
 * @return
 *   An array of operations. Each operation is an associative array that may
 *   contain the following key-value pairs:
 *   - label: (required) The label for the operation, displayed in the dropdown
 *     menu.
 *   - callback: (required) The function to call for the operation.
 *   - callback arguments: (optional) An array of additional arguments to pass
 *     to the callback function.
 */
function hook_donation_operations() {
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
    'sum' => array(
      'label' => t('Sum columns of selected donations'),
      'callback' => 'donation_mass_update',
      'callback arguments' => array('updates' => array('sum' => 'all')),
    ),
  );
  return $operations;
}

