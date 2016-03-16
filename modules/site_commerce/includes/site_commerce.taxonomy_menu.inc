<?php

/**
 * @file
 * Файл интеграции модуля интернет-магазина SiteCommerce с модулем taxonomy_menu.
 *
 * Интеграция необходима для изменения видимости - удаления категорий из меню
 * в случае, если категория не содержит товаров или ее товары запрещено отображать в категории.
*/

/**
 * Implements hook_taxonomy_menu_insert().
 *
 * @param $item
 *  array with the following key/value pairs:
 *   'tid' => the term id (if 0 then updating the vocab as an item)
 *   'name' => new menu name
 *   'description' => new menu description, used as to build the title attribute
 *   'weight' => new menu weight
 *   'vid' => the new vocabulary's id
 *   'ptid' => the new parent tid
 *   'remove' => if this is set to TRUE then the $item is not added as a menu
 *
 * @return $item
 */
function site_commerce_taxonomy_menu_insert(&$item) {
  // Статусы, для которых разрешено отображать позицию.
  $allow_view_statuses = variable_get('site_commerce_catalog_allow_statuses', site_commerce_position_status());
  $count_positions = site_commerce_count_positions($item['tid'], $allow_view_statuses, SITE_COMMERCE_PUBLISHED, TRUE, TRUE);
  if (!$count_positions) {
    $item['remove'] = TRUE;
  }
}

/**
 * Implements hook_taxonomy_menu_update().
 *
 * @param $item
 *  array with the following key/value pairs:
 *   'tid' => the term id (if 0 then updating the vocab as an item)
 *   'name' => new menu name
 *   'description' => new menu description, used as to build the title attribute
 *   'weight' => new menu weight
 *   'vid' => the new vocabulary's id
 *   'ptid' => the new parent tid
 *   'mlid' => mlid that needs to be edited
 *   'remove' => if this is set to TRUE then the $item is not added as a menu
 *
 */
function site_commerce_taxonomy_menu_update(&$item) {
  // Статусы, для которых разрешено отображать позицию.
  $allow_view_statuses = variable_get('site_commerce_catalog_allow_statuses', site_commerce_position_status());
  $count_positions = site_commerce_count_positions($item['tid'], $allow_view_statuses, SITE_COMMERCE_PUBLISHED, TRUE, TRUE);
  if (!$count_positions) {
    $item['remove'] = TRUE;
  }
}