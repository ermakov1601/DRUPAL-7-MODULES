<?php
// $Id$

/**
 * @file
 * Библиотека функций привязки позиций друг к другу.
 */

/**
 * Страница администрирования связанных позиций.
 */
function site_commerce_position_index($pid, $tid = NULL) {
  // Считываем текущий поисковый запрос.
  $search_string = '';
  if (isset($_SESSION['site_commerce_search_string'])) {
    $search_string = trim($_SESSION['site_commerce_search_string']);
  }

  $position = site_commerce_position_load($pid);

  $output = '';

  $output .= '<div id="site-commerce-position-index" class="clearfix">';
  $output .= '<div id="site-commerce-position-index-search">';

  $form = drupal_get_form('site_commerce_position_index_search_form', $pid, $tid, $search_string);
  $output .= drupal_render($form);

  $output .= '</div>';

  $output .= '<div id="site-commerce-position-index-current">';
  $output .= '<h1>' . $position->title . '</h1>';
  $output .= theme('site_commerce_images', array('pid' => $pid, 'all' => FALSE, 'style_imagecache_first_image' => 'thumbnail'));
  $output .= '</div>';

  $output .= '</div>';

  $output .= '<div id="site-commerce-position-index" class="clearfix">';

  $output .= '<div id="site-commerce-position-index-results">';
  $output .= '<h2>' . t('The available products') . '</h2>';

  $search_result = "";
  if ($search_string || $tid) {
    $search_result = site_commerce_search_positions($search_string, $tid);
  }
  if ($search_result) {
    foreach ($search_result as $result => $pid_result) {
      $rpid = $pid_result['pid'];
      if ($pid <> $rpid && !site_commerce_position_index_exist($pid, $rpid)) {
        $form = drupal_get_form('site_commerce_position_index_actions_form', $pid, $rpid, FALSE);
        $output .= drupal_render($form);
      }
    }
  }
  $output .= '</div>';

  $output .= '<div id="site-commerce-position-index-exist">';
  $output .= '<h2>' . t('The added products') . '</h2>';
  $output .= site_commerce_position_index_load($pid);
  $output .= '</div>';

  $output .= '</div>';

  unset($_SESSION['site_commerce_search_string']);

  return $output;
}

/**
 * Форма действий.
 */
function site_commerce_position_index_search_form($form, &$form_state, $pid, $tid = NULL, $search_string = NULL) {
  $form = array();
  $form['#attributes'] = array('id' => 'site-commerce-position-index-search-form-' . $pid, 'class' => array('site-commerce-actions-form'));

  $form['search_string'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter your keywords'),
    '#default_value' => $search_string,
    '#required' => FALSE,
    '#size' => 30,
  );

  $form['pid'] = array(
    '#type' => 'value',
    '#value' => $pid,
  );

  $position = site_commerce_position_load($pid);
  $term = taxonomy_term_load($position->tid);
  $form['tid'] = array(
    '#type' => 'select',
    '#title' => t('The category'),
    '#options' => kvantstudio_select_terms($term->vid),
    '#default_value' => isset($tid) ? $tid : '',
    '#required' => FALSE,
  );

  // Now we add our submit button, for submitting the form results.
  //
  // The 'actions' wrapper used here isn't strictly necessary for tabledrag,
  // but is included as a Form API recommended practice.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Callback for site_commerce_search_form.
 */
function site_commerce_position_index_search_form_submit($form, &$form_state) {
  $search_string = $form_state['values']['search_string'];
  $pid = $form_state['values']['pid'];
  $tid = $form_state['values']['tid'];

  // Устанавливаем cookies текущий запрос.
  $_SESSION['site_commerce_search_string'] = $search_string;

  // Выполняем редирект.
  $form_state['redirect'] = 'admin/content/site-commerce/position-index/' . $pid . '/' . $tid;
}

/**
* Поиск позиций по заданным ключевым словам или категории.
*/
function site_commerce_search_positions($search_string = NULL, $tid = NULL) {
  $find = array();

  $position = new stdClass();

  $string_array_keys = kvantstudio_search_request_prepare($search_string);

  $matches = array();
  $matches_id = array();

  $string_array = explode(" ", $string_array_keys);
  $request = '';
  $arguments = array();
  $i = 1;
  $count_array_string = count($string_array);
  foreach ($string_array as $key) {
    $arguments[':key' . $i] = $key;
    if ($count_array_string > $i) {
      $request .= 'INSTR(i.data, :key' . $i . ') > 0 AND ';
    }
    else {
      $request .= 'INSTR(i.data, :key' . $i . ') > 0';
    }
    $i++;
  }

  // Поисковый запрос.
  $query = db_select('search_dataset', 'i');
  $query->fields('i', array('sid'));
  $query->join('site_commerce', 's', 's.pid = i.sid');

  if ($search_string <> "") {
    $query->where($request, $arguments);
  }

  if ($tid) {
    $query->condition('s.tid', $tid);
  }
  $query->orderBy('s.title', 'ASC');

  $results = $query->execute();

  foreach ($results as $row) {
    if (!array_key_exists($row->sid, $matches_id)) {
      $find[] = array(
        'pid' => $row->sid,
      );
      $matches_id[$row->sid] = $row->sid;
    }
  }

  return $find;
}