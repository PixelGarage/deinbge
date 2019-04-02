<?php
/**
 * @file
 * Bootstrap 12 template for Display Suite.
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
  <div class="row">
    <<?php print $central_wrapper; ?> class="col-sm-12 <?php print $central_classes; ?>">
      <?php print $central; ?>
    </<?php print $central_wrapper; ?>>
  </div>
</<?php print $layout_wrapper ?>>


<!-- Needed to activate display suite support on forms -->
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
