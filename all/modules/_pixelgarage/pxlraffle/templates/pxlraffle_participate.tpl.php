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
  <!-- The participation section-->
  <div class="participation">
    <?php if (!empty($title)): ?>
      <h2 class="block-header"><?php print $title; ?></h2>
    <?php endif; ?>
    <div class="participation-slogan"><?php print $participation_slogan; ?></div>
    <div class="participate-accordion">
      <div class="panel panel-default">
        <div class="panel-heading">
          <h4 class="panel-title">
            <a class="accordion-toggle"
               data-toggle="collapse"
               href="#collapse-accordion">
              <?php print $participation_acc_title ?>
            </a>
          </h4>
        </div>

        <div id="collapse-accordion" class="panel-collapse collapse">
          <?php print render($participation_acc_body); ?>
        </div>
      </div>
    </div>
  </div>
</div>
