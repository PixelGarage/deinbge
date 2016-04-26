<?php
/**
 * Contains the API function of the stripe button module.
 *
 * Created: ralph
 */

/**
 * Hook called after the stripe transaction has been successfully performed.
 *
 * Can be used to update Drupal internal data structures.
 *
 * @param $amount     The charged amount in currency
 * @param $app_fees   The application fees in currency
 * @param $currency   The currency of the charged amount.
 */
function hook_charge_completed($amount, $app_fees = 0, $currency = 'CHF') {

}
