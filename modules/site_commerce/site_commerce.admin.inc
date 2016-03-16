<?php

/**
 * @file
 * Admin page callbacks for the site_commerce.admin module.
 */

/**
 * Return form for site commerce administration filters.
 */
function site_commerce_filter_form() {
  $session = isset($_SESSION['site_commerce_overview_filter']) ? $_SESSION['site_commerce_overview_filter'] : array();
  $filters = site_commerce_filters();

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only positions where'),
    '#theme' => 'exposed_filters__node',
  );
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    if ($type == 'term') {
      // Load term name from DB rather than search and parse options array.
      $value = module_invoke('taxonomy', 'term_load', $value);
      $value = $value->name;
    }
    else {
      $value = $filters[$type]['options'][$value];
    }
    $t_args = array('%property' => $filters[$type]['title'], '%value' => $value);
    if ($i++) {
      $form['filters']['current'][] = array('#markup' => t('and where %property is %value', $t_args));
    }
    else {
      $form['filters']['current'][] = array('#markup' => t('where %property is %value', $t_args));
    }
    if (in_array($type, array('type', 'language'))) {
      // Remove the option if it is already being filtered on.
      unset($filters[$type]);
    }
  }

  $form['filters']['status'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('clearfix')),
    '#prefix' => ($i ? '<div class="additional-filters">' . t('and where') . '</div>' : ''),
  );
  $form['filters']['status']['filters'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('filters')),
  );
  foreach ($filters as $key => $filter) {
    $form['filters']['status']['filters'][$key] = array(
      '#type' => 'select',
      '#options' => $filter['options'],
      '#title' => $filter['title'],
      '#default_value' => '[any]',
    );
  }

  $form['filters']['status']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['status']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => count($session) ? t('Refine') : t('Filter'),
  );
  if (count($session)) {
    $form['filters']['status']['actions']['undo'] = array('#type' => 'submit', '#value' => t('Undo'));
    $form['filters']['status']['actions']['reset'] = array('#type' => 'submit', '#value' => t('Reset'));
  }

  drupal_add_js('misc/form.js');

  return $form;
}

/**
 * Process result from site commerce administration filter form.
 */
function site_commerce_filter_form_submit($form, &$form_state) {
  $filters = site_commerce_filters();
  switch ($form_state['values']['op']) {
    case t('Filter'):
    case t('Refine'):
      // Apply every filter that has a choice selected other than 'any'.
      foreach ($filters as $filter => $options) {
        if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '[any]') {
          // Flatten the options array to accommodate hierarchical/nested options.
          $flat_options = form_options_flatten($filters[$filter]['options']);
          // Only accept valid selections offered on the dropdown, block bad input.
          if (isset($flat_options[$form_state['values'][$filter]])) {
            $_SESSION['site_commerce_overview_filter'][] = array($filter, $form_state['values'][$filter]);
          }
        }
      }
      break;
    case t('Undo'):
      array_pop($_SESSION['site_commerce_overview_filter']);
      break;
    case t('Reset'):
      $_SESSION['site_commerce_overview_filter'] = array();
      break;
  }
}

/**
 * Apply filters for site commerce administration filters based on session.
 *
 * @param $query
 *   A SelectQuery to which the filters should be applied.
 */
function site_commerce_build_filter_query(SelectQueryInterface $query) {
  // Build query
  $filter_data = isset($_SESSION['site_commerce_overview_filter']) ? $_SESSION['site_commerce_overview_filter'] : array();

  foreach ($filter_data as $index => $filter) {
    list($key, $value) = $filter;

    switch ($key) {
      case 'status':
        // Note: no exploitable hole as $key/$value have already been checked when submitted
        list($key, $value) = explode('-', $value, 2);
      case 'type':
        $query->condition('n.' . $key, $value);
        break;
    }
  }
}

/**
 * List site commerce administration filters that can be applied.
 */
function site_commerce_filters() {
  // Regular filters
  $filters['status'] = array(
    'title' => t('status'),
    'options' => array(
      '[any]' => t('any'),
      'disabled-0' => t('published'),
      'disabled-1' => t('not published'),
      'sticky-1' => t('novelty'),
      'sticky-0' => t('not novelty'),
    ),
  );

//  $filters['type'] = array(
//    'title' => t('type'),
//    'options' => array(
//      '[any]' => t('any'),
//    ) + node_type_get_names(),
//  );

  return $filters;
}

/**
 * Menu callback: products administration.
 */
function site_commerce_admin_content($form, $form_state) {
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    return site_commerce_multiple_delete_confirm($form, $form_state, array_filter($form_state['values']['site_commerces']));
  }
  $form['filter'] = site_commerce_filter_form();
  $form['#submit'][] = 'site_commerce_filter_form_submit';
  $form['admin'] = site_commerce_admin_products();

  return $form;
}

/**
 * Form builder: Builds the products administration overview.
 */
