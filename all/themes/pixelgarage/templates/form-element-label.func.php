<?php

/**
 * @file
 * Stub file for bootstrap_form_element_label().
 */

/**
 * Returns HTML for a form element label and required marker.
 *
 * Adapts the checkbox and radio buttons in such a way, that the styling can be
 * completely overridden.
 *
 * - classes for label added: checkbox-container, radio-container
 * - Adds a checkmark span element at the end of the checkbox- or radio-container
 *
 * @see theme_form_element_label()
 *
 * @ingroup theme_functions
 */
function pixelgarage_form_element_label(array &$variables) {
  $element = $variables['element'];

  // Extract variables.
  $output = '';

  $title = !empty($element['#title']) ? filter_xss_admin($element['#title']) : '';

  // Only show the required marker if there is an actual title to display.
  $marker = array('#theme' => 'form_required_marker', '#element' => $element);
  if ($title && $required = !empty($element['#required']) ? drupal_render($marker) : '') {
    $title .= ' ' . $required;
  }

  $display = isset($element['#title_display']) ? $element['#title_display'] : 'before';
  $type = !empty($element['#type']) ? $element['#type'] : FALSE;
  $checkbox = $type && $type === 'checkbox';
  $radio = $type && $type === 'radio';

  // Immediately return if the element is not a checkbox or radio and there is
  // no label to be rendered.
  if (!$checkbox && !$radio && ($display === 'none' || !$title)) {
    return '';
  }

  // Retrieve the label attributes array.
  $attributes = &_bootstrap_get_attributes($element, 'label_attributes');

  // Add Bootstrap label class.
  $attributes['class'][] = 'control-label';
  if ($checkbox) {
    $attributes['class'][] = 'checkbox-container';
  }
  else if ($radio) {
    $attributes['class'][] = 'radio-container';
  }

  // Add the necessary 'for' attribute if the element ID exists.
  if (!empty($element['#id'])) {
    $attributes['for'] = $element['#id'];
  }

  // Checkboxes and radios must construct the label differently.
  if ($checkbox || $radio) {
    if ($display === 'before') {
      $output .= $title;
    }
    elseif ($display === 'none' || $display === 'invisible') {
      $output .= '<span class="element-invisible">' . $title . '</span>';
    }
    // Inject the rendered checkbox or radio element inside the label.
    if (!empty($element['#children'])) {
      $output .= $element['#children'];
      // add checkmark element for custom styling
      $output .= '<span class="checkmark"></span>';
    }
    if ($display === 'after') {
      $output .= $title;
    }
  }
  // Otherwise, just render the title as the label.
  else {
    // Show label only to screen readers to avoid disruption in visual flows.
    if ($display === 'invisible') {
      $attributes['class'][] = 'element-invisible';
    }
    $output .= $title;
  }

  // The leading whitespace helps visually separate fields from inline labels.
  return ' <label' . drupal_attributes($attributes) . '>' . $output . "</label>\n";
}
