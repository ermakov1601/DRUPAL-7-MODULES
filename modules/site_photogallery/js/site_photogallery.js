var $jQuery = jQuery.noConflict();
$jQuery(document).ready(function() {
  (function($) {
    if ($.isFunction($().colorbox)) {
      $("a.site-photogallery-colorbox").colorbox({
        current: "Изображение {current} из {total}",
        rel: true,
        arrowKey: true,
        preloading: true,
        maxWidth: '90%',
        maxHeight: '90%',
      });
    }
  }(jQuery));
});