<?php
// $Id:

/**
 * @file
 * Page callbacks for the site_faq module.
 */

/**
 * Функция: отображение новых вопросов.
 */
function site_faq_questions() {

  // Подключаем файлы.
  drupal_add_css(drupal_get_path('module', 'site_faq') .'/css/site_faq.css');
  drupal_add_js(drupal_get_path('module', 'site_faq').'/js/site_faq_admin.js');

  // Подключаем необходимые ajax библиотеки.
  drupal_add_library('system', 'drupal.ajax');
  drupal_add_library('system', 'jquery.form');
  drupal_add_library('system', 'drupal.collapse');

  drupal_add_library('system', 'ui.dialog');
  drupal_add_js('jQuery(document).ready(function(){jQuery("#dialog").dialog({ modal: true });});', 'inline');

  // Формируем breadcrumb ссылки.
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), '<front>');
  $breadcrumb[] = l(t(variable_get('site_faq_view_title', 'Questions and answers')), variable_get('site_faq_link', 'otvety'));
  $breadcrumb[] = l(t(variable_get('site_faq_create_title', 'To ask a question')), variable_get('site_faq_create_link', 'zadat-vopros'));
  $breadcrumb[] = t('New questions');
  drupal_set_breadcrumb($breadcrumb);

  $output = '';

  // Вывод перечня новых вопросов.
  $result = db_query("SELECT * FROM {site_faq_questions} n WHERE n.status = :status", array(':status' => 0));

  $rows_table = '';
  $i = 1;
  foreach ($result as $row) {

    if(strtolower(gettype($i/2)) == 'double') {
      $class_tr = "even";
    } else {
      $class_tr = "odd";
    }

    $qid = $row->qid;
    $question = $row->question;
    $data = unserialize($row->data);

    $contact_data = '';

    if (isset($data['full_name'])) {
      $contact_data .= '<div><b>ФИО:</b> '.$data['full_name'].'</div>';
    }
    if (isset($data['phone'])) {
      $contact_data .= '<div><b>Телефон:</b> '.$data['phone'].'</div>';
    }
    if (isset($data['hostname'])) {
      $contact_data .= '<div><b>IP адрес:</b> '.$data['hostname'].'</div>';
    }
    $form = drupal_get_form('site_faq_answer_form', $qid);
    $rows_table .= '
      <tr id="question-' . $qid . '"  class="' . $class_tr . '">
        <td width="50" align="center">'.$i.'</td>
        <td>
          <div class="text_question">'.$question.'</div>
          '. drupal_render($form) .'
        </td>
        <td width="300" valign="top">
          '.$contact_data.'
        </td>
      </tr>

<!--      <tr id="question-links-' . $qid . '">
        <td colspan="3">
        ' . l(t('To answer'), 'site-faq-answer/site-faq-answer-form-' . $qid . '/nojs', array('attributes' => array('class' => array('use-ajax', 'admin-link'), 'title' => t('To answer a question')))) . '
        ' . l(t('To delete'), 'site-faq-del/' . $qid . '/nojs', array('attributes' => array('class' => array('use-ajax', 'admin-link'), 'title' => t('To remove a question of the FAQ')))) . '
        </td>
      </tr>-->
    ';

    $i++;

  }

  $output .= '
    <table width="100%" border="0" cellpadding="5">
      <tr>
        <td>№</td>
        <td>' . t('Question') . '</td>
        <td>Контактные данные отправителя</td>
      </tr>
      '.$rows_table.'
    </table>
  ';

  return $output;
}

/**
 * Функция: форма регистрация ответа.
 */
