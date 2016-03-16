<?php

/**
 * @file
 * Settings page of module kvantstudio.
 */

/**
 * Функция: форма вызова настроек модуля - kvantstudio.
 */
function kvantstudio_settings_form($form, &$form_state) {
  $form['kvantstudio_allow_litebox'] = array(
    '#type'          => 'checkbox',
    '#title'         => t("Allow the connection <a href='https://github.com/joemottershaw/litebox' target='_blank'>LiteBox</a>"),
    '#default_value' => variable_get('kvantstudio_allow_litebox', TRUE),
    '#description' => t("Default project modules KVANTSTUDIO use this library to display pop-up images. Wherever you need to call the pop-up image is added to the link class <a class='litebox'></a>. For groups of images added to the links parameter data-litebox-group. For example, data-litebox-group='site-commerce-litebox-images'."),
  );

  $form['kvantstudio_allow_zoom'] = array(
    '#type'          => 'checkbox',
    '#title'         => t("Allow the connection <a href='http://www.jacklmoore.com/zoom' target='_blank'>jQuery Zoom</a>"),
    '#default_value' => variable_get('kvantstudio_allow_zoom', TRUE),
    '#description' => t("A plugin to enlarge images on touch, click, or mouseover."),
  );

  $form['kvantstudio_allow_readmore'] = array(
    '#type'          => 'checkbox',
    '#title'         => t("Allow the connection <a href='https://github.com/jedfoster/Readmore.js' target='_blank'>Readmore</a>"),
    '#default_value' => variable_get('kvantstudio_allow_readmore', TRUE),
    '#description' => t("A plugin for collapsing and expanding long blocks of text. Use css class='readmore-block' for blocks of text."),
  );

  $form['kvantstudio_allow_equalheights'] = array(
    '#type'          => 'checkbox',
    '#title'         => t("Allow the connection <a href='https://github.com/mattbanks/jQuery.equalHeights' target='_blank'>equalheights</a>"),
    '#default_value' => variable_get('kvantstudio_allow_equalheights', TRUE),
    '#description' => t("Automatically aligns the columns in a row to the same height as a function of the content."),
  );

  // Определяем html5 элементы.
  $form['kvantstudio_html5'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings of HTML5'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['kvantstudio_html5']['kvantstudio_allow_microdata_breadcrumblist'] = array(
    '#type'          => 'checkbox',
    '#title'         => t("Allow to create a breadcrumb format microdata, <a href='http://kvantstudio.ru/node/40' target='_blank'>see details</a>"),
    '#default_value' => variable_get('kvantstudio_allow_microdata_breadcrumblist', FALSE),
    '#description' => "",
  );

  // Определяем fieldset.
  $form['kvantstudio_mail'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings of e-mail'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['kvantstudio_mail']['kvantstudio_manager_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail address of the manager of a site'),
    '#default_value' => variable_get('kvantstudio_manager_mail'),
    '#required' => TRUE,
    '#description' => t('This address is used to send different notifications to the manager of the site. For example responsible for the execution of orders from the online store SiteCommerce.'),
  );
  $form['kvantstudio_mail']['kvantstudio_admin_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail address of the system administrator of a site'),
    '#default_value' => variable_get('kvantstudio_admin_mail'),
    '#required' => TRUE,
    '#description' => t('This address is used to send various error notification on the site that can occur when using the modules of the project KVANTSTUDIO.'),
  );
  $form['kvantstudio_mail']['kvantstudio_developer_mail_allow'] = array(
    '#type' => 'checkbox',
    '#title' => t('To allow to shape reports on mistakes in modules of the project kvantstudio to the developer'),
    '#default_value' => variable_get('kvantstudio_developer_mail_allow', TRUE),
  );

  $form['#submit'][] = 'kvantstudio_settings_form_submit';

  $form = system_settings_form($form);

  return $form;
}

/**
 * Сохранение настроек.
 */
function kvantstudio_settings_form_submit($form, &$form_state) {
  cache_clear_all();
}