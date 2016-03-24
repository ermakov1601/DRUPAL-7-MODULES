<?php

/**
 * Импортируем товары.
 */
function site_commerce_auto_import_page() {
  drupal_set_title(t('Delete positions'));

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
    '#value' => t('To execute delete'),
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
      if ($line = fgetcsv($handle, 4096, ";")) {
        /**
         * Validate the uploaded CSV here.
         *
         * The example CSV happens to have cell A1 ($line[0]) as
         * below; we validate it only.
         *
         * You'll probably want to check several headers, eg:
         *   if ( $line[0] == 'Index' || $line[1] != 'Supplier' || $line[2] != 'Title' )
         */
        // if ( $line[0] != 'Example CSV for csvimport.module - http://github.com/xurizaemon/csvimport' ) {
        //   form_set_error('csvfile', t('Sorry, this file does not match the expected format.')) ;
        // }
      }
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
    'file' => drupal_get_path('module', 'site_commerce_auto') . '/site_commerce_auto_delete_csv.inc',
  );

  if (isset($form_state['values']['csvupload'])) {
    if (($handle = fopen($form_state['values']['csvupload'], "r")) !== FALSE) {
      // Отключаем видимость всех позиций.
      site_commerce_auto_reset();

      $batch['operations'][] = array('site_commerce_auto_import_csv_filename', array($form_state['values']['filename']));
      while (($data = fgetcsv($handle, 4096, ";")) !== FALSE) {
        /**
         * we use base64_encode to ensure we don't overload the batch
         * processor by stuffing complex objects into it
         */
        if ($data[0] <> 'NAME') {
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
  $context['results']['rows_imported']++;
  $data = array_map('base64_decode', $data);

  // Формируем данные.
  $recid = trim($data[0]);

  $query = db_select('field_data_field_site_commerce_recid', 'i');
  $query->fields('i', array('entity_id'));
  $query->condition('i.field_site_commerce_recid_value', $recid);
  $pid = $query->execute()->fetchField();

  if ($pid) {
    // Удаление в БД.
    site_commerce_delete($pid);

    /**
     * Alternatively, our example CSV happens to have the title in the
     * third column, so we can uncomment this line to display "Importing
     * Blahblah" as each row is parsed.
     *
     * You can comment out the line above if you uncomment this one.
     */

    $context['message'] = t('The position has been deleted %title', array('%title' => $data[0]));
  }
  else {
    $context['message'] = t('The position is not found %title', array('%title' => $recid));
  }

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
    drupal_set_message(t('Import of a file <strong>@file</strong> is completed.', array('@file' => $results['uploaded_filename'])));
  }
  else {
    drupal_set_message(t('Import file <strong>@file</strong> completed with errors.', array('@file' => $results['uploaded_filename'])), 'error');
  }
  drupal_goto('admin/content/site-commerce');
}
