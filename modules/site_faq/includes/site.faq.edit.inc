<?php
// $Id:

/**
 * @file
 * Page callbacks for the site_faq module.
 */


/**
 * Функция: форма редактирования вопроса.
 */
function site_faq_edit_form($form, &$form_state, $qid) {

  // Формируем breadcrumb ссылки.
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), '<front>');
  $breadcrumb[] = l(t(variable_get('site_faq_view_title', 'Questions and answers')), variable_get('site_faq_link', 'otvety'));
  $breadcrumb[] = l(t(variable_get('site_faq_create_title', 'To ask a question')), variable_get('site_faq_create_link', 'zadat-vopros'));
  $breadcrumb[] = t('To edit a question and the answer');
  drupal_set_breadcrumb($breadcrumb);

  if (!$qid || !is_numeric($qid)) return FALSE;

  // Выполняем запрос перечня вопросов.
  $query = db_select('site_faq_questions', 'q');
  $query->join('site_faq_answers', 'a', 'q.qid = a.qid');
  $query->fields('q', array('tid', 'qid', 'question'));
  $query->fields('a', array('answer'));
  $query->condition('q.qid', $qid, 'LIKE');

  $result = $query->execute();

  foreach ($result as $row) {
    $tid = $row->tid;
    $question = check_plain($row->question);
    $answer = check_plain($row->answer);
  }

  $form = array();

  // Определяем какие поля обязательные.
  $form_fields_required = array();
  $form_fields_required = variable_get('site_faq_fields_required', array());
  extract($form_fields_required);

  $form['tid'] = array(
    '#type' => 'select',
    '#title' => t('Category of question'),
    '#default_value' => $tid,
    '#options' => kvantstudio_select_terms(variable_get('site_faq_vocabulary', 0)),
    '#required' => $tid_required,
  );

  $form['question'] = array(
    '#type' => 'textarea',
    '#title' => t('Question'),
    '#default_value' => $question,
    '#rows' => 4,
    '#required' => TRUE,
  );

  $form['answer'] = array(
    '#type' => 'textarea',
    '#title' => t('The answer'),
    '#rows' => 4,
    '#default_value' => $answer,
    '#required' => TRUE,
  );

  $form['qid'] = array(
    '#type' => 'hidden',
    '#value' => $qid,
    '#name' => 'qid',
  );

  // Кнопка сохранения результата.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#prefix' => '<div>',
  );

  $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('site_faq_delete_question_submite'),
      '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Функция: валидатор формы редактирования вопроса.
 * Implementation of hook_validate().
 */
function site_faq_edit_form_validate($form, &$form_state) {
  // Проверка на необходимость указывать категорию, к которой принадлежит вопрос.
  // Определяем обязательно ли долна быть выбрана категория вопроса.
  $form_fields_required = array();
  $form_fields_required = variable_get('site_faq_fields_required', array());
  extract($form_fields_required);
  if (isset($form_state['values']['tid']) && $form_state['values']['tid'] == 0 && $tid_field_required && variable_get('site_faq_vocabulary_default_tid', 0) == 0) {
    form_set_error('tid', t('It is necessary to specify necessarily a category which the question concerns.'));
  }
}

/**
 * Функция: обработчик формы редактирования вопрос-ответ.
 * Implementation of hook_submit().
 */
function site_faq_edit_form_submit($form, &$form_state) {

  global $user;

  // Определяем параметры формы.
  $qid = $form_state['values']['qid'];
  $tid = $form_state['values']['tid'];
  $question = trim($form_state['values']['question']);
  $answer = trim($form_state['values']['answer']);

  if ($form_state['values']['tid'] == 0 && variable_get('site_faq_vocabulary_default_tid', 0)) {
    $tid = variable_get('site_faq_vocabulary_default_tid', 0);
  }

  $result = db_update('site_faq_questions')
  ->fields(array(
    'tid' => $tid,
    'question' => $question,
    'updated' => REQUEST_TIME,
  ))
  ->condition('qid', $qid)
  ->execute();

  $result = db_update('site_faq_answers')
  ->fields(array(
    'answer' => $answer,
    'updated' => REQUEST_TIME,
  ))
  ->condition('qid', $qid)
  ->execute();

  drupal_set_message('Изменения успешно сохранены.');

}

/**
 * Функция: редирект на форму подтверждения удаления вопроса.
 */
function site_faq_delete_question_submite($form, &$form_state) {
  if (isset($form_state['values']['qid'])) {
    $form_state['redirect'] = 'site-faq/delete-confirm/' . $form_state['values']['qid'];
  }
}

/**
 * Функция: подтверждение об удалении сообщения.
 */
function site_faq_delete_confirm_form($form, &$form_state, $qid) {

  $name = db_query("SELECT n.title FROM {site_faq_questions} n WHERE n.qid=:qid", array(':qid' => $qid))->fetchField();

  // Формируем breadcrumb ссылки.
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), '<front>');
  $breadcrumb[] = l(t(variable_get('site_faq_view_title', 'Questions and answers')), variable_get('site_faq_link', 'otvety'));
  $breadcrumb[] = l(t($name), 'site-faq/edit/' . $qid);
  $breadcrumb[] = t('Confirming of removal of a question');
  drupal_set_breadcrumb($breadcrumb);

  $form['qid'] = array(
    '#type' => 'value',
    '#value' => $qid,
  );
  return confirm_form(
    $form,
    t('Confirming of removal of a question'),
    variable_get('site_faq_link', 'otvety'),
    t('You wish to remove a question <b>@name</b>? A question will be irrevocably removed!', array('@name' => $name)),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Функция: редирект на форму подтверждения удаления вопроса.
 */
function site_faq_delete_confirm_form_submit($form, &$form_state) {
  site_faq_delete_question($form_state['values']['qid']);
  drupal_set_message(t('The question has been removed.'));
  $form_state['redirect'] = variable_get('site_faq_link', 'otvety');
}