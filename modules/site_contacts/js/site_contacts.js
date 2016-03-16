var $jQuery = jQuery.noConflict();
$jQuery(document).ready(function() {
  (function($) {
    if (typeof CKEDITOR != "undefined") {
      CKEDITOR.replace('site-contacts-other-textarea');
      CKEDITOR.replace('site-contacts-management-textarea');
    }
  }(jQuery));
});