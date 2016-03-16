<?php

/**
 * Формирует отображение перечня категорий пресс-центра.
 */
function site_news_press_center() {
  $output = "";

  // Выполняем редирект к странице настроек категорий новостей.
  // Если нет категорий.
  if (!site_news_term_count()) {
    drupal_set_message(t('Add on this page of a category of the press centre. For example, news of the company.'), 'warning');
    drupal_goto('admin/structure/taxonomy/site_news_vocabulary');
  }

  // Ссылки на категории пресс-центра предоставляемые модулем.
  $output .= '<p>' . t('Welcome in our press centre. Choose an interesting category.') . '</p>';

  // Формируем категорий пресс-центра.
  if (site_news_term_count()) {
    $tree = site_news_taxonomy_tree('site_news_vocabulary');
    if ($tree) {
      $output .= $tree;
    }
  }

  // Ссылки на категории пресс-центра, которые предоставляют другие модули.
  $data = module_invoke_all('site_news_press_center');
  if (count($data)) {
    $output .= theme('item_list', array('items' => $data));
  }

  return $output;
}

/**
 * Функция: отображение перечня материалов по заданной категории.
 */
function site_news_category($tid) {
  global $language;

  module_load_include('inc', 'pathauto');

  // Создаем обьекты термина.
  $term = taxonomy_term_load($tid);
  $parents = array_reverse(taxonomy_get_parents_all($tid));
  if (empty($term->alias)) {
    $term->alias = 'taxonomy/term/' . $term->tid;
  }

  // Количество терминов.
  $term_data = site_news_taxonomy_tree('site_news_vocabulary');

  if (isset($term->language) || $term->language == 'und') {
    $langcode = $language->language;
  }
  else {
    $langcode = $language->language;
  }

  // Если активирован модуль поддержки перевода переменных (для мультиязычных сайтов).
  if (module_exists("i18n_variable")) {
    // Заголовок на текущем языке сайта.
    $title = i18n_variable_get('site_news_title', $language->language);
  }
  else {
    // Заголовок по умолчанию для одного языка.
    $title = variable_get('site_news_title', t('Press center'));
  }

  $path = pathauto_cleanstring($title);

  // Количество терминов.
  $term_count = site_news_term_count();

  // Формируем breadcrumb ссылки.
  $breadcrumb = drupal_get_breadcrumb();
  $count_breadcrumb = count($breadcrumb) + 1;
  $title = '<span itemprop="name">' . $title . '</span><meta itemprop="position" content="' . $count_breadcrumb . '" />';
  $breadcrumb[] = l($title, $path, array('html' => TRUE, 'attributes' => array('itemprop' => 'item')));
  if ($term_count) {
    while (count($parents) - 1) {
      $current = array_shift($parents);
      $alias = db_query("SELECT n.alias FROM {url_alias} n WHERE n.source=:source", array(':source' => 'taxonomy/term/' . $current->tid))->fetchField();

      $count_breadcrumb++;
      $title = '<span itemprop="name">' . $current->name . '</span><meta itemprop="position" content="' . $count_breadcrumb . '" />';
      $breadcrumb[] = l($title, $alias, array('html' => TRUE, 'attributes' => array('itemprop' => 'item')));
    }
  }
  drupal_set_breadcrumb($breadcrumb);

  // Устанавливаем заголовок страницы.
  drupal_set_title($term->name, PASS_THROUGH);

  // Если разрешено отображать новости, то продолжаем выполнение скрипта.
  if (user_access('administer site news') || user_access('view site news content')) {

    $array_tid = array();

    $output = '';

    // Определяем есть ли в текущей категории позиции.
    $count_site_commerce = db_query("SELECT COUNT(n.nid) FROM {taxonomy_index} n WHERE n.tid = :tid", array(':tid' => $tid))->fetchField();
    if ($count_site_commerce > 0) {
      $array_tid = array($tid);
    }
    else {
      // Определяем имеются ли дочерние элементы у текущего каталога.
      $count_children = db_query("SELECT COUNT(*) FROM {taxonomy_term_hierarchy} n WHERE n.parent = :parent", array(':parent' => $tid))->fetchField();
      if ($count_children) {
        $result = kvantstudio_taxonomy_get_children_all($tid);
        foreach ($result as $key) {
          $array_tid[] = $key->tid;
        }
      }
    }

    // Если есть категории для вывода.
    if (count($array_tid)) {
      // Выбираем таблицу.
      $query = db_select('node', 'n');
      // Выбираем поля и условия.
      $query->fields('n', array('nid', 'title', 'created'));
      $query->condition('n.type', 'site_news');
      $query->condition('n.status', 1);
      $query->condition('n.sticky', 0);
      // Связываем таблицы.
      $query->innerJoin('taxonomy_index', 't', 'n.nid = t.nid');
      $query->fields('t', array('tid'));
      $query->condition('t.tid', $array_tid, 'IN');
      // Сортировать по.
      $query->orderBy('n.created', 'DESC');
      // Для нумерации страниц.
      $query = $query->extend('PagerDefault')->limit(20);
      $result = $query->execute();

      // Выводим новости, которые закреплены в списке.
      $output .= site_news_sticky_category($array_tid);

      // Выводим постраничную навигацию.
      $output .= theme('pager', array('quantity' => 3));

      $output .= '<ul class="news">';
      foreach ($result as $row) {

        $node = node_load($row->nid);

        // Опрределяем разрешено ли отображать полный текст публикации.
        $array_summary_with_body = variable_get('site_news_summary_with_body_taxonomy_terms', array());
        $full_view = FALSE;
        if (in_array($row->tid, $array_summary_with_body)) {
          $full_view = TRUE;
        }

        $max_length = variable_get('site_news_summary_lenght', '300');
        $summary_view = variable_get('site_news_summary_taxonomy_terms', TRUE);
        $value = kvantstudio_node_summary($node, $max_length, TRUE, $full_view, $summary_view);

        $output .= '<li>';
        if (variable_get('node_submitted_' . $node->type, TRUE)) {
          $output .= '<div class="site-news-date">' . format_date($row->created, 'short') . '</div>';
        }
        $output .= '<div class="site-news-link-wrapper">' . l(t('@value', array('@value' => $row->title)), 'node/' . $row->nid, array('attributes' => array('class' => array('site-news-link')))) . '</div>';
        if ($value) {
          $output .= '<div class="site-news-summary">' . $value . '</div>';
        }
        $output .= '</li>';
      }
      $output .= '</ul>';

      // Выводим постраничную навигацию.
      $output .= theme('pager', array('quantity' => 3));
    }
  }
  else {
    $output = '<div class="site-news-note-if-access-denied">' . t('Access denied') . '</div>';
  }
  return $output;
}

