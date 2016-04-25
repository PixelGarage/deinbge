/**
 * Created by ralph on 25.04.16.
 */

(function ($) {

  /**
   * Animates the odometer when shown.
   */
  Drupal.behaviors.pxlRaffleParticipate = {
    attach: function () {
      var $toggle = $('.participation-accordion .accordion-toggle'),
          isCollapsed = Drupal.settings.pxlraffle_participate.collapsed;

      $toggle.off('click');
      $toggle.on('click', function() {
        var $this = $(this),
            showLabel = Drupal.settings.pxlraffle_participate.show_label,
            hideLabel = Drupal.settings.pxlraffle_participate.hide_label;

        if (isCollapsed) {
          $this.html(hideLabel);
          isCollapsed = false;
        }
        else {
          $this.html(showLabel);
          isCollapsed = true;
        }
      });
    }
  }

})(jQuery);

