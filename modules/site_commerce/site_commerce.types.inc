<?php
// $Id$

/**
 * @file
 * Библиотека функций управления типами товаров.
 */

/**
 * Returns HTML for a site commerce type description for the content type admin overview page.
 *
 * @param $variables
 *   An associative array containing:
 *   - name: The human-readable name of the site commerce type.
 *   - type: An object containing the 'type' (machine name) and 'description' of
 *     the site commerce type.
 *
 * @ingroup themeable
 */
function theme_site_commerce_admin_overview($variables) {
  $name = $variables['name'];
  $type = $variables['type'];

  $output = check_plain($name);
  $output .= ' <small>' . t('(Machine name: @type)', array('@type' => $type->type)) . '</small>';
  $output .= '<div class="description">' . filter_xss_admin($type->description) . '</div>';
  return $output;
}

/**
* Выводит перечень всех bundles (типов товаров) нашей сущности site_commerce.
*/
function site_commerce_types_list() {
  $types = site_commerce_type_get_types();
  $names = site_commerce_type_get_names();
  $field_ui = module_exists('field_ui');
  $header = array(t('Name'), array('data' => t('Operations'), 'colspan' => $field_ui ? '4' : '2'));
  $rows = array();

  foreach ($names as $key => $name) {
    $type = $types[$key];

    $type_url_str = str_replace('_', '-', $type->type);

    $row = array(theme('site_commerce_admin_overview', array('name' => $name, 'type' => $type)));

    // Set the edit column.
    $row[] = array('data' => l(t('edit'), 'admin/structure/site-commerce-types/manage/' . $type_url_str));

    if ($field_ui) {
      // Manage fields.
      $row[] = array('data' => l(t('manage fields'), 'admin/structure/site-commerce-types/manage/' . $type_url_str . '/fields'));

      // Display fields.
      $row[] = array('data' => l(t('manage display'), 'admin/structure/site-commerce-types/manage/' . $type_url_str . '/display'));
    }

    // Set the delete column.
    $row[] = array('data' => l(t('delete'), 'admin/structure/site-commerce-types/manage/' . $type_url_str . '/delete'));

    $rows[] = $row;

  }

  $build['site_commerce_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No content types available. <a href="@link">Add content type</a>.', array('@link' => url('admin/structure/site-commerce-types/add'))),
  );

  return $build;

}

/**
 * Генерация формы создания нового типа товаров.
 */
function site_commerce_type_form($form, &$form_state, $type = NULL) {
  // Если тип не создан.
  if (!isset($type->type)) {
    $type = site_commerce_type_set_defaults();
  }

  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => isset($type->name) ? $type->name : '',
    '#description' => t('The human-readable name of this product type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($type->type) ? $type->type : '',
    '#maxlength' => 32,
    '#disabled' => FALSE,
    '#machine_name' => array(
      'exists' => 'site_commerce_type_load',
      'source' => array('name'),
    ),
    '#description' => t('A unique machine-readable name for this product type. It must only contain lowercase letters, numbers, and underscores.'),
    '#access' => $type->type ? FALSE : TRUE,
  );

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => isset($type->description) ? $type->description : '',
    '#description' => t('Description about the product type.'),
    '#rows' => 2,
  );

  $form['path'] = array(
    '#title' => t('Path'),
    '#type' => 'textfield',
    '#default_value' => isset($type->path) ? $type->path : '',
    '#description' => t('The name (path-alias) of which are shaped алиасы links of the catalogue. For example, www.site.ru/catalog/path-alias/position/1.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Формируем поля для выбора значений в словарях.
  $form['countries_vocabulary'] = array(
    '#type' => 'select',
    '#title' => t('The country of the manufacturer'),
    '#options' => kvantstudio_select_vocabularies(),
    '#default_value' => isset($type->countries_vocabulary) ? $type->countries_vocabulary : '',
    '#required' => TRUE,
  );
  $form['manufactures_vocabulary'] = array(
    '#type' => 'select',
    '#title' => t('Manufacturers and brands'),
    '#options' => kvantstudio_select_vocabularies(),
    '#default_value' => isset($type->manufactures_vocabulary) ? $type->manufactures_vocabulary : '',
    '#required' => TRUE,
  );
  $form['contractors_vocabulary'] = array(
    '#type' => 'select',
    '#title' => t('Сontractors'),
    '#options' => kvantstudio_select_vocabularies(),
    '#default_value' => isset($type->contractors_vocabulary) ? $type->contractors_vocabulary : '',
    '#required' => TRUE,
  );
  $form['categories_vocabulary'] = array(
    '#type' => 'select',
    '#title' => t('Categories the online store'),
    '#options' => kvantstudio_select_vocabularies(),
    '#default_value' => isset($type->categories_vocabulary) ? $type->categories_vocabulary : '',
    '#required' => TRUE,
  );

  $form['status'] = array(
    '#title'         => t('To include display to a site'),
    '#type'          => 'checkbox',
    '#default_value' => isset($type->status) ? $type->status : TRUE,
  );

  $form['status_term_alias_delete'] = array(
    '#title'         => t('To resolve removal existing URL alias categories of the catalogue at their updating'),
    '#type'          => 'checkbox',
    '#default_value' => isset($type->status_term_alias_delete) ? $type->status_term_alias_delete : FALSE,
  );

  $form['old_type'] = array(
    '#type' => 'value',
    '#value' => isset($type->type) ? $type->type : FALSE,
  );

  $form['module'] = array(
    '#type' => 'value',
    '#value' => $type->module,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save product type'),
    '#weight' => 40,
  );

  if (!empty($type->type)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete product type'),
      '#weight' => 45,
    );
  }
  return $form;
}

