<?php

/**
 * Return rendered taxonomy tree
 */
function site_photogallery_categories() {
  global $language;

  // Подключаем специализированные файлы.
  drupal_add_css(drupal_get_path('module', 'site_photogallery') . '/css/site_photogallery.css');

  // Формируем breadcrumb ссылки.
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), '<front>');
  drupal_set_breadcrumb($breadcrumb);

  $output = '';
  $vid = taxonomy_vocabulary_machine_name_load('site_photogallery_vocabulary')->vid;
  $terms = taxonomy_get_tree($vid);

  // Выбираем режим отображения. С учетом или без учета категорий.
  if (variable_get('site_photogallery_none_categories', FALSE)) {
    foreach ($terms as $term) {
//      if ($term->language == 'und') {
//        $output .= site_photogallery_category($term->tid, FALSE);
//      }
//      else if ($term->language == $language->language) {
//        $output .= site_photogallery_category($term->tid, FALSE);
//      }
      $output .= site_photogallery_category($term->tid, FALSE);
    }
  }
  else {
    foreach ($terms as $term) {
//      if ($term->language == 'und') {
//        $output .= theme('site_photogallery_terms', array('tid' => $term->tid));
//      }
//      else if ($term->language == $language->language) {
//        $output .= theme('site_photogallery_terms', array('tid' => $term->tid));
//      }
      $output .= theme('site_photogallery_terms', array('tid' => $term->tid));
    }
  }

  return $output;
}

/**
 * Темизация вывода категорий фотоальбомов.
 */
function theme_site_photogallery_terms($variables) {
  // Получаем переменные.
  $tid = $variables['tid'];

  // Загружаем профайл фотоальбома.
  $terms = entity_load('taxonomy_term', array($tid));
  $term = $terms[$tid];

  $admin_access = user_access('administer site photogallery');

  // Проверяем разрещено ли отображать текущий фотоальбом.
  $view_access = FALSE;

  if (!$term->field_site_photogallery_status['und'][0]['value'] && $admin_access) {
    $view_access = TRUE;
  }
  if ($term->field_site_photogallery_status['und'][0]['value']) {
    $view_access = TRUE;
  }

  if (!$view_access) {
    return '';
  }

  $row_data = '';

  // Количество изображений в альбоме.
  $count_images = 0;
  $nids = db_query("SELECT n.nid FROM {taxonomy_index} n WHERE n.tid = :tid", array(':tid' => $tid));
  foreach ($nids as $row) {
    $node = node_load($row->nid);
    $count_images = $count_images + count($node->field_site_photogallery_image['und']);
  }

  // Название альбома.
  if ($count_images) {
    $row_title = '<div class="site-photogallery-album-name">' . l($term->name, 'taxonomy/term/' . $tid) . '</div>';
  }
  else {
    $row_title = '<div class="site-photogallery-album-name">' . $term->name;
    $row_title .= '<span class="site-photogallery-album-annotation"> (' . t('Images are not loaded.') . ')</span>';
    $row_title .= '</div>';
  }

  // Сообщение о том. что фотоальбом не отображается для посетителей.
  if (!$term->field_site_photogallery_status['und'][0]['value']) {
    $row_title .= '<span class="album-annotation">' . t('It is not displayed on a site.') . '</span>';
  }

  // Описание альбома.
  if ($term->description) {
    $row_data .= '<div class="site-photogallery-album-description">' . $term->description . '</div>';
  }

  // Ссылки управления.
  if ($admin_access) {
    $row_title .= '<div class="site-photogallery-admin-links">
      ' . l(t('To edit a category'), 'taxonomy/term/' . $term->tid . '/edit') . '
    </div>';
  }

  // Вывод фотоизображения обложки.
  $row_image = '';
  if (!empty($term->field_image)) {
    $variables = array(
      'style_name' => variable_get('site_photogallery_album_imagecache_preset', 'thumbnail'),
      'style_name_popup' => 'large',
      'image_path' => $term->field_image['und'][0]['uri'],
      'alt' => $term->description,
      'title' => $term->name,
      'width' => '',
      'height' => '',
      'attributes' => array(),
      'attributes_link' => array('rel'=> 'nofollow'),
      'class' => array(),
      'link' => TRUE,
      'url' => 'taxonomy/term/' . $tid,
      'gid' => NULL,
    );
    $row_image = theme('kvantstudio_image', $variables);
  }

  // Формируем массив с данными.
  $output = $row_title;
  $output .= '<div class="site-photogallery-item-wrapper">';
    if ($row_image) {
      $output .= '<div class="site-photogallery-item-image">' . $row_image . '</div>';
    }
    $output .= '<div class="site-photogallery-item-data">' . $row_data . '</div>';
  $output .= '</div>';

  return $output;
}

