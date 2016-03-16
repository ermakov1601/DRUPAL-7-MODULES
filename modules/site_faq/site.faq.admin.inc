<?php
// $Id:

/**
 * @file
 * Admin page callbacks for the site_send_messages module.
 */

/**
 * Функция: форма вызова настроек модуля - отправка сообщений с сайта.
 */
function site_faq_adjustments_form($form, &$form_state) {

  // Определяем fieldsets.
  $form['site_faq_adjustments'] = array(
    '#type' => 'fieldset',
    '#title' => 'Основные настройки',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['site_faq_systems_pages'] = array(
    '#type' => 'fieldset',
    '#title' => 'Настройки названия страниц',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['site_faq_systems_links'] = array(
    '#type' => 'fieldset',
    '#title' => 'Настройки названия ссылок',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['site_faq_adjustments']['site_faq_vocabulary'] = array(
    '#type' => 'select',
    '#title' => 'Название словаря содержащего перечень категорий, к которым будут привязываться вопросы посетителей',
    '#default_value' => variable_get('site_faq_vocabulary', 0),
    '#options' => kvantstudio_select_vocabularies(),
    '#required' => TRUE,
    '#description'   => l('Редактировать категории', 'admin/structure/taxonomy/site_faq_vocabulary'), 
  );

  $form['site_faq_adjustments']['site_faq_vocabulary_default_tid'] = array(
    '#type' => 'select',
    '#title' => 'Название категории, которая будет выбрана по умолчанию для вопросов без категории.',
    '#default_value' => variable_get('site_faq_vocabulary_default_tid'),
    '#options' => kvantstudio_select_terms(variable_get('site_faq_vocabulary')),
    '#required' => TRUE,
  );

  $form['site_faq_adjustments']['site_faq_count_on_page'] = array(
    '#type' => 'textfield',
    '#title' => 'Количество отображаемых вопросов на одной странице для посетителей.',
    '#default_value' => variable_get('site_faq_count_on_page', 10),
    '#required' => TRUE,
  );

  $form['site_faq_adjustments']['site_faq_count_last_questions'] = array(
    '#type' => 'textfield',
    '#title' => 'Количество отображаемых вопросов в блоке последние вопросы.',
    '#default_value' => variable_get('site_faq_count_last_questions', 5),
    '#required' => TRUE,
  );

  $form['site_faq_adjustments']['site_faq_messages_captcha'] = array(
    '#title' => 'Разрешить защиту от спама средствами модуля CAPTCHA?',
    '#type' => 'checkbox',
    '#default_value' => variable_get('site_faq_messages_captcha', false),
    '#description'   => 'Если отмечено, защита от спама будет включена.'
  );

  $form['site_faq_adjustments']['site_faq_view_count_category'] = array(
    '#title' => 'Разрешить отображать количество вопросов у категорий в виде раскрывающегося списка?',
    '#type' => 'checkbox',
    '#default_value' => variable_get('site_faq_view_count_category', false),
    '#description'   => 'Если отмечено, будет отображаться количество вопросов принадлежащих той или иной категории.'
  );

  $form['site_faq_adjustments']['site_faq_note'] = array(
    '#type' => 'textarea',
    '#title' => 'Текст примечания перед отправкой вопроса',
    '#default_value' => t(variable_get('site_faq_note', 'The contact information is not published and used anywhere only for answers to your messages.')),
    '#rows' => 3,
    '#required' => FALSE,
  );

  $form['site_commerce_order']['site_faq_fields'] = array(
    '#type' => 'checkboxes',
    '#return_value' => 'fields',
    '#title' => t('Доступные поля формы при регистрации вопроса'),
    '#default_value' => variable_get('site_faq_fields', array('first_name', 'mail')),
    '#options' => array(
      'tid' => t('Category of question'),
      'full_name' => t('Full name'),
      'last_name' => t('Last name'),
      'first_name' => t('First name'),
      'patronymic' => t('Patronymic'),
      'phone' => t('Contact phone'),
      'mail' => t('E-mail'),
    ),
  );
  $form['site_commerce_order']['site_faq_fields_required'] = array(
    '#type' => 'checkboxes',
    '#return_value' => 'fields',
    '#title' => t('Обязательные поля формы регистрации вопроса'),
    '#default_value' => variable_get('site_faq_fields_required', array('first_name_required')),
    '#options' => array(
      'tid_required' => t('Category of question'),
      'full_name_required' => t('Full name'),
      'last_name_required' => t('Last name'),
      'first_name_required' => t('First name'),
      'patronymic_required' => t('Patronymic'),
      'phone_required' => t('Contact phone'),
      'mail_required' => t('E-mail'),
    ),
  );

  // Настройка названия страниц.
  $form['site_faq_systems_pages']['site_faq_view_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Заголовок страницы с перечнем вопросов и ответов',
    '#description' => 'При изменении значения не забудьте обновить кэш сайта.',
    '#default_value' => variable_get('site_faq_view_title', 'Questions and answers'),
    '#required' => TRUE,
  );
  $form['site_faq_systems_pages']['site_faq_create_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Заголовок страницы, чтобы задать вопрос',
    '#description' => 'При изменении значения не забудьте обновить кэш сайта.',
    '#default_value' => variable_get('site_faq_create_title', 'To ask a question'),
    '#required' => TRUE,
  );

  // Настройки названия ссылок (не изменять на работающем сайте!)
  $form['site_faq_systems_links']['site_faq_link'] = array(
    '#type' => 'textfield',
    '#title' => 'Название ссылки страницы с перечнем вопросов и ответов',
    '#description' => 'Не изменяйте это значение на готовом сайте! Значение должно быть на латинице, слова через тире с маленькой буквы. При изменении значения не забудьте обновить кэш сайта.',
    '#default_value' => variable_get('site_faq_link', 'otvety'),
    '#required' => TRUE,
  );
  $form['site_faq_systems_links']['site_faq_create_link'] = array(
    '#type' => 'textfield',
    '#title' => 'Название ссылки страницы, чтобы задать вопрос',
    '#description' => 'Не изменяйте это значение на готовом сайте! Значение должно быть на латинице, слова через тире с маленькой буквы. При изменении значения не забудьте обновить кэш сайта.',
    '#default_value' => variable_get('site_faq_create_link', 'zadat-vopros'),
    '#required' => TRUE,
  );

  return system_settings_form($form);

}