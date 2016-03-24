<?php

/**
 * Импортируем товары.
 */
function site_commerce_auto_set_taxonomy_page() {
  $form = drupal_get_form('site_commerce_auto_set_taxonomy_form');
  return drupal_render($form);
}

/**
 * Build a form to upload CSV to.
 */
function site_commerce_auto_set_taxonomy_form($form, &$form_state) {
  $form['#attributes'] = array('enctype' => 'multipart/form-data');

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update the categories of positions'),
  );

  return $form;
}

/**
 * Handle form submission. Read the CSV into a set of batch operations
 * and fire them off.
 */
function site_commerce_auto_set_taxonomy_form_submit($form, &$form_state) {
  $batch = array(
    'title' => t('The update is in progress, please wait...'),
    'operations' => array(),
    'init_message' => t('Preparation of data'),
    'progress_message' => t('It is executed @current from @total.'),
    'error_message' => t('An error occurred during processing.'),
    'finished' => 'site_commerce_auto_set_taxonomy_data_finished',
    'file' => drupal_get_path('module', 'site_commerce_auto') . '/site_commerce_auto.taxonomy.inc',
  );

  // Перечень всех позиций.
  $result = db_query("SELECT n.pid, n.title FROM {site_commerce} n");
  foreach ($result as $row) {
    $data['pid'] = $row->pid;
    $data['title'] = $row->title;
    $batch['operations'][] = array('site_commerce_auto_set_taxonomy_data', array(array_map('base64_encode', $data)));
  }

  batch_set($batch);
}

/**
 * Process a single line.
 */
function site_commerce_auto_set_taxonomy_data($data, &$context) {
  if(isset($context['results']['rows_imported'])) {
    $context['results']['rows_imported'] = $context['results']['rows_imported'] + 1;
  }
  else {
    $context['results']['rows_imported'] = 1;
  }

  $data = array_map('base64_decode', $data);

  site_commerce_auto_set_taxonomy_data_save($data);

  /**
   * Alternatively, our example CSV happens to have the title in the
   * third column, so we can uncomment this line to display "Importing
   * Blahblah" as each row is parsed.
   *
   * You can comment out the line above if you uncomment this one.
   */

  $context['message'] = $data['title'];

  /**
   * In order to slow importing and debug better, we can uncomment
   * this line to make each import slightly slower.
   */
  //usleep(100);
}

/**
 * Handle batch completion.
 */
function site_commerce_auto_set_taxonomy_data_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('Update categories of positions successfully completed.'));
  }
  else {
    drupal_set_message(t('Update categories of positions completed with errors.'), 'error');
  }
  drupal_goto('admin/content/site-commerce');
}

/**
 * Сохраняем данные в БД.
 */
function site_commerce_auto_set_taxonomy_data_save($data) {
  // Назначаем категорию каталога для позиции.
  $field_site_commerce_auto_term = site_commerce_auto_set_tid($data['title']);

  // Загружаем базовый тип.
  $type = site_commerce_type_get_type('site_commerce_auto');

  // Создаем обьект позиции.
  $site_commerce = new stdClass();
  $site_commerce = site_commerce_load($data['pid']);

  $field_name = 'field_category_' . $type->categories_vocabulary;

  $site_commerce->$field_name = array(
    LANGUAGE_NONE => array(
      '0' => array(
        'tid' => $field_site_commerce_auto_term,
      ),
    ),
  );

  // Сохранение в БД.
  site_commerce_save($site_commerce);

  return $field_site_commerce_auto_term;
}