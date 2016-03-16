<?php
// $Id:

/**
 * @file
 * Page callbacks for the site_faq module.
 */

/**
 * Функция: формируем форму поиска вопросов.
 */
function site_faq_search_form($form, &$form_state, $tid, $view = false) {

  if (empty($tid) && is_numeric($tid)) {
    $tid = variable_get('site_faq_vocabulary_default_tid', 0);
  }

  // Количество зарегистрированных вопросов с ответами.
  $count = db_query("SELECT COUNT(*) FROM {site_faq_questions} n WHERE n.status = :status AND n.tid LIKE :tid", array(':status' => 1, ':tid' => $tid))->fetchField();

  if ($count || $view) {

//    // Выводим текст примечания.
//    $form[] = array(
//      '#prefix' => t('Choose a category and push a button TO SHOW QUESTIONS.'),
//    );

    $form['tid'] = array(
      '#type' => 'select',
      '#default_value' => $tid,
      '#options' => kvantstudio_select_terms_faq(variable_get('site_faq_vocabulary', 0)),
      '#required' => TRUE,
    );

    // Кнопка поиска.
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('To show questions'),
      '#suffix' => '&nbsp;',
      '#prefix' => '<div id="site-faq-edit-button-form">',
    );

    $form['create'] = array(
      '#type' => 'submit',
      '#value' => t('To ask a question'),
      '#submit' => array('site_faq_search_form_create'),
      '#suffix' => '</div>'
    );
  }
  else {
     $form['create'] = array(
      '#type' => 'submit',
      '#value' => t('To ask a question'),
      '#submit' => array('site_faq_search_form_create'),
      '#prefix' => '<div id="site-faq-edit-button-form">',
      '#suffix' => '</div>'
    );
  }

  return $form;
}

/**
 * Функция: обработчик нажатия кнопки - $form['create'].
 */
function site_faq_search_form_create($form, &$form_state) {
  drupal_goto(variable_get('site_faq_create_link', 'zadat-vopros'));
}

/**
 * Функция: обработчик формы поиска.
 */
function site_faq_search_form_submit($form, &$form_state) {
  $tid = $form_state['values']['tid'];
  $source = 'taxonomy/term/' . $tid;
  if (empty($tid)) {
    $source = variable_get('site_faq_link', 'otvety');
  }
  $form_state['redirect'] = $source;
}

/**
 * Функция: редирект формирование перечня вопросов и ответов.
 */
function site_faq_taxonomy_term_page($tid) {
  return site_faq_view($tid);
}

/**
 * Функция: формирование перечня вопросов и ответов.
 */
function site_faq_view($tid = NULL, $qid = NULL) {
  // Подключаем файлы.
  drupal_add_css(drupal_get_path('module', 'site_faq') . '/css/site_faq.css');
  drupal_add_js(drupal_get_path('module', 'site_faq') . '/js/site_faq.js');

  // Формируем breadcrumb ссылки.
  $term = '';
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), '<front>');
  if (isset($tid) && is_numeric($tid)) {
    $breadcrumb[] = l(t(variable_get('site_faq_view_title', 'Questions and answers')), variable_get('site_faq_link', 'otvety'));
    $tidn = $tid;
    $term = taxonomy_term_load($tidn);
    $terms_parents = taxonomy_get_parents($tid);
    foreach ($terms_parents as $term_parent) {
      $alias = db_query("SELECT n.alias FROM {url_alias} n WHERE n.source=:source", array(':source' => 'taxonomy/term/' . $term_parent->tid))->fetchField();
      $breadcrumb[] = l(t($term_parent->name), $alias);
    }
    //$breadcrumb[] = t($term->name);
  }
  drupal_set_breadcrumb($breadcrumb);

  // Устанавливаем заголовок.
  if (is_object($term)) {
    drupal_set_title(t($term->name));
  }

  // Номер термина, по которому будет осуществляться поиск.
  if (empty($tid) || $tid == 0) $tid = '%';
  if (empty($qid)) $qid  = '%';

  $output = '';
  $line = '';
  $name_terms = '';
  $question_form = '';

  // Формируем заголовок текущего раздела FAQ.
  if ($tid == '%') {
    $name_terms = t('The category is not chosen, questions are displayed all.') . "&nbsp;";
    $name_terms .= t('Choose a category and push a button TO SHOW QUESTIONS.');
  }
  else {
    $line = '&nbsp;<img src="/'.drupal_get_path('module', 'kvantstudio') . '/images/icon_link.png" alt="Разделитель" align="absmiddle" />&nbsp;';
    $n = 0;
    foreach ($terms_parents as $term_parent) {
      $alias = db_query("SELECT n.alias FROM {url_alias} n WHERE n.source=:source", array(':source' => 'taxonomy/term/' . $term_parent->tid))->fetchField();
      $name_terms .= l(t($term_parent->name), $alias);
      $name_terms .= $line;

    }
    $name_terms .= t($term->name);
  }

  // Подсчитываем есть ли доступные для просмотра вопросы.
  $count = db_query("SELECT COUNT(*) FROM {site_faq_questions} n WHERE n.status = :status AND n.tid LIKE :tid", array(':status' => 1, ':tid' => $tid))->fetchField();

  if ($count > 0) {
    $name_terms = $name_terms . '<br /><span class="site-faq-annotation">' . t('it is set questions - @count', array('@count' => $count)) . '</span>';

    $output .= '<div class="title-section-faq">' . $name_terms . '</div>';
    $form = drupal_get_form('site_faq_search_form', $tid);
    $output .= drupal_render($form);

    $question_form .= site_faq_questions_load($tid, $qid);

  } else {
    if (is_numeric($tid)) {
      // Определяем имеются ли дочерние элементы у текущего каталога.
      if (count(taxonomy_get_children($tid))) {

        $vid = variable_get('site_faq_vocabulary', 0);
        $taxonomy_object = taxonomy_get_tree($vid, $tid);
        $level = 0;
        $count_term = 0;

        foreach ($taxonomy_object as $term_view) {
          $question_form .= site_faq_questions_load($term_view->tid, $qid, true);
          $count_term = $count_term + db_query("SELECT COUNT(*) FROM {site_faq_questions} n WHERE n.status = :status AND n.tid LIKE :tid", array(':status' => 1, ':tid' => $term_view->tid))->fetchField();
        }

        $name_terms = $term->name . '<br /><span class="site-faq-annotation">' . t('it is set questions - @count', array('@count' => $count_term)). '</span>';

        $output .= '<div class="title-section-faq">' . $name_terms . '</div>';

        $output .= drupal_render(drupal_get_form('site_faq_search_form', $tid, true));

      } else {

        $name_terms = t('It is not set any question. You can be the first who will ask to us a question!');
        $output .= '<div class="title-section-faq">' . $name_terms . '</div>';
        $output .= drupal_render(drupal_get_form('site_faq_search_form', $tid));

      }

    } else {
      $name_terms = t('It is not set any question. You can be the first who will ask to us a question!');
      $output .= '<div class="title-section-faq">' . $name_terms . '</div>';
      $output .= drupal_render(drupal_get_form('site_faq_search_form', $tid));
    }
  }

  $output .= theme('pager');
  $output .= $question_form;
  $output .= theme('pager');

  return $output;

}

