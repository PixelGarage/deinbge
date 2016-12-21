<?php
/**
 * Contains the API function of the stripe button module.
 *
 * Created: ralph
 */

/**
 * After a stripe transaction has been successfully performed, this hook is called.
 * This hook is called inside a try-catch clause catching all Stripe exceptions, so no
 * special exception handling has to be done in this hook.
 *
 * An associative array with all charge parameters is transfered to the hook as input parameter.
 *
 * @param $charge_params       array
 *    The associative array with the following charge parameters as key-value pairs:
 *      currency:         The currency of the charged amount.
 *      amount:           The charged amount in currency.
 *      stripe_fee:       The stripe fee in currency.
 *      app_fee:          The application fees in currency.
 *      stripe_api_mode:  The stripe API mode, e.g. test | live.
 */
function hook_stripe_charge_completed($charge_params) {
  watchdog(
    'stripe_button',
    '@amount @curr charged including stripe fee = @stripe_fee @curr and application fee = @app_fee @curr',
    array(
      '@amount' => $charge_params['amount'],
      '@stripe_fee' => $charge_params['stripe_fee'],
      '@app_fee' => $charge_params['app_fee'],
      '@curr' => $charge_params['currency'],
    ), WATCHDOG_DEBUG);
}
