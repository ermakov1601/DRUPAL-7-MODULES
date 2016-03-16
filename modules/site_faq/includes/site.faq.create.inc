<?php
// $Id:

/**
 * @file
 * Page callbacks for the site_faq module.
 */

/**
 * Функция: формируем форму регистрации вопроса.
 */
function site_faq_create() {
  // Формируем breadcrumb ссылки.
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), '<front>');
  $breadcrumb[] = l(t(variable_get('site_faq_view_title', 'Questions and answers')), variable_get('site_faq_link', 'otvety'));
  drupal_set_breadcrumb($breadcrumb);

  $form = drupal_get_form('site_faq_create_form');
  return drupal_render($form);
}

/**
 * Функция: формируем форму регистрации вопроса.
 */
function site_faq_create_form($form, &$form_state) {

  // Определяем какие поля отображать.
  $form_fields = array();
  $form_fields = variable_get('site_faq_fields');
  extract($form_fields);

  // Определяем какие поля обязательные.
  $form_fields_required = array();
  $form_fields_required = variable_get('site_faq_fields_required');
  extract($form_fields_required);

  $form = array();

  // Выводим текст примечания.
  $form[] = array(
    '#prefix' => '<div>' . t(variable_get('site_faq_note', 'The contact information is not published and used anywhere only for answers to your messages.')) . '</div>',
  );

  $form['user'] = array(
    '#type' => 'fieldset',
    '#title' => t('The contact information'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['user']['full_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Full name'),
    '#default_value' => isset($profile->full_name) ? $profile->full_name : NULL,
    '#maxlength' => 255,
    '#required' => isset($full_name_required) ? $full_name_required : FALSE,
    '#access' => isset($full_name) ? $full_name : FALSE,
  );

  $form['user']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#default_value' => isset($profile->last_name) ? $profile->last_name : NULL,
    '#maxlength' => 255,
    '#required' => isset($last_name_required) ? $last_name_required : FALSE,
    '#access' => isset($last_name) ? $last_name : FALSE,
  );

  $form['user']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#default_value' => isset($profile->first_name) ? $profile->first_name : NULL,
    '#maxlength' => 255,
    '#required' => isset($first_name_required) ? $first_name_required : FALSE,
    '#access' => isset($first_name) ? $first_name : FALSE,
  );

  $form['user']['patronymic'] = array(
    '#type' => 'textfield',
    '#title' => t('Patronymic'),
    '#default_value' => isset($profile->patronymic) ? $profile->patronymic : NULL,
    '#maxlength' => 255,
    '#required' => isset($patronymic_required) ? $patronymic_required : FALSE,
    '#access' => isset($patronymic) ? $patronymic : FALSE,
  );

  $form['user']['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Contact phone'),
    '#default_value' => isset($profile->phone) ? $profile->phone : NULL,
    '#maxlength' => 255,
    '#required' => isset($phone_required) ? $phone_required : FALSE,
    '#access' => isset($phone) ? $phone : FALSE,
    '#attributes' => array(
      'class' => array('site-commerce-order-form-item-phone'),
    ),
  );

  $form['user']['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
    '#maxlength' => 255,
    '#required' => isset($mail_required) ? $mail_required : FALSE,
    '#access' => isset($mail) ? $mail : FALSE,
  );

  $form['tid'] = array(
    '#type' => 'select',
    '#title' => t('Category of question'),
    '#default_value' => variable_get('site_faq_vocabulary_default_tid'),
    '#options' => kvantstudio_select_terms(variable_get('site_faq_vocabulary', 0)),
    '#required' => isset($tid_required) ? $tid_required : FALSE,
    '#access' => isset($tid) ? $tid : FALSE,
  );

  $form['question'] = array(
    '#type' => 'textarea',
    '#title' => t('Question'),
    '#rows' => 5,
    '#required' => TRUE,
  );

  // Определяем вывод Captcha.
  if (module_exists('captcha') && variable_get('site_faq_captcha', false)) {
    $form['captcha'] = array(
      '#type' => 'captcha',
      '#captcha_type' => 'image_captcha/Image',
    );
  }

  // Кнопка сохранения результата.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('To send a question'),
    '#prefix' => '<div class="site-commerce-annotation">' . t('The fields marked *, it is necessary to fill necessarily.') . '</div>',
  );

  return $form;
}

/**
 * Функция: валидатор формы регистрации вопроса.
 * Implementation of hook_validate().
 */
function site_faq_create_form_validate($form, &$form_state) {
  // Проверка на необходимость указывать категорию, к которой принадлежит вопрос.
  // Определяем обязательно ли долна быть выбрана категория вопроса.
  $form_fields_required = array();
  $form_fields_required = variable_get('site_faq_fields_required');
  extract($form_fields_required);
  if (isset($form_state['values']['tid']) && $form_state['values']['tid'] == 0 && $tid_field_required && variable_get('site_faq_vocabulary_default_tid') == 0) {
    form_set_error('tid', t('It is necessary to specify necessarily a category which the question concerns.'));
  }

  // Проверка mail на корректность ввода.
  if (isset($form_state['values']['mail']) && $form_state['values']['mail']) {
    if (!valid_email_address($form_state['values']['mail'])) {
      form_set_error('mail', t('The e-mail address %mail is not valid.', array('%mail' => $form_state['values']['mail'])));
    }
  }
}

