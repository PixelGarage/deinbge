<?php
/**
 * @file
 * Bootstrap 12 template for Display Suite.
 */

if ($ds_switch === 'recurring_payment' && !$logged_in) {
  // recurring payments only possible with logged in user, display register information
  global $base_url, $language;
  $url_register = $base_url . '/' . $language->language . '/user';
  $label_register = t('Register?');
  $hint_register = t('You need an account to subscribe successfully to a recurring payment!');
  $display_register_info = true;
}
else {
  $display_register_info = false;
}

?>

<?php if ($display_register_info): ?>
  <div class="registration-information">
    <div class="registration-message">
      <div class="registration-hint"><?php print $hint_register; ?></div>
      <a class="btn btn-error link-register" href="<?php print $url_register; ?>"><?php print $label_register; ?></a>
    </div>
  </div>
<?php endif; ?>

<<?php print $layout_wrapper; print $layout_attributes; ?> class="<?php print $classes; ?>">
<?php if (isset($title_suffix['contextual_links'])): ?>
  <?php print render($title_suffix['contextual_links']); ?>
<?php endif; ?>
<div class="row">
    <<?php print $central_wrapper; ?>
    class="col-sm-12 <?php print $central_classes; ?>">
  <?php print $central; ?>
</<?php print $central_wrapper; ?>>
</div>
</<?php print $layout_wrapper ?>>


<!-- Needed to activate display suite support on forms -->
<?php if (!empty($drupal_render_children)): ?>
  <?php print $drupal_render_children ?>
<?php endif; ?>
