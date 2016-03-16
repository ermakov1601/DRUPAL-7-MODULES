<?php

/**
 * @file
 * Views integration for site_commerce tables.
 *
 * @see http://drupal.org/project/views
 */

/**
 * Implements hook_views_data().
 */
function site_commerce_views_data() {
  $data = array();

  // Значение ключа 'group' будет использовано в UI для группировки полей,
  // фильтров, сортировки и т.д.
  $data['site_commerce']['table']['group'] = t('Product');

  // Описываем базовую таблицу, из которой Views будет доставать данные.
  $data['site_commerce']['table']['base'] = array(
    'field' => 'pid',
    'title' => t('Products of SiteCommerce'),
    'help' => '',
    'weight' => -10,
  );

  // Идентификатор.
  $data ['site_commerce']['pid'] = array(
    'title' => t('Product ID'),
    'help' => t('The unique identifier of the goods in the main table site_commerce.'),
    'field' => array(
      'handler' => 'site_commerce_handler_field_product_id',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'site_commerce_handler_argument_product_id',
      'name field' => 'pid',
      'numeric' => TRUE,
      'validate type' => 'pid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Идентификатор из 1С.
  $data ['site_commerce']['import_id'] = array(
    'title' => t('Product ID'),
    'help' => t('The primary identifier for the imported product in other database.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Тип.
  $data ['site_commerce']['type'] = array(
    'title' => t('Type'),
    'help' => t('Type of position.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Категория.
  $data ['site_commerce']['tid'] = array(
    'title' => t('Category'),
    'help' => t('Category of position.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Название.
  $data ['site_commerce']['title'] = array(
    'title' => t('Title'),
    'help' => t('Title of position.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Статус.
  $data ['site_commerce']['status'] = array(
    'title' => t('Status'),
    'help' => t('Status position in stock.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Признак новинки.
  $data ['site_commerce']['sticky'] = array(
    'title' => t('Novelty'),
    'help' => t('Status of novelty.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Состояние товара.
  $data ['site_commerce']['type_condition'] = array(
    'title' => t('Condition'),
    'help' => t('Condition of product.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}

/**
 * Field handler to provide simple renderer that allows linking to a product.
 */
class site_commerce_handler_field_product_id extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);

    // Если выбрана опция "Выводить в виде ссылки", до достаем id продукта.
    if (!empty($this->options['link_to_product'])) {
      $this->additional_fields['pid'] = array('table' => 'site_commerce', 'field' => 'pid');
    }
  }

  /**
   * Обьявляем настройки для поля.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_product'] = array('default' => 'none');
    return $options;
  }

  /**
   * Описываем форму с натсройками.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_product'] = array(
      '#title' => t('Link this field to the original piece of product'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_product']),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Выводим поле в виде ссылки на продукт.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_product']) && !empty($this->additional_fields['pid'])) {
      $pid = $this->get_value($values, 'pid');
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'site-commerce/' . $pid;
    }

    return $data;
  }

  /**
   * Выводим поле.
   */
  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}

/**
 * Argument handler to accept a product id.
 */
class site_commerce_handler_argument_product_id extends views_handler_argument_numeric {
  /**
   * Override the behavior of title().
   */
  function title_query() {
    $titles = array();

    $result = db_query("SELECT n.title FROM {site_commerce} n WHERE n.pid IN (:pids)", array(':pids' => $this->value));
    foreach ($result as $product) {
      $titles[] = check_plain($product->title);
    }

    return $titles;
  }
}