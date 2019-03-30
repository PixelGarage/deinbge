<?php
/**
 * @file
 * Bootstrap 6-6 stacked template for Display Suite.
 */
?>


<<?php print $layout_wrapper; print $layout_attributes; ?> class="<?php print $classes; ?>">
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
    <<?php print $left_wrapper; ?> class="column col-xs-12 col-md-6 left <?php print $left_classes; ?>">
      <div class="inner-column col-xs-12 col-sm-6">
        <?php print render($content['field_medienkontakt']); ?>
      </div>
      <div class="inner-column col-xs-12 col-sm-6">
        <?php print render($content['body']); ?>
      </div>
    </<?php print $left_wrapper; ?>>

    <<?php print $right_wrapper; ?> class="column col-xs-12 col-md-6 right <?php print $right_classes; ?>">
      <div class="inner-column col-xs-12 col-sm-6">
        <?php print render($content['field_impressum']); ?>
      </div>
      <div class="inner-column col-xs-12 col-sm-6">
        <?php print render($content['follow_site']); ?>
        <?php print render($content['language_field']); ?>
      </div>
    </<?php print $right_wrapper; ?>>

  </div>
<?php endif; ?>
<?php if ($bottom): ?>
  <div class="row">
    <<?php print $bottom_wrapper; ?> class="col-sm-12 <?php print $bottom_classes; ?>">
      <?php print $bottom; ?>
    </<?php print $bottom_wrapper; ?>>
  </div>
<?php endif; ?>
</<?php print $layout_wrapper ?>>


<!-- Needed to activate display suite support on forms -->
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
