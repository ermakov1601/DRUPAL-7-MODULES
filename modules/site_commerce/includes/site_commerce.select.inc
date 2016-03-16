<?php
// $Id$

/**
* @file
* Вспомогательный файл модуля site commerce.
*
* Содержит в себе функции, которые реализуют
* функционал выбора параметров в карточке товара с помощью всплывающего окна на Jquery.
*/

/**
 * Ajax_callback функция вызова диалога выбора параметра.
 */
function site_commerce_select_ajax_callback($pid, $param, $mode = NULL) {
  // Если у посетителя отключён javascript, то показываем ему сообщение.
  if ($mode != 'ajax') {
    drupal_set_message(t('Turn javascript in you browser.'));
    drupal_goto(isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '<front>');
  }

  $commands = array();

  // Отображение всплывающего окна.
  $data = theme('site_commerce_select', array('pid' => $pid, 'param' => $param));

  $machine_name = str_replace('-', '_', $param);
  $query = db_select('site_commerce_parametrs', 'n');
  $query->fields('n', array('vid', 'name'));
  $query->condition('n.machine_name', $machine_name);
  $result = $query->execute();
  foreach ($result as $row) {
    $vid = $row->vid;
    $name = $row->name;
  }

  $vocabulary = taxonomy_vocabulary_load($vid);

  // Определяем текущую ширину экрана браузера.
  $width = $_COOKIE['userWinWidth'] - 100;

  $commands[] = ajax_command_remove('site-commerce-select-element-hidden-' . $param . '-' . $pid . '');
  $commands[] = ajax_command_append('body', '<div id="site-commerce-select-element-hidden-' . $param . '-' . $pid . '" class="element-hidden"><div id="site-commerce-select-' . $param . '-' . $pid . '" class="site-commerce-select"></div></div>');
  $commands[] = ajax_command_html('#site-commerce-select-' . $param . '-' . $pid, $data);
  $commands[] = ajax_command_invoke('#site-commerce-select-' . $param . '-' . $pid, 'dialog', array(
    array('modal' => TRUE),
    array('minWidth' => $width),
    array('title' => t('Select') . ' ' . t($name)),
  ));

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Process variables for site-commerce-select.tpl.php.
 * @see site-commerce-select.tpl.php.
 */
function template_preprocess_site_commerce_select(&$variables) {
  $pid = $variables['pid'];
  $param = $variables['param'];

  $machine_name = str_replace('-', '_', $param);
  $query = db_select('site_commerce_parametrs', 'n');
  $query->fields('n', array('vid'));
  $query->condition('n.machine_name', $machine_name);
  $vid = $query->execute()->fetchField();
  $terms = kvantstudio_select_terms($vid, FALSE, FALSE);
  foreach ($terms as $tid => $name) {
    $variables['terms'][$tid] = $name;
  }
}

/**
 * Ajax_callback функция загрузки категорий в регион.
 */
function site_commerce_select_data_load_ajax_callback($tid, $pid, $param, $mode = NULL) {
  // Если у посетителя отключён javascript, то показываем ему сообщение.
  if ($mode != 'ajax') {
    drupal_set_message(t('Turn javascript in you browser.'));
    drupal_goto(isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '<front>');
  }

  $commands = array();

  $term = taxonomy_term_load($tid);
  $terms = taxonomy_get_tree($term->vid, $tid);

  $data = '<div class="site-commerce-select__category"><span class="site-commerce-select__step site-commerce-select__step_2"></span>' . t('@value', array('@value' => $term->name)). '</div>';

  foreach ($terms as $key) {
    $term = taxonomy_term_load($key->tid);
    // Определяем стиль изображения.
    $style_name = variable_get('site_commerce_imagecache_select', 'sc_select');
    // Путь к изображению категории по умолчанию.
    $uri = site_commerce_default_term_image_uri($term->tid);
    if ($uri) {
      // Вывод фотоизображения.
      $variables = array(
        'style_name' => $style_name,
        'image_path' => $uri,
        'attributes' => array('class' => array('site-commerce-select__category-image')),
      );
      $row_image =  theme('kvantstudio_image', $variables);
    }
    else {
      $row_image = NULL;
    }

    $link = l($row_image, 'site-commerce-select-data-load-tid/' . $term->tid . '/' . $pid . '/' . $param . '/nojs', array('html' => TRUE, 'attributes' => array(
      'class' => array('use-ajax'),
      'title' => t('@value', array('@value' => $term->name)),
    )));

    $data .= $link;
  }

  $commands[] = ajax_command_html('#site-commerce-select-data-' . $param . '-' . $pid, $data);

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Ajax_callback функция загрузки выбранного термина в регион.
 */
function site_commerce_select_data_load_tid_ajax_callback($tid, $pid, $param, $mode = NULL) {
  // Если у посетителя отключён javascript, то показываем ему сообщение.
  if ($mode != 'ajax') {
    drupal_set_message(t('Turn javascript in you browser.'));
    drupal_goto(isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '<front>');
  }

  $commands = array();
  $data = '';

  $term = taxonomy_term_load($tid);

  // Кнопка выбора параметра.
  $image = drupal_get_path('module', 'site_commerce') . '/images/select.png';
  $text = '<img src="' . base_path() . $image . '"><span class="link-js">' . t('To choose') . '</span>';
  $link = l($text, 'site-commerce-select-choose/' . $term->tid . '/' . $pid . '/' . $param . '/nojs', array('html' => TRUE, 'attributes' => array(
    'class' => array('use-ajax'),
    'title' => t('To choose') . ': ' . t('@value', array('@value' => $term->name)),
  )));
  $data .= '<div class="site-commerce-select-choose-link"><span class="site-commerce-select__step site-commerce-select__step_3"></span>' . $link . '</div>';

  // Определяем заголовок.
  $data .= '<div class="site-commerce-select-title">' . t('@value', array('@value' => $term->name)) . '</div>';

  // Определяем стиль изображения.
  // Вывод фотоизображения.
  $image_style_url = NULL;
  $style_name = variable_get('site_commerce_imagecache_select_load', 'sc_select_load');
  if ($uri = site_commerce_default_term_image_uri($term->tid)) {
    $image_url = file_create_url($uri);
    $image_style_url = image_style_url($style_name, $uri);
  }
  $data .= '<div class="site-commerce-select-image"><img class="site-commerce-select__image" src="' . $image_style_url . '" alt=""></div>';

  // Системное имя параметра.
  $machine_name = str_replace('-', '_', $param);

  // Формируем стоимость.
  // Определяем стоимость текущего термина.
  $field = field_get_items('taxonomy_term', $term, 'field_site_commerce_cost');
  $field = field_view_value('taxonomy_term', $term, 'field_site_commerce_cost', $field[0]);
  $cost = str_replace(' ', '', $field['#markup']);

  // Если стоимость текущего термина равна 0 пытаемся получить стоимость с родителя.
  if ($cost == '0.00') {
    $parents = taxonomy_get_parents($tid);
    $parent = reset($parents);
    $field = field_get_items('taxonomy_term', $parent, 'field_site_commerce_cost');
    $field = field_view_value('taxonomy_term', $parent, 'field_site_commerce_cost', $field[0]);
    $cost = str_replace(' ', '', $field['#markup']);
  }
  if ($cost <> '0.00') {
    $data .= '<div class="site-commerce-select-cost">' . t('Cost') . ' - ' . site_commerce_cost_format($cost) . '&nbsp;' . variable_get('site_commerce_default_currency') . '</div>';
  }
  if ($cost == '0.00' && !variable_get($machine_name . '_allow_arithmetic', TRUE)) {
    $cost = 0;
  }

  // Итоговая стоимость.
  $cost_result_markup = site_commerce_get_cost_parametrs($pid, 0, $cost, $machine_name);
  $cost_result_markup = round($cost_result_markup, 2) + site_commerce_cost_value($pid);

  $data .= '<div class="site-commerce-select-cost">' . t('Final cost') . ' - ' . $cost_result_markup . '&nbsp;' . variable_get('site_commerce_default_currency') . '</div>';

  // Описание параметра.
  if (!$term->description) {
    $term->description = '<span class="site-commerce-select-none-description">' . t('A detailed description is not available.') . '</span>';
  }
  $data .= '<div class="site-commerce-select-description">' . $term->description . '</div>';

  $commands[] = ajax_command_html('#site-commerce-select-data-more-' . $param . '-' . $pid, $data);

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Ajax_callback функция закрытия формы выбора.
 */
function site_commerce_select_choose_ajax_callback($tid, $pid, $param, $mode = NULL) {
  // Если у посетителя отключён javascript, то показываем ему сообщение.
  if ($mode != 'ajax') {
    drupal_set_message(t('Turn javascript in you browser.'));
    drupal_goto(isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '<front>');
  }

  $commands = array();

  $term = taxonomy_term_load($tid);

  // Системное имя параметра.
  $machine_name = str_replace('-', '_', $param);

  // Формируем стоимость.
  // Определяем стоимость текущего термина.
  $field = field_get_items('taxonomy_term', $term, 'field_site_commerce_cost');
  $field = field_view_value('taxonomy_term', $term, 'field_site_commerce_cost', $field[0]);
  $cost = str_replace(' ', '', $field['#markup']);

  // Если стоимость текущего термина равна 0 пытаемся получить стоимость с родителя.
  if ($cost == '0.00') {
    $parents = taxonomy_get_parents($tid);
    $parent = reset($parents);
    $field = field_get_items('taxonomy_term', $parent, 'field_site_commerce_cost');
    $field = field_view_value('taxonomy_term', $parent, 'field_site_commerce_cost', $field[0]);
    $cost = str_replace(' ', '', $field['#markup']);
  }

  // Регистрируем параметр.
  site_commerce_add_cart_parametr($pid, $tid, $machine_name, $cost);

  // Название текущего параметра.
  $query = db_select('site_commerce_parametrs', 'n');
  $query->fields('n', array('name'));
  $query->condition('n.machine_name', $machine_name);
  $name = $query->execute()->fetchField();

  // Единица измерения стоимости по умолчанию на сайте.
  $currency = variable_get('site_commerce_default_currency');

  // Итоговая стоимость.
  $cost_result_markup = site_commerce_get_cost_parametrs($pid);
  $cost_result_markup = round($cost_result_markup, 2) + site_commerce_cost_value($pid);

  // Отображаем итоговую стоимость.
  if ($cost_result_markup) {
    $commands[] = ajax_command_html('#site-commerce-total-cost-value-' . $pid, $cost_result_markup . " " . $currency);
    $commands[] = ajax_command_invoke('#site-commerce-total-cost-' . $pid, 'removeClass', array('element-invisible'));
  }

  // Формируем описание выбранного параметра.
  $user_parametrs = site_commerce_get_user_parametrs($pid);
  $commands[] = ajax_command_html('#site-commerce-selected-parametrs-items-' . $pid, $user_parametrs);
  $commands[] = ajax_command_invoke('#site-commerce-selected-parametrs-' . $pid, 'removeClass', array('element-invisible'));

  // Очищаем кэш.
  if (variable_get('site_commerce_catalog_cache', FALSE)) {
    cache_clear_all('site_commerce_category_pid:' . $pid, 'cache_site_commerce');
  }

  // Устанавливаем значение в скрытом элементе формы корзины.
  $commands[] = ajax_command_invoke('#site-commerce-' . $param . '-' . $pid, 'val', array($tid));

  // Закрываем диалог выбора.
  $commands[] = ajax_command_invoke('.site-commerce-select', 'dialog', array("close"));

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Ajax_callback функция удаление выбранного параметра.
 */
function site_commerce_parametr_delete_ajax_callback($cpid, $pid, $mode = NULL) {
  // Если у посетителя отключён javascript, то показываем ему сообщение.
  if ($mode != 'ajax') {
    drupal_set_message(t('Turn javascript in you browser.'));
    drupal_goto(isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '<front>');
  }

  // Удаляем все записи в таблице site_commerce_taxonomy_index.
  db_delete('site_commerce_cart_parametrs')->condition('cpid', $cpid)->execute();

  // Единица измерения стоимости по умолчанию на сайте.
  $currency = variable_get('site_commerce_default_currency');

  // Итоговая стоимость.
  $cost_result_markup = site_commerce_get_cost_parametrs($pid);
  $cost_result_markup = round($cost_result_markup, 2) + site_commerce_cost_value($pid);

  $commands = array();
  $commands[] = ajax_command_remove('#site-commerce-selected-parametr-item-' . $cpid . '');

  // Скрываем отображение формы выбранных параметров, если их нет.
  $user_parametrs = site_commerce_get_user_parametrs($pid);
  if (!$user_parametrs) {
    $commands[] = ajax_command_invoke('#site-commerce-selected-parametrs-' . $pid, 'addClass', array('element-invisible'));
  }

  // Отображаем итоговую стоимость.
  if ($cost_result_markup) {
    $commands[] = ajax_command_html('#site-commerce-total-cost-value-' . $pid, $cost_result_markup . " " . $currency);
    $commands[] = ajax_command_invoke('#site-commerce-total-cost-' . $pid, 'removeClass', array('element-invisible'));
  }

  return array('#type' => 'ajax', '#commands' => $commands);
}