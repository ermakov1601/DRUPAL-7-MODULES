<?php

/**
 * @file
 * Библиотека функций обновления адресов позиций в каталоге.
 */

/**
 * Menu callback -- ask for confirmation of product update url alias.
 */
function site_commerce_update_url_alias_confirm($form, &$form_state) {
  return confirm_form($form,
    t('Update url alias for all positions'),
    'admin/content/site-commerce',
    t('Are you sure you want to update all url alias the positions? Recovery will be impossible.'),
    t('Update'),
    t('Cancel')
  );
}

/**
 * Execute product update url alias.
 */
function site_commerce_update_url_alias_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    // Обновляем текушие значения алиасов категорий позиций.
    site_commerce_taxonomy_url_alias_update();

    $query = db_select('site_commerce', 'n');
    $query->fields('n', array('pid'));
    $result = $query->execute();
    foreach ($result as $row) {
      $operations[] = array('site_commerce_update_url_alias_batch', array($row->pid));
    }

    $batch = array(
      // Массив операций и их параметров
      'operations' => $operations,
      // Функция, которая будет выполнена после окончания всех операций
      'finished' => 'site_commerce_update_url_alias_batch_finished',
      // Заголовок страницы с прогресс баром.
      // Опционально, по умолчанию t('Processing')
      'title' => t('Processing'),
      // Сообщение, показываемое при инициализации.
      // Опционально, по умолчанию t('Initializing.')
      'init_message' => t('Initializing.'),
      // Сообщение, показывемое при выполнении операций.
      // Опционально, по умолчанию t('Completed @current of @total.')
      'progress_message' => t('Completed @current of @total.'),
      // Сообщение показываемое при ошибке выполнения операции.
      // Опционально, по умолчанию t('An error has occurred.')
      'error_message' => t('An error has occurred.'),
      'file' => drupal_get_path('module', 'site_commerce') . '/site_commerce.update.url.alias.inc',
    );

    batch_set($batch);

  }
}

/**
 * Обновление.
 */
function site_commerce_update_url_alias_batch($pid, &$context) {
  $site_commerce = site_commerce_load($pid);
  site_commerce_alias($site_commerce, 'update');

  // Сообщение выводимое под прогресс баром после окончания текущей операции.
  $context['message'] = $site_commerce->path['alias'];
}

/**
 * Выполняем функцию по окончанию удаления позиций.
 */
function site_commerce_update_url_alias_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('All url aliases available positions have been successfully updated.'));
  }
  else {
    drupal_set_message(t('The update url alias positions is completed with errors.'), 'error');
  }
  drupal_goto('admin/content/site-commerce');
}