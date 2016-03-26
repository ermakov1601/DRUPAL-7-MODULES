<?php

/**
 * @file
 * Бибилиотека импорта товаров модуля интернет-магазина SiteCommerce.
 *
 * Файл содержит основные функции, которые реализуют импорт товаров из csv файла.
*/

/**
 * Создаёт содержимое страницы импрта товаров.
 */
function site_commerce_import_page() {
	//Подключаем инструменты для чтения Excel-файла и открываем файл.
	require_once drupal_get_path('module', 'kvantstudio') . '/includes/excel_reader2.php';

  drupal_set_title(t('Import images'));

  $form = drupal_get_form('site_commerce_import_form');
  return drupal_render($form);
}

/**
 * Build a form to upload CSV to.
 */
function site_commerce_import_form($form, &$form_state) {
  $form['#attributes'] = array('enctype' => 'multipart/form-data');

  $form['field'] = array(
    '#title' => t('Key field'),
    '#type' => 'select',
    '#default_value' => 0,
    '#description' => t('Select key field.'),
    '#options' => site_commerce_import_keys_fields(),
  );

  $form['file'] = array(
    '#title' => t('Choose file for import'),
    '#type'  => 'file',
    '#description' => ($max_size = parse_size(ini_get('upload_max_filesize'))) ? t('The <strong>maximum upload file size is !max_size</strong>.', array('!max_size' => format_size($max_size))) : '',
  );

  $form['delete'] = array(
    '#title' => t('Delete current image'),
    '#type' => 'checkbox',
    '#default_value' => 0,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('To execute import'),

  );

  $form['#validate'] = array(
    'site_commerce_import_form_validate_fileupload',
    'site_commerce_import_form_validate',
  );

  return $form;
}

/**
 * Validate the file upload. It must be a CSV, and we must
 * successfully save it to our import directory.
 */
function site_commerce_import_form_validate_fileupload($form, &$form_state) {
  $validators = array(
    'file_validate_extensions' => array('xls'),
  ) ;

  if ($file = file_save_upload('file', $validators)) {
    // The file was saved using file_save_upload() and was added to
    // the files table as a temporary file. We'll make a copy and let
    // the garbage collector delete the original upload.
    $directory = file_default_scheme() . '://import';
    if (file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
      $destination = $directory . '/' . $file->filename;
      if (file_copy($file, $destination, FILE_EXISTS_REPLACE)) {
        $form_state['values']['file_upload'] = $destination;
        $form_state['values']['filename'] = $file->filename;
      }
      else {
        form_set_error('csvimport', t('Unable to copy upload file to !dest.', array('!dest' => $destination)));
      }
    }
  }
}

/**
 * Validate the upload. Ensure that the CSV looks something like we
 * expect it to.
 */
function site_commerce_import_form_validate($form, &$form_state) {
  if (isset($form_state['values']['file_upload'])) {
    // Подготавливаем директории.
    $directory = file_default_scheme() . '://import_pictures';
    file_prepare_directory($directory, FILE_MODIFY_PERMISSIONS);
    file_unmanaged_delete_recursive($directory);
  	file_prepare_directory($directory, FILE_CREATE_DIRECTORY);

    $directory = file_default_scheme() . '://styles';
    file_prepare_directory($directory, FILE_MODIFY_PERMISSIONS);
    file_unmanaged_delete_recursive($directory);

    $directory = file_default_scheme() . '://pictures/site_commerce';
  	file_prepare_directory($directory, FILE_CREATE_DIRECTORY);

    if (($handle = fopen($form_state['values']['file_upload'], "r")) !== FALSE) {
      fclose($handle);
    }
    else {
      form_set_error('file', t('Unable to read uploaded file !filepath.', array('!filepath' => $form_state['values']['file_upload'])));
    }
  }
}

/**
 * Handle form submission. Read the CSV into a set of batch operations
 * and fire them off.
 */
