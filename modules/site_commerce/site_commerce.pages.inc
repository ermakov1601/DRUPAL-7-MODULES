<?php
// $Id$

/**
 * @file
 * Page callbacks for adding, editing, deleting, and revisions management for products.
 */

/**
 * Page callback: Displays add content links for available site commerce types.
 *
 * Redirects to site-commerce/add/[type] if only one site commerce type is available.
 *
 * @see site_commerce_menu()
 */
function site_commerce_add_page() {
  foreach (site_commerce_type_get_types() as $type) {
    $type_url_str = str_replace('_', '-', $type->type);
    $content[$type->type] = array(
      'title' => $type->name,
      'description' => $type->description,
      'href' => 'admin/content/site-commerce/add/' . $type_url_str,
    );
  }
  return theme('site_commerce_add_list', array('content' => $content));
}

/**
 * Returns HTML for a list of available node types for node creation.
 *
 * @param $variables
 *   An associative array containing:
 *   - content: An array of content types.
 *
 * @ingroup themeable
 */
function theme_site_commerce_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="site-commerce-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    $output = '<p>' . t('You have not created any product types yet. Go to the <a href="@create-content">product type creation page</a> to add a new product type.', array('@create-content' => url('admin/structure/site-commerce-types/add'))) . '</p>';
  }
  return $output;
}

/**
 * Создание товара определенного типа.
 */
function site_commerce_add($type) {
  global $user;
  $site_commerce_type = site_commerce_type_load($type);
  $site_commerce = (object) array('uid' => $user->uid, 'name' => (isset($user->name) ? $user->name : ''), 'type' => $type, 'language' => LANGUAGE_NONE);
  drupal_set_title(t('Create @name', array('@name' => $site_commerce_type->name)));
  $output = drupal_get_form('site_commerce_form', $site_commerce);
  return $output;
}

/**
 * Форма создания и редактирования товара.
 */
function site_commerce_form($form, &$form_state, $site_commerce) {

  global $user;

  // During initial form build, add the site commerce entity to the form state for use
  // during form building and processing. During a rebuild, use what is in the
  // form state.
  if (!isset($form_state['site_commerce'])) {
    if (!isset($site_commerce->title)) {
      $site_commerce->title = NULL;
    }
    site_commerce_object_prepare($site_commerce);
    $form['site_commerce'] = array(
      '#type' => 'value',
      '#value' => $site_commerce,
    );
  }
  else {
    $site_commerce = $form_state['site_commerce'];
  }

  // Basic information.
  // These elements are just values so they are not even sent to the client.
  foreach (array('pid', 'uid', 'created', 'type', 'language') as $key) {
    $form[$key] = array(
      '#type' => 'value',
      '#value' => isset($site_commerce->$key) ? $site_commerce->$key : NULL,
    );
  }

  // Формируем экстра поля.
  $form['field_site_commerce_title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Name'),
    '#default_value' => $site_commerce->title,
  );
  $form['field_site_commerce_status'] = array(
    '#type' => 'select',
    '#title' => t('Status position in stock'),
    '#options' => site_commerce_position_status(),
    '#default_value' => $site_commerce->status,
  );
  $form['field_site_commerce_type_sale'] = array(
    '#type' => 'select',
    '#title' => t('Type of sale'),
    '#options' => site_commerce_types_sale(),
    '#default_value' => $site_commerce->type_sale,
  );
  $form['field_site_commerce_package'] = array(
    '#type' => 'select',
    '#title' => t('Package of goods'),
    '#options' => site_commerce_get_packages(),
    '#default_value' => site_commerce_get_pid_package($site_commerce->pid),
    '#states' => array(
      'visible' => array(
        array(
          array(':input[name=field_site_commerce_type_sale]' => array('value' => 2)),
          array(':input[name=field_site_commerce_type_sale]' => array('value' => 3)),
        ),
      ),
    ),
  );
  $form['field_site_commerce_cost_set'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display the value of the set of goods'),
    '#default_value' => $site_commerce->cost_set,
    '#description' => t('The cost of the goods will be calculated as the sum of the value of goods, of which it is composed. The item must be of type sale - consists of a set of goods.'),
    '#states' => array(
      'visible' => array(
        array(
          array(':input[name=field_site_commerce_type_sale]' => array('value' => 4)),
          array(':input[name=field_site_commerce_type_sale]' => array('value' => 5)),
        ),
      ),
    ),
  );
  $form['field_site_commerce_sticky'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Novelty'),
    '#default_value' => $site_commerce->sticky,
  );
  $form['field_site_commerce_disabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Do not display on the site'),
    '#default_value' => $site_commerce->disabled,
  );
  $form['field_site_commerce_type_condition'] = array(
    '#type' => 'select',
    '#title' => t('Condition of product'),
    '#options' => site_commerce_get_conditions(),
    '#default_value' => $site_commerce->type_condition,
  );

  // Формируем поля сущности.
  field_attach_form('site_commerce', $site_commerce, $form, $form_state);

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
  );

  // Определяем элементы управления.
  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('site_commerce_form_submit'),
  );

  // Если режим редактирования, отображаем кнопку удаления.
  if (!empty($site_commerce->pid) && site_commerce_access('edit', $site_commerce)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('site_commerce_form_delete_submit'),
    );
  }

  return $form;
}

