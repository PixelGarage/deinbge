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
    <div class="inner-current-raffle">
      <?php if (!empty($title)): ?>
        <h2 class="block-header"><?php print $title; ?></h2>
      <?php endif; ?>
      <div class="odometer-container">
        <?php if ($odometer_label_on_top): ?>
          <div class="odometer-label"><?php print $odometer_label; ?></div>
          <div class="odometer"><?php print $user_count; ?></div>
        <?php else: ?>
          <div class="odometer"><?php print $user_count; ?></div>
          <div class="odometer-label"><?php print $odometer_label; ?></div>
        <?php endif; ?>
      </div>
      <div class="total-amount">CHF <?php print $total_amount; ?></div>
      <div class="progress">
        <div class="progress-bar" role="progressbar" aria-valuenow="<?php print $fill_level; ?>" aria-valuemin="0" aria-valuemax="100" style="width: <?php print $fill_level; ?>%;">
          <span class="sr-only"><?php print $fill_level; ?>% Complete</span>
        </div>
      </div>
      <div class="current-amount-container">
        <span class="fa fa-caret-up" style="position: relative; left: <?php print $fill_level; ?>%;"></span>
        <div class="current-amount"  style="position: relative; left: <?php print $fill_level; ?>%;">CHF <?php print $current_amount; ?></div>
      </div>
      <div class="explanation"><?php print $explanation; ?></div>
      <div class="actions">
        <a class="btn btn-default link-participate" href="<?php print $url_participate; ?>"><?php print $label_participate; ?></a>
      </div>
    </div>
  </div>
</div>