function site_faq_answer_form($form, &$form_state, $qid) {

  $form = array();
  $form = array(
    '#prefix' => '<div id="site-faq-answer-form-'.$qid.'">',
    '#suffix' => '</div>'
  );

  $form['site_faq_answer_wrapper'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="site-faq-answer-wrapper-'.$qid.'" >',
    '#suffix' => '</div>',
  );

  $form['text'] = array(
    '#type' => 'textarea',
    '#title' => t('To answer a question'),
    '#rows' => 4,
    '#default_value' => '',
    '#resizable' => FALSE,
    '#ajax' => array(
        'callback' => 'site_faq_answer_form_text_validate',
        'wrapper' => 'site-faq-answer-wrapper-'.$qid,
        'method' => 'html',
        'effect' => 'none',
        'progress' => array(
            'message' => NULL,
        ),
    ),
    '#prefix' => '<div id="site-faq-answer-form-data-'.$qid.'">',
  );

  $form['qid'] = array(
    '#type'  => 'hidden',
    '#value' => $qid,
    '#name'  => 'qid',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#ajax' => array(
      'callback' => 'site_faq_answer_form_ajax_callback',
      'wrapper' => 'site-faq-answer-wrapper-'.$qid,
      'method' => 'append',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber', 'message' => null),
    ),
    '#prefix' => "<div>" . t('Specify in this floor your answer to a question.') . "</div>",
    '#suffix' => '&nbsp;',
  );

  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#ajax' => array(
      'callback' => 'site_faq_answer_form_delete_ajax_callback',
      'wrapper' => 'site-faq-answer-wrapper-'.$qid,
      'method' => 'append',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber', 'message' => null),
    ),
    '#suffix' => '</div>',
  );

  return $form;
}

function site_faq_answer_form_text_validate($form, $form_state) {
    $output = '';
    if (empty($form_state['values']['text'])) {
    $output = "<div class='messages error'>" . t('The field is required to fill the text.') . "</div>";
    }
    return $output;
}

/**
 * Функция: удаление вопроса.
 */
function site_faq_answer_form_delete_ajax_callback($form, &$form_state) {
  $qid = $form_state['values']['qid'];
  // Выполняем операции с БД.
  site_faq_delete_question($qid);

  $commands = array();
  $commands[] = ajax_command_invoke("#site-faq-answer-form-data-".$qid, 'hide');
  $commands[] = ajax_command_invoke("#question-".$qid, 'hide');
  return array('#type' => 'ajax', '#commands' => $commands);
}


function site_faq_answer_form_ajax_callback($form, &$form_state) {

  global $user;

  // Получаем параметры.
  $qid = $form_state['values']['qid'];
  $text = $form_state['values']['text'];

  $commands = array();

  // Выполняем проверку.
  if (empty($text)) {
    $commands[] = ajax_command_replace("#site-faq-answer-wrapper-".$qid, "<div class='messages error'>" . t('The field is required to fill the text.') . "</div>");
    return array('#type' => 'ajax', '#commands' => $commands);
  }

  $count_answer = db_query("SELECT COUNT(*) FROM {site_faq_answers} n WHERE n.qid = :qid", array(':qid' => $qid))->fetchField();
  if (!$count_answer) {
    // Регистрация ответа.
    $result = db_insert('site_faq_answers')
    ->fields(array(
      'qid' => $qid,
      'uid' => $user->uid,
      'answer' => $text,
      'created' => REQUEST_TIME,
    ))
    ->execute();

    // Обновляем статус в таблице параметров вопроса.
    $result = db_update('site_faq_questions')
    ->fields(array('status' => 1))
    ->condition('qid', $qid)
    ->execute();

    if ($result) {
      $commands = array();
      $commands[] = ajax_command_invoke("#site-faq-answer-form-data-".$qid, 'hide');
      $commands[] = ajax_command_invoke("#question-".$qid, 'hide');
      return array('#type' => 'ajax', '#commands' => $commands);
    } else {
      // Отправка системных уведомлений на электронную почту.
      $text = 'Error in function () in file kvantstudio.module';
      if (variable_get('site_developer_mail_allow', true)) {
        kvantstudio_send_developer_report($text);
      }
      watchdog('kvantstudio', $text, NULL, WATCHDOG_ERROR);
      $commands[] = ajax_command_replace("#site-faq-answer-wrapper-".$qid, '<div class="messages error">' . t('On a site there was a mistake. Try to repeat operation once again. If the mistake will repeat, we necessarily shall correct it in the near future.') . '</div>');
      return array('#type' => 'ajax', '#commands' => $commands);
    }
  }
}