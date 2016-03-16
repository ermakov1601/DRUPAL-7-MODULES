<?php
// $Id:

/**
 * @file
 * Admin page callbacks for the site_photogallery module.
 */

/**
 * Вспомогательная функция для перевода строк.
 */
function site_photogallery_translations() {
  $translations = array(
    'text' => t('Photo albums'),
  );
}

/**
 * Вызов формы настроек модуля site_photogallery module.
 */
function site_photogallery_settings_form($form, &$form_state) {

  // Определяем fieldsets.
  $form['site_photogallery_adjustments'] = array(
    '#type' => 'fieldset',
    '#title' => t('The basic settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['site_photogallery_adjustments']['site_photogallery_album_imagecache_preset'] = array(
    '#type'          => 'select',
    '#title'         => t('Style imаgeсасhe when viewing a image of photo album'),
    '#default_value' => variable_get('site_photogallery_album_imagecache_preset', 'thumbnail'),
    '#options'       => kvantstudio_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );

  $form['site_photogallery_adjustments']['site_photogallery_image_imagecache_preset'] = array(
    '#type'          => 'select',
    '#title'         => t('Style imаgeсасhe when viewing a image in photo album'),
    '#default_value' => variable_get('site_photogallery_image_imagecache_preset', 'thumbnail'),
    '#options'       => kvantstudio_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );

  $form['site_photogallery_adjustments']['site_photogallery_colorbox_imagecache_preset'] = array(
    '#type'          => 'select',
    '#title'         => t('Style imаgeсасhe when viewing a image in Cоlоrbоx'),
    '#default_value' => variable_get('site_photogallery_colorbox_imagecache_preset', 'large'),
    '#options'       => kvantstudio_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );

  $form['site_photogallery_adjustments']['site_photogallery_none_categories'] = array(
    '#title'         => t('To display photo albums without categories'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_photogallery_none_categories', FALSE),
  );

  // Настройки названия страниц (не изменять на работающем сайте!).
  $form['site_photogallery_systems_pages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Setting of the name of pages'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['site_photogallery_systems_pages']['site_photogallery_catalog_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Heading of page of the catalogue of photo albums'),
    '#description' => t("Do not modify this value on your final site! The value must be in Latin, the word through the dash with a small letter. When the value of the don't forget to refresh the cache of the site."),
    '#default_value' => variable_get('site_photogallery_catalog_title', 'Photo albums'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Страница вызова настроек полей формы.
 * @param
 *   (string) $tid - название группы полей.
 */
function site_photogallery_category_settings($tid = 0) {
  $form = drupal_get_form('site_photogallery_category_select_form', $tid);
  $output = drupal_render($form);
  if ($tid) {
    $form = drupal_get_form('site_photogallery_category_settings_form', $tid);
    $output .= drupal_render($form);
  }
  return $output;
}

/**
 * Форма выбора категорий.
 */
function site_photogallery_category_select_form($form, &$form_state, $tid) {
  $vocabulary = taxonomy_vocabulary_load(variable_get('site_photogallery_vocabulary'));

  // Массив с доступными категориями.
  $query = db_select('taxonomy_term_data', 'n');
  $query->fields('n', array('name', 'tid'));
  $query->condition('n.vid', $vocabulary->vid);
  $query->orderBy('n.weight', 'DESC');
  $result = $query->execute();

  // Iterate through each database result.
  $tids = array();
  foreach ($result as $row) {
    $tids[$row->tid] = t($row->name);
  }

  $form = array();

  $form['tid'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#default_value' => $tid,
    '#options' => $tids,
  );

  // Now we add our submit button, for submitting the form results.
  //
  // The 'actions' wrapper used here isn't strictly necessary for tabledrag,
  // but is included as a Form API recommended practice.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array( '#type' => 'submit', '#value' => t('Send'));

  return $form;
}

/**
 * Submit callback for the site_photogallery_category_select_form form.
 */
function site_photogallery_category_select_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/kvantstudio/site-photogallery/photo-album-settings/' . $form_state['values']['tid'];
}

/**
 * Вызов формы настроек категории.
 * @param
 *   (int) $tid - идентификатор категории.
 */
function site_photogallery_category_settings_form($form, &$form_state, $tid) {

  $form = array();

  $form[$tid]['#tree'] = TRUE;

  $form['#attributes'] = array(
    'enctype' => 'multipart/form-data',
    'tid' => $tid,
  );

  // Перечень доступных фотоальбомов.
  $query = db_select('node', 'n');
  $query->fields('n', array('nid','title'));
  // Связываем таблицы.
  $query->innerJoin('field_data_field_site_photogallery_taxonomy', 'f', 'n.nid = f.entity_id');
  $query->condition('f.field_site_photogallery_taxonomy_tid', $tid);
  $query->innerJoin('field_data_field_site_photogallery_weight', 'w', 'n.nid = w.entity_id');
  $query->fields('w', array('field_site_photogallery_weight_value'));
  $query->orderBy('w.field_site_photogallery_weight_value', 'ASC');
  $result = $query->execute();

  // Iterate through each database result.
  foreach ($result as $row) {
    $form[$tid][$row->nid]['nid'] = array(
      '#type' => 'value',
      '#value' => $row->nid,
    );

    $form[$tid][$row->nid]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#default_value' => $row->field_site_photogallery_weight_value,
      '#delta' => 10,
      '#title-display' => 'invisible',
    );

    $form[$tid][$row->nid]['title'] = array(
      '#type' => 'textfield',
      '#default_value' => t($row->title),
      '#size'=> 20,
      '#maxlength' => 255,
      '#required' => TRUE,
    );

  }

  // Now we add our submit button, for submitting the form results.
  //
  // The 'actions' wrapper used here isn't strictly necessary for tabledrag,
  // but is included as a Form API recommended practice.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array( '#type' => 'submit', '#value' => t('Save changes'));

  return $form;
}

/**
 * Темизация формы редактирования.
 */
function theme_site_photogallery_category_settings_form($variables) {

  $form = $variables['form'];
  $tid = $form['#attributes']['tid'];

  // Initialize the variable which will store our table rows
  $rows = array();

  foreach (element_children($form[$tid]) as $key) {

    // Before we add our 'weight' column to the row, we need to give the
    // element a custom class so that it can be identified in the
    // drupal_add_tabledrag call.
    //
    // This could also have been done during the form declaration by adding
    //     '#attributes' => array('class' => 'example-item-weight'),
    // directy to the 'weight' element in tabledrag_example_simple_form().
    $form[$tid][$key]['weight']['#attributes']['class'] = array('site-photogallery-form-item-weight');

    // We are now ready to add each element of our $form data to the $rows
    // array, so that they end up as individual table cells when rendered
    // in the final table.  We run each element through the drupal_render()
    // function to generate the final html markup for that element.
    $rows[] = array(
      'data' => array(
        array('data' => drupal_render($form[$tid][$key]['title'])),
        array('data' => drupal_render($form[$tid][$key]['weight'])),
      ),
      // To support the tabledrag behaviour, we need to assign each row of the
      // table a class attribute of 'draggable'. This will add the 'draggable'
      // class to the <tr> element for that row when the final table is
      // rendered.
      'class' => array('draggable'),
    );
  }

  // We now define the table header values.  Ensure that the 'header' count
  // matches the final column count for your table.
  $header = array(t('Title'), t('Weight'));

  // We also need to pass the drupal_add_tabledrag() function an id which will
  // be used to identify the <table> element containing our tabledrag form.
  // Because an element's 'id' should be unique on a page, make sure the value
  // you select is NOT the same as the form ID used in your form declaration.
  $table_id = 'site-photogallery-form-items-table';

  // We can render our tabledrag table for output.
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => $table_id)));

  // And then render any remaining form elements (such as our submit button)
  $output .= drupal_render_children($form);

  // We now call the drupal_add_tabledrag() function in order to add the
  // tabledrag.js productness onto our page.
  //
  // For a basic sortable table, we need to pass it:
  //   - the $table_id of our <table> element,
  //   - the $action to be performed on our form items ('order'),
  //   - a string describing where $action should be applied ('siblings'),
  //   - and the class of the element containing our 'weight' element.
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'site-photogallery-form-item-weight');

  return $output;
}

/**
 * Submit callback for the site_send_messages_request_admin_form form.
 */
function site_photogallery_category_settings_form_submit($form, &$form_state) {
  $tid = $form['#attributes']['tid'];
  foreach ($form_state['values'][$tid] as $key) {
    $record = array(
      'entity_type' => 'node',
      'entity_id' => $key['nid'],
      'field_site_photogallery_weight_value' => $key['weight'],
    );
    // Store information.
    drupal_write_record('field_data_field_site_photogallery_weight', $record, array('entity_type', 'entity_id'));
  }
  drupal_set_message(t('Configuration saved.'));
}