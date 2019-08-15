<?php
/**
 * @file
 * Bootstrap 4-4-4 Stacked template for Display Suite.
 */
$full_width = $field_full_width_paragraph[0]['value'] == '1' ? 'full-width ' : '';
if (isset($field_anchor[0]['value'])) {
  $layout_attributes .= ' id="' . $field_anchor[0]['value'] . '"';
}
?>


<<?php print $layout_wrapper; print $layout_attributes; ?> class="<?php print $full_width; print $classes; ?>">
  <?php if (isset($title_suffix['contextual_links'])): ?>
    <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>
  <?php if ($top): ?>
    <div class="row">
      <<?php print $top_wrapper; ?> class="col col-sm-12 <?php print $top_classes; ?>">
        <?php print $top; ?>
      </<?php print $top_wrapper; ?>>
    </div>
  <?php endif; ?>
  <?php if ($left || $central || $right): ?>
    <div class="row">
      <<?php print $left_wrapper; ?> class="col col-md-12 col-lg-4 <?php print $left_classes; ?>">
        <?php print $left; ?>
      </<?php print $left_wrapper; ?>>
      <<?php print $central_wrapper; ?> class="col col-md-6 col-lg-4 <?php print $central_classes; ?>">
        <?php print $central; ?>
      </<?php print $central_wrapper; ?>>
      <<?php print $right_wrapper; ?> class="col col-md-6 col-lg-4 <?php print $right_classes; ?>">
        <?php print $right; ?>
      </<?php print $right_wrapper; ?>>
    </div>
  <?php endif; ?>
  <?php if ($bottom): ?>
    <div class="row">
      <<?php print $bottom_wrapper; ?> class="col col-sm-12 <?php print $bottom_classes; ?>">
        <?php print $bottom; ?>
      </<?php print $bottom_wrapper; ?>>
    </div>
  <?php endif; ?>
</<?php print $layout_wrapper ?>>


<!-- Needed to activate display suite support on forms -->
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
