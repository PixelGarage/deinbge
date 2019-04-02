<?php
/**
 * @file
 * Bootstrap 6-6 stacked template for Display Suite.
 */
$switch = $field_switch_fields[0]['value'] == '1' ? 'switch ' : '';
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
      <<?php print $top_wrapper; ?> class="col-sm-12 <?php print $top_classes; ?>">
        <?php print $top; ?>
      </<?php print $top_wrapper; ?>>
    </div>
  <?php endif; ?>
  <?php if ($left || $right): ?>
    <div class="row">
      <<?php print $left_wrapper; ?> class="col-sm-6 col-md-4 left <?php print $switch; print $left_classes; ?>">
        <?php print $left; ?>
        <div class="field-text-extra left"><?php print $bottom; ?></div>
      </<?php print $left_wrapper; ?>>
      <<?php print $right_wrapper; ?> class="col-sm-6 col-md-8 right <?php print $switch; print $right_classes; ?>">
        <?php print $right; ?>
        <div class="field-text-extra right"><?php print $bottom; ?></div>
      </<?php print $right_wrapper; ?>>
    </div>
  <?php endif; ?>
</<?php print $layout_wrapper ?>>


<!-- Needed to activate display suite support on forms -->
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
