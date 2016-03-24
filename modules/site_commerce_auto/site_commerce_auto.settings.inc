<?php

/**
 * @file
 * Admin page callbacks for the settings module.
 */

/**
 * Настройки модуля.
 */
function site_commerce_auto_settings_form($form, &$form_state) {
  $vocabulary = taxonomy_vocabulary_machine_name_load('site_commerce_auto_vocabulary');
  $description_site_commerce_auto_vocabulary = '';
  if (is_object($vocabulary)) {
    $description_site_commerce_auto_vocabulary = l(t('Edit'), 'admin/structure/taxonomy/' . $vocabulary->machine_name);
  }

  //----------------------------------------------------------------------------
  // Общие настройки.
  //----------------------------------------------------------------------------
  $form['site_commerce_auto_basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
    '#collapsible' => FALSE,
    '#collapsed' => TRUE,
  );
  $form['site_commerce_auto_basic']['site_commerce_auto_default_term'] = array(
    '#type' => 'select',
    '#title' => t('Category of the goods by default'),
    '#default_value' => variable_get('site_commerce_auto_default_term'),
    '#options' => kvantstudio_select_terms($vocabulary->vid),
    '#required' => TRUE,
  );

  $form['#submit'][] = 'site_commerce_auto_settings_form_submit';

  $form = system_settings_form($form);
  $form['actions']['submit'] += array(
    '#access' => user_access('administer site commerce'),
  );

  return $form;
}

/**
 * Проверка формы.
 */
function site_commerce_auto_settings_form_validate($form, &$form_state) {
  if (!user_access('administer site commerce')) {
    form_set_error('form', t('Access denied'));
  }
}

/**
 * Сохранение настроек.
 */
function site_commerce_auto_settings_form_submit($form, &$form_state) {
  cache_clear_all();
}