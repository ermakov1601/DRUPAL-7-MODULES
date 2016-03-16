<?php
// $Id:

/**
 * Return rendered taxonomy tree
 */
function site_photogallery_categories() {
  // Подключаем специализированные файлы.
  drupal_add_css(drupal_get_path('module', 'site_photogallery') . '/css/site_photogallery.css');

  // Формируем breadcrumb ссылки.
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), '<front>');
  $breadcrumb[] = t(variable_get('site_photogallery_catalog_title', 'Photo albums'));
  drupal_set_breadcrumb($breadcrumb);

  $output = '';
  $vid = taxonomy_vocabulary_machine_name_load('site_photogallery_vocabulary')->vid;
  $terms = taxonomy_get_tree($vid);

  // Выбираем режим отображения. С учетом или без учета категорий.
  if (variable_get('site_photogallery_none_categories', FALSE)) {
    foreach ($terms as $term) {
      $output .= site_photogallery_category($term->tid, FALSE);
    }
  }
  else {
    foreach ($terms as $term) {
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

  // Количесво изображений в альбоме.
  $count_images = 0;
  $nids = db_query("SELECT n.nid FROM {taxonomy_index} n WHERE n.tid = :tid", array(':tid' => $tid));
  foreach ($nids as $row) {
    $node = node_load($row->nid);
    $count_images = $count_images + count($node->field_site_photogallery_image['und']);
  }

  // Название альбома.
  if ($count_images) {

    $row_data .= '<div class="site-photogallery-album-name">' . l($term->name, 'taxonomy/term/' . $tid) . '</div>';
  }
  else {
    $row_data .= '<div class="site-photogallery-album-name">' . $term->name;
    $row_data .= '<span class="site-photogallery-album-annotation"> (' . t('Images are not loaded.') . ')</span>';
    $row_data .= '</div>';
  }

  // Сообщение о том. что фотоальбом не отображается для посетителей.
  if (!$term->field_site_photogallery_status['und'][0]['value']) {
    $row_data .= '<img align="absmiddle" src="/' . drupal_get_path('module', 'site_photogallery') . '/images/warning.png" alt="'.t('Warning').'" />&nbsp;<span class="album-annotation">' . t('It is not displayed on a site.') . '</span>';
  }

  // Описание альбома.
  if ($term->description) {
    $row_data .= '<div class="site-photogallery-album-description">' . $term->description . '</div>';
  }

//  // Количество фотографий в альбоме.
//  if ($count_images) {
//    $row_data .= '<div class="site-photogallery-album-annotation">' . t('In total images') . ' - ' . $count_images . '</div>';
//  }

  // Ссылки управления.
  if ($admin_access) {
    $row_data .= '<div class="site-photogallery-admin-links">
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
      'class' => array(),
      'link' => TRUE,
      'url' => 'taxonomy/term/' . $tid,
      'gid' => NULL,
    );
    $row_image = theme('kvantstudio_image', $variables);
  }

  // Формируем массив с данными.
  $output = '
    <div class="site-photogallery-item-wrapper">
      <div class="site-photogallery-item-image">' . $row_image . '</div>
      <div class="site-photogallery-item-data">' . $row_data . '</div>
    </div>
  ';

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
    $breadcrumb[] = l(t(variable_get('site_photogallery_catalog_title', 'Photo albums')), variable_get('site_photogallery_catalog_link', 'fotoalbomy'));
    while (count($parents) - 1) {
      $current = array_shift($parents);
      $alias = db_query("SELECT n.alias FROM {url_alias} n WHERE n.source=:source", array(':source' => 'taxonomy/term/' . $current->tid))->fetchField();
      $breadcrumb[] = l(t($current->name), $alias);
    }
    $breadcrumb[] = t($term->name);
    drupal_set_breadcrumb($breadcrumb);

    // Устанавливаем заголовок страницы.
    drupal_set_title(t($term->name));

    // Выводим ссылку на редактирование глобальных настроек представления фотоальбомов категории.
    if (user_access('administer site photogallery')) {
      $output .= '<div class="site-photogallery-photo-album-settings">' . l(t('Settings of display of category'), 'admin/config/kvantstudio/site-photogallery/photo-album-settings/' . $tid) . '</div>';
    }
  }

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
    $query->condition('n.type', 'site_photogallery');
    $query->condition('n.status', 1);
    // Связываем таблицы.
    $query->innerJoin('taxonomy_index', 't', 'n.nid = t.nid');
    $query->condition('t.tid', $array_tid, 'IN');
    $query->innerJoin('field_data_field_site_photogallery_weight', 'w', 'n.nid = w.entity_id');
    $query->orderBy('w.field_site_photogallery_weight_value', 'ASC');
    // Для нумерации страниц.
    $query = $query->extend('PagerDefault')->limit(20);
    $result = $query->execute();

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
    $row_data .= '<div class="site-photogallery-album-name">' . l($node->title, 'node/' . $node->nid) . '</div>';
  }
  else {
    $row_data .= '<div class="site-photogallery-album-name">' . $node->title;
    $row_data .= '<span class="site-photogallery-album-annotation"> (' . t('In this photo album still there are no images.') . ')</span>';
    $row_data .= '</div>';
  }

  // Сообщение о том. что фотоальбом не отображается для посетителей.
  if (!$node->field_site_photogallery_status['und'][0]['value']) {
    $row_data .= '<img align="absmiddle" src="/' . drupal_get_path('module', 'site_photogallery') . '/images/warning.png" alt="'.t('Warning').'" />&nbsp;<span class="album-annotation">' . t('It is not displayed on a site.') . '</span>';
  }

