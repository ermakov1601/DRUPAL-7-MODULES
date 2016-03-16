<?php

/**
 * @file
 * Библиотека функций интеграции с yandex market (находиться в стадии разработки).
 */

/**
 * Menu callback for XML yandex market.
 */
function site_commerce_yandex_market() {
  $output = '';
  $output .= '<?xml version="1.0" encoding="windows-1251"?>';
  $output .= '<!DOCTYPE yml_catalog SYSTEM "shops.dtd">';
  // Атрибут date элемента < yml_catalog> должен соответствовать дате и времени
  // генерации YML-файла на стороне магазина.
  $output .= '<yml_catalog date="' . date('Y-n-d h:i') . '">';
    $output .= '<shop>';
      $output .= '<name>BestShop</name>';
      $output .= '<company>Best online seller Inc.</company>';
      $output .= '<url>http://best.seller.ru/</url>';
      $output .= '<platform>CMS</platform>';
      $output .= '<version>2.3</version>';
      $output .= '<agency>Agency</agency>';
      $output .= '<email>CMS@CMS.ru</email>';

      //Элемент <currencies> задает список курсов валют магазина.
      // Каждая из валют описывается отдельным элементом <currency>.
      $output .= '<currencies>';
        $output .= '<currency id="RUB" rate="1"/>';
      $output .= '</currencies>';

      // В элементе <categories> содержится список категорий магазина.
      // Каждая категория описывается отдельным элементом <category>.
      $output .= '<categories>';
        $output .= site_commerce_yandex_market_categories();
      $output .= '</categories>';

      // В элементе <local_delivery_cost> указывается общая стоимость доставки для своего региона.
      $output .= '<local_delivery_cost></local_delivery_cost>';

      // В элементе <offers> содержится список товарных предложений магазинов.
      // Каждое товарное предложение описывается отдельным элементом <offer>.
      $output .= '<offers>';

      $output .= '</offers>';
    $output .= '</shop>';
  $output .= '</yml_catalog>';

  drupal_add_http_header('Content-Type', 'text/xml; utf-8');
  print $output;
  exit;

  //return $output;
}

/**
 * Перечень всех категорий.
 */
function site_commerce_yandex_market_categories() {
  $tree = taxonomy_get_tree(variable_get('site_commerce_vocabulary'));
  $output = '';
  foreach ($tree as $term) {
    if ($term->parents[0]) {
      $output .= '<category id="' . $term->tid . '" parentId="' . $term->parents[0] . '">' . $term->name . '</category>';
    }
    else {
      $output .= '<category id="' . $term->tid . '">' . $term->name . '</category>';
    }

  }
  $output = iconv("utf-8","cp1251", $output);
  return $output;
}

/**
 * Перечень всех позиций.
 */
function site_commerce_yandex_market_offers() {
  $output = '';

  $result = db_query('SELECT n.pid FROM {site_commerce} n WHERE n.disabled = :disabled', array(':disabled' => 0));
  foreach ($result as $row) {
  $position = site_commerce_load($row->pid);

  $output .= '<offer id="' . $row->pid . '" type="vendor.model" available="true">';
    $output .= '<url>http://best.seller.ru/product_page.asp?pid=12344</url>';

  $output .= '</offer>';

  }
  return $output;
}