/**
 * Отображение фотоальбомов по заданной категории.
 */
function site_photogallery_category($tid, $page_view = TRUE) {
  // Определяем переменные по умолчанию.
  $array_tid = array();

  $output = '';

  if ($page_view) {
    // Создаем обьекты термина.
    $term = taxonomy_term_load($tid);
    $parents = array_reverse(taxonomy_get_parents_all($tid));
    if (empty($term->alias)) {
      $term->alias = 'taxonomy/term/' . $term->tid;
    }

    // Формируем breadcrumb ссылки.
    $breadcrumb = array();
    $breadcrumb[] = l(t('Home'), '<front>');
    $breadcrumb[] = l(t(variable_get('site_photogallery_title', 'Photo albums')), variable_get('site_photogallery_catalog_link', 'fotoalbomy'));
    while (count($parents) - 1) {
      $current = array_shift($parents);
      $alias = db_query("SELECT n.alias FROM {url_alias} n WHERE n.source=:source", array(':source' => 'taxonomy/term/' . $current->tid))->fetchField();
      $breadcrumb[] = l(t($current->name), $alias);
    }
    drupal_set_breadcrumb($breadcrumb);

    // Устанавливаем заголовок страницы.
    drupal_set_title(t('@value', array('@value' => $term->name)));

    // Выводим ссылку на редактирование глобальных настроек представления фотоальбомов категории.
    if (user_access('administer site photogallery')) {
      $output .= '<div class="site-photogallery-photo-album-settings">' . l(t('Settings of display of category'), 'admin/config/kvantstudio/site-photogallery/photo-album-settings/' . $tid) . '</div>';
    }
  }

  // Определяем есть ли в текущей категории фотоальбомы.
  $query = db_select('node', 'n');
  $query->fields('n', array('nid'));
  $query->condition('n.status', 1);
  $query->innerJoin('taxonomy_index', 't', 'n.nid = t.nid');
  $query->condition('t.tid', $tid);
  $result = $query->execute()->rowCount();

  if ($result > 0) {
    $array_tid = array($tid);
  }
  else {
    // Определяем имеются ли дочерние элементы у текущего каталога.
    // TODO Существует недароботка. Код ниже не учитывает статус нод, которые должны проверятся на активность как выше.
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
    $query->condition('n.type', 'site_photogallery');
    $query->condition('n.status', 1);
    // Связываем таблицы.
    $query->innerJoin('taxonomy_index', 't', 'n.nid = t.nid');
    $query->condition('t.tid', $array_tid, 'IN');
    $query->innerJoin('field_data_field_site_photogallery_weight', 'w', 'n.nid = w.entity_id');
    $query->orderBy('w.field_site_photogallery_weight_value', 'ASC');

    if (!user_access('administer site photogallery')) {
      $query->innerJoin('field_revision_field_site_photogallery_status', 's', 'n.nid = s.entity_id');
      $query->condition('s.field_site_photogallery_status_value', 1);
    }

    // Для нумерации страниц.
    $query = $query->extend('PagerDefault')->limit(20);
    $result = $query->execute();

    // Если в категории всего один фотоальбом делаем на него автоматический редирект.
    if ($result->rowCount() == 1) {
      foreach ($result as $row) {
        $node = node_load($row->nid);
        drupal_goto('node/' . $row->nid);
      }
    }

    $output .= '<div class="site-photogallery-albums-wrapper">';
    $output .= '<ul class="site-photogallery-albums">';

    foreach ($result as $row) {
      $node = node_load($row->nid);
      $output .= '<li>';
      $output .= theme('site_photogallery_node', array('node' => $node));
      $output .= '</li>';
    }

    $output .= '</ul>';
    $output .= '</div>';

    // Выводим постраничную навигацию.
    $output .= theme('pager', array('quantity' => 3));
  }

  return $output;
}