//  // Количество фотографий в альбоме.
//  if ($count_images) {
//    $row_data .= '<div class="site-photogallery-album-annotation">' . t('In total images') . ' - ' . $count_images . '</div>';
//  }

  // Ссылки управления.
  if ($admin_access) {
    $row_data .= '<div class="site-photogallery-admin-links">
      ' . l(t('To edit a photo album'), 'node/' . $node->nid . '/edit') . '
     </div>';
  }

//  // Описание альбома.
//  $body = '';
//  if (!empty($node->body)) {
//    $summary = $node->body['und'][0]['summary'];
//    if (empty($summary)) {
//      $body = $node->body['und'][0]['value'];
//    }
//    else {
//      $body = $summary;
//    }
//    $row_data .= '<div class="site-photogallery-album-description">' . $body . '</div>';
//  }

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
    'class' => array(),
    'link' => TRUE,
    'url' => 'node/' . $node->nid,
    'gid' => NULL,
  );
  $row_image = theme('kvantstudio_image', $variables);

  // Формируем массив с данными.
  $output = '
    <div class="site-photogallery-item-wrapper">
      <div class="site-photogallery-item-data">' . $row_data . '</div>
      <div class="site-photogallery-item-image">' . $row_image . '</div>
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
        'class' => array('site-photogallery-colorbox'),
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

///**
// * Темизация вывода фотоальбома.
// */
//function theme_site_photogallery_images($variables) {
//  // Получаем переменные.
//  $node = $variables['node'];
//
//  $output = '';
//
//  // Определяем параметры вывода изображений.
//  // Количество изображений в одном ряду.
//  $count_img = variable_get('site_photogallery_image_in_row', '4');
//  // Переменная начала нового ряда. По умолчанию 0.
//  $сount_row = 0;
//  // Общее количество изображений в альбоме. Перемнная счетчик.
//  $paste_img = count($node->field_site_photogallery_image['und']);
//
//  foreach ($node->field_site_photogallery_image['und'] as $row) {
//
//    // Выводим тег начала нового ряда если переменная $сount_row = 0.
//    if ($сount_row == 0) {
//      $output .= '<ul>';
//    }
//
//    $output .= '<li>';
//
//    $variables = array(
//      'style_name' => variable_get('site_photogallery_image_imagecache_preset', 'thumbnail'),
//      'style_name_popup' => variable_get('site_photogallery_colorbox_imagecache_preset', 'large'),
//      'image_path' => $row['uri'],
//      'alt' => $row['alt'],
//      'title' => $row['title'],
//      'width' => $row['width'],
//      'height' => $row['height'],
//      'attributes' => array(),
//      'class' => array('colorbox'),
//      'link' => TRUE,
//      'gid' => $row['fid'],
//    );
//    $output .= theme('kvantstudio_image', $variables);
//
//    $output .= '</li>';
//
//    // Увеличиваем счетчик рядов и уменьшаем счетчик вставленных изображений.
//    $сount_row ++;
//    $paste_img = $paste_img - 1;
//    // Делаем проверку если $сount_row = $count_img то закрываем ряд и сбрасываем счетчик $сount_row.
//    if ($сount_row == $count_img AND $paste_img > 0) {
//      $output .= '</ul>';
//      $сount_row = 0;
//    }
//    // Если изображений не хватает, чтобы полностью заполнить ряд, то закрываем его принудительно.
//    if ($сount_row < $count_img AND $paste_img == 0) {
//      $output .= '</ul>';
//    }
//
//  }
//
//  return $output;
//}