<?php
// $Id$

/**
 * @file
 * Библиотека функций реализации личного кабинета покупателя (находиться в стадии разработки).
 */

/**
 * Функция: отображение страницы регистрации пользователя.
 */
function site_commerce_user_register() {
  // Запрет на индексацию страницы.
  $data = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array('name' => 'robots', 'content' => 'noindex, nofollow'),
  );
  drupal_add_html_head($data, 'site-commerce-cart');

  // Формируем breadcrumb ссылки.
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), '<front>');
  $breadcrumb[] = variable_get('site_commerce_catalog_title', 'Online store');
  drupal_set_breadcrumb($breadcrumb);

  $output = '';

  $output .= drupal_render(drupal_get_form('site_commerce_user_register_form'));

  return $output;
}

/**
 * Форма регистрации пользователя.
 */
function site_commerce_user_register_form($form, &$form_state) {
  $form = array();

  $form['#tree'] = TRUE;

  $form['#attributes'] = array();

  $form['organization'] = array(
    '#type' => 'textfield',
    '#title' => t('Organization'),
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#required' => TRUE,
  );

  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#required' => TRUE,
  );

  // Now we add our submit button, for submitting the form results.
  //
  // The 'actions' wrapper used here isn't strictly necessary for tabledrag,
  // but is included as a Form API recommended practice.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );

  return $form;
}

/**
 * Функция: сабмит формы регистрации пользователя.
 */
function site_commerce_user_register_form_submit($form, &$form_state) {
  // Пароль пользователя.
  $password = user_password();

  // Создаём объект нового пользователя.
  $new_user = array(
    'name' => $form_state['values']['name'],
    'pass' => $password,
    'mail' => $form_state['values']['mail'],
    'init' => $form_state['values']['mail'],
    'field_first_name' => array(LANGUAGE_NONE => array(array('value' => $form_state['values']['field_first_name']))),
    'field_last_name' => array(LANGUAGE_NONE => array(array('value' => $form_state['values']['field_last_name']))),
    'field_phone_number' => array(LANGUAGE_NONE => array(array('value' => $form_state['values']['field_phone_number']))),
    'status' => 0,
    'access' => REQUEST_TIME,
  );

  // $account returns user object
  $account = user_save(null, $new_user);
}