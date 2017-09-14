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
    <!-- print button -->
      <a class="call2action-container" href="<?php print $field_link[0]['url']; ?>">
        <!--?php print render($content['field_weight']); ?-->
        <!--?php print render($content['field_animation_icon']); ?-->
        <?php print render($content['title']); ?>
      </a>
      <!-- print body -->
      <div class="body">
        <?php print render($content['body']); ?>
      </div>
    </<?php print $central_wrapper; ?>>
  </div>
</<?php print $layout_wrapper ?>>


<!-- Needed to activate display suite support on forms -->
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