/**
 * Form submission handler for site_commerce_type_form().
 *
 * @see site_commerce_type_form_validate().
 */
function site_commerce_type_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  $type = new stdClass();
  $type->type = trim($form_state['values']['type']);
  $type->name = trim($form_state['values']['name']);
  $type->description = trim($form_state['values']['description']);
  $type->path = trim($form_state['values']['path']);
  $type->status = $form_state['values']['status'];
  $type->countries_vocabulary = $form_state['values']['countries_vocabulary'];
  $type->manufactures_vocabulary = $form_state['values']['manufactures_vocabulary'];
  $type->contractors_vocabulary = $form_state['values']['contractors_vocabulary'];
  $type->categories_vocabulary = $form_state['values']['categories_vocabulary'];
  $type->status_term_alias_delete = $form_state['values']['status_term_alias_delete'];
  $type->old_type = isset($form_state['values']['old_type']) ? $form_state['values']['old_type'] : $type->type;
  $type->module = $form_state['values']['module'];

  // Выполняем корректировку алиаса.
  $type->path = str_replace('_', '-', $type->path);
  $type->path = str_replace(' ', '-', $type->path);

  // Выполняем если производится удаление.
  if ($op == t('Delete product type')) {
    $form_state['redirect'] = 'admin/structure/site-commerce-types/manage/' . str_replace('_', '-', $type->old_type) . '/delete';
    return FALSE;
  }

  // Сохраняем тип.
  $status = site_commerce_type_save($type);

  // Если производится обновление типа товара.
  if ($status == SAVED_UPDATED) {
    drupal_set_message(t('The products type %name has been updated.', array('%name' => $type->name)));
  }
  // Если производится создание нового типа товара.
  elseif ($status == SAVED_NEW) {
    // Loads a module's installation hooks.
    module_load_install('site_commerce');

    // Создаём поля и сущности для базового типа товара.
    $types = site_commerce_type_get_types();
    _site_commerce_type_installed_instances($types[$type->type]);

    // Назначаем права доступа к модулю. Которые должны быть выставлены по умолчанию.
    site_commerce_default_role_permissions();

    drupal_set_message(t('The products type %name has been added.', array('%name' => $type->name)));
    watchdog('site_commerce', 'Added content type %name.', array('%name' => $type->name), WATCHDOG_NOTICE, l(t('view'), 'admin/structure/site-commerce-types'));

    // Flushes all cached data on the site.
    drupal_flush_all_caches();
  }

  $form_state['redirect'] = 'admin/structure/site-commerce-types';
  return FALSE;
}

/**
 * Menu callback; delete a single content type.
 */
function site_commerce_type_delete_confirm($form, &$form_state, $type) {
  $form['type'] = array('#type' => 'value', '#value' => $type->type);
  $form['name'] = array('#type' => 'value', '#value' => $type->name);

  $message = t('Are you sure you want to delete the content type %type?', array('%type' => $type->name));
  $caption = '';

  $num_nodes = db_query("SELECT COUNT(*) FROM {site_commerce} WHERE type = :type", array(':type' => $type->type))->fetchField();
  if ($num_nodes) {
    $caption .= '<p>' . format_plural($num_nodes, '%type is used by 1 piece of content on your site. If you remove this content type, you will not be able to edit the %type content and it may not display correctly.', '%type is used by @count pieces of content on your site. If you remove %type, you will not be able to edit the %type content and it may not display correctly.', array('%type' => $type->name)) . '</p>';
  }

  $caption .= '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, $message, 'admin/structure/site-commerce-types', $caption, t('Delete'));
}

/**
 * Process content type delete confirm submissions.
 */
function site_commerce_type_delete_confirm_submit($form, &$form_state) {
  site_commerce_type_delete($form_state['values']['type']);

  variable_del('site_commerce_preview_' . $form_state['values']['type']);
  $t_args = array('%name' => $form_state['values']['name']);
  drupal_set_message(t('The content type %name has been deleted.', $t_args));
  watchdog('site_commerce', 'Deleted content type %name.', $t_args, WATCHDOG_NOTICE);

  // Перестраиваем кэш меню.
  menu_rebuild();

  $form_state['redirect'] = 'admin/structure/site-commerce-types';
  return;
}