function site_commerce_admin_products() {
  $admin_access = user_access('administer site commerce products');

  // Форма параметров обновления.
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
    '#access' => $admin_access,
  );

  $options = array();
  $options['unpublish'] = t('Unpublish the selected positions');
  $options['publish'] = t('Publish the selected positions');
  $options['publish_all'] = t('Publish all positions');
  $options['delete'] = t('Delete the selected positions');
  $options['delete_all'] = t('To remove all positions');

  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'approve',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#validate' => array('site_commerce_admin_products_validate'),
    '#submit' => array('site_commerce_admin_products_submit'),
  );

  // Enable language column if translation module is enabled or if we have any
  // node with language.
  $multilanguage = (module_exists('translation') || db_query_range("SELECT 1 FROM {site_commerce} WHERE language <> :language", 0, 1, array(':language' => LANGUAGE_NONE))->fetchField());

  // Определяем заголовки таблицы с сортировкой.
  $header = array(
    'image' => array('data' => t('Image')),
    'title' => array('data' => t('Title'), 'field' => 'n.title'),
    'type' => array('data' => t('Type'), 'field' => 'n.type'),
    'disabled' => array('data' => t('Display status'), 'field' => 'n.disabled'),
    'changed' => array('data' => t('Updated'), 'field' => 'n.changed', 'sort' => 'desc')
  );
  if ($multilanguage) {
    $header['language'] = array('data' => t('Language'), 'field' => 'n.language');
  }
  $header['operations'] = array('data' => t('Operations'));

  $query = db_select('site_commerce', 'n')->extend('PagerDefault')->extend('TableSort');
  site_commerce_build_filter_query($query);

  $pids = $query
    ->fields('n', array('pid'))
    ->limit(100)
    ->orderByHeader($header)
    ->addTag('site_commerce_access')
    ->execute()
    ->fetchCol();

  $site_commerces = site_commerce_load_multiple($pids);

  // Prepare the list of products.
  $languages = language_list();

  $destination = drupal_get_destination();

  $options = array();

  foreach ($site_commerces as $site_commerce) {
    $langcode = entity_language('site_commerce', $site_commerce);
    $l_options = $langcode != LANGUAGE_NONE && isset($languages[$langcode]) ? array('language' => $languages[$langcode]) : array();

    // Формируем главное изображение.
    if (!empty($site_commerce->field_site_commerce_image[LANGUAGE_NONE][0])) {
      $variables = array(
        'style_name' => 'thumbnail',
        'style_name_popup' => '',
        'image_path' => $site_commerce->field_site_commerce_image[LANGUAGE_NONE][0]['uri'],
        'alt' => $site_commerce->field_site_commerce_image[LANGUAGE_NONE][0]['alt'],
        'title' => $site_commerce->field_site_commerce_image[LANGUAGE_NONE][0]['title'],
        'width' => $site_commerce->field_site_commerce_image[LANGUAGE_NONE][0]['width'],
        'height' => $site_commerce->field_site_commerce_image[LANGUAGE_NONE][0]['height'],
        'attributes' => array(),
        'class' => array(''),
        'gid' => NULL,
        'link' => FALSE,
      );
      $image =  theme('kvantstudio_image', $variables);
    }
    else {
      $image = '';
    }

    $options[$site_commerce->pid] = array(
      'image' => $image,
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $site_commerce->title,
          '#href' => 'site-commerce/' . $site_commerce->pid,
          '#options' => $l_options,
        ),
      ),
      'type' => check_plain(site_commerce_type_get_name($site_commerce)),
      'disabled' => !$site_commerce->disabled ? t('published') : t('not published'),
      'changed' => format_date($site_commerce->changed, 'short'),
    );
    if ($multilanguage) {
      if ($langcode == LANGUAGE_NONE || isset($languages[$langcode])) {
        $options[$site_commerce->pid]['language'] = $langcode == LANGUAGE_NONE ? t('Language neutral') : t($languages[$langcode]->name);
      }
      else {
        $options[$site_commerce->pid]['language'] = t('Undefined language (@langcode)', array('@langcode' => $langcode));
      }
    }
    // Формируем список доступных операций для позиции товара.
    $operations = array();
    if (site_commerce_access('edit', $site_commerce)) {
      $operations['edit'] = array(
        'title' => t('edit'),
        'href' => 'admin/content/site-commerce/' . $site_commerce->pid . '/edit',
        'query' => $destination,
      );
    }
    if (site_commerce_access('delete', $site_commerce)) {
      $operations['delete'] = array(
        'title' => t('delete'),
        'href' => 'admin/content/site-commerce/' . $site_commerce->pid . '/delete',
        'query' => $destination,
      );
    }
    $options[$site_commerce->pid]['operations'] = array();
    if (count($operations) > 1) {
      // Render an unordered list of operations links.
      $options[$site_commerce->pid]['operations'] = array(
        'data' => array(
          '#theme' => 'links__node_operations',
          '#links' => $operations,
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
    }
    elseif (!empty($operations)) {
      // Render the first and only operation as a link.
      $link = reset($operations);
      $options[$site_commerce->pid]['operations'] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => $link['title'],
          '#href' => $link['href'],
          '#options' => array('query' => $link['query']),
        ),
      );
    }
  }

  // Only use a tableselect when the current user is able to perform any
  // operations.
  if ($admin_access) {
    $form['site_commerces'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $options,
      '#empty' => t('No content available.'),
    );
  }
  // Otherwise, use a simple table.
  else {
    $form['site_commerces'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $options,
      '#empty' => t('No content available.'),
    );
  }

  $form['pager'] = array('#markup' => theme('pager'));
  return $form;
}

