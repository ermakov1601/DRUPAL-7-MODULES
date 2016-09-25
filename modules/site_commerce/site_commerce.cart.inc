<?php

/**
 * @file
 * Файл управления корзиной модуля интернет-магазина SiteCommerce.
 */

/**
 * Формирует содержимое корзины.
 */
function site_commerce_cart() {
  // Запрет на индексацию страницы.
  $data = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array('name' => 'robots', 'content' => 'noindex, nofollow'),
  );
  drupal_add_html_head($data, 'site-commerce-cart');

  // Формируем breadcrumb ссылки.
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), '<front>');
  $breadcrumb[] = l(variable_get('site_commerce_catalog_title', 'Online store'), variable_get('site_commerce_catalog_link', 'tovary'));
  drupal_set_breadcrumb($breadcrumb);

  $output = '';

  // Проверяем разрешена ли регистрация заказов.
  if (user_access('site commerce order create')) {
    // Сообщение для менеджера о запрете просмотра каталога посетителям.
    if (!user_access('site commerce order create') && module_exists('site_commerce_order')) {
      drupal_set_message(variable_get('site_commerce_order_off_description', 'To make out orders for our site it is temporarily forbidden. Technical works are conducted. We apologize for inconveniences.'), "warning");
    }
    // Формируем позиции корзины.
    $output .= theme('site_commerce_cart');
  }
  else {
    $message = variable_get('site_commerce_order_off_description', 'To make out orders for our site it is temporarily forbidden. Technical works are conducted. We apologize for inconveniences.');
    drupal_set_message($message, 'warning');
    drupal_goto(variable_get('site_commerce_catalog_link', 'tovary'));
  }

  return $output;
}

/**
 * Process variables for site-commerce-cart.tpl.php.
 * @see site-commerce-cart.tpl.php.
 */
function template_preprocess_site_commerce_cart(&$variables) {
  // Определяем переменную корзину.
  $variables['cart'] = array(
   '0' => array(), // Были удалены и не оформлены в заказ.
   '1' => array(), // Готовые товары к заказу.
   '2' => array(), // Отложенные товары.
  );

  // Определяем позиции в корзине, которые готовы к заказу.
  // Получаем содержимое корзины.
  $user_id = site_commerce_user_id();
  if (is_numeric($user_id)) {
    $result = db_query("SELECT * FROM {site_commerce_cart} n WHERE n.oid = :order AND n.uid = :uid", array(':order' => 0, ':uid' => $user_id));
  }
  else {
    $result = db_query("SELECT * FROM {site_commerce_cart} n WHERE n.oid = :order AND n.sid = :sid", array(':order' => 0, ':sid' => $user_id));
  }

  // Параметр того, что хотя бы у одной из позиции стоимость равна нулю.
  $is_position_cost_zero = FALSE;

  // Валюта по умолчанию.
  $variables['currency'] = variable_get('site_commerce_default_currency');

  // Итоговая стоимость.
  $variables['cost_result'] = array();

  foreach ($result as $row) {
    // Наименование позиции.
    $position = site_commerce_position_load($row->pid, 'sc_parametr_cart');

    // Количество.
    $variables['cart'][$row->status][$row->cid]['quantity'] = $row->quantity;
    $variables['cart'][$row->status][$row->cid]['min_quantity'] = $position->field_site_commerce_min_quantity;

    // Стоимость товара в корзине.
    $cost = round($row->cost, 2);

    // Выполняем проверку: если цена товара в каталоге изменилась и позиция не входит в спецпредложения, изменяем стоимость в корзине.
    // Предполагаем, что если позицию добавили по спецпредложению, то ее стоимость нельзя изменять для покупателя.
    $variables['cart'][$row->status][$row->cid]['cost_changed'] = FALSE;
    if (variable_get('site_commerce_allow_update_cost_cart', FALSE)) {
      // Определяем текущую основную стоимость товара.
      $cost_current = site_commerce_cost_value($row->pid);

      // Если стоимость не совпадает выполняем обновление стоимости в корзине.
      if ($cost <> $cost_current) {
        $result = db_update('site_commerce_cart')
        ->fields(array(
        'cost' => $cost_current,
        ))
        ->condition('cid', $row->cid)
        ->execute();
        $cost = $cost_current;
        $variables['cart'][$row->status][$row->cid]['cost_changed'] = TRUE;
      }
    }

    // Единица измерения количества.
    $variables['cart'][$row->status][$row->cid]['measure'] = '';
    if ($position->field_site_commerce_measure) {
      $variables['cart'][$row->status][$row->cid]['measure'] = trim($position->field_site_commerce_measure);
    }

    // Определяем стоимость дополнительных опций.
    $cost_result_markup = 0;
    $text_markup = "";
    $user_parametrs = site_commerce_get_user_parametrs($row->pid, FALSE, TRUE, TRUE);
    if ($user_parametrs) {
      $text_markup = $user_parametrs;
      // Итоговая стоимость.
      $cost_result_markup = site_commerce_get_cost_parametrs($row->pid);
    }

    // Общая стоимость с учетом дополнительных опций.
    if ($cost_result_markup > 0) {
      $cost = $cost + $cost_result_markup;
    }

    // Стоимость позиции в корзине в единичном количестве.
    $variables['cart'][$row->status][$row->cid]['cost'] = $cost;

    // Стоимость позиции в корзине с учетом количества.
    $variables['cart'][$row->status][$row->cid]['cost_all'] = round($cost * $row->quantity, 2);

    // Увеличиваем итоговую стоимость.
    if (!isset($variables['cost_result'][$row->status])) {
      $variables['cost_result'][$row->status] = 0;
    }
    $variables['cost_result'][$row->status] = $variables['cost_result'][$row->status] + $variables['cart'][$row->status][$row->cid]['cost_all'];

    // Если у позиции не установлена валюта, устанавливаем валюту, которая по умолчанию на сайте.
    if (empty($position->currency)) {
      $position->currency = variable_get('site_commerce_default_currency');
    }

    // Информация о позиции в корзине.
    $variables['cart'][$row->status][$row->cid]['position'] = $position;

    // Параметр того, что хотя бы у одной из позиции стоимость равна нулю.
    if (site_commerce_cost_value($position) == 0 && $row->status === 1) {
      $is_position_cost_zero = TRUE;
    }
  }

  // Определяем массив данных с результирующей стоимостью.
  $variables['cart_result'] = array();
  if (count($variables['cost_result'])) {
    foreach ($variables['cost_result'] as $status => $cost_result) {
      // Статус позиций в результатах.
      $variables['cart_result'][$status]['status'] = $status;

      // Стоимость с учетом скидки за объем заказанного товара.
      $variables['cart_result'][$status]['cost_discount'] = '';
      $cost_result_discount = site_commerce_order_discount($cost_result);
      if ($cost_result_discount['cost'] <> $cost_result) {
        $discount = $cost_result_discount['discount'];
        $cost_discount = (int) ($cost_result - $cost_result_discount['cost']);
        $cost_result = (int) $cost_result_discount['cost'];

        $variables['cart_result'][$status]['discount'] = $discount;
        $variables['cart_result'][$status]['cost_discount'] = $cost_discount;
      }

      // Примечание.
      $variables['cart_result'][$status]['cost_result_description'] = '';
      if ($cost_result > 0) {
        if ($is_position_cost_zero) {
          $variables['cart_result'][$status]['cost_result_description'] = t('Cost in view of delivery will be certain by our manager after registration of the order. We shall contact you for confirming cost of the order.');
        }
      }
      else {
        $variables['cart_result'][$status]['cost_result_description'] = t('Cost in view of delivery will be certain by our manager after registration of the order. We shall contact you for confirming cost of the order.');
      }

      // Итоговая стоимость.
      $variables['cart_result'][$status]['cost_result'] = $cost_result;
    }
  }

  // Переменная пустой корзины для товаров готовых к заказу.
  if (count($variables['cart'][1])) {
    $variables['cart_empty_class'] = 'element-invisible';
  }
  else {
    $variables['cart_empty_class'] = '';
  }

  // Форма оформления заказа.
  $variables['order_form'] = '';
  if (user_access('site commerce order create') && count($variables['cart'][1])) {
    $form = drupal_get_form('site_commerce_order_form');
    $variables['order_form'] = drupal_render($form);
  }
}

/**
 * Определяем есть ли позиции, которые запрешено помещать в заказ.
 */
function site_commerce_cart_update_ajax_callback($cid, $quantity, $status = 1, $mode = NULL) {
  // Если у посетителя отключён javascript, то показываем ему сообщение.
  if ($mode != 'ajax') {
    drupal_set_message(t('Turn javascript in you browser.'));
    drupal_goto('cart');
  }

  $commands = array();

  // Проверка входных параметров.
  if (!is_numeric($cid) || !is_numeric($quantity) || !is_numeric($status)) {
    return array('#type' => 'ajax', '#commands' => $commands);
  }
  if (!$quantity || $quantity < 0) {
    $quantity = 1;
  }

  // Обновление позиции в корзине.
  $result = db_update('site_commerce_cart')
  ->fields(array(
    'quantity' => $quantity,
    'status' => $status,
  ))
  ->condition('cid', $cid)
  ->execute();

  $result = db_query("SELECT * FROM {site_commerce_cart} n WHERE n.cid = :cid", array(':cid' => $cid));
  foreach ($result as $row) {
    // Стоимость товара в корзине.
    $cost = round($row->cost, 2);

    // Определяем стоимость дополнительных опций.
    $cost_result_markup = 0;
    $user_parametrs = site_commerce_get_user_parametrs($row->pid, FALSE, TRUE, TRUE);
    if ($user_parametrs) {
      // Итоговая стоимость.
      $cost_result_markup = site_commerce_get_cost_parametrs($row->pid);
    }

    // Общая стоимость с учетом дополнительных опций.
    if ($cost_result_markup > 0) {
      $cost = $cost + $cost_result_markup;
    }

    $cost_all = round($cost * $row->quantity, 2);
  }

  // Обновляем количество.
  $commands[] = ajax_command_invoke('#quantity-' . $cid, 'val', array($quantity));

  // Скрываем позицию.
  if (!$status) {
    $commands[] = ajax_command_invoke('#site-commerce-cart__item-' . $cid, 'addClass', array('element-invisible'));
    $commands[] = ajax_command_invoke('#site-commerce-cart__item-action-' . $cid, 'addClass', array('element-invisible'));
    $commands[] = ajax_command_invoke('#site-commerce-cart__note-' . $cid, 'addClass', array('element-invisible'));
  }

  // Итоговая стоимость текущей позиции с учётом нового количества.
  $message = '<span id="site-commerce-cart__item-cost-' . $cid . '" class="site-commerce-cart__item-cost">' . $cost_all . '</span>';
  $commands[] = ajax_command_replace('#site-commerce-cart__item-cost-' . $cid, $message);

  // Итоговая стоимость заказа.
  $cost_result = site_commerce_count_position_in_cart(1)->cost;
  $message = '<span class="site-commerce-cart__cost-result">' . site_commerce_cost_format($cost_result) . '</span>';
  $commands[] = ajax_command_replace('.site-commerce-cart__cost-result', $message);

  // Скидка за объем заказанного товара.
  $cost_result_discount = site_commerce_order_discount($cost_result);
  if ($cost_result_discount['cost'] <> $cost_result) {
    $discount = $cost_result_discount['discount'];
    $cost_discount = (int) ($cost_result - $cost_result_discount['cost']);
    // Валюта по умолчанию.
    $currency = variable_get('site_commerce_default_currency');
    $message = t('We give you the discount @discount%', array('@discount' => $discount)) . ' (' . $cost_discount . ' ' . $currency . ')';
    $commands[] = ajax_command_html('.site-commerce-cart__item-discount', $message);
  }

  // Итоговая стоимость заказа.
  $cost_result = $cost_result_discount['cost'];
  $message = '<span class="site-commerce-cart__cost-result">' . $cost_result . '</span>';
  $commands[] = ajax_command_replace('.site-commerce-cart__cost-result', $message);

  // Обновляем виджет корзину.
  $commands[] = ajax_command_html("#block-site-commerce-cart .content", site_commerce_block_cart());

  // Скрываем корзину и форму заказа, если нет позиций в корзине.
  $cart_status_1 = site_commerce_count_position_in_cart(1);
  $cart_status_2 = site_commerce_count_position_in_cart(2);
  if (!$cart_status_1->cost) {
    $commands[] = ajax_command_invoke('.site-commerce-cart__ready', 'addClass', array('element-invisible'));
    $commands[] = ajax_command_invoke('.site-commerce-cart-order-form', 'addClass', array('element-invisible'));
  }
  if (!$cart_status_2->cost) {
    $commands[] = ajax_command_invoke('#site-commerce-cart__postponed', 'addClass', array('element-invisible'));
  }
  if (!$cart_status_1->cost && !$cart_status_2->cost) {
    $commands[] = ajax_command_invoke('.site-commerce-cart__empty', 'removeClass', array('element-invisible'));
  }

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Определяем есть ли позиции, которые запрешено помещать в заказ.
 */
function site_commerce_cart_disabled_positions() {
  // Получаем содержимое корзины.
  $user_id = site_commerce_user_id();
  if (is_numeric($user_id)) {
    $result = db_query("SELECT * FROM {site_commerce_cart} n WHERE n.oid = :order AND n.uid = :uid AND n.status > :status", array(':order' => 0, ':uid' => $user_id, ':status' => 0));
  }
  else {
    $result = db_query("SELECT * FROM {site_commerce_cart} n WHERE n.oid = :order AND n.sid = :sid AND n.status > :status", array(':order' => 0, ':sid' => $user_id, ':status' => 0));
  }

  $disabled_positions = FALSE;

  foreach ($result as $row) {
    // Наименование позиции.
    $position = site_commerce_position_load($row->pid);
    if ($position->disabled && $row->status == 1) {
      // Обновление позиции в корзине.
      // Автоматически переносим ее в отложенные товары.
      $result = db_update('site_commerce_cart')
      ->fields(array(
        'status' => 2,
      ))
      ->condition('cid', $row->cid)
      ->execute();
      $disabled_positions = TRUE;
    }
  }
  return $disabled_positions;
}