/**
 * Функция: отображение перечня новостей по заданной категории, которые закреплены в списке.
 */
function site_news_sticky_category($array_tid) {
  $output = '';

  // Если есть категории для вывода.
  if (count($array_tid)) {
    // Выбираем таблицу.
    $query = db_select('node', 'n');
    // Выбираем поля и условия.
    $query->fields('n', array('nid', 'title', 'created'));
    $query->condition('n.type', 'site_news');
    $query->condition('n.status', 1);
    $query->condition('n.sticky', 1);
    // Связываем таблицы.
    $query->innerJoin('taxonomy_index', 't', 'n.nid = t.nid');
    $query->fields('t', array('tid'));
    $query->condition('t.tid', $array_tid, 'IN');
    // Сортировать по.
    $query->orderBy('n.created', 'DESC');
    $result = $query->execute();

    if ($result->rowCount()) {
      $output .= '<ul class="news-sticky">';
      foreach ($result as $row) {

        $node = node_load($row->nid);

        // Опрределяем разрешено ли отображать полный текст публикации.
        $array_summary_with_body = variable_get('site_news_summary_with_body_taxonomy_terms', array());
        $full_view = FALSE;
        if (in_array($row->tid, $array_summary_with_body)) {
          $full_view = TRUE;
        }

        $max_length = variable_get('site_news_summary_lenght', '200');
        $summary_view = variable_get('site_news_summary_taxonomy_terms', TRUE);
        $value = kvantstudio_node_summary($node, $max_length, TRUE, $full_view, $summary_view);

        $output .= '<li>';
        $output .= '<div class="site-news-link-wrapper">' . l($row->title, 'node/' . $row->nid, array('attributes' => array('class' => array('site-news-link')))) . '</div>';
        if (variable_get('node_submitted_' . $node->type, TRUE)) {
          $output .= '<div class="site-news-date">' . format_date($row->created, 'short') . '</div>';
        }
        if ($value) {
          $output .= '<div class="site-news-summary">' . $value . '</div>';
        }
        $output .= '</li>';
      }
      $output .= '</ul>';
    }

  }
  return $output;
}