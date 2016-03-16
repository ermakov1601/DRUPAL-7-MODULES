<?php
// $Id$

/**
 * @file
 * Admin page callbacks for the site_contacts module.
 */

/**
 * Функция: форма вызова настроек модуля site_contacts.
 */
function site_contacts_adjustments_form($form, &$form_state) {
  $form['coordinates'] = array(
    '#type' => 'fieldset',
    '#title' => t('The general information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['coordinates']['site_contacts_name'] = array(
    '#type' => 'textfield',
    '#title' => t('The name of the organization'),
    '#default_value' => variable_get('site_contacts_name'),
    '#required' => FALSE,
  );

  $form['coordinates']['site_contacts_description'] = array(
    '#type' => 'textarea',
    '#title' => t('The description of the organization'),
    '#default_value' => variable_get('site_contacts_description'),
    '#rows' => 3,
    '#required' => FALSE,
  );

  $form['coordinates']['site_contacts_postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t('The postal index'),
    '#default_value' => variable_get('site_contacts_postal_code'),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_address_country'] = array(
    '#type' => 'textfield',
    '#title' => t('The country'),
    '#default_value' => variable_get('site_contacts_address_country'),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_address_region'] = array(
    '#type' => 'textfield',
    '#title' => t('Region'),
    '#default_value' => variable_get('site_contacts_address_region'),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_address_locality'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => variable_get('site_contacts_address_locality'),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_street_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Street, the house, body, structure and т. Item (that is the address to within a building)'),
    '#default_value' => variable_get('site_contacts_street_address'),
    '#size' => 120,
    '#required' => FALSE,
  );

  $form['coordinates']['site_contacts_telephone'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone'),
    '#default_value' => variable_get('site_contacts_telephone'),
    '#description' => t('Preferential format: {a code of the country} ({a code of city}) {number}, for example: 7(890) 123-45-67'),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_fax_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Fax'),
    '#default_value' => variable_get('site_contacts_fax_number'),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
    '#default_value' => variable_get('site_contacts_email'),
    '#required' => FALSE,
  );

  $form['coordinates']['site_contacts_opening_hours_days'] = array(
    '#type' => 'checkboxes',
    '#return_value' => 'fields',
    '#title' => t('Days of work'),
    '#default_value' => variable_get('site_contacts_opening_hours_days', array()),
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
  $form['coordinates']['site_contacts_opening_hours_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Operating time'),
    '#default_value' => variable_get('site_contacts_opening_hours_time'),
    '#required' => FALSE,
  );
  $form['coordinates']['site_contacts_opening_hours_note'] = array(
    '#type' => 'textfield',
    '#title' => t('The note on an operating time'),
    '#default_value' => variable_get('site_contacts_opening_hours_note'),
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

  $form['essential_elements'] = array(
    '#type' => 'fieldset',
    '#title' => t('Requisites of the organization'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['essential_elements']['site_contacts_inn'] = array(
    '#type' => 'textfield',
    '#title' => t('INN'),
    '#default_value' => variable_get('site_contacts_inn'),
    '#required' => FALSE,
  );
  $form['essential_elements']['site_contacts_kpp'] = array(
    '#type' => 'textfield',
    '#title' => t('KPP'),
    '#default_value' => variable_get('site_contacts_kpp'),
    '#required' => FALSE,
  );
  $form['essential_elements']['site_contacts_ogrn'] = array(
    '#type' => 'textfield',
    '#title' => t('OGRN'),
    '#default_value' => variable_get('site_contacts_ogrn'),
    '#required' => FALSE,
  );
  $form['essential_elements']['site_contacts_okved'] = array(
    '#type' => 'textfield',
    '#title' => t('OKVED'),
    '#default_value' => variable_get('site_contacts_okved'),
    '#required' => FALSE,
  );
  $form['essential_elements']['site_contacts_okpo'] = array(
    '#type' => 'textfield',
    '#title' => t('OKPO'),
    '#default_value' => variable_get('site_contacts_okpo'),
    '#required' => FALSE,
  );
  $form['essential_elements']['site_contacts_okugu'] = array(
    '#type' => 'textfield',
    '#title' => t('OKUGU'),
    '#default_value' => variable_get('site_contacts_okugu'),
    '#required' => FALSE,
  );
  $form['essential_elements']['site_contacts_okato'] = array(
    '#type' => 'textfield',
    '#title' => t('OKATO'),
    '#default_value' => variable_get('site_contacts_okato'),
    '#required' => FALSE,
  );

  $form['bank_elements'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bank requisites'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['bank_elements']['site_contacts_bank'] = array(
    '#type' => 'textfield',
    '#title' => t('Bank'),
    '#default_value' => variable_get('site_contacts_bank'),
    '#required' => FALSE,
  );
  $form['bank_elements']['site_contacts_bank_address'] = array(
    '#type' => 'textarea',
    '#title' => t('The address of bank'),
    '#default_value' => variable_get('site_contacts_bank_address'),
    '#rows' => 3,
    '#required' => FALSE,
  );
  $form['bank_elements']['site_contacts_bank_inn'] = array(
    '#type' => 'textfield',
    '#title' => t('INN/KPP of bank'),
    '#default_value' => variable_get('site_contacts_bank_inn'),
    '#required' => FALSE,
  );
  $form['bank_elements']['site_contacts_bank_ogrn'] = array(
    '#type' => 'textfield',
    '#title' => t('OGRN'),
    '#default_value' => variable_get('site_contacts_bank_ogrn'),
    '#required' => FALSE,
  );
  $form['bank_elements']['site_contacts_bank_account_r'] = array(
    '#type' => 'textfield',
    '#title' => t('r/account'),
    '#default_value' => variable_get('site_contacts_bank_account_r'),
    '#required' => FALSE,
  );
  $form['bank_elements']['site_contacts_bank_account_k'] = array(
    '#type' => 'textfield',
    '#title' => t('k/account'),
    '#default_value' => variable_get('site_contacts_bank_account_k'),
    '#required' => FALSE,
  );
  $form['bank_elements']['site_contacts_bank_bik'] = array(
    '#type' => 'textfield',
    '#title' => t('BIK'),
    '#default_value' => variable_get('site_contacts_bank_bik'),
    '#required' => FALSE,
  );

  // Блок настроек карты проезда.
  $form['site_contacts_development'] = array(
    '#type' => 'fieldset',
    '#title' => t('Adjustments of a card of travel'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['site_contacts_development']['site_contacts_map'] = array(
    '#type' => 'textarea',
    '#title' => t('JavaScript a code for an insert of a sitemap'),
    '#description' => t('Code for an insert a Yandex of a card or Google card.'),
    '#default_value' => variable_get('site_contacts_map'),
    '#rows' => 5,
    '#required' => FALSE,
  );

  return system_settings_form($form);

}
