<?php
// $Id:

/**
 * Отображение всех вопросов и ответов.
 */
function site_faq_categories($tid = NULL) {
  // Подключаем зависимые файлы.
  drupal_add_css(drupal_get_path('module', 'site_faq') .'/css/site_faq.css');

  // Формируем breadcrumb ссылки.
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), '<front>');
  $breadcrumb[] = l(t(variable_get('site_news_title', 'The press centre')), variable_get('site_news_link', 'press-centr'));
  drupal_set_breadcrumb($breadcrumb);

  $output = '';

  // Описание страницы.
  if (variable_get('site_faq_note_catalog_description')) {
    $output  .= '<div class="site-faq-note-catalog-description">' . t(variable_get('site_faq_note_catalog_description', 'Welcome to our online store!')) . '</div>';
  }

  // Определяем словарь.
  $vid = variable_get('site_faq_vocabulary');

  $array_tid = array();

  $output = '';

  // Форма регистрации вопроса.
  $output .= site_faq_create();

  // Определяем категории вопросов.
  $result = db_query("SELECT n.tid FROM {taxonomy_term_data} n WHERE n.vid = :vid", array(':vid' => $vid));
  foreach ($result as $key) {
    $array_tid[] = $key->tid;
  }

  // Если есть категории для вывода.
  if (count($array_tid)) {
    // Выбираем таблицу.
    $query = db_select('node', 'n');
    // Выбираем поля и условия.
    $query->fields('n', array('nid', 'title', 'created'));
    $query->condition('n.type', 'site_faq');
    $query->condition('n.status', 1);
    // Связываем таблицы.
    $query->innerJoin('taxonomy_index', 't', 'n.nid = t.nid');
    $query->condition('t.tid', $array_tid, 'IN');
    // Сортировать по.
    $query->orderBy('nid', 'DESC');
    // Для нумерации страниц.
    $query = $query->extend('PagerDefault')->limit(20);
    $result = $query->execute();

    $output .= '<ul class="site-faq-categories">';
    foreach ($result as $row) {

      $node = node_load($row->nid);

      $value = site_faq_node_summary($node);
      if (!empty($node->field_site_faq_creator)) {
        $creator = $node->field_site_faq_creator['und'][0]['value'];
      }
      else {
        $creator = t('is not specified');
      }

      $output .= '<li>';
      $output .= l($row->title, 'node/' . $row->nid, array('attributes' => array('class' => array('site-faq-link'))));
      $output .= '<div class="site-faq-date">' . format_date($row->created, 'short') . ' | ' . t('Creator') . ': ' . $creator . '</div>';
      if ($value) {
        $output .= '<div class="site-faq-value">' . $value . '</div>';
      }
      // Отображение кнопок Яндекс на ссылки в социальные сети.
      if (variable_get('site_faq_yandex_buttons')) {
        $uri = kvantstudio_node_uri($node);
        $output .= '<div class="yashare-auto-init" data-yashareLink="' . $uri . '" data-yashareTitle="' . $row->title . '" data-yashareL10n="ru" data-yashareType="icon" data-yashareQuickServices="vkontakte,facebook,twitter,odnoklassniki,moimir,lj,gplus"></div>';
      }
      $output .= '</li>';
    }
    $output .= '</ul>';

    // Выводим постраничную навигацию.
    $output .= theme('pager', array('quantity' => 3));
  }

  return $output;
}

/**
 * Вспомогательная функция вызова Формы регистрации вопроса.
 */
function site_faq_create($button = TRUE) {
  drupal_add_library('system', 'drupal.ajax');
  drupal_add_library('system', 'jquery.form');

  // Подключаем библиотеку javascript модуля site_send_messages.js.
  drupal_add_js(drupal_get_path('module', 'site_faq') . '/js/site_faq.js');

  $output = '';

  $form = drupal_get_form('site_faq_create_form');
  $form = drupal_render($form);

  if ($button) {
    $output .= l(t('To ask a question'), '', array('fragment' => 'site-faq-create-form-wrapper', 'external' => TRUE, 'attributes' => array('class' => array('site-faq-create-button'))));
  }

  $output .= '<div style="display: none;"><div id="site-faq-create-form-wrapper">' . $form . '</div></div>';

  return $output;

}