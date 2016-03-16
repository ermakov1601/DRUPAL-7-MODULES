<?php
// $Id$

/**
 * @file
 * Бибилиотека функций импорта тегов для таксономии.
 */

/**
 * Импортируем товары.
 */
function site_commerce_import_page() {
  return drupal_render(drupal_get_form('site_commerce_import_form'));
}

/**
 * Build a form to upload CSV to.
 */
function site_commerce_import_form($form, &$form_state) {
  $form['#attributes'] = array('enctype' => 'multipart/form-data');

  $form['site_commerce_vocabulary'] = array(
    '#type' => 'select',
    '#title' => t('A dictionary for import'),
    '#options' => kvantstudio_select_vocabularies(),
    '#required' => TRUE,
  );

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
  );

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
    'file' => drupal_get_path('module', 'site_commerce') . '/site_commerce.import.taxonomy.tags.csv.inc',
  );

  if (isset($form_state['values']['csvupload'])) {
    if (($handle = fopen($form_state['values']['csvupload'], "r")) !== FALSE) {
      $batch['operations'][] = array('site_commerce_import_csv_filename', array($form_state['values']['filename']));
      while (($data = fgetcsv($handle, 4096, ";")) !== FALSE) {
        // Словарь для указания словаря для импорта.
        $data[2] = $form_state['values']['site_commerce_vocabulary'];
        /**
         * we use base64_encode to ensure we don't overload the batch
         * processor by stuffing complex objects into it
         */
        $batch['operations'][] = array('site_commerce_import_csv_data', array(array_map('base64_encode', $data)));
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
  $context['results']['rows_imported']++;
  $data = array_map('base64_decode', $data);

  $tid = site_commerce_import_save($data);

  /**
   * Alternatively, our example CSV happens to have the title in the
   * third column, so we can uncomment this line to display "Importing
   * Blahblah" as each row is parsed.
   *
   * You can comment out the line above if you uncomment this one.
   */

  $context['message'] = t('The term <strong>%tid</strong> was updated.', array('%tid' => $tid));

  /**
   * In order to slow importing and debug better, we can uncomment
   * this line to make each import slightly slower.
   */
  //usleep(10000);
}

/**
 * Handle batch completion.
 */
function site_commerce_import_csv_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('Import of a file <strong>@file</strong> is completed.', array('@file' => $results['uploaded_filename'])));
  }
  else {
    drupal_set_message(t('Import file <strong>@file</strong> completed with errors.', array('@file' => $results['uploaded_filename'])), 'error');
  }
  drupal_goto('admin/structure/taxonomy/site_commerce_csv_import');
}

/**
 * Сохраняем данные в БД.
 */
function site_commerce_import_save($data) {
  // Формируем данные.
  $name = trim($data[0]);
  $tags = trim($data[1]);
  $vid = trim($data[2]);

  $tid = db_query("SELECT n.tid FROM {taxonomy_term_data} n WHERE n.name = :name AND n.vid = :vid", array(':name' => $name, ':vid' => $vid))->fetchField();
  $term = taxonomy_term_load($tid);
  if (is_object($term)) {
    $term->field_site_commerce_tags[LANGUAGE_NONE][0]['value'] = $tags;
    taxonomy_term_save($term);
  }
  return $term->tid;
}