function site_commerce_import_form_submit($form, &$form_state) {
  $batch = array(
    'title' => t('Import of images is carried out, wait...'),
    'operations' => array(),
    'init_message' => t('Preparation of data'),
    'progress_message' => t('It is executed @current from @total.'),
    'error_message' => t('An error occurred during processing.'),
    'finished' => 'site_commerce_import_finished',
    'file' => drupal_get_path('module', 'site_commerce') . '/site_commerce.import.images.inc',
  );

  setlocale(LC_ALL, 'ru_RU.UTF-8');

  if (isset($form_state['values']['file_upload'])) {
    if (($handle = fopen($form_state['values']['file_upload'], "r")) !== FALSE) {
      $batch['operations'][] = array('site_commerce_import_filename', array($form_state['values']['filename']));

      $excel_reader = new Spreadsheet_Excel_Reader($form_state['values']['file_upload'], FALSE);

      //Количество строк.
		  $xlsrcnt = $excel_reader->sheets[0]['numRows'] + 1;

      $data = array();

      for($i = 1; $i < $xlsrcnt; $i++) {

        if (!in_array($excel_reader->val($i, 1), site_commerce_import_keys_fields())) {
          // Формируем данные.
          $data['field'] = $form_state['values']['field'];
          $data['value'] = $excel_reader->val($i, 1);
          $data['url'] = $excel_reader->val($i, 2);
          $data['title_suffix'] = $excel_reader->val($i, 3);
          $data['delete'] = $form_state['values']['delete'];

          $batch['operations'][] = array('site_commerce_import_data', array(array_map('base64_encode', $data)));
        }
      }
      fclose($handle);
    }
  }

  // Запуск batch.
  batch_set($batch);
}

/**
 * Remember the uploaded CSV filename.
 *
 * @TODO is there a better way to pass a value from inception of the
 * batch to the finished function?
 */
function site_commerce_import_filename($filename, &$context) {
  $context['results']['uploaded_filename'] = $filename;
}

/**
 * Process a single line.
 */
function site_commerce_import_data($data, &$context) {
  module_load_include('inc', 'pathauto');

  if (isset($context['results']['rows_imported'])) {
    $context['results']['rows_imported'] = $context['results']['rows_imported'] + 1;
  }
  else {
    $context['results']['rows_imported'] = 1;
  }

  $data = array_map('base64_decode', $data);

  site_commerce_import_save($data);

  if ($data['title']) {
    $context['message'] = t('The images has been added for position: <b>%title</b>.', array('%title' => $data['title']));
  }
  else {
    $context['message'] = t('The key <b>%title</b> not found.', array('%title' => trim($data['value'])));
    watchdog('site_commerce_import_images', t('The key <b>%title</b> not found.', array('%title' => trim($data['value']))), NULL, WATCHDOG_WARNING);
    drupal_set_message(t('The key <b>%title</b> not found.', array('%title' => trim($data['value']))), 'warning');
  }

  /**
   * In order to slow importing and debug better, we can uncomment
   * this line to make each import slightly slower.
   */
  //usleep(2500);
}

/**
 * Сохраняем данные в БД.
 */
