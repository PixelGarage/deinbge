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
  <!-- The next raffle section-->
  <div class="next-raffle">
    <div class="next-raffle-slogan"><?php print $next_raffle_slogan; ?></div>
    <div class="next-raffle-label"><?php print $next_raffle_label; ?></div>
    <div class="next-raffle-date"><?php print $next_raffle_date; ?></div>
    <div class="next-raffle-info"><?php print $next_raffle_info; ?></div>
    <div class="actions">
      <a class="btn btn-default link-participate"
         href="<?php print $url_participate; ?>"><?php print $label_participate; ?>
      </a>
    </div>
  </div>
</div>
