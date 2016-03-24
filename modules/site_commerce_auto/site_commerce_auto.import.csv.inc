<?php

/**
 * Импортируем товары.
 */
function site_commerce_auto_import_page() {
  $form = drupal_get_form('site_commerce_auto_import_form');
  return drupal_render($form);
}

/**
 * Build a form to upload CSV to.
 */
function site_commerce_auto_import_form($form, &$form_state) {
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
    'site_commerce_auto_import_form_validate_fileupload',
    'site_commerce_auto_import_form_validate',
  );

  return $form;
}

/**
 * Validate the file upload. It must be a CSV, and we must
 * successfully save it to our import directory.
 */
function site_commerce_auto_import_form_validate_fileupload($form, &$form_state) {
  $validators = array(
    'file_validate_extensions' => array('csv'),
  ) ;

  if ($file = file_save_upload('csvfile', $validators)) {
    // The file was saved using file_save_upload() and was added to
    // the files table as a temporary file. We'll make a copy and let
    // the garbage collector delete the original upload.
    $directory = file_default_scheme() . '://import';
    if (file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
      $destination = $directory .'/' . $file->filename;
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
function site_commerce_auto_import_form_validate($form, &$form_state) {
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
function site_commerce_auto_import_form_submit($form, &$form_state) {
  $batch = array(
    'title' => t('Import of positions is carried out, wait...'),
    'operations' => array(),
    'init_message' => t('Preparation of data'),
    'progress_message' => t('It is executed @current from @total.'),
    'error_message' => t('An error occurred during processing.'),
    'finished' => 'site_commerce_auto_import_csv_finished',
    'file' => drupal_get_path('module', 'site_commerce_auto') . '/site_commerce_auto.import.csv.inc',
  );

  setlocale(LC_ALL, 'ru_RU.UTF-8');

  if (isset($form_state['values']['csvupload'])) {
    if (($handle = fopen($form_state['values']['csvupload'], "r")) !== FALSE) {
      // Отключаем видимость всех позиций.
      site_commerce_auto_reset();

      $batch['operations'][] = array('site_commerce_auto_import_csv_filename', array($form_state['values']['filename']));
      while (($data = fgetcsv($handle, 0, ";")) !== FALSE) {
        if ($data[0] <> 'pid') {
          $batch['operations'][] = array('site_commerce_auto_import_csv_data', array(array_map('base64_encode', $data)));
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
function site_commerce_auto_import_csv_filename($filename, &$context) {
  $context['results']['uploaded_filename'] = $filename;
}

/**
 * Process a single line.
 */
function site_commerce_auto_import_csv_data($data, &$context) {
  if (isset($context['results']['rows_imported'])) {
    $context['results']['rows_imported'] = $context['results']['rows_imported'] + 1;
  }
  else {
    $context['results']['rows_imported'] = 1;
  }

  $data = array_map('base64_decode', $data);

  site_commerce_auto_import_save($data);

  /**
   * Alternatively, our example CSV happens to have the title in the
   * third column, so we can uncomment this line to display "Importing
   * Blahblah" as each row is parsed.
   *
   * You can comment out the line above if you uncomment this one.
   */
  $context['message'] = t('The position has been added %title', array('%title' => $data[3]));

  /**
   * In order to slow importing and debug better, we can uncomment
   * this line to make each import slightly slower.
   */
  //usleep(1000);
}

/**
 * Handle batch completion.
 */
function site_commerce_auto_import_csv_finished($success, $results, $operations) {
  if ($success) {
    $uri = file_default_scheme() . '://styles';
    file_unmanaged_delete_recursive($uri);

    drupal_set_message(t('Import of a file <strong>@file</strong> is completed.', array('@file' => $results['uploaded_filename'])));
  }
  else {
    drupal_set_message(t('Import file <strong>@file</strong> completed with errors.', array('@file' => $results['uploaded_filename'])), 'error');
  }
  drupal_goto('admin/content/site-commerce');
}

/**
 * Сохраняем данные в БД.
 */
function site_commerce_auto_import_save($data) {
  // Формируем данные.
  $pid          = trim($data[0]);
  $recid        = trim($data[1]);
  $supid        = trim($data[2]);
  $title        = kvantstudio_title_correct(($data[3]));
  $body_summary = trim($data[4]);
  $body_value   = trim($data[5]);
  $code         = trim($data[6]);
  $oem          = trim($data[7]);
  $orig         = trim($data[8]);
  $trade_mark   = trim($data[9]);
  $car          = trim($data[10]);
  $cost         = trim($data[11]);
  $quantity     = trim($data[12]);
  $measure      = trim($data[13]);
  $status       = trim($data[14]);
  $sticky       = trim($data[15]);
  $condition    = trim($data[16]);
  $image        = trim($data[17]);
  $image_delete = trim($data[18]);

  // Проверка входных параметров.
  if (!is_numeric($sticky) || !$sticky) {
    $sticky = 0;
  }

  // Назначаем категорию каталога для позиции.
  $text = kvantstudio_search_request_prepare(mb_strtolower($title, "UTF-8"));
  $tid = site_commerce_auto_set_tid($text);

  // Создаем обьект позиции.
  $site_commerce = new stdClass();
  $site_commerce->language = LANGUAGE_NONE;
  $site_commerce->type = 'site_commerce_auto';

  // Загружаем тип.
  $type = site_commerce_type_get_type('site_commerce_auto');

  $field_name = 'field_category_' . $type->categories_vocabulary;
  $site_commerce->$field_name = array(
    LANGUAGE_NONE => array(
      '0' => array(
        'tid' => $tid,
      ),
    ),
  );

  $site_commerce->body[LANGUAGE_NONE][0]['value'] = $body_value;
  $site_commerce->body[LANGUAGE_NONE][0]['summary'] = $body_summary;
  $site_commerce->body[LANGUAGE_NONE][0]['format'] = 'full_html';
  $site_commerce->field_site_commerce_code[LANGUAGE_NONE][0]['value'] = $code;
  $site_commerce->field_site_commerce_oem[LANGUAGE_NONE][0]['value'] = $oem;
  $site_commerce->field_site_commerce_orig[LANGUAGE_NONE][0]['value'] = $orig;
  $site_commerce->field_site_commerce_trade_mark[LANGUAGE_NONE][0]['value'] = $trade_mark;
  $site_commerce->field_site_commerce_car[LANGUAGE_NONE][0]['value'] = $car;
  $site_commerce->field_site_commerce_recid[LANGUAGE_NONE][0]['value'] = $recid;
  $site_commerce->field_site_commerce_supid[LANGUAGE_NONE][0]['value'] = $supid;
  $site_commerce->field_site_commerce_cost[LANGUAGE_NONE][0]['value'] = site_commerce_import_prepare_cost($cost);
  $site_commerce->field_site_commerce_quantity[LANGUAGE_NONE][0]['value'] = $quantity;
  $site_commerce->field_site_commerce_measure[LANGUAGE_NONE][0]['value'] = $measure;
  $site_commerce->status = $status;
  $site_commerce->sticky = $sticky;

  // Формируем заголовок.
  $site_commerce->title = $title . ' ' . $oem . ' ' . $trade_mark;
  $site_commerce->title = kvantstudio_title_correct($site_commerce->title);

  // Выполняем проверку, существует ли позиция в базе.
  // Проверку выполняем по идентификатору recid.
  if (!$pid && $recid) {
    $query = db_select('field_data_field_site_commerce_recid', 'n');
    $query->fields('n', array('entity_id'));
    $query->condition('n.entity_type', 'site_commerce');
    $query->condition('n.bundle', 'site_commerce_auto');
    $query->condition('n.field_site_commerce_recid_value', $recid);
    $pid = $query->execute()->fetchField();
  }

  // Если позиция существует.
  if ($pid && is_numeric($pid) && $pid > 0) {
    $site_commerce->pid = $pid;
    $site_commerce->disabled = 0;

    // Удаляем текущие привязанные фотоизображения к карточке.
    if (is_numeric($image_delete) && $image_delete) {
      $query = db_select('field_data_field_site_commerce_image', 'n');
      $query->fields('n', array('field_site_commerce_image_fid'));
      $query->condition('n.entity_type', 'site_commerce');
      $query->condition('n.bundle', 'site_commerce_auto');
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

  // Загружаем файлы изображений.
  if (is_numeric($image) && $image) {
    $count_upload_files = 0;
    $images_count = 10;
    $field_import = $oem;
    for ($i = 1; $i <= $images_count; $i++) {
      $file = new stdClass();
      $source = 'sites/default/files/import_pictures/' . $field_import . '.' . $i . '.jpg';
      $exist = 'sites/default/files/pictures/site_commerce/' . $field_import . '.' . $i . '.jpg';
      if (file_exists($source) && !file_exists($exist)) {
        $file->uri = $source;

        $string_search = array(' ','  ',',','-','__');
        $string_replace = array('_','_','_','_','_');
        $filename = transliteration_get(str_replace($string_search, $string_replace, $field_import));
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

  // Сохранение в БД.
  site_commerce_save($site_commerce);
}