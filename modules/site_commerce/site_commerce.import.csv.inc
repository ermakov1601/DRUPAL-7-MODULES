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
  drupal_set_title(t('Import'));
  $form = drupal_get_form('site_commerce_import_form');
  return drupal_render($form);
}

/**
 * Build a form to upload CSV to.
 */
function site_commerce_import_form($form, &$form_state) {
  $form['#attributes'] = array('enctype' => 'multipart/form-data');

  $form['csvfile'] = array(
    '#title' => t('Choose CSV a file for import'),
    '#type'  => 'file',
    '#description' => ($max_size = parse_size(ini_get('upload_max_filesize'))) ? t('The <strong>maximum upload file size is !max_size</strong>.', array('!max_size' => format_size($max_size))) : '',
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
    'file_validate_extensions' => array('csv'),
  ) ;

  if ($file = file_save_upload('csvfile', $validators)) {
    // The file was saved using file_save_upload() and was added to
    // the files table as a temporary file. We'll make a copy and let
    // the garbage collector delete the original upload.
    $directory = file_default_scheme() . '://import';
    if (file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
      $destination = $directory . '/' . $file->filename;
      if (file_copy($file, $destination, FILE_EXISTS_REPLACE)) {
        $form_state['values']['csvupload'] = $destination;
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
  if (isset($form_state['values']['csvupload'])) {
    if (($handle = fopen($form_state['values']['csvupload'], "r")) !== FALSE) {
      fclose($handle);
    }
    else {
      form_set_error('csvfile', t('Unable to read uploaded file !filepath.', array('!filepath' => $form_state['values']['csvupload'])));
    }
  }
}

/**
 * Handle form submission. Read the CSV into a set of batch operations
 * and fire them off.
 */
function site_commerce_import_form_submit($form, &$form_state) {
  $batch = array(
    'title' => t('Import of positions is carried out, wait...'),
    'operations' => array(),
    'init_message' => t('Preparation of data'),
    'progress_message' => t('It is executed @current from @total.'),
    'error_message' => t('An error occurred during processing.'),
    'finished' => 'site_commerce_import_csv_finished',
    'file' => drupal_get_path('module', 'site_commerce') . '/site_commerce.import.csv.inc',
  );

  setlocale(LC_ALL, 'ru_RU.UTF-8');

  if (isset($form_state['values']['csvupload'])) {
    if (($handle = fopen($form_state['values']['csvupload'], "r")) !== FALSE) {
      // Отключаем видимость всех позиций.
      site_commerce_reset();

      $batch['operations'][] = array('site_commerce_import_csv_filename', array($form_state['values']['filename']));
      while (($data = fgetcsv($handle, 0, ";")) !== FALSE) {
        if ($data[0] <> 'Идентификатор товара на сайте') {
          $batch['operations'][] = array('site_commerce_import_csv_data', array(array_map('base64_encode', $data)));
        }
      }
      fclose($handle);
    }
  }
  batch_set($batch);
}

/**
 * Remember the uploaded CSV filename.
 *
 * @TODO is there a better way to pass a value from inception of the
 * batch to the finished function?
 */
function site_commerce_import_csv_filename($filename, &$context) {
  $context['results']['uploaded_filename'] = $filename;
}

/**
 * Process a single line.
 */
function site_commerce_import_csv_data($data, &$context) {
  if (isset($context['results']['rows_imported'])) {
    $context['results']['rows_imported'] = $context['results']['rows_imported'] + 1;
  }
  else {
    $context['results']['rows_imported'] = 1;
  }

  $data = array_map('base64_decode', $data);

  // Выполняет импорт позиции.
  site_commerce_import_save($data);

  $context['message'] = t('The position has been added %title', array('%title' => $data[9]));
}

/**
 * Handle batch completion.
 */
function site_commerce_import_csv_finished($success, $results, $operations) {
  if ($success) {
    if (isset($results['uploaded_filename'])) {
      $uri = file_default_scheme() . '://styles';
      file_unmanaged_delete_recursive($uri);

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

/**
 * Сохраняем данные в БД.
 */
function site_commerce_import_save($data) {
  // Описание массива, строки входных данных передаваемых из csv файла.
  $pid          = trim($data[0]); // Идентификатор товара на сайте.
  $import_id    = trim($data[1]); // Идентификатор товара в базе 1С или т.п.
  $type         = trim($data[2]); // Тип товара. По умолчанию site_commerce_basic.
  $country      = trim($data[3]); // Название страны производителя товара.
  $manufacture  = trim($data[4]); // Название производителя товара. .
  $brand        = trim($data[5]); // Название торговой марки товара.
  $category     = trim($data[6]); // Название категории товара.
  $contractor   = trim($data[7]); // Название поставщика товара. (В настоящее время не используется).
  $code         = trim($data[8]); // Артикул товара.
  $title        = trim($data[9]); // Название товара.
  $body_summary = trim($data[10]); // Краткое описание товара.
  $body_value   = trim($data[11]); // Полное описание товара.
  $cost_min     = trim($data[12]); // Минимальная стоимость товара.
  $cost         = trim($data[13]); // Стоимость товара.
  $quantity     = trim($data[14]); // Количество на складе.
  $measure      = trim($data[15]); // Единица измерения количества.
  $status       = trim($data[16]); // Статус позиции на складе.
  $sticky       = trim($data[17]); // Признак новинки.
  $condition    = trim($data[18]); // Состояние. (В настоящее время не используется).
  $image        = trim($data[19]); // Признак загрузки изображений.
  $image_delete = trim($data[20]); // Признак удаления изображений.

  // Создаем обьект товара.
  $site_commerce = new stdClass();

  // Язык товара.
  $site_commerce->language = LANGUAGE_NONE;

  // Выполняем проверку, существует ли позиция в базе.
  // Проверку выполняем по идентификатору import_id и типу товара.
  if (!$pid && $import_id) {
    $query = db_select('site_commerce', 'n');
    $query->fields('n', array('pid'));
    $query->condition('n.type', $type);
    $query->condition('n.import_id', $import_id);
    $pid = $query->execute()->fetchField();
  }

  // Если позиция существует.
  if ($pid && is_numeric($pid) && $pid > 0) {
    // Присваиваем идентификатор позиции.
    $site_commerce->pid = $pid;

    // Включаем видимость позиции.
    // До импорта мы все позиции отключали.
    $site_commerce->disabled = 0;

    // Удаляем текущие привязанные фотоизображения к карточке.
    if (is_numeric($image_delete) && $image_delete) {
      $query = db_select('field_data_field_site_commerce_image', 'n');
      $query->fields('n', array('field_site_commerce_image_fid'));
      $query->condition('n.entity_type', 'site_commerce');
      $query->condition('n.bundle', $type);
      $query->condition('n.entity_id', $site_commerce->pid);
      $result = $query->execute();
      foreach ($result as $row) {
        $file = file_load($row->field_site_commerce_image_fid);
        if (is_object($file)) {
          file_delete($file, TRUE);
        }
        db_delete('field_data_field_site_commerce_image')
        ->condition('field_site_commerce_image_fid', $row->field_site_commerce_image_fid)
        ->execute();
      }
    }
  }

  // Идентификатор позиции из базы 1С.
  $site_commerce->import_id = $import_id;

  // Тип позиции.
  $site_commerce->type = $type;

  // Загружаем тип полностью.
  $type = site_commerce_type_get_type($site_commerce->type);

  // Определяем поле - страна.
  if ($tid = site_commerce_term_tid_by_name($country)) {
    $field_name = 'field_country_' . $type->countries_vocabulary;
    $site_commerce->$field_name = array(
      LANGUAGE_NONE => array(
        '0' => array(
          'tid' => $tid,
        ),
      ),
    );
  }

  // Определяем поле - производитель.
  if ($tid = site_commerce_term_tid_by_name($manufacture)) {
    $field_name = 'field_manufacture_' . $type->manufactures_vocabulary;
    $site_commerce->$field_name = array(
      LANGUAGE_NONE => array(
        '0' => array(
          'tid' => $tid,
        ),
      ),
    );
  }

  // Определяем поле - бренд.
  if ($tid = site_commerce_term_tid_by_name($brand)) {
    $field_name = 'field_brand_' . $type->manufactures_vocabulary;
    $site_commerce->$field_name = array(
      LANGUAGE_NONE => array(
        '0' => array(
          'tid' => $tid,
        ),
      ),
    );
  }

  // Определяем поле - поставщик.
  if ($tid = site_commerce_term_tid_by_name($contractor)) {
    $field_name = 'field_category_' . contractors_vocabulary;
    $site_commerce->$field_name = array(
      LANGUAGE_NONE => array(
        '0' => array(
          'tid' => $tid,
        ),
      ),
    );
  }

  // Определяем поле - категория.
  if ($tid = site_commerce_term_tid_by_name($category)) {
    $field_name = 'field_category_' . $type->categories_vocabulary;
    $site_commerce->$field_name = array(
      LANGUAGE_NONE => array(
        '0' => array(
          'tid' => $tid,
        ),
      ),
    );
  }

  // Артикул.
  if (!$code) {
    $code = "";
  }
  $site_commerce->field_site_commerce_code[LANGUAGE_NONE][0]['value'] = $code;

  // Заголовок.
  $site_commerce->title = kvantstudio_title_correct($title);

  // Содержимое.
  $site_commerce->body[LANGUAGE_NONE][0]['value'] = $body_value;
  $site_commerce->body[LANGUAGE_NONE][0]['summary'] = $body_summary;
  $site_commerce->body[LANGUAGE_NONE][0]['format'] = 'full_html';

  // Стоимость.
  $site_commerce->field_site_commerce_cost_min[LANGUAGE_NONE][0]['value'] = site_commerce_import_prepare_cost($cost_min);
  $site_commerce->field_site_commerce_cost[LANGUAGE_NONE][0]['value'] = site_commerce_import_prepare_cost($cost);

  // Количество на складе.
  $site_commerce->field_site_commerce_quantity[LANGUAGE_NONE][0]['value'] = $quantity;

  // Единица измерения количества.
  $site_commerce->field_site_commerce_measure[LANGUAGE_NONE][0]['value'] = $measure;

  // Статус позиции на складе.
  if (!is_numeric($status) || !$status) {
    if ($quantity > 0) {
      $status = 10;
    }
    if ($quantity == 0) {
      $status = 30;
    }
  }
  $site_commerce->status = $status;

  // Признак новинки.
  if (!is_numeric($sticky) || !$sticky) {
    $sticky = 0;
  }
  $site_commerce->sticky = $sticky;

  // Импорт файлов изображений.
  $field_image_import = $import_id;
  if (is_numeric($image) && $image && $field_image_import) {
    $count_upload_files = 0;
    $images_count = 10;
    for ($i = 1; $i <= $images_count; $i++) {
      $file = new stdClass();
      $source = 'sites/default/files/import_pictures/' . $field_image_import . '.' . $i . '.jpg';
      $exist = 'sites/default/files/pictures/site_commerce/' . $field_image_import . '.' . $i . '.jpg';
      if (file_exists($source) && !file_exists($exist)) {
        $file->uri = $source;

        $string_search = array(' ','  ',',','-','__');
        $string_replace = array('_','_','_','_','_');
        $filename = transliteration_get(str_replace($string_search, $string_replace, $field_image_import));
        $filename = mb_strtolower($filename, "UTF-8");

        $directory = file_default_scheme() . '://pictures/site_commerce';
        if (file_prepare_directory($directory, FILE_MODIFY_PERMISSIONS)) {
          $destination = $directory . '/' . $filename . '.' . $i . '.jpg';
          $file = file_copy($file, $destination);
          $file->status = FILE_STATUS_PERMANENT;
          file_save($file);
          $site_commerce->field_site_commerce_image[LANGUAGE_NONE][$count_upload_files]['fid'] = $file->fid;
          $count_upload_files++;
        }
      }
    }
  }

  // Сохранение позиции.
  site_commerce_save($site_commerce);
}

/**
 * Возвращает идентификатор термина по его названию.
 */
function site_commerce_term_tid_by_name($name) {
  if (!$name) {
    return 0;
  }
  if (is_numeric($name)) {
    $tid = $name;
  }
  if (is_string($name)) {
    $tid = db_query("SELECT n.tid FROM {taxonomy_term_data} n WHERE n.name = :name", array(':name' => $name))->fetchField();
  }
  if (!$tid) {
    return 0;
  }
  return $tid;
}