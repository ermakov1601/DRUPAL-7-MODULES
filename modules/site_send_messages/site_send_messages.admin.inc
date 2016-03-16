<?php
// $Id$

/**
 * @file
 * Admin page callbacks for the site_send_messages module.
 */

/**
 * Форма основных настроек.
 */
function site_send_messages_adjustments_form($form, &$form_state) {
  // Основные настройки.
  $form['site_send_messages_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings of the form of sending of the message'),
    '#collapsible' => FALSE,
  );
  $form['site_send_messages_form']['site_send_messages_count_files'] = array(
    '#type' => 'textfield',
    '#title' => t('Quantity fields for an attachment of files'),
    '#description' => t('If you change the values do not forget to refresh the cache site.'),
    '#default_value' => variable_get('site_send_messages_count_files', 1),
    '#size' => 5,
    '#required' => TRUE,
  );
  $form['site_send_messages_form']['site_send_messages_captcha_phone'] = array(
    '#title'         => t('To resolve check of telephone number on a mask of input: 8 (code) number.'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_send_messages_captcha_phone', FALSE),
  );
  $form['site_send_messages_form']['site_send_messages_default_note'] = array(
    '#title' => t('To display the text of the note'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('site_send_messages_default_note', FALSE),
  );
  $form['site_send_messages_form']['site_send_messages_note'] = array(
    '#type' => 'textarea',
    '#title' => t('The text of the note'),
    '#default_value' => variable_get('site_send_messages_note'),
    '#rows' => 3,
    '#required' => FALSE,
  );
  $form['site_send_messages_form']['site_send_messages_call_note'] = array(
    '#type' => 'textarea',
    '#title' => t('The text of the message above the form submit a claim for the return call'),
    '#default_value' => variable_get('site_send_messages_call_note'),
    '#rows' => 3,
    '#required' => FALSE,
  );
  $form['site_send_messages_form']['site_send_messages_call_submite_note'] = array(
    '#type' => 'textarea',
    '#title' => t('The text of the message of the successful sending a claim for the return call'),
    '#default_value' => variable_get('site_send_messages_call_submite_note', t('The message is sent!')),
    '#rows' => 3,
    '#required' => FALSE,
  );

  // Защита от спама.
  $form['captcha'] = array(
    '#type' => 'fieldset',
    '#title' => t('Protection from spam'),
    '#collapsible' => FALSE,
  );
  $form['captcha']['site_send_messages_captcha'] = array(
    '#title' => t('To resolve protection against a spam means of module CAPTCHA?'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('site_send_messages_captcha', FALSE),
  );

  // Настройка названия страниц.
  $form['site_send_messages_systems_pages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Adjustments of the name of pages'),
    '#collapsible' => FALSE,
  );
  $form['site_send_messages_systems_pages']['site_send_messages_title'] = array(
    '#type' => 'textfield',
    '#title' => t('The title of the page with the form send the message'),
    '#description' => t('If you change the values do not forget to refresh the cache site.'),
    '#default_value' => variable_get('site_send_messages_title', t('To write to us')),
    '#required' => TRUE,
  );

  // Настройки названия ссылок (не изменять на работающем сайте!)
  $form['site_send_messages_systems_links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Adjustments of the name of links'),
    '#collapsible' => FALSE,
  );
  $form['site_send_messages_systems_links']['site_send_messages_link'] = array(
    '#type' => 'textfield',
    '#title' => t('The name of the link page with a form to send a message'),
    '#description' => t('Do not change this value on a ready site! Value should be in a latin, words through a dash from the small letter. At variation of value do not forget to update a cache of a site.'),
    '#default_value' => variable_get('site_send_messages_link', 'otpravit-soobshchenie'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Страница вызова настроек полей формы.
 * @param
 *   (string) $form_group - название группы полей.
 */
function site_send_messages_settings($form_group = NULL) {
  $form = drupal_get_form('site_send_messages_select_group_form', $form_group);
  $output  = drupal_render($form);
  if ($form_group) {
    $form = drupal_get_form('site_send_messages_settings_form', $form_group);
    $output .= drupal_render($form);
  }
  return $output;
}

/**
 * Форма выбора группы, для которой вызываются настройки.
 */
function site_send_messages_select_group_form($form, &$form_state, $form_group = NULL) {

  // Массив с доступными зарегистрированными группами.
  $query = db_select('site_send_messages_groups', 'n');
  $query->fields('n', array('title', 'form_group'));
  $query->orderBy('title', 'DESC');
  $result = $query->execute();

  // Iterate through each database result.
  $groups = array();
  foreach ($result as $row) {
    $groups[$row->form_group] = t($row->title);
  }

  $form = array();

  $form['mode'] = array(
    '#type' => 'select',
    '#title' => t('Mode'),
    '#default_value' => $form_group,
    '#options' => $groups,
  );

  // Now we add our submit button, for submitting the form results.
  //
  // The 'actions' wrapper used here isn't strictly necessary for tabledrag,
  // but is included as a Form API recommended practice.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array( '#type' => 'submit', '#value' => t('Edit'));

  return $form;

}

/**
 * Submit callback for the site_send_messages_select_group_form form.
 */
function site_send_messages_select_group_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/kvantstudio/site-send-messages/settings-form/' . $form_state['values']['mode'];
}

/**
 * Вызов настроек формы отправки заявок.
 * @param
 *   (string) $form_group - название группы полей.
 */
function site_send_messages_settings_form($form, &$form_state, $form_group) {

  $form = array();

  $form[$form_group]['#tree'] = TRUE;

  $form['#attributes'] = array(
    'enctype' => 'multipart/form-data',
    'form_group' => $form_group,
  );

  // Перечень доступных полей формы.
  $query = db_select('site_send_messages_settings_form', 'n');
  $query->fields('n');
  $query->condition('n.form_group', $form_group);
  $query->orderBy('weight', 'ASC');
  $result = $query->execute();

  // Iterate through each database result.
  foreach ($result as $row) {

    $form[$form_group][$row->cid]['cid'] = array(
      '#type' => 'value',
      '#value' => $row->cid,
    );

    $form[$form_group][$row->cid]['form_key'] = array(
      '#type' => 'value',
      '#value' => $row->form_key,
    );

    $form[$form_group][$row->cid]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#default_value' => $row->weight,
      '#delta' => 10,
      '#title-display' => 'invisible',
    );

    $form[$form_group][$row->cid]['title'] = array(
      '#type' => 'textfield',
      '#default_value' => t($row->title),
      '#size'=> 20,
      '#maxlength' => 255,
      '#required' => TRUE,
    );

    $form[$form_group][$row->cid]['description'] = array(
      '#type' => 'textfield',
      '#default_value' => $row->description,
      '#size' => 20,
      '#maxlength' => 255,
      '#required' => FALSE,
    );

    $form[$form_group][$row->cid]['value'] = array(
      '#type' => 'textfield',
      '#default_value' => $row->value,
      '#maxlength' => 255,
      '#size'=> 30,
      '#required' => FALSE,
    );

    $form[$form_group][$row->cid]['required'] = array(
      '#type' => 'checkbox',
      '#default_value' => $row->required,
    );

    $form[$form_group][$row->cid]['disabled'] = array(
      '#type' => 'checkbox',
      '#default_value' => $row->disabled,
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
 * Submit callback for the site_send_messages_request_admin_form form.
 */
function site_send_messages_settings_form_submit($form, &$form_state) {
  $form_group = $form['#attributes']['form_group'];
  foreach ($form_state['values'][$form_group] as $key) {
    $record = array(
      'cid' => $key['cid'],
      'title' => $key['title'],
      'description' => $key['description'],
      'value' => $key['value'],
      'required' => $key['required'],
      'disabled' => $key['disabled'],
      'weight' => $key['weight']
    );
    // Store the form elements.
    drupal_write_record('site_send_messages_settings_form', $record, array('cid'));
  }
  drupal_set_message(t('Configuration saved.'));
}

/**
 * Темизация формы редактирования.
 */
function theme_site_send_messages_settings_form($variables) {

  $form = $variables['form'];
  $form_group = $form['#attributes']['form_group'];

  // Initialize the variable which will store our table rows
  $rows = array();

  foreach (element_children($form[$form_group]) as $key) {

    // Before we add our 'weight' column to the row, we need to give the
    // element a custom class so that it can be identified in the
    // drupal_add_tabledrag call.
    //
    // This could also have been done during the form declaration by adding
    //     '#attributes' => array('class' => 'example-item-weight'),
    // directy to the 'weight' element in tabledrag_example_simple_form().
    $form[$form_group][$key]['weight']['#attributes']['class'] = array('site-send-messages-form-item-weight');

    // We are now ready to add each element of our $form data to the $rows
    // array, so that they end up as individual table cells when rendered
    // in the final table.  We run each element through the drupal_render()
    // function to generate the final html markup for that element.
    $rows[] = array(
      'data' => array(
        array('data' => drupal_render($form[$form_group][$key]['title'])),
        array('data' => drupal_render($form[$form_group][$key]['description'])),
        array('data' => drupal_render($form[$form_group][$key]['value'])),
        array('data' => drupal_render($form[$form_group][$key]['required'])),
        array('data' => drupal_render($form[$form_group][$key]['disabled'])),
        array('data' => drupal_render($form[$form_group][$key]['weight'])),
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
  $header = array(t('Title'), t('Description'), t('Default value'), t('Required'), t('Disabled'), t('Weight'));

  // We also need to pass the drupal_add_tabledrag() function an id which will
  // be used to identify the <table> element containing our tabledrag form.
  // Because an element's 'id' should be unique on a page, make sure the value
  // you select is NOT the same as the form ID used in your form declaration.
  $table_id = 'site-send-messages-form-items-table';

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
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'site-send-messages-form-item-weight');

  return $output;
}