/**
 * Validate admin products form submissions.
 */
function site_commerce_admin_products_validate($form, &$form_state) {
  $operation = $form_state['values']['operation'];
  // Массив с операциями, которые применяются для выполнения в batch.
  $batch_operation = array('delete_all');
  if (!in_array($operation, $batch_operation)) {
    $form_state['values']['site_commerces'] = array_diff($form_state['values']['site_commerces'], array(0));
    // We can't execute any 'Update options' if no comments were selected.
    if (count($form_state['values']['site_commerces']) == 0) {
      form_set_error('', t('Select one or more positions to perform the update on.'));
    }
  }
}

/**
 * Process admin products form submissions.
 *
 * Execute the chosen 'Update option' on the selected products, such as
 * publishing, unpublishing or deleting.
 */
function site_commerce_admin_products_submit($form, &$form_state) {
  $operation = $form_state['values']['operation'];
  $pids = $form_state['values']['site_commerces'];

  // Массив с операциями, которые применяются для выполнения в batch.
  $batch_operation = array('delete_all');

  // Удаляем все позиции.
  if ($operation == 'delete_all') {
    $form_state['redirect'] = 'admin/content/site-commerce/delete';
  }

  // Выполняем если выбранная операция не предназначенна для выполнения в batch.
  if (!in_array($operation, $batch_operation)) {
    if ($operation == 'delete') {
      site_commerce_delete_multiple($pids);
    }
    elseif ($operation == 'unpublish' || $operation == 'publish') {
      foreach ($pids as $pid => $value) {
        $site_commerce = site_commerce_load($value);
        if ($operation == 'unpublish') {
          $site_commerce->disabled = SITE_COMMERCE_NOT_PUBLISHED;
        }
        elseif ($operation == 'publish') {
          $site_commerce->disabled = SITE_COMMERCE_PUBLISHED;
        }
        site_commerce_save($site_commerce);
      }
    }

    // Устанавливаем принудительно публикацию для всех позиций.
    if ($operation == 'publish_all') {
      $result = db_update('site_commerce')
      ->fields(array('disabled' => 0))
      ->execute();
      drupal_set_message(t('All positions are published.'));
    }

    $form_state['redirect'] = 'admin/content/site-commerce';
    drupal_set_message(t('The update has been performed.'));
  }

  cache_clear_all();
}

/**
 * Menu callback -- ask for confirmation of product deletion.
 */
function site_commerce_delete_all_positions_confirm($form, &$form_state) {
  return confirm_form($form,
    t('To remove all positions'),
    'admin/content/site-commerce',
    t('Are you sure you want to delete all the positions? Recovery will be impossible.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute product deletion.
 */
function site_commerce_delete_all_positions_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $query = db_select('site_commerce', 'n');
    $query->fields('n', array('pid'));
    $result = $query->execute();
    foreach ($result as $row) {
      $operations[] = array('site_commerce_delete_all_positions_batch', array($row->pid));
    }

    $batch = array(
      // Массив операций и их параметров
      'operations' => $operations,
      // Функция, которая будет выполнена после окончания всех операций
      'finished' => 'site_commerce_delete_all_positions_batch_finished',
      // Заголовок страницы с прогресс баром.
      // Опционально, по умолчанию t('Processing')
      'title' => t('To remove all positions'),
      // Сообщение, показываемое при инициализации.
      // Опционально, по умолчанию t('Initializing.')
      'init_message' => 'Подготовка данных',
      // Сообщение, показывемое при выполнении операций.
      // Опционально, по умолчанию t('Completed @current of @total.')
      'progress_message' => 'Выполнено @current из @total.',
      // Сообщение показываемое при ошибке выполнения операции.
      // Опционально, по умолчанию t('An error has occurred.')
      'error_message' => 'Произошла ошибка.',
      'file' => drupal_get_path('module', 'site_commerce') . '/site_commerce.admin.inc',
    );

    batch_set($batch);

  }
  //$form_state['redirect'] = 'admin/content/site-commerce';
}

/**
 * Удаление позиции.
 */
function site_commerce_delete_all_positions_batch($pid, &$context) {
  $site_commerce = site_commerce_load($pid);
  site_commerce_delete($pid);

  // Дополнительные операции по удалению.
  $criteria = array();
  $criteria['source'] = 'site-commerce/' . $pid;
  path_delete($criteria);

  // Сообщение выводимое под прогресс баром после окончания текущей операции.
  $context['message'] = t('The position is removed') . ' - ' . $site_commerce->title;
}

/**
 * Выполняем функцию по окончанию удаления позиций.
 */
function site_commerce_delete_all_positions_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('All of the items have been removed.'));
  }
  else {
    drupal_set_message(t('To remove a completed with errors.'), 'error');
  }
  drupal_goto('admin/content/site-commerce');
}
