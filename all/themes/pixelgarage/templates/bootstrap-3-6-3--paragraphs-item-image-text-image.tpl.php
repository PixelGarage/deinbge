<?php
/**
 * @file
 * Bootstrap 3-6-3 template for Display Suite.
 */
$url1 = $field_link_1[0]['url'];
$url2 = $field_link_2[0]['url'];
$button_text = t('Zu den Resultaten');
?>


<<?php print $layout_wrapper; print $layout_attributes; ?> class="<?php print $classes; ?>">
  <?php if (isset($title_suffix['contextual_links'])): ?>
    <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>
  <div class="row">
    <<?php print $central_wrapper; ?> class="col-sm-6 col-sm-push-3 <?php print $central_classes; ?>">
      <?php print $central; ?>
    </<?php print $central_wrapper; ?>>
    <<?php print $left_wrapper; ?> class="col-xxs-6-sm-3 col-sm-pull-6 <?php print $left_classes; ?>">
      <div class="col-inner">
        <?php if (!empty($url1)): ?>
          <?php print $left; ?>
          <a class="button-survey" href="<?php print $url1; ?>"><?php print $button_text; ?></a>
        <?php else: ?>
          <?php print $left; ?>
        <?php endif; ?>
      </div>
    </<?php print $left_wrapper; ?>>
    <<?php print $right_wrapper; ?> class="col-xxs-6-sm-3 <?php print $right_classes; ?>">
      <div class="col-inner">
        <?php if (!empty($url2)): ?>
          <?php print $right; ?>
          <a class="button-survey" href="<?php print $url2; ?>"><?php print $button_text; ?></a>
        <?php else: ?>
          <?php print $right; ?>
        <?php endif; ?>
      </div>
    </<?php print $right_wrapper; ?>>
  </div>
</<?php print $layout_wrapper ?>>


<!-- Needed to activate display suite support on forms -->
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