function site_commerce_import_save(&$data) {
  global $user;

  // Описание массива, строки входных данных передаваемых из csv файла.
  $field = $data['field']; // Ключевое поле.
  $value = trim($data['value']); // Идентификатор поля.
  $url = trim($data['url']); // URL файла.
  $title_suffix = trim($data['title_suffix']);
  $delete = (int) $data['delete']; // Удалить все файлы.

  // Идентификатор по умолчанию.
  if ($field == 'pid') {
    $pid = db_query("SELECT n.pid FROM {site_commerce} n WHERE n.pid = :value", array(':value' => (int) $value))->fetchField();
  }

  // Если поиск по артикулу.
  if ($field == 'code') {
    $pid = db_query("SELECT n.entity_id FROM {field_data_field_site_commerce_code} n WHERE n.field_site_commerce_code_value = :value", array(':value' => $value))->fetchField();
  }

  // Если поиск по oem.
  if ($field == 'oem') {
    $pid = db_query("SELECT n.entity_id FROM {field_data_field_site_commerce_oem} n WHERE n.field_site_commerce_oem_value = :value", array(':value' => $value))->fetchField();
  }

  // Загружаем товар.
  $site_commerce = new stdClass();
  $site_commerce->pid = 0;
  if ($pid && is_numeric($pid)) {
    $site_commerce = site_commerce_load($pid);
  }

  $data['title'] = "";

  if ($site_commerce->pid) {
    // Удаление файлов.
    if ($delete) {
      // Получаем файлы изображений.
      $query = db_select('file_usage', 'f');
      $query->innerJoin('file_managed', 'm', 'f.fid = m.fid');
      $query->condition('f.module', 'file');
      $query->condition('f.type', 'site_commerce');
      $query->condition('f.id', $site_commerce->pid);
      $query->condition('m.filemime', array('image/gif', 'image/png', 'image/jpg', 'image/jpeg'), 'IN');
      $query->fields('m', array('fid'));
      $result_file_images = $query->execute();

      foreach ($result_file_images as $file_image) {
        $file = file_load($file_image->fid);
        file_delete($file, TRUE);
        db_delete('field_data_field_site_commerce_image')
          ->condition('entity_id', $file_image->fid)
          ->execute();
        db_delete('field_revision_field_site_commerce_image')
          ->condition('entity_id', $file_image->fid)
          ->execute();
      }
    }

    // Формирует новое имя файла.
    if ($title_suffix) {
      $title_suffix = ' ' . $title_suffix;
    }
    $filename = $site_commerce->title . $title_suffix . '-' . $pid . '-' . kvantstudio_random_string(5);

    // Имя файла для подсистемы автозапчастей.
    if ($field == 'oem') {
      $filename = $field . '-' . $value . '-articul-' . $site_commerce->pid . '-random-' . kvantstudio_random_string(5);
    }

    $filename = mb_strtolower($filename, "UTF-8");
    $filename = pathauto_cleanstring($filename) . '.' . kvantstudio_get_extension($url);

    // Импортирует файл с удалённого сервера во временную папку.
    $destination = 'sites/default/files/import_pictures/' . $filename;

    // Подготавливаем директорию для импорта.
    $directory = file_default_scheme() . '://import_pictures';
    if (file_prepare_directory($directory, FILE_MODIFY_PERMISSIONS)) {
      // Импортируем файл.
      site_commerce_save_image($url, $destination);

      // Привязка изображения к позиции по идентификатору.
      // Проверяем существует ли такой файл в основной папке.
      $exist = 'sites/default/files/pictures/site_commerce/' . $filename;
      if (!file_exists($exist)) {
        $file = new stdClass();
        $file->uri = $destination;

        $directory = file_default_scheme() . '://pictures/site_commerce';
        if (file_prepare_directory($directory, FILE_MODIFY_PERMISSIONS)) {
          $destination = $directory . '/' . $filename;
          $file->uid  = $user->uid;
          $file = file_copy($file, $destination);
          $file->status = FILE_STATUS_PERMANENT;
          $file->filemime = file_get_mimetype($file->uri);
          file_save($file);
          $count = count($site_commerce->field_site_commerce_image[LANGUAGE_NONE]);
          $site_commerce->field_site_commerce_image[LANGUAGE_NONE][$count]['fid'] = $file->fid;

          $text_alt = kvantstudio_summary($site_commerce->title . $title_suffix, 512);
          $text_title = kvantstudio_summary($site_commerce->title . $title_suffix, 1024);
          $site_commerce->field_site_commerce_image[LANGUAGE_NONE][$count]['alt'] = $text_alt;
          $site_commerce->field_site_commerce_image[LANGUAGE_NONE][$count]['title'] = $text_title;
        }
      }

      // Сохранение позиции.
      site_commerce_save($site_commerce);
      $data['title'] = $site_commerce->title;
    }
  }
}

/**
 * Функция сохраняет изображение средствами cURL,
 * когда опция allow_url_fopen отключена.
 */
function site_commerce_save_image($url, $destination) {
  $ch = curl_init($url);
  $fp = fopen($destination, 'wb');
  curl_setopt($ch, CURLOPT_FILE, $fp);
  curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_exec($ch);
  curl_close($ch);
  fclose($fp);
}

/**
 * Handle batch completion.
 */
function site_commerce_import_finished($success, $results, $operations) {
  if ($success) {
    if (isset($results['uploaded_filename'])) {
      // Очищаем директории.
      $directory = file_default_scheme() . '://import_pictures';
      file_prepare_directory($directory, FILE_MODIFY_PERMISSIONS);
      file_unmanaged_delete_recursive($directory);
    	file_prepare_directory($directory, FILE_CREATE_DIRECTORY);

      $directory = file_default_scheme() . '://styles';
      file_prepare_directory($directory, FILE_MODIFY_PERMISSIONS);
      file_unmanaged_delete_recursive($directory);

      drupal_set_message(t('Import of a file <strong>@file</strong> is completed.', array('@file' => $results['uploaded_filename'])));
    }
    else {
      drupal_set_message(t('File not loaded. Please select a file for uploading.'), 'warning');
    }
  }
  else {
    drupal_set_message(t('Import file <strong>@file</strong> completed with errors.', array('@file' => $results['uploaded_filename'])), 'error');
  }
  drupal_goto('admin/content/site-commerce');
}