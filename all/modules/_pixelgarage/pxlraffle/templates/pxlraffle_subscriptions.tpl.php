<?php
/**
 * @file
 * Default theme implementation to display a raffle current info block.
 *
 * Available variables are:
 * - $container_id: the id of the raffle info container.
 *
 * @see template_preprocess_pxlraffle_current_info()
 *
 * @ingroup themeable
 */
?>

<div id="<?php print $container_id; ?>">
    <!-- The subscriptions section-->
    <div class="subscriptions">
      <?php if (!empty($title)): ?>
          <h2 class="block-header"><?php print $title; ?></h2>
      <?php endif; ?>
        <div class="subscription-slogan"><?php print $subscription_slogan; ?></div>
        <div class="actions">
            <a class="btn btn-default link-subscribe"
               href="<?php print $subscription_link_url; ?>"><?php print $subscription_link_label; ?></a>
        </div>
        <div class="odometer-amount">
            <div class="odometer-label"><?php print $odometer_label; ?></div>
            <div class="odometer"><?php print $subscribed_amount; ?></div>
            <span class="currency">CHF</span>
        </div>
        <div class="subscription-fee-statement"><?php print $subscribtion_fee_statement; ?></div>
    </div>
</div>
