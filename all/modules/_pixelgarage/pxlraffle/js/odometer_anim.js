/**
 * This file contains behaviors for the odometer animation.
 *
 * Created by ralph on 31.01.15.
 */

(function ($) {

  /**
   * Animates the odometer when shown.
   */
  Drupal.behaviors.pxlRaffleOdometerAnim = {
    attach: function () {
      var $odometer = $('.odometer'),
          value = $odometer.html(),
          counter = parseFloat(value),
          animatedRange = Drupal.settings.pxlraffle_odometer.animated_range,
          format = Drupal.settings.pxlraffle_odometer.format,
          startValue = Math.max(counter - animatedRange, 0),
          odometer = new Odometer({ el: $odometer[0], value: startValue, format: format });

      // set the odometer value (reduced by the range)
      //$odometer.html(startValue);

      setTimeout(function() {
        $odometer.html(counter);
      }, 400);
    }
  }

})(jQuery);

