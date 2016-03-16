/**
* @file
* Файл библиотеки javascript модуля site commerce order.
*/
var $jQuery = jQuery.noConflict();
$jQuery(document).ready(function() {
  (function($) {
    // Инициализация библиотеки mask.
    if ($.isFunction($().mask)) {
      var form_tel_maskedinput = Drupal.settings.site_commerce_captcha_phone;
      if (form_tel_maskedinput) {
        $(".form-tel-maskedinput").mask("+79999999999");
      }

      Modernizr.load({
        Test: Modernizr.inputtypes.date,
        Nope: function () {
          // Нет встроенной поддержки для <input type="date">.
          var pattern_date_maskedinput = Drupal.settings.pattern_date_maskedinput;
          $(".form-date-maskedinput").mask("99.99.9999", {placeholder:pattern_date_maskedinput});
          $(".form-time-maskedinput").mask("99:99", {placeholder:"--:--"});
        },
      });
    }
  }(jQuery));
});