<?php

/**
 * @file
 * Admin page callbacks for the settings module.
 */

/**
 * Настройки модуля.
 */
function site_commerce_settings_form($form, &$form_state) {
  //----------------------------------------------------------------------------
  // Общие настройки.
  //----------------------------------------------------------------------------
  $form['site_commerce_basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['site_commerce_basic']['site_commerce_default_currency'] = array(
    '#type'          => 'select',
    '#title'         => t('Default currency'),
    '#default_value' => variable_get('site_commerce_default_currency'),
    '#options'       => site_commerce_position_сurrency(),
  );
  $form['site_commerce_basic']['site_commerce_full_path'] = array(
    '#title'         => t('To shape links on a position of the products in view of the name of a category'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_full_path', TRUE),
  );
  $form['site_commerce_basic']['site_commerce_default_image_formatter'] = array(
    '#title'         => t('Allow to use the standard image output format in the card goods'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_default_image_formatter', TRUE),
  );
  $form['site_commerce_basic']['site_commerce_set_reed_more'] = array(
    '#title'         => t('Allow button to display the detailed view on the item card.'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_set_reed_more', FALSE),
  );

  //----------------------------------------------------------------------------
  // Настройки стилей фотоизображений.
  //----------------------------------------------------------------------------
  $form['site_commerce_image_styles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings of image styles'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['site_commerce_image_styles']['site_commerce_imagecache_taxonomy'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of the image at display of a category'),
    '#default_value' => variable_get('site_commerce_imagecache_taxonomy', 'sc_taxonomy'),
    '#options'       => site_commerce_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );
  $form['site_commerce_image_styles']['site_commerce_imagecache_taxonomy_block'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of the image at display of a category in block'),
    '#default_value' => variable_get('site_commerce_imagecache_taxonomy_block', 'sc_taxonomy_block'),
    '#options'       => site_commerce_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );
  $form['site_commerce_image_styles']['site_commerce_imagecache_subtaxonomy'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of the image at display of a subcategory'),
    '#default_value' => variable_get('site_commerce_imagecache_subtaxonomy', 'sc_subtaxonomy'),
    '#options'       => site_commerce_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );
  $form['site_commerce_image_styles']['site_commerce_imagecache_select'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of the image at display of select form'),
    '#default_value' => variable_get('site_commerce_imagecache_select', 'sc_select'),
    '#options'       => site_commerce_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );
  $form['site_commerce_image_styles']['site_commerce_imagecache_select_card'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of the image at display of select form in card'),
    '#default_value' => variable_get('site_commerce_imagecache_select_card', 'sc_select_card'),
    '#options'       => site_commerce_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );

  // Стили изображений позиций.
  $form['site_commerce_image_styles']['site_commerce_image_styles_positions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Styles of the image of positions'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['site_commerce_image_styles']['site_commerce_image_styles_positions']['site_commerce_imagecache_first'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of the main image at display of a position'),
    '#default_value' => variable_get('site_commerce_imagecache_first', 'sc_first'),
    '#options'       => site_commerce_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );
  $form['site_commerce_image_styles']['site_commerce_image_styles_positions']['site_commerce_imagecache_second'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of the image at display of a position'),
    '#default_value' => variable_get('site_commerce_imagecache_second', 'sc_thumbnail'),
    '#options'       => site_commerce_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );
  $form['site_commerce_image_styles']['site_commerce_image_styles_positions']['site_commerce_imagecache_taxonomy_term'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of the image at display of a position in the catalogue'),
    '#default_value' => variable_get('site_commerce_imagecache_taxonomy_term', 'sc_taxonomy_term'),
    '#options'       => site_commerce_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );
  $form['site_commerce_image_styles']['site_commerce_image_styles_positions']['site_commerce_imagecache_other'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of the image at display similar positions'),
    '#default_value' => variable_get('site_commerce_imagecache_other', 'sc_first'),
    '#options'       => site_commerce_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );
  $form['site_commerce_image_styles']['site_commerce_image_styles_positions']['site_commerce_imagecache_index'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of the image at display connected positions'),
    '#default_value' => variable_get('site_commerce_imagecache_index', 'sc_thumbnail'),
    '#options'       => site_commerce_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );
  $form['site_commerce_image_styles']['site_commerce_image_styles_positions']['site_commerce_imagecache_full'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of the image at display of a position to all screen'),
    '#default_value' => variable_get('site_commerce_imagecache_full', 'sc_full'),
    '#options'       => site_commerce_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );
  $form['site_commerce_image_styles']['site_commerce_image_styles_positions']['site_commerce_imagecache_block'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of the image at display of a position in the block'),
    '#default_value' => variable_get('site_commerce_imagecache_block', 'sc_block'),
    '#options'       => site_commerce_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );

  // Стили фотоизображений для отображения новинок.
  $form['site_commerce_image_styles']['site_commerce_image_styles_positions_new'] = array(
    '#type' => 'fieldset',
    '#title' => t('Styles of the image of positions of novelties'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['site_commerce_image_styles']['site_commerce_image_styles_positions_new']['site_commerce_imagecache_first_new'] = array(
    '#type'          => 'select',
    '#title'         => t("Style of the main image at display of a position"),
    '#default_value' => variable_get('site_commerce_imagecache_first_new', 'sc_first'),
    '#options'       => site_commerce_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );
  $form['site_commerce_image_styles']['site_commerce_image_styles_positions_new']['site_commerce_imagecache_taxonomy_term_new'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of the image at display of a position in the catalogue'),
    '#default_value' => variable_get('site_commerce_imagecache_taxonomy_term_new', 'sc_taxonomy_term'),
    '#options'       => site_commerce_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );
  $form['site_commerce_image_styles']['site_commerce_image_styles_positions_new']['site_commerce_imagecache_other_new'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of the image at display similar positions'),
    '#default_value' => variable_get('site_commerce_imagecache_other_new', 'sc_first'),
    '#options'       => site_commerce_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );
  $form['site_commerce_image_styles']['site_commerce_image_styles_positions_new']['site_commerce_imagecache_index_new'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of the image at display connected positions'),
    '#default_value' => variable_get('site_commerce_imagecache_index_new', 'sc_thumbnail'),
    '#options'       => site_commerce_select_imagecache_presets(),
    '#description'   => l(t('Edit'), 'admin/config/media/image-styles'),
  );

  //----------------------------------------------------------------------------
  // Настройки каталога.
  //----------------------------------------------------------------------------
  $form['site_commerce_catalog'] = array(
    '#type' => 'fieldset',
    '#title' => t('Set up of the catalogue'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['site_commerce_catalog']['site_commerce_catalog_products_default_mode'] = array(
    '#type' => 'select',
    '#title' => t('Display mode products by category'),
    '#default_value' => variable_get('site_commerce_catalog_products_default_mode', 'site_commerce_term'),
    '#options' => array('site_commerce_term' => t('mode in one line'), 'site_commerce_term_compact' => t('mode into the net')),
  );
  $form['site_commerce_catalog']['site_commerce_catalog_images'] = array(
    '#title'         => t('Allow to display images of categories of the catalogue'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_catalog_images', TRUE),
  );
  $form['site_commerce_catalog']['site_commerce_catalog_block_images'] = array(
    '#title'         => t('Allow to display images of categories of the catalogue in block'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_catalog_block_images', FALSE),
  );
  $form['site_commerce_catalog']['site_commerce_subcatalog_images'] = array(
    '#title'         => t('Allow to display images of subcategories of the catalogue'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_subcatalog_images', FALSE),
  );
  $form['site_commerce_catalog']['site_commerce_allow_date_and_time'] = array(
    '#title'         => t('To display date and time of last updating of the catalogue of the goods'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_date_and_time', FALSE),
  );
  $form['site_commerce_catalog']['site_commerce_allow_sort'] = array(
    '#title'         => t('To allow to display the form of sorting of positions'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_sort', TRUE),
  );
  $form['site_commerce_catalog']['site_commerce_allow_count_main_catalog'] = array(
    '#title'         => t('Allow to display the number of positions next to the name category'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_count_main_catalog', FALSE),
  );
  $form['site_commerce_catalog']['site_commerce_allow_count_catalog'] = array(
    '#title'         => t('Allow to display the number of positions next to the name subcategory'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_count_catalog', FALSE),
  );
  $form['site_commerce_catalog']['site_commerce_allow_null_category'] = array(
    '#title'         => t('Allow to display categories in which there are no positions'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_null_category', TRUE),
  );
  $form['site_commerce_catalog']['site_commerce_allow_view_subcategories'] = array(
    '#title'         => t('Allow display subcategories in the main catalog page'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_view_subcategories', TRUE),
  );
  $form['site_commerce_catalog']['site_commerce_allow_view_subcategories_overlay'] = array(
    '#title'         => t('Allow display subcategories on the main catalog page in the overlay layer'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_view_subcategories_overlay', TRUE),
  );
  $form['site_commerce_catalog']['site_commerce_allow_pager'] = array(
    '#title'         => t('Allow display stating navigation'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_pager', TRUE),
  );
  $form['site_commerce_catalog']['site_commerce_catalog_allow_status'] = array(
    '#title'         => t('Allow to display the status of the position when viewing the catalogue'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_catalog_allow_status', TRUE),
  );
  $form['site_commerce_catalog']['site_commerce_catalog_sort_type'] = array(
    '#title'         => t('Allow sorting categories of the catalogue on weight'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_catalog_sort_type', FALSE),
  );
  $form['site_commerce_catalog']['site_commerce_other_positions_upsell'] = array(
    '#title'         => t('To allow to display similar positions in view of principle Upsell'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_other_positions_upsell', FALSE),
    '#description' => t('It is recommended to resolve sorting, only if the basic cost of all positions is filled. Otherwise other positions will not be displayed. This function allows to motivate the buyer to buy higher-end model.'),
  );
  $form['site_commerce_catalog']['site_commerce_allow_view_positions_subcategories'] = array(
    '#title'         => t('Allow to display positions of subcategories'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_view_positions_subcategories', TRUE),
  );
  $form['site_commerce_catalog']['site_commerce_allow_view_all_categories'] = array(
    '#title'         => t('Allow always display all subcategories root partition current subcategories'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_view_all_categories', FALSE),
    '#description' => t('It is recommended to include, if you want that at viewing a category of the catalogue other categories from a root category into which enters current were displayed all.'),
  );
  $form['site_commerce_catalog']['site_commerce_catalog_position_delete'] = array(
    '#title'         => t('To allow to delete positions of the catalogue at removal of a category'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_catalog_position_delete', FALSE),
  );
  $form['site_commerce_catalog']['site_commerce_catalog_cache'] = array(
    '#title'         => t('To resolve cache cards of the goods at a browsing the catalog'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_catalog_cache', FALSE),
  );
  $form['site_commerce_catalog']['site_commerce_allow_summary_in_category'] = array(
    '#title'         => t('Allow display a brief contents product card when you are viewing a catalogue'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_summary_in_category', TRUE),
  );
  $form['site_commerce_catalog']['site_commerce_catalog_allow_statuses'] = array(
    '#type' => 'checkboxes',
    '#return_value' => 'fields',
    '#title' => t('Statuses at which it is authorized to view a position in a catalog'),
    '#default_value' => variable_get('site_commerce_catalog_allow_statuses', array()),
    '#options' => site_commerce_position_status(),
  );
  $form['site_commerce_catalog']['site_commerce_other_positions_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('The number of other positions in card view'),
    '#default_value' => variable_get('site_commerce_other_positions_limit', 8),
    '#required' => TRUE,
  );
  $form['site_commerce_catalog']['site_commerce_block_novelties_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('The number of positions in block novelties'),
    '#default_value' => variable_get('site_commerce_block_novelties_limit', 5),
    '#required' => TRUE,
  );
  $form['site_commerce_catalog']['site_commerce_num_per_page'] = array(
    '#type' => 'textfield',
    '#title' => t('The number of items on one page'),
    '#default_value' => variable_get('site_commerce_num_per_page', 20),
    '#required' => TRUE,
  );
  $form['site_commerce_catalog']['site_commerce_catalog_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Heading of page of the catalogue'),
    '#default_value' => variable_get('site_commerce_catalog_title'),
    '#required' => TRUE,
  );
  $form['site_commerce_catalog']['site_commerce_catalog_link'] = array(
    '#type' => 'textfield',
    '#title' => t('The name of the link of page of the catalogue'),
    '#default_value' => variable_get('site_commerce_catalog_link'),
    '#required' => TRUE,
    '#description'   => t('The name of the link of page of the catalogue in English. Do not change the name on a ready site!'),
  );

  $vocabulary_categories = taxonomy_vocabulary_machine_name_load('site_commerce_categories');

  //----------------------------------------------------------------------------
  // Настройки выбора параметров.
  //----------------------------------------------------------------------------
  $form['site_commerce_parametrs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure the parametrs'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Форма регистрации новых параметров.
  $form['site_commerce_parametrs']['create'] = array(
    '#type' => 'fieldset',
    '#title' => t('Create a parameter'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['site_commerce_parametrs']['create']['select_vocabulary'] = array(
    '#type' => 'select',
    '#title' => t('Select vocabulary'),
    '#options' => kvantstudio_select_vocabularies(),
  );
  $form['site_commerce_parametrs']['create']['select_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Rename the parameter'),
    '#description'   => t('If you do not fill out this field, The setting will be created, and his name will be the same as the selected dictionary.'),
  );

  // Now we add our submit button, for submitting the form results.
  //
  // The 'actions' wrapper used here isn't strictly necessary for tabledrag,
  // but is included as a Form API recommended practice.
  $form['site_commerce_parametrs']['create']['actions'] = array('#type' => 'actions');
  $form['site_commerce_parametrs']['create']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['site_commerce_parametrs']['site_commerce_sum_parameters_arithmetic'] = array(
    '#title'         => t('Calculate the value of the parameters as arithmetic mean'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_sum_parameters_arithmetic', TRUE),
    '#description'   => t('The cost of options is calculated and added to the cost of the goods. If the box is not ticked, the value of the parameters will be calculated as the sum of all selected parameters.'),
  );

  // Перечень доступных параметров.
  $query = db_select('site_commerce_parametrs', 'n');
  $query->fields('n');
  $result = $query->execute();

  $form['site_commerce_parametrs']['delete']['actions'] = array('#type' => 'actions');
  foreach ($result as $row) {
    $vocabulary = taxonomy_vocabulary_load($row->vid);
    $form['site_commerce_parametrs'][$row->machine_name] = array(
      '#type' => 'fieldset',
      '#title' => t('Configure the @name', array('@name' => $row->name)),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['site_commerce_parametrs'][$row->machine_name][$row->machine_name . '_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Parameter name'),
      '#default_value' => $row->name,
      '#required' => TRUE,
    );
    $form['site_commerce_parametrs'][$row->machine_name][$row->machine_name . '_allow'] = array(
      '#title'         => t('Allow display select button'),
      '#type'          => 'checkbox',
      '#default_value' => variable_get($row->machine_name . '_allow', FALSE),
    );
    $form['site_commerce_parametrs'][$row->machine_name][$row->machine_name . '_allow_arithmetic'] = array(
      '#title'         => t('Allow to consider cost in calculating the arithmetic mean'),
      '#type'          => 'checkbox',
      '#default_value' => variable_get($row->machine_name . '_allow_arithmetic', TRUE),
    );
    $form['site_commerce_parametrs'][$row->machine_name][$row->machine_name . '_allow_for_terms'] = array(
      '#type' => 'checkboxes',
      '#return_value' => 'fields',
      '#title' => t('Categories for which are authorized to display the parameter'),
      '#default_value' => variable_get($row->machine_name . '_allow_for_terms', array()),
      '#options' => kvantstudio_select_terms($vocabulary_categories->vid, FALSE, TRUE),
    );
    $form['site_commerce_parametrs'][$row->machine_name][$row->machine_name . '_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#parametr_id' => $row->id,
    );
    $form['site_commerce_parametrs'][$row->machine_name][$row->machine_name . '_save'] = array(
      '#type' => 'submit',
      '#value' => t('Save parametr'),
      '#parametr_id' => $row->id,
      '#parametr_machine_name' => $row->machine_name,
    );
  }

  //----------------------------------------------------------------------------
  // Настройки поиска.
  //----------------------------------------------------------------------------
  if (module_exists('search')) {
    $form['site_commerce_search'] = array(
      '#type' => 'fieldset',
      '#title' => t('Setup search'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['site_commerce_search']['site_commerce_search_form'] = array(
      '#title'         => t('To allow to display the form of search under the catalogue'),
      '#type'          => 'checkbox',
      '#default_value' => variable_get('site_commerce_search_form', TRUE),
      '#description'   => t('If the goods are not available in the catalog, search form will not be displayed.'),
    );
    $form['site_commerce_search']['site_commerce_search_cron'] = array(
      '#title'         => t('To allow the update index search catalog items by using cron'),
      '#type'          => 'checkbox',
      '#default_value' => variable_get('site_commerce_search_cron', TRUE),
    );
    $form['site_commerce_search']['site_commerce_search_autocomplete'] = array(
      '#title'         => t('To resolve autocompletion of input in the form of search'),
      '#type'          => 'checkbox',
      '#default_value' => variable_get('site_commerce_search_autocomplete', TRUE),
    );
    $form['site_commerce_search']['site_commerce_search_requests_save'] = array(
      '#title'         => t('Allow to save search queries'),
      '#type'          => 'checkbox',
      '#default_value' => variable_get('site_commerce_search_requests_save', FALSE),
      '#description' => t('Inclusion of this parameter can slow down speed of search if the parameter autocompletion of input is included.'),
    );
    $form['site_commerce_search']['site_commerce_search_engine'] = array(
      '#title'         => t('To allow to use search in connection with words'),
      '#type'          => 'checkbox',
      '#default_value' => variable_get('site_commerce_search_engine', TRUE),
      '#description'   => t('If noted, the standard search form Drupаl will search for the part of each the entered words.'),
    );
    $form['site_commerce_search']['site_commerce_search_cron_limit'] = array(
      '#type' => 'textfield',
      '#title' => t('Quantity of positions for which the search index for one start a cron will be updated'),
      '#default_value' => variable_get('site_commerce_search_cron_limit', 1000),
      '#required' => TRUE,
    );
    $form['site_commerce_search']['site_commerce_search_limit'] = array(
      '#type' => 'textfield',
      '#title' => t('The number of items on one search page'),
      '#default_value' => variable_get('site_commerce_search_limit', '10'),
      '#required' => TRUE,
    );
    $form['site_commerce_search']['site_commerce_search_form_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title of the form of search'),
      '#default_value' => variable_get('site_commerce_search_form_title'),
      '#description'   => t('Leave this field blank if you want to name formed automatically based on the number of goods. For example, «Search within N products».'),
      '#required' => FALSE,
    );
    $form['site_commerce_search']['site_commerce_search_form_alt'] = array(
      '#type' => 'textfield',
      '#title' => t('The emerging help at prompting the mouse on a floor of input of the search form'),
      '#default_value' => variable_get('site_commerce_search_form_alt'),
      '#required' => FALSE,
    );
    $form['site_commerce_search']['site_commerce_search_form_up'] = array(
      '#type' => 'textarea',
      '#title' => t('The help above a field of input of the search form'),
      '#default_value' => variable_get('site_commerce_search_form_up'),
      '#required' => FALSE,
      '#rows' => 2,
    );
    $form['site_commerce_search']['site_commerce_search_form_bottom'] = array(
      '#type' => 'textarea',
      '#title' => t('The help under a field of input of the search form'),
      '#default_value' => variable_get('site_commerce_search_form_bottom'),
      '#required' => FALSE,
      '#rows' => 2,
    );
  }

  //----------------------------------------------------------------------------
  // Настройки корзины.
  //----------------------------------------------------------------------------
  $form['site_commerce_cart'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure the shopping cart'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['site_commerce_cart']['site_commerce_allow_cart'] = array(
    '#title'         => t('To allow display of a cart'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_cart', TRUE),
    '#description'   => t('Allows you to add items to your cart, display unit with a basket.'),
  );
  $form['site_commerce_cart']['site_commerce_allow_button_add_to_cart'] = array(
    '#title'         => t('To allow to display the button of addition in a cart'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_button_add_to_cart', TRUE),
  );
  $form['site_commerce_cart']['site_commerce_allow_button_add_to_cart_in_catalog'] = array(
    '#title'         => t('To allow to display the button of addition in a cart in the catalog'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_button_add_to_cart_in_catalog', FALSE),
  );
  $form['site_commerce_cart']['site_commerce_order_block'] = array(
    '#title'         => t('To allow to display the button of addition in a cart in the block'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_order_block', FALSE),
  );
  $form['site_commerce_cart']['site_commerce_allow_quantity_cart'] = array(
    '#title'         => t('To allow to specify quantity at addition of a position in the shopping cart'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_quantity_cart', TRUE),
  );
  $form['site_commerce_cart']['site_commerce_allow_total_cost_in_cart'] = array(
    '#title'         => t('To allow to display a total cost of the order in the block with a cart'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_total_cost_in_cart', FALSE),
  );
  $form['site_commerce_cart']['site_commerce_allow_update_cost_cart'] = array(
    '#title'         => t('To allow the update cost in the shopping cart before ordering'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_update_cost_cart', TRUE),
    '#description'   => t('If checked, the value of position in the shopping cart will automatically change, when the price changes in the price list.'),
  );
  $form['site_commerce_cart']['site_commerce_cart_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Heading of page of the shopping cart'),
    '#default_value' => variable_get('site_commerce_cart_title'),
    '#required' => TRUE,
  );
  $form['site_commerce_cart']['site_commerce_cart_title_button_add'] = array(
    '#type' => 'textfield',
    '#title' => t('Heading of button add to cart'),
    '#default_value' => variable_get('site_commerce_cart_title_button_add'),
    '#required' => TRUE,
  );
  $form['site_commerce_cart']['site_commerce_cart_text_empty'] = array(
    '#type' => 'textfield',
    '#title' => t('Text of empty cart'),
    '#default_value' => variable_get('site_commerce_cart_text_empty'),
    '#required' => TRUE,
  );
  $form['site_commerce_cart']['site_commerce_add_to_cart_allow_statuses'] = array(
    '#type' => 'checkboxes',
    '#return_value' => 'fields',
    '#title' => t('Statuses at which it is authorized to add a position in a cart'),
    '#default_value' => variable_get('site_commerce_add_to_cart_allow_statuses', array()),
    '#options' => site_commerce_position_status(),
  );
  $form['site_commerce_cart']['site_commerce_cart_button_submit_css'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS classes, which will be added to the button in the cart'),
    '#default_value' => variable_get('site_commerce_cart_button_submit_css', 'btn btn-default'),
    '#required' => FALSE,
  );

  //----------------------------------------------------------------------------
  // Настройки формы оформления заказа.
  //----------------------------------------------------------------------------
  if (module_exists('site_commerce_order')) {
    $form['site_commerce_order'] = array(
      '#type' => 'fieldset',
      '#title' => t('Configuring the order registration'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['site_commerce_order']['site_commerce_allow_create_order_cost_zero'] = array(
      '#title'         => t('To resolve registration of orders if cost of a position it is equal to zero'),
      '#type'          => 'checkbox',
      '#default_value' => variable_get('site_commerce_allow_create_order_cost_zero', TRUE),
    );
    $form['site_commerce_order']['site_commerce_hide_order_number'] = array(
      '#title'         => t('To hide a sequential order number'),
      '#type'          => 'checkbox',
      '#default_value' => variable_get('site_commerce_hide_order_number', FALSE),
      '#description'   => t('If noted, the as a order numbers in the notifications will be displayed a specially the generated code , instead of an actual number.'),
    );
    $form['site_commerce_order']['site_commerce_captcha_phone'] = array(
      '#title'         => t('To resolve check of telephone number on a mask of input: 8 (code) number.'),
      '#type'          => 'checkbox',
      '#default_value' => variable_get('site_commerce_captcha_phone', FALSE),
    );
    if (module_exists('captcha')) {
      $form['site_commerce_order']['site_commerce_order_captcha'] = array(
        '#title' => t('To resolve protection against a spam in the form of registration of the order by means of module CAPTCHA'),
        '#type' => 'checkbox',
        '#default_value' => variable_get('site_commerce_order_captcha', FALSE),
      );
    }
    $form['site_commerce_order']['site_commerce_order_cost_limit'] = array(
      '#type' => 'textfield',
      '#title' => t('The minimal sum of the order at which it can be issued'),
      '#default_value' => variable_get('site_commerce_order_cost_limit', 0),
      '#required' => FALSE,
    );
    $form['site_commerce_order']['site_commerce_order_discounts'] = array(
      '#type' => 'textfield',
      '#title' => t('Discounts at registration of the order'),
      '#default_value' => variable_get('site_commerce_order_discounts', 0),
      '#description' => t('The discount which operates at registration of the order if a total amount exceeds or is equal to the certain number. A format of a writing COST-DISCOUNT (for example, the discount of 18 % at registration of the order from 3000 will become 3000-18). It is possible to specify some ranges of the discount. Each range is necessary for dividing a blank (for example, 3000-18 10000-20).'),
      '#required' => FALSE,
    );
    $form['site_commerce_order']['site_commerce_order_fields'] = array(
      '#type' => 'checkboxes',
      '#return_value' => 'fields',
      '#title' => t('Available fields of the form of registration of the order'),
      '#default_value' => variable_get('site_commerce_order_fields', array()),
      '#options' => array(
        'full_name' => t('Full name'),
        'last_name' => t('Last name'),
        'first_name' => t('First name'),
        'patronymic' => t('Patronymic'),
        'organization' => t('Organization'),
        'position' => t('Position in the organization'),
        'postcode' => t('Postcode'),
        'city' => t('City'),
        'address_delivery' => t('The address of delivery'),
        'phone' => t('Contact phone'),
        'email' => t('E-mail'),
        'date' => t('Date'),
        'time' => t('Time'),
        'comment' => t('The comment to the order'),
      ),
    );
    $form['site_commerce_order']['site_commerce_order_fields_required'] = array(
      '#type' => 'checkboxes',
      '#return_value' => 'fields',
      '#title' => t('Mandatory for filling a field of the form of registration of the order'),
      '#default_value' => variable_get('site_commerce_order_fields_required', array()),
      '#options' => array(
        'full_name_required' => t('Full name'),
        'last_name_required' => t('Last name'),
        'first_name_required' => t('First name'),
        'patronymic_required' => t('Patronymic'),
        'organization_required' => t('Organization'),
        'position_required' => t('Position in the organization'),
        'postcode_required' => t('Postcode'),
        'city_required' => t('City'),
        'address_delivery_required' => t('The address of delivery'),
        'phone_required' => t('Contact phone'),
        'email_required' => t('E-mail'),
        'date_required' => t('Date'),
        'time_required' => t('Time'),
        'comment_required' => t('The comment to the order'),
      ),
    );
    foreach (site_commerce_order_stage_delivery() as $type_delivery => $key) {
      $options[$type_delivery] = $key['title'];
    }
    $form['site_commerce_order']['site_commerce_order_ways_of_delivery'] = array(
      '#type' => 'checkboxes',
      '#return_value' => 'fields',
      '#title' => t('Available ways of delivery of the products'),
      '#default_value' => variable_get('site_commerce_order_ways_of_delivery', array()),
      '#options' => $options,
    );
    $form['site_commerce_order']['site_commerce_order_conditions'] = array(
      '#type' => 'textarea',
      '#title' => t('Conditions of payment and delivery'),
      '#default_value' => variable_get('site_commerce_order_conditions'),
      '#rows' => 2,
      '#required' => FALSE,
    );
  }

  //----------------------------------------------------------------------------
  // Настройки покупки в один клик.
  //----------------------------------------------------------------------------
  $form['site_commerce_buy_in_one_click'] = array(
    '#type' => 'fieldset',
    '#title' => t('Set up of function of purchase in one click'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['site_commerce_buy_in_one_click']['site_commerce_allow_buy_in_one_click'] = array(
    '#title'         => t('To resolve purchase in one click'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_commerce_allow_buy_in_one_click', TRUE),
  );
  $form['site_commerce_buy_in_one_click']['site_commerce_buy_in_one_click_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title of button of purchase in one click'),
    '#default_value' => variable_get('site_commerce_buy_in_one_click_title'),
    '#required' => TRUE,
  );
  $form['site_commerce_buy_in_one_click']['site_commerce_buy_in_one_click_form_data'] = array(
    '#type' => 'textarea',
    '#title' => t('Form data'),
    '#default_value' => variable_get('site_commerce_buy_in_one_click_form_data'),
    '#rows' => 2,
    '#required' => FALSE,
  );

  //----------------------------------------------------------------------------
  // Настройки модуля спецпредложения.
  //----------------------------------------------------------------------------
  if (module_exists('site_commerce_special_offers')) {
    $form['site_commerce_special_offers'] = array(
      '#type' => 'fieldset',
      '#title' => t('Set up of special offers'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['site_commerce_special_offers']['site_commerce_special_offers_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Heading of page of special offers'),
      '#default_value' => variable_get('site_commerce_special_offers_title'),
      '#required' => TRUE,
    );
  }

  //----------------------------------------------------------------------------
  // Яндекс Маркет.
  //----------------------------------------------------------------------------
  $form['site_commerce_yandex_market'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings of Yandex Market'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );



  //----------------------------------------------------------------------------
  // Примечания.
  //----------------------------------------------------------------------------
  $form['site_commerce_notes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuring the notes'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['site_commerce_notes']['site_commerce_note_global_cost'] = array(
    '#type' => 'textfield',
    '#title' => t('The note which is finished in a floor cost to all goods (for example, cost is specified from the VAT)'),
    '#default_value' => variable_get('site_commerce_note_global_cost', ''),
    '#required' => FALSE,
  );
  $form['site_commerce_notes']['site_commerce_note_catalog_description'] = array(
    '#type' => 'textarea',
    '#title' => t('The text which is displayed at a browsing the catalog above categories of the goods'),
    '#default_value' => variable_get('site_commerce_note_catalog_description'),
    '#rows' => 2,
    '#required' => FALSE,
  );
  $form['site_commerce_notes']['site_commerce_note_catalog_description_bottom'] = array(
    '#type' => 'textarea',
    '#title' => t('The text which is displayed at a browsing the catalog under categories of the goods'),
    '#default_value' => variable_get('site_commerce_note_catalog_description_bottom'),
    '#rows' => 2,
    '#required' => FALSE,
  );
  $form['site_commerce_notes']['site_commerce_note_cart_description'] = array(
    '#type' => 'textarea',
    '#title' => t('The text which is displayed at viewing the shopping cart'),
    '#default_value' => variable_get('site_commerce_note_cart_description'),
    '#rows' => 2,
    '#required' => FALSE,
  );
  $form['site_commerce_notes']['site_commerce_note_if_order_denied'] = array(
    '#type' => 'textarea',
    '#title' => t('The text which is displayed at viewing the shopping cart if it is forbidden registrations of orders'),
    '#default_value' => variable_get('site_commerce_note_if_order_denied'),
    '#rows' => 2,
    '#required' => FALSE,
  );
  $form['site_commerce_notes']['site_commerce_note_order_description'] = array(
    '#type' => 'textarea',
    '#title' => t('The text which is displayed in e-mail the message of the customer after registration of the order'),
    '#default_value' => variable_get('site_commerce_note_order_description'),
    '#rows' => 2,
    '#required' => FALSE,
  );
  $form['site_commerce_notes']['site_commerce_note_minimal_cost'] = array(
    '#type' => 'textarea',
    '#title' => t('The text of the note which is displayed near to the name of a position if the minimal cost is specified'),
    '#default_value' => variable_get('site_commerce_note_minimal_cost'),
    '#rows' => 2,
    '#required' => FALSE,
  );
  $form['site_commerce_notes']['site_commerce_note_none_cost'] = array(
    '#type' => 'textarea',
    '#title' => t('The text of the note which is displayed near to the name of a position if cost is not specified'),
    '#default_value' => variable_get('site_commerce_note_none_cost'),
    '#rows' => 2,
    '#required' => FALSE,
  );
  $form['site_commerce_notes']['site_commerce_note_none_quantity'] = array(
    '#type' => 'textarea',
    '#title' => t('The text of the note which is displayed near to the name of a position if quantity is not specified'),
    '#default_value' => variable_get('site_commerce_note_none_quantity'),
    '#rows' => 2,
    '#required' => FALSE,
  );
  $form['site_commerce_notes']['site_commerce_note_index_position'] = array(
    '#type' => 'textarea',
    '#title' => t('The text of heading which is displayed above the block of the associated positions in a card of the goods'),
    '#default_value' => variable_get('site_commerce_note_index_position'),
    '#rows' => 2,
    '#required' => FALSE,
  );

  $form['#submit'][] = 'site_commerce_settings_form_submit';

  $form = system_settings_form($form);
  $form['actions']['submit'] += array(
    '#access' => user_access('administer site commerce'),
  );

  return $form;
}

/**
 * Проверка формы.
 */
function site_commerce_settings_form_validate($form, &$form_state) {
  // Проверка безопасности при сохранении настроек формы.
  if (!user_access('administer site commerce')) {
    form_set_error('form', t('Access denied'));
  }

  // Выполняем проверку на корректность заполнения поля скидка.
  if (isset($form_state['values']['site_commerce_order_discounts'])) {
    $array = array();
    $strDiscounts = trim($form_state['values']['site_commerce_order_discounts']);
    if ($strDiscounts) {
      $arrayDiscounts = explode(" ", $strDiscounts);
      foreach ($arrayDiscounts as $key) {
        if (strpos($key, "-")) {
          $array[] = $key;
        }
      }
      $form_state['values']['site_commerce_order_discounts'] = implode(" ", $array);
    }
    else {
      $form_state['values']['site_commerce_order_discounts'] = '0';
    }
  }
}

/**
 * Сохранение настроек.
 */
function site_commerce_settings_form_submit($form, &$form_state) {
  // Сохранение скидок.
  if (isset($form_state['values']['site_commerce_order_discounts'])) {
    $array = array();
    $strDiscounts = trim($form_state['values']['site_commerce_order_discounts']);
    if ($strDiscounts) {
      $arrayDiscounts = explode(" ", $strDiscounts);
      foreach ($arrayDiscounts as $key) {
        if (strpos($key, "-")) {
          list($cost, $discount) = explode('-', $key);
          $array[] = array(
            'cost' => $cost,
            'discount' => $discount
          );
        }
      }
      $value = serialize($array);
    }
    else {
      $value = '';
    }
    variable_set('site_commerce_order_discounts_values', $value);
  }

  // Создание параметров товара.
  $vid = (int) $form_state['values']['select_vocabulary'];
  if ($vid > 0) {
    site_commerce_create_parametr_by_vid($vid, $form_state['values']['select_name']);
  }

  // Удаление параметров.
  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $id = (int) $form_state['clicked_button']['#parametr_id'];
    db_delete('site_commerce_parametrs')
      ->condition('id', $id)
      ->execute();
  }

  // Обновление параметра.
  if ($form_state['clicked_button']['#value'] == t('Save parametr')) {
    $id = (int) $form_state['clicked_button']['#parametr_id'];
    $machine_name = (string) $form_state['clicked_button']['#parametr_machine_name'];
    $name = $form_state['values'][$machine_name . '_title'];
    $fields = array(
      'name' => $name,
    );
    db_update('site_commerce_parametrs')
      ->fields($fields)
      ->condition('id', $id)
      ->execute();
  }

  cache_clear_all();
}