function site_faq_questions_load($tid, $qid = null, $set_name_term = false) {

  $question_form = '';

  // Устанавливаем заголовок.
  if ($set_name_term) {
    // Формируем название категории вопроса если отображаются все вопросы.
    $term = taxonomy_term_load($tid);
    $question_form .= '<div class="title-section-faq">' . $term->name . '</div>';
  }

  // Выполняем запрос перечня вопросов.
  $query = db_select('site_faq_questions', 'q');
  $query->join('site_faq_answers', 'a', 'q.qid = a.qid');
  $query->fields('q', array('tid', 'qid', 'question', 'data' , 'created'));
  $query->fields('a', array('answer'));
  $query->condition('q.status', '1');
  $query->condition('q.tid', $tid, 'LIKE');
  $query->condition('q.qid', $qid, 'LIKE');
  $query->orderBy('q.qid', 'DESC');
  $query = $query->extend('PagerDefault')->limit(variable_get('site_faq_count_on_page', 10));
  $result = $query->execute();

  foreach ($result as $row) {

    $qid = $row->qid;
    $question = check_plain($row->question);
    $data = unserialize($row->data);

    $contact_data = '';

    if (isset($data['full_name'])) {
      $contact_data = '' . t('Author') . ':&nbsp;' . $data['full_name'];
    }

    // Формируем название категории вопроса если отображаются все вопросы.
    $name = db_query("SELECT n.name FROM {taxonomy_term_data} n WHERE n.tid = :tid", array(':tid' => $row->tid))->fetchField();

    if (!empty($name)) {
      // Формируем ссылку на категорию.
      $source = 'taxonomy/term/' . $row->tid;
      $alias = db_query("SELECT n.alias FROM {url_alias} n WHERE n.source = :source", array(':source' => $source))->fetchField();
      if ($alias) {
        $source = $alias;
      }

      // Формируем вопрос.
      $question_form .= '
        <div class="question">
          <div class="text-question">' . $question . '</div>
          <div class="site-faq-annotation">' . t('Category') . ' :' . l($name, $source) . '</div>
          <div class="site-faq-annotation">' . $contact_data . '</div>
          <div class="site-faq-annotation">' . t('Date of registration') . ':&nbsp;' . format_date($row->created, 'custom', 'd.m.Y') . '</div>
        </div>

        <div class="answer"><b>Ответ:</b><br />' . $row->answer . '</div>
      ';
    }

    // Добавляем опции администрирования.
    if (user_access('site faq settings')) {
      $form = drupal_get_form('site_faq_edit_button_form', $row->qid);
      $question_form .= drupal_render($form);
    }

  }

  return $question_form;
}

/**
 * Функция: формируем форму регистрации вопроса.
 */
function site_faq_edit_button_form($form, &$form_state, $qid) {

  $form['qid'] = array(
    '#name' => 'qid',
    '#type' => 'hidden',
    '#value' => $qid,
  );

  // Кнопка поиска.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Edit'),
    '#prefix' => '<div id="site-faq-edit-button-form">',
    '#suffix' => '</div>'
  );

  return $form;
}

/**
 * Функция: обработчик формы поиска.
 */
function site_faq_edit_button_form_submit($form, &$form_state) {
  if ($form_state['values']['qid'] > 0) {
    $form_state['redirect'] = 'site-faq/edit/' . $form_state['values']['qid'];
  }
}