/**
* @file
* Файл библиотеки javascript модуля site commerce.
*/
var $jQuery = jQuery.noConflict();
$jQuery(document).ready(function() {
  (function($) {
    $("a#site-commerce-auto-applicability-note-link").click(function() {
      $("div.site-commerce-auto-applicability").css({"height":"auto"});
      $("a#site-commerce-auto-applicability-note-link").hide();
    });
  }(jQuery));
});