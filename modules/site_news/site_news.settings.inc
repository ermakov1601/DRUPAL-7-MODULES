<?php

/**
 * @file
 * Admin page callbacks for the settings of site_news module.
 */

/**
 * Форма настроек.
 */
function site_news_settings_form($form, &$form_state) {
  // Общие настройки.
  $form['site_news_basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $vocabulary = taxonomy_vocabulary_machine_name_load('site_news_vocabulary');
  $terms = kvantstudio_select_terms($vocabulary->vid, FALSE);
  if (count($terms)) {
    $form['site_news_basic']['site_news_summary_with_body_taxonomy_terms'] = array(
      '#type' => 'checkboxes',
      '#return_value' => 'fields',
      '#title' => t('Categories of the press center for which it is necessary to display the full text of the publication at viewing contents on a category'),
      '#default_value' => variable_get('site_news_summary_with_body_taxonomy_terms', array()),
      '#options' => kvantstudio_select_terms($vocabulary->vid, FALSE),
      '#required' => FALSE,
    );
  }

  $form['site_news_basic']['site_news_summary_taxonomy_terms'] = array(
    '#title'         => t('Allow display a brief contents'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_news_summary_taxonomy_terms', TRUE),
    '#description'   => t('Brief contents of news will be displayed entitled each news at viewing the list of news on a category.'),
  );

  $form['site_news_basic']['site_news_summary_lenght'] = array(
    '#type'          => 'textfield',
    '#title'         => t('The number of characters, which is clipped a brief contents'),
    '#default_value' => variable_get('site_news_summary_lenght', '200'),
    '#required'      => TRUE,
    '#size' => 3,
  );

  $form['site_news_basic']['site_news_default_main_imagecache_preset'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of display of the main image'),
    '#default_value' => variable_get('site_news_default_main_imagecache_preset', 'large'),
    '#options'       => kvantstudio_select_imagecache_presets(),
    '#required'      => TRUE,
    '#description'   => l(t('To pass to management of styles of images'), 'admin/config/media/image-styles'),
  );

  $form['site_news_basic']['site_news_default_imagecache_preset'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of display of the attached images'),
    '#default_value' => variable_get('site_news_default_imagecache_preset', 'thumbnail'),
    '#options'       => kvantstudio_select_imagecache_presets(),
    '#required'      => TRUE,
    '#description'   => l(t('To pass to management of styles of images'), 'admin/config/media/image-styles'),
  );

  $form['site_news_basic']['site_news_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Heading of page of the press center'),
    '#default_value' => variable_get('site_news_title', t('Press center')),
    '#required' => FALSE,
    '#description'   => t('Default value — «Press center».'),
  );

  // Настройки отображения в блоке.
  $form['site_news_block'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings of display in the block'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  if (count($terms)) {
    $form['site_news_block']['site_news_block_taxonomy_terms'] = array(
      '#type' => 'checkboxes',
      '#return_value' => 'fields',
      '#title' => t('Categories of the press center for display in the block the LATEST NEWS'),
      '#default_value' => variable_get('site_news_block_taxonomy_terms', array()),
      '#options' => kvantstudio_select_terms($vocabulary->vid, FALSE),
      '#required' => FALSE,
    );
  }

  $form['site_news_block']['site_news_block_limit_nodes'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Quantity of news in the block'),
    '#default_value' => variable_get('site_news_block_limit_nodes', '5'),
    '#required'      => TRUE,
    '#size' => 2,
  );

  $form['site_news_block']['site_news_node_summary_in_block'] = array(
    '#title'         => t('Allow display a brief contents'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_news_node_summary_in_block'),
  );
  $form['site_news_block']['site_news_image_in_block'] = array(
    '#title'         => t('Allow display images'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_news_image_in_block'),
  );
  $form['site_news_block']['site_news_default_imagecache_preset_in_block'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of display of the attached images'),
    '#default_value' => variable_get('site_news_default_imagecache_preset_in_block', 'thumbnail'),
    '#options'       => kvantstudio_select_imagecache_presets(),
    '#required'      => TRUE,
    '#description'   => l(t('To pass to management of styles of images'), 'admin/config/media/image-styles'),
  );

  return system_settings_form($form);
}