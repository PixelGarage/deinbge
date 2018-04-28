<?php
/**
 * @file
 * Bootstrap 12 template for Display Suite.
 */
?>


<<?php print $layout_wrapper; print $layout_attributes; ?> class="<?php print $classes; ?>">
  <?php if (isset($title_suffix['contextual_links'])): ?>
    <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>
  <div class="row">
    <<?php print $central_wrapper; ?> class="col-sm-12 <?php print $central_classes; ?>">
      <?php if (isset($content['field_image'])): ?>
        <!-- print image -->
        <div class="image">
          <?php print render($content['field_image']); ?>
        </div>
      <?php endif; ?>
      <?php if (isset($content['field_video'])): ?>
        <!-- print video -->
        <div class="video">
          <?php print render($content['field_video']); ?>
        </div>
      <?php endif; ?>
      <!-- print body -->
      <div class="body">
        <?php print render($content['body']); ?>
      </div>
      <?php if (isset($content['field_image'])): ?>
        <!-- print button -->
        <a class="block-button" href="<?php print $field_link[0]['url']; ?>" target="_blank">
          <?php print render($content['title']); ?>
        </a>
      <?php endif; ?>
    </<?php print $central_wrapper; ?>>
  </div>
</<?php print $layout_wrapper ?>>


<!-- Needed to activate display suite support on forms -->
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