/**
 * Обработчик формы регистрации вопроса.
 */
function site_faq_create_form_submit($form, &$form_state) {

  global $user;

  $profile = new stdClass();
  $profile->full_name = null;
  $profile->last_name = null;
  $profile->first_name = null;
  $profile->patronymic = null;
  $profile->phone = null;
  $profile->mail = null;

  // Определяем переменные.
  if (isset($form_state['values']['full_name'])) {
    $profile->full_name = trim($form_state['values']['full_name']);// ФИО.
  }

  if (isset($form_state['values']['last_name'])) {
    $profile->last_name = trim($form_state['values']['last_name']);// Фамилия.
  }

  if (isset($form_state['values']['first_name'])) {
    $profile->first_name = trim($form_state['values']['first_name']);// Имя.
  }

  if (isset($form_state['values']['patronymic'])) {
    $profile->patronymic = trim($form_state['values']['patronymic']);// Отчество.
  }

  if (isset($form_state['values']['phone'])) {
    $profile->phone = trim($form_state['values']['phone']);// Контактный телефон пользователя.
  }

  if (isset($form_state['values']['mail'])) {
    $profile->mail = trim($form_state['values']['mail']);// Адресс электронной почты пользователя.
  }

  if (isset($form_state['values']['tid'])) {
    $tid = $form_state['values']['tid'];// Категория к которой принадлежит вопрос.
  }

  $question = trim($form_state['values']['question']);// Текст вопроса.

  // Определяем привязку вопроса к категории.
  if (empty($tid)) {
    $tid = variable_get('site_faq_vocabulary_default_tid');
  }

  // Определяем ФИО.
  if (empty($profile->full_name) && empty($profile->first_name)) {
    $profile->full_name = t('The visitor of a site');
  }
  else if (empty($profile->full_name) && !empty($profile->first_name)) {
    $profile->full_name = trim($profile->last_name) . ' ' . trim($profile->first_name) . ' ' . trim($profile->patronymic);
    $profile->full_name = trim($profile->full_name);
  }

  // Подготавливаем массив с дополнительными параметрами.
  $data = serialize(array(
    'full_name' => $profile->full_name,
    'phone' => $profile->phone,
    'mail' => $profile->mail,
    'hostname' => $_SERVER["REMOTE_ADDR"],
  ));

  global $language;

  db_insert('site_faq_questions')
  ->fields(array(
    'tid' => $tid,
    'question' => $question,
    'data' => $data,
    'language' => $language->language,
    'created' => REQUEST_TIME,
  ))
  ->execute();

  // Формируем html тело сообщения.
  $body = '<div style="color:#FFF;background-color:#036;padding:10px;font-size:14px;font-family:Arial;">' . t('The contact information') . ':</div>';

  $body .= '<ul>';

    if ($profile->full_name) {
      $body .= '<li><b>' . t('Full name') . ':</b>&nbsp;' . check_plain($profile->full_name) . '</li>';
    }

    if ($profile->mail) {
      $body .= '<li><b>E-mail:</b>&nbsp;<a href="mailto:' . check_plain($profile->mail) . '">'. check_plain($profile->mail) . '</a></li>';
    }

    if ($profile->phone) {
      $body .= '<li><b>' . t('Contact phone') . ':</b>&nbsp;' . check_plain($profile->phone) . '</li>';
    }

  $body .= '</ul>';

  $body .= '<b>' . t('Question') . ':</b><p>' . $question . '</p>';

  // Отправка сообщения менеджеру.
  kvantstudio_mail_send(array(
    'module'      => 'kvantstudio',
    'key'         => 'kvantstudio_manager_mail',
    'to'          => '',
    'from'        => '',
    'subject'     => variable_get('site_name', '') . ': ' . t('the new question from the visitor of a site has acted on consideration'),
    'body'        => $body,
    'set_message' => FALSE,
    'attachments' => array(),
  ));

//  // Отправка сообщения посетителю.
//  if ($profile->mail) {
//    // Информация для покупателя.
//    if (variable_get('site_commerce_note_order_description')) {
//      $body .= '<div style="margin:10px;padding:10px;background-color:#CCC;border:1px bolder #c3c3c3;border-radius:10px">' . variable_get('site_commerce_note_order_description') . '</div>';
//    }
//    kvantstudio_mail_send(array(
//      'module'      => 'kvantstudio',
//      'key'         => 'site_send_order',
//      'to'          => $profile->mail,
//      'from'        => '',
//      'subject'     => variable_get('site_name', '') . ': ' . t('the order is accepted'),
//      'body'        => $body,
//      'set_message' => FALSE,
//      'attachments' => array(),
//    ));
//  }

  drupal_set_message(t('Thanks for your question. It will be published after check.'));
  $form_state['redirect'] = variable_get('site_faq_link', 'otvety');
}