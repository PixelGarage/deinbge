<?php
/**
 * @file
 * Bootstrap 12 template for Display Suite.
 */

if ($ds_switch === 'recurring_payment' && !$logged_in) {
  // recurring payments only possible with logged in user, display login form
  $login_form = drupal_get_form('user_login', 'recurring_payment');
  $extended_login_form = array(
    'description' => array(
      '#markup' => t('Subscribe now to one of our subscription plans and automatically be part of any future raffle.'),
    ),
    'form' => $login_form,
  );
  $central = drupal_render($extended_login_form);
}
?>


<<?php print $layout_wrapper;
print $layout_attributes; ?> class="<?php print $classes; ?>">
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
