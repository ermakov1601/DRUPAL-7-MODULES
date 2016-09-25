/**
* @file
* Файл библиотеки javascript модуля site commerce.
*/
var $jQuery = jQuery.noConflict();
$jQuery(document).ready(function() {
  (function($) {
    // Относительный путь.
    var basePath = Drupal.settings.basePath;

    // Реализация поведения главного каталога товаров при наведении мыши.
    // Выполняем только для размеров экрана от 640 px.
    if ($.isFunction($().equalHeights) && Drupal.settings.kvantstudio_allow_equalheights) {
      if($(window).width() >= 640) {
        $(".site-commerce-categories__item").equalHeights();
      }
    }
    if (Drupal.settings.site_commerce_catalog_overlay) {
      if($(window).width() >= 640) {
        $(".site-commerce-categories__item").hover(
          function() {
            $(this).find(".site-commerce-category__item_childrens").css({"opacity":1});
          },
          function() {
            $(this).find(".site-commerce-category__item_childrens").css({"opacity":0});
          }
        );
      }
    }

    // Функции обновления количества в корзине.
    $("input[type='number']").change(function() {
      var сid = $(this).attr("сid");
      var quantity = parseInt($("#quantity-" + сid).val());
      var cart_value = parseInt($("#quantity-" + сid).attr("cart_value"));
      var cart_min_value = parseInt($("#quantity-" + сid).attr("min"));

      if (quantity <= 0) {
        $("#quantity-" + сid).val(cart_value);
      }

      if (quantity < cart_min_value) {
        $("#quantity-" + сid).val(cart_min_value);
        quantity = cart_min_value;
      }

      if (quantity > 0) {
        // Выполняет запрос ajax.
        var ajax = new Drupal.ajax(false, false, {url : basePath + 'cart/update/'+ сid + '/' + quantity + '/1/nojs'});
        ajax.eventResponse(ajax, {});
      }
    });

    // Функция проверки маски ввода телефона для формы покупки в один клик.
    if ($.isFunction($().mask)) {
      $(".site-commerce-by-to-1-click-form-item-phone").mask("8 (999) 999-9999");
    }

    /* Инициализируем параметр - ширина экрана. */
    if ($.isFunction($.cookie)) {
      $.cookie('userWinWidth', null);
      $.cookie('userWinWidth', $(window).width(), {
        expires: 1,
        path: basePath,
        secure: false
      });
      $(window).resize(function() {
        $.cookie('userWinWidth', null);
        $.cookie('userWinWidth', $(window).width(), {
          expires: 1,
          path: basePath,
          secure: false
        });
      });
    }

    if ($.isFunction($().on)) {
      // Функции удаления из корзины.
      $("body").on("click", "a.site-commerce-cart__link_delete", function() {
        var сid = $(this).attr("сid");
        var quantity = parseInt($("#quantity-" + сid).val());
        // Выполняет запрос ajax.
        var ajax = new Drupal.ajax(false, false, {url : basePath + 'cart/update/'+ сid + '/' + quantity + '/0/nojs'});
        ajax.eventResponse(ajax, {});
      });

      // Функция закрытия диалогового окна формы отображения добавленой в корзину позиции.
      if ($.isFunction($().dialog)) {
        $("body").on("click", "a.site-commerce-goto-buy", function() {
          var pid = $(this).attr("id");
          $('#site-commerce-goto-order-confirm-' + pid).dialog("close");
        });
        $("body").on("click", ".ui-widget-overlay", function() {
          var pid = $(this).attr("id");
          $('#site-commerce-goto-order-confirm-' + pid).dialog("close");
        });
      }

      // Отображение полного описания в карточке товара.
      // Применяется для ссылки подробнее в шаблоне site_commerce.tpl.php
      $("body").on("click", "a.site-commerce-content__switcher-link_detail", function() {
        $('.site-commerce-content__body').show('slow', function() {
          $('.site-commerce-content__body').removeClass('element-invisible');
          $('.site-commerce-content__switcher-link_briefly').removeClass('site-commerce-content__switcher-link_active');

          $('.site-commerce-content__body-title').addClass('element-invisible');
          $('.site-commerce-content__switcher-link_detail').addClass('site-commerce-content__switcher-link_active');
        });
      });

      // Применяется для ссылки кратко в шаблоне site_commerce.tpl.php
      $("body").on("click", "a.site-commerce-content__switcher-link_briefly", function() {
        $('.site-commerce-content__body').show('slow', function() {
          $('.site-commerce-content__body').addClass('element-invisible');
          $('.site-commerce-content__switcher-link_briefly').addClass('site-commerce-content__switcher-link_active');

          $('.site-commerce-content__body-title').removeClass('element-invisible');
          $('.site-commerce-content__switcher-link_detail').removeClass('site-commerce-content__switcher-link_active');
        });
      });
    }
  }(jQuery));
});