/**
 * Темизация вывода фотоальбома.
 */
function theme_site_photogallery_node($variables) {
  // Получаем переменные.
  $node = $variables['node'];

  $admin_access = user_access('administer site photogallery');

  // Проверяем разрещено ли отображать текущий фотоальбом.
  $view_access = FALSE;

  if (!$node->field_site_photogallery_status['und'][0]['value'] && $admin_access) {
    $view_access = TRUE;
  }
  if ($node->field_site_photogallery_status['und'][0]['value']) {
    $view_access = TRUE;
  }

  if (!$view_access) {
    return '';
  }

  $row_data = '';

  // Количесво изображений в альбоме.
  $count_images = count($node->field_site_photogallery_image['und']);

  // Название альбома.
  if ($count_images) {
    $row_title = '<div class="site-photogallery-album-name">' . l($node->title, 'node/' . $node->nid) . '</div>';
  }
  else {
    $row_title  = '<div class="site-photogallery-album-name">' . $node->title;
    $row_title .= '<span class="site-photogallery-album-annotation"> (' . t('In this photo album still there are no images.') . ')</span>';
    $row_title .= '</div>';
  }

  // Сообщение о том. что фотоальбом не отображается для посетителей.
  if (!$node->field_site_photogallery_status['und'][0]['value']) {
    $row_title .= '<span class="album-annotation">' . t('It is not displayed on a site.') . '</span>';
  }

  // Ссылки управления.
  if ($admin_access) {
    $row_title .= '<div class="site-photogallery-admin-links">
      ' . l(t('To edit a photo album'), 'node/' . $node->nid . '/edit') . '
     </div>';
  }

  // Вывод фотоизображения обложки.
  $variables = array(
    'style_name' => variable_get('site_photogallery_album_imagecache_preset', 'thumbnail'),
    'style_name_popup' => 'large',
    'image_path' => $node->field_site_photogallery_image['und'][0]['uri'],
    'alt' => $node->field_site_photogallery_image['und'][0]['alt'],
    'title' => $node->field_site_photogallery_image['und'][0]['title'],
    'width' => $node->field_site_photogallery_image['und'][0]['width'],
    'height' => $node->field_site_photogallery_image['und'][0]['height'],
    'attributes' => array(),
    'attributes_link' => array('rel'=> 'nofollow'),
    'class' => array(),
    'link' => TRUE,
    'url' => 'node/' . $node->nid,
    'gid' => NULL,
  );
  $row_image = theme('kvantstudio_image', $variables);

  // Формируем массив с данными.
  $output  = $row_title;
  $output .= '
    <div class="site-photogallery-item-wrapper">
      <div class="site-photogallery-item-image">' . $row_image . '</div>
      <div class="site-photogallery-item-data">'  . $row_data . '</div>
    </div>
  ';

  return $output;
}

/**
 * Темизация вывода фотоальбома.
 */
function theme_site_photogallery_images($variables) {
  // Получаем переменные.
  $node = $variables['node'];

  $output = '';

  // Общее количество изображений в альбоме. Перемнная счетчик.
  $paste_img = count($node->field_site_photogallery_image['und']);

  if ($paste_img) {
    $output .= '<ul>';

    foreach ($node->field_site_photogallery_image['und'] as $row) {

      $variables = array(
        'style_name' => variable_get('site_photogallery_image_imagecache_preset', 'thumbnail'),
        'style_name_popup' => variable_get('site_photogallery_colorbox_imagecache_preset', 'large'),
        'image_path' => $row['uri'],
        'alt' => $row['alt'],
        'title' => $row['title'],
        'width' => $row['width'],
        'height' => $row['height'],
        'attributes' => array(),
        'attributes_link' => array('data-litebox-group' => 'site-commerce-litebox-images'),
        'class' => array('litebox'),
        'link' => TRUE,
        'gid' => $row['fid'],
      );
      $row_image = theme('kvantstudio_image', $variables);

      $output .= '<li><div>';
        $output .= '<div>' . $row_image  . '</div>';
      $output .= '</div></li>';

    }

    $output .= '</ul>';
  }

  return $output;
}