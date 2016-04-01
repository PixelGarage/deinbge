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
  <!-- The current raffle section-->
  <div class="current-raffle">
    <?php if (!empty($title)): ?>
      <h2 class="block-header"><?php print $title; ?></h2>
    <?php endif; ?>
    <?php if (!empty($title)): ?>
      <div class="current-raffle-summary"><?php print $summary; ?></div>
    <?php endif; ?>
    <div class="total-amount">CHF <?php print $total_amount; ?></div>
    <div class="progress">
      <div class="progress-bar" role="progressbar" aria-valuenow="<?php print $fill_level; ?>" aria-valuemin="0" aria-valuemax="100" style="width: <?php print $fill_level; ?>%;">
        <span class="sr-only"><?php print $fill_level; ?>% Complete</span>
      </div>
    </div>
    <div class="current-amount-container">
      <span class="fa fa-caret-up"></span>
      <div class="current-amount">CHF <?php print $current_amount; ?></div>
    </div>
  </div>
</div>
