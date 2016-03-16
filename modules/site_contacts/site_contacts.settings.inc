<?php
// $Id$

/**
 * @file
 * Admin page callbacks for the site_contacts module.
 */

/**
 * Функция: форма вызова настроек модуля site_contacts.
 */
function site_contacts_settings_form($form, &$form_state) {
  global $language;

  // Подключаем ckeditor.
  // Загружаем модуль ckeditor для поля комментария к заказу.
  if (module_exists('libraries') && module_exists('ckeditor')) {
    $path = libraries_get_path('ckeditor');
    drupal_add_js($path . '/ckeditor.js', array('group' => JS_LIBRARY, 'cache' => FALSE));
    drupal_add_js(drupal_get_path('module', 'site_contacts') . '/js/site_contacts.js', array('group' => JS_LIBRARY));
  }

  $languages = locale_language_list('name');
  if (count($languages) > 1) {
    drupal_set_message(t('On your site includes several languages. In order to provide contact information for another language and go for it again, go to the settings. If your site does not require multi-language support, disable unused languages.'));
  }

  $form['coordinates'] = array(
    '#type' => 'fieldset',
    '#title' => t('The general information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['coordinates']['site_contacts_name_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('The name of the organization'),
    '#default_value' => variable_get('site_contacts_name_' . $language->language),
    '#required' => FALSE,
  );

  $form['coordinates']['site_contacts_description_' . $language->language] = array(
    '#type' => 'textarea',
    '#title' => t('The description of the organization'),
    '#default_value' => variable_get('site_contacts_description_' . $language->language),
    '#rows' => 3,
    '#required' => FALSE,
  );

  $form['coordinates']['site_contacts_postal_code_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('The postal index'),
    '#default_value' => variable_get('site_contacts_postal_code_' . $language->language),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_address_country_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('The country'),
    '#default_value' => variable_get('site_contacts_address_country_' . $language->language),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_address_region_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('Region'),
    '#default_value' => variable_get('site_contacts_address_region_' . $language->language),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_address_locality_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => variable_get('site_contacts_address_locality_' . $language->language),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_street_address_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('Street, the house, body, structure and т. Item (that is the address to within a building)'),
    '#default_value' => variable_get('site_contacts_street_address_' . $language->language),
    '#size' => 120,
    '#required' => FALSE,
  );

  $form['coordinates']['site_contacts_phone_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('Phone'),
    '#default_value' => variable_get('site_contacts_phone_' . $language->language),
    '#description' => t('Preferential format: {a code of the country} ({a code of city}) {number}, for example: 7(890) 123-45-67'),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_fax_number_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('Fax'),
    '#default_value' => variable_get('site_contacts_fax_number_' . $language->language),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_email_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
    '#default_value' => variable_get('site_contacts_email_' . $language->language),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_skype_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('Skype'),
    '#default_value' => variable_get('site_contacts_skype_' . $language->language),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_youtube_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('YouTube'),
    '#default_value' => variable_get('site_contacts_youtube_' . $language->language),
    '#description' => t('Full reference to the channel YouTube. For example, http //www.youtube.com/channel/UCauHuMfq47WpBPjZJhYiWCw'),
    '#required' => FALSE,
  );

  $form['coordinates']['site_contacts_opening_hours_days_' . $language->language] = array(
    '#type' => 'checkboxes',
    '#return_value' => 'fields',
    '#title' => t('Days of work'),
    '#default_value' => variable_get('site_contacts_opening_hours_days_' . $language->language, array()),
    '#options' => array(
      'Mo' => t('Mo'),
      'Tu' => t('Tu'),
      'We' => t('We'),
      'Th' => t('Th'),
      'Fr' => t('Fr'),
      'Sa' => t('Sa'),
      'Su' => t('Su'),
    ),
    '#description' => t('Choose days on which you work.'),
  );
  $form['coordinates']['site_contacts_opening_hours_time_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('Operating time'),
    '#default_value' => variable_get('site_contacts_opening_hours_time_' . $language->language),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_opening_hours_note_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('The note on an operating time'),
    '#default_value' => variable_get('site_contacts_opening_hours_note_' . $language->language),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_url_ru'] = array(
    '#type' => 'textfield',
    '#title' => t('The address of a site in zone RU'),
    '#default_value' => variable_get('site_contacts_url_ru'),
    '#description' => t('Specify the name of a site without www.'),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_url_rf'] = array(
    '#type' => 'textfield',
    '#title' => t('The address of a site in zone РФ'),
    '#default_value' => variable_get('site_contacts_url_rf'),
    '#description' => t('Specify the name of a site without www.'),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_url_com'] = array(
    '#type' => 'textfield',
    '#title' => t('The address of a site in zone COM'),
    '#default_value' => variable_get('site_contacts_url_com'),
    '#description' => t('Specify the name of a site without www.'),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_other_' . $language->language] = array(
    '#type' => 'textarea',
    '#title' => t('Other information'),
    '#default_value' => variable_get('site_contacts_other_' . $language->language),
    '#rows' => 5,
    '#required' => FALSE,
    '#attributes' => array('id' => 'site-contacts-other-textarea'),
  );

  $form['management'] = array(
    '#type' => 'fieldset',
    '#title' => t('Management'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['management']['site_contacts_management_' . $language->language] = array(
    '#type' => 'textarea',
    '#title' => t('Information on a management of the organization'),
    '#default_value' => variable_get('site_contacts_management_' . $language->language),
    '#rows' => 5,
    '#required' => FALSE,
    '#description' => t('HTML a code carrying the information about a management of the organization, a photo, the description, the contact information.'),
    '#attributes' => array('id' => 'site-contacts-management-textarea'),
  );

  $form['essential_elements'] = array(
    '#type' => 'fieldset',
    '#title' => t('Requisites of the organization'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['essential_elements']['site_contacts_inn_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('INN'),
    '#default_value' => variable_get('site_contacts_inn_' . $language->language),
    '#required' => FALSE,
  );
  $form['essential_elements']['site_contacts_kpp_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('KPP'),
    '#default_value' => variable_get('site_contacts_kpp_' . $language->language),
    '#required' => FALSE,
  );
  $form['essential_elements']['site_contacts_ogrn_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('OGRN'),
    '#default_value' => variable_get('site_contacts_ogrn_' . $language->language),
    '#required' => FALSE,
  );
  $form['essential_elements']['site_contacts_ogrnip_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('OGRNIP'),
    '#default_value' => variable_get('site_contacts_ogrnip_' . $language->language),
    '#required' => FALSE,
  );
  $form['essential_elements']['site_contacts_okved_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('OKVED'),
    '#default_value' => variable_get('site_contacts_okved_' . $language->language),
    '#required' => FALSE,
  );
  $form['essential_elements']['site_contacts_okpo_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('OKPO'),
    '#default_value' => variable_get('site_contacts_okpo_' . $language->language),
    '#required' => FALSE,
  );
  $form['essential_elements']['site_contacts_okugu_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('OKUGU'),
    '#default_value' => variable_get('site_contacts_okugu_' . $language->language),
    '#required' => FALSE,
  );
  $form['essential_elements']['site_contacts_okato_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('OKATO'),
    '#default_value' => variable_get('site_contacts_okato_' . $language->language),
    '#required' => FALSE,
  );

  $form['bank_elements'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bank requisites'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['bank_elements']['site_contacts_bank_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('Bank'),
    '#default_value' => variable_get('site_contacts_bank_' . $language->language),
    '#required' => FALSE,
  );
  $form['bank_elements']['site_contacts_bank_address_' . $language->language] = array(
    '#type' => 'textarea',
    '#title' => t('The address of bank'),
    '#default_value' => variable_get('site_contacts_bank_address_' . $language->language),
    '#rows' => 3,
    '#required' => FALSE,
  );
  $form['bank_elements']['site_contacts_bank_inn_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('INN/KPP of bank'),
    '#default_value' => variable_get('site_contacts_bank_inn_' . $language->language),
    '#required' => FALSE,
  );
  $form['bank_elements']['site_contacts_bank_ogrn_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('OGRN'),
    '#default_value' => variable_get('site_contacts_bank_ogrn_' . $language->language),
    '#required' => FALSE,
  );
  $form['bank_elements']['site_contacts_bank_account_r_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('r/account'),
    '#default_value' => variable_get('site_contacts_bank_account_r_' . $language->language),
    '#required' => FALSE,
  );
  $form['bank_elements']['site_contacts_bank_account_k_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('k/account'),
    '#default_value' => variable_get('site_contacts_bank_account_k_' . $language->language),
    '#required' => FALSE,
  );
  $form['bank_elements']['site_contacts_bank_bik_' . $language->language] = array(
    '#type' => 'textfield',
    '#title' => t('BIK'),
    '#default_value' => variable_get('site_contacts_bank_bik_' . $language->language),
    '#required' => FALSE,
  );

  // Блок настроек карты проезда.
  $form['site_contacts_development'] = array(
    '#type' => 'fieldset',
    '#title' => t('Adjustments of a card of travel'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['site_contacts_development']['site_contacts_map_' . $language->language] = array(
    '#type' => 'textarea',
    '#title' => t('JavaScript a code for an insert of a sitemap'),
    '#description' => t('Code for an insert a Yandex of a card or Google card.'),
    '#default_value' => variable_get('site_contacts_map_' . $language->language),
    '#rows' => 5,
    '#required' => FALSE,
  );

  return system_settings_form($form);

}