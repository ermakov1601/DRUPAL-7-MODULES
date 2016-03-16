<?php
// $Id:

/**
 * @file
 * Admin page callbacks for the site_faq module.
 */

/**
 * Вызов формы настроек модуля site_faq module.
 */
function site_faq_settings_form($form, &$form_state) {

  $form['site_faq_block_limit_nodes'] = array(
    '#type'          => 'textfield',
    '#title'         => 'Количество сообщений новостей в блоке "Последние новости"',
    '#description'   => 'Установите желаемое значение параметра, не рекомендуется устанавливать значения ниже 5 и выше 10.',
    '#default_value' => variable_get('site_faq_block_limit_nodes', '5'),
    '#required'      => TRUE,
    '#size' => 2,
  );

  $form['site_faq_default_main_imagecache_preset'] = array(
    '#type'          => 'select',
    '#title'         => 'Стиль отображения главного изображения',
    '#default_value' => variable_get('site_faq_default_main_imagecache_preset', 'large'),
    '#options'       => kvantstudio_select_imagecache_presets(),
    '#required'      => TRUE,
    '#description'   => l(t('To pass to management of styles of images'), 'admin/config/media/image-styles'),
  );

  $form['site_faq_default_imagecache_preset'] = array(
    '#type'          => 'select',
    '#title'         => 'Стиль отображения прикрепленных изображений',
    '#default_value' => variable_get('site_faq_default_imagecache_preset', 'thumbnail'),
    '#options'       => kvantstudio_select_imagecache_presets(),
    '#required'      => TRUE,
    '#description'   => l(t('To pass to management of styles of images'), 'admin/config/media/image-styles'),
  );

  $form['site_faq_default_imagecache_preset_in_block'] = array(
    '#type'          => 'select',
    '#title'         => t('Style of display of the attached images in the block the latest news'),
    '#default_value' => variable_get('site_faq_default_imagecache_preset_in_block', 'thumbnail'),
    '#options'       => kvantstudio_select_imagecache_presets(),
    '#required'      => TRUE,
    '#description'   => l(t('To pass to management of styles of images'), 'admin/config/media/image-styles'),
  );

  $form['site_faq_image_in_block'] = array(
    '#title'         => t('To display image in the block of the latest news'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_faq_image_in_block'),
  );

  $form['site_faq_node_summary_in_block'] = array(
    '#title'         => t('To display brief contents of news in the block the latest news'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_faq_node_summary_in_block'),
  );

  $form['site_faq_yandex_buttons'] = array(
    '#title'         => t('To display buttons Yandex for the publication of links in social networks'),
    '#type'          => 'checkbox',
    '#default_value' => variable_get('site_faq_yandex_buttons'),
  );

  $form['site_faq_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Заголовок страницы вопрос-ответ',
    '#description' => 'При изменении значения не забудьте обновить кэш сайта.',
    '#default_value' => t(variable_get('site_faq_title', 'Question-answer')),
    '#required' => TRUE,
  );

  $form['site_faq_link'] = array(
    '#type' => 'textfield',
    '#title' => 'Название ссылки страницы вопрос-ответ',
    '#description' => 'Не изменяйте это значение на готовом сайте! Значение должно быть на латинице, слова через тире с маленькой буквы. При изменении значения не забудьте обновить кэш сайта.',
    '#default_value' => variable_get('site_faq_link', variable_get('site_news_link', 'press-centr') . '/' .'otvety'),
    '#required' => TRUE,
  );

  return system_settings_form($form);

}