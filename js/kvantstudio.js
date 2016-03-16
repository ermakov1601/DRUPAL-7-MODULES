/**
* @file
* Файл библиотеки javascript модуля kvantstudio.
*/

var $jQuery = jQuery.noConflict();
$jQuery(document).ready(function() {
  (function($) {
    /* Инициализация zoom для ссылок изображений. */
    if ($.isFunction($().zoom)) {
      $('a.zoom')
      .zoom({
        url: $('a.zoom').attr("href"),
      });
    }

    /* Инициализация readmore. */
    if ($.isFunction($().readmore)) {
      $('.readmore-block').readmore({
        speed: 100,
        collapsedHeight: Drupal.settings.kvantstudio_readmore_height,
        moreLink: '<a href="#" class="readmore-link readmore-link__more">' + Drupal.settings.kvantstudio_readmore_more + '</a>',
        lessLink: '<a href="#" class="readmore-link readmore-link__less">' + Drupal.settings.kvantstudio_readmore_less + '</a>'
      });
    }

  }(jQuery));
});

(function($) {
    Drupal.behaviors.litebox = {
     attach: function (context, settings) {
      /* Инициализация litebox для ссылок изображений. */
      if ($.isFunction($().liteBox)) {
        $('.litebox').liteBox({
          revealSpeed: 100,
          background: 'rgba(0,0,0,.8)',
          overlayClose: true,
          escKey: true,
          navKey: true,
          callbackInit: function() {},
          callbackBeforeOpen: function() {},
          callbackAfterOpen: function() {},
          callbackBeforeClose: function() {},
          callbackAfterClose: function() {},
          callbackError: function() {},
          callbackPrev: function() {},
          callbackNext: function() {},
          errorMessage: 'Error loading content.'
        });
      }
    }}
}(jQuery));

/* Выводит на печать содержимое элемента по id. */
function kvantstudio_print_id(id) {
  var text = document.getElementById(id).innerHTML;
  var title = window.document.title;
  var location = window.document.location;
  var WinPrint = window.open('','','left=center,top=0,width=1000,height=800,toolbar=0,scrollbars=1,status=0');
  WinPrint.document.writeln('<html><head><title>Версия для печати<\/title><\/head><body text="#000000" bgcolor="#FFFFFF"><div onselectstart="return false;" oncopy="return false;">');
  WinPrint.document.writeln('<h1>' + title + '<\/h1>');
  WinPrint.document.writeln(text);
  WinPrint.document.writeln('<hr><div style="font-size:10pt;">Адрес страницы: ' + location + '<\/div>');
  WinPrint.document.writeln('<\/div><\/body><\/html>');
  WinPrint.document.close();
  WinPrint.focus();
  WinPrint.print();
  WinPrint.close();
}