/**
 * Implementation of hook_validate().
 */
function site_commerce_form_validate($form, &$form_state) {
  $type = site_commerce_type_get_type($form_state['values']['type']);
  $tid = $form_state['values']['field_category_' . $type->categories_vocabulary][LANGUAGE_NONE][0]['tid'];
  $term = taxonomy_term_load($tid);
  $terms = taxonomy_get_children($tid);
  if (count($terms)) {
    form_set_error('field_site_commerce_category', t('In a category @name it is forbidden to add positions. The reason: the chosen category has affiliated subcategories. Specify one of them.', array('@name' => $term->name)));
  }
}

/**
 * Обработчик формы создания/редактирования товара.
 */
function site_commerce_form_submit($form, &$form_state) {
  $site_commerce = (object) $form_state['values']['site_commerce'];
  $site_commerce->title = $form_state['values']['field_site_commerce_title'];
  $site_commerce->status = $form_state['values']['field_site_commerce_status'];
  $site_commerce->sticky = $form_state['values']['field_site_commerce_sticky'];
  $site_commerce->disabled = $form_state['values']['field_site_commerce_disabled'];
  $site_commerce->disabled = $form_state['values']['field_site_commerce_disabled'];
  $site_commerce->type_sale = $form_state['values']['field_site_commerce_type_sale'];
  $site_commerce->package = $form_state['values']['field_site_commerce_package'];
  $site_commerce->cost_set = $form_state['values']['field_site_commerce_cost_set'];
  $site_commerce->type_condition = $form_state['values']['field_site_commerce_type_condition'];

  // Attach submit handlers from Field module.
  field_attach_submit('site_commerce', $site_commerce, $form, $form_state);

  entity_form_submit_build_entity('site_commerce', $site_commerce, $form, $form_state);

  // Сохраняем данные.
  site_commerce_save($site_commerce);

  // Clear the page and block caches.
  cache_clear_all();

  drupal_set_message(t('The update has been performed.'));
  $form_state['redirect'] = 'admin/content/site-commerce/add/' . str_replace('_', '-', $site_commerce->type);
}

/**
 * Button submit function: handle the 'Delete' button on the site commerce form.
 */
function site_commerce_form_delete_submit($form, &$form_state) {
  $site_commerce = (object) $form_state['values']['site_commerce'];
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $form_state['redirect'] = array('site-commerce/' . $site_commerce->pid . '/delete', array('query' => $destination));
}

/**
 * Menu callback -- ask for confirmation of product deletion.
 */
function site_commerce_delete_confirm($form, &$form_state, $site_commerce) {
  $form['#site_commerce'] = $site_commerce;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['pid'] = array('#type' => 'value', '#value' => $site_commerce->pid);
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $site_commerce->title)),
    'site-commerce/' . $site_commerce->pid,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute product deletion.
 */
function site_commerce_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $site_commerce = site_commerce_load($form_state['values']['pid']);
    site_commerce_delete($form_state['values']['pid']);
    watchdog('site_commerce', '@type: deleted %title.', array('@type' => $site_commerce->type, '%title' => $site_commerce->title));
    drupal_set_message(t('@type %title has been deleted.', array('@type' => site_commerce_type_get_name($site_commerce), '%title' => $site_commerce->title)));
  }

  $form_state['redirect'] = '<front>';
}
