<?php
/**
 * @file
 * Bootstrap 6-6 stacked template for Display Suite.
 */

global $language;
$path = drupal_get_path('theme', 'pixelgarage') . '/images/';
$logo = file_create_url($path . 'deinbge_logo_d.svg');

switch ($language->language) {
  case 'en':
    $logo = str_replace('_d.svg', '_e.svg', $logo);
    $header = t('Your basic income');
    break;
  case 'fr':
    $logo = str_replace('_d.svg', '_f.svg', $logo);
    $header = t('Ton revenue de base');
    break;
  case 'it':
    $logo = str_replace('_d.svg', '_i.svg', $logo);
    $header = t('Il tuo reditto di base');
    break;
  case 'de':
  default:
    $logo = str_replace('.png', '.svg', $logo);
    $header = t('Dein Grundeinkommen');
    break;
}
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
      <div class="col-menu col-sm-12 col-lg-3 left">
        <?php print render($content['body']); ?>
      </div>

      <div class="col-lang col-sm-12 col-lg-3 middle">
        <?php print render($content['language_field']); ?>
        <?php print render($content['follow_site']); ?>
      </div>

      <div class="col-contact col-sm-12 col-lg-6 order-lg-first right">
        <img class="footer-logo" src="<?php print $logo; ?>"/>
        <div class="footer-header"><?php print $header; ?></div>
        <?php print render($content['field_medienkontakt']); ?>
      </div>
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
