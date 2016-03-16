/**
* @file
* Файл библиотеки javascript модуля site_send_messages.
*/
var $jQuery = jQuery.noConflict();
$jQuery(document).ready(function() {
  (function($) {
    // Инициализация библиотеки mask.
    if ($.isFunction($().mask)) {
      $(".site-send-messages-phone").mask("8 (999) 999-9999");
      $(".site-send-messages-date").mask("99.99.2012");
      $(".site-send-messages-time").mask("99:99");
    }

    // Инициализация библиотеки colorbox.
    if ($.isFunction($().colorbox)) {
      $(".site-send-messages-call-button").colorbox({
        inline: true,
        scrolling: false,
        opacity: 	0.5,
        speed: 	100,
        transition: 	"none",
      });
      $.fn.siteSendMessagesCallFormResize = function() {
        $(".site-send-messages-call-button").colorbox.resize({});
      };
      $(".site-send-messages-request-button").colorbox({
        inline: true,
        scrolling: false,
        opacity: 	0.5,
        speed: 	100,
        transition: 	"none",
      });
      $.fn.siteSendMessagesFormResize = function() {
        $(".site-send-messages-request-button").colorbox.resize({});
      };
    }

    // Инициализация функций для jquery-ui-timepicker-addon.js.
    if ($.isFunction($().datepicker) || $.isFunction($().timepicker)) {
      if($(".datepicker").length) {
        $.datepicker.regional['ru'] = {
        	closeText: 'Закрыть',
        	prevText: '<Пред',
        	nextText: 'След>',
        	currentText: 'Сегодня',
        	monthNames: ['Январь','Февраль','Март','Апрель','Май','Июнь',
        	'Июль','Август','Сентябрь','Октябрь','Ноябрь','Декабрь'],
        	monthNamesShort: ['Янв','Фев','Мар','Апр','Май','Июн','Июл','Авг','Сен','Окт','Ноя','Дек'],
        	dayNames: ['воскресенье','понедельник','вторник','среда','четверг','пятница','суббота'],
        	dayNamesShort: ['вск','пнд','втр','срд','чтв','птн','сбт'],
        	dayNamesMin: ['Вс','Пн','Вт','Ср','Чт','Пт','Сб'],
        	weekHeader: 'Не',
        	dateFormat: "dd.mm.yy",
        	firstDay: 1,
        	isRTL: false,
        	showMonthAfterYear: false,
        	yearSuffix: '',
          autoSize: true,
          changeMonth: true,
          changeYear: true,
        };
        $.datepicker.setDefaults($.datepicker.regional['ru']);
        $(".datepicker").datepicker({
          dateFormat: "dd.mm.yy",
          autoSize: true,
          changeMonth: true,
          changeYear: true,
        });
      }
      if($(".timepicker").length) {
        $.timepicker.regional['ru'] = {
        	timeOnlyTitle: 'Выберите время',
        	timeText: 'Время',
        	hourText: 'Часы',
        	minuteText: 'Минуты',
        	secondText: 'Секунды',
        	millisecText: 'Миллисекунды',
        	timezoneText: 'Часовой пояс',
        	currentText: 'Сейчас',
        	closeText: 'Закрыть',
        	timeFormat: 'hh:mm tt',
        	amNames: ['AM', 'A'],
        	pmNames: ['PM', 'P'],
        	ampm: false,
        	isRTL: false
        };
        $.timepicker.setDefaults($.timepicker.regional['ru']);
        $('.timepicker').timepicker({});
      }
    }
  }(jQuery));
});