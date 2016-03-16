<?php

/**
 * @file
 * Файл модуля интернет-магазина SiteCommerce.
 *
 * Определяет функции управления заказами магазина.
*/

/**
 * Формирует объект заказа со всеми возможными полями и значениями по умолчанию.
 * @return object order.
 */
function site_commerce_orders() {

  $result = db_query("SELECT n.oid, n.number, n.password, n.created FROM {site_commerce_orders} n ORDER BY n.oid DESC");
  $rows = array();
  foreach ($result as $row) {
    $rows[] = array(
      'data' => array(
        array('data' => $row->oid),
        array('data' => $row->number),
        array('data' => format_date($row->created)),
        array('data' => l(t('view'), variable_get('site_commerce_catalog_link', 'tovary') . '/orders/item/' . $row->number . '/' . $row->password, array('attributes' => array('target' => '_blank')))),
      ),
      'class' => array('site-commerce-order-row'),
    );
  }

  $header = array(
    array('data' => 'id'),
    array('data' => t('Number of the order')),
    array('data' => t('Created')),
    array('data' => t('View')),

  );
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('site-commerce-orders'))));

  return $output;
}