<?php

/**
 * @file
 * Site commerce translatable variables.
 */

/**
 * Implements hook_variable_group_info().
 */
function site_commerce_variable_group_info() {
  $groups['site_commerce'] = array(
    'title' => t('Online store'),
    'access' => 'administer site commerce',
  );
  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function site_commerce_variable_info($options) {
  $variable['site_commerce_catalog_title'] = array(
    'type' => 'string',
    'title' => t('Heading of page of the catalogue'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_cart_title'] = array(
    'type' => 'string',
    'title' => t('Heading of page of the shopping cart'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_cart_title_button_add'] = array(
    'type' => 'string',
    'title' => t('Heading of button add to cart'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_cart_text_empty'] = array(
    'type' => 'string',
    'title' => t('Text of empty cart'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_buy_in_one_click_title'] = array(
    'type' => 'string',
    'title' => t('Title of button of purchase in one click'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_note_global_cost'] = array(
    'type' => 'string',
    'title' => t('The note which is finished in a floor cost to all goods (for example, cost is specified from the VAT)'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_note_catalog_description'] = array(
    'type' => 'string',
    'title' => t('The text which is displayed at a browsing the catalog above categories of the goods'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_note_catalog_description_bottom'] = array(
    'type' => 'string',
    'title' => t('The text which is displayed at a browsing the catalog under categories of the goods'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_note_cart_description'] = array(
    'type' => 'string',
    'title' => t('The text which is displayed at viewing the shopping cart'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_note_if_order_denied'] = array(
    'type' => 'string',
    'title' => t('The text which is displayed at viewing the shopping cart if it is forbidden registrations of orders'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_note_order_description'] = array(
    'type' => 'string',
    'title' => t('The text which is displayed in e-mail the message of the customer after registration of the order'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_note_minimal_cost'] = array(
    'type' => 'string',
    'title' => t('The text of the note which is displayed near to the name of a position if the minimal cost is specified'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_note_none_cost'] = array(
    'type' => 'string',
    'title' => t('The text of the note which is displayed near to the name of a position if cost is not specified'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_note_none_quantity'] = array(
    'type' => 'string',
    'title' => t('The text of the note which is displayed near to the name of a position if quantity is not specified'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_note_index_position'] = array(
    'type' => 'string',
    'title' => t('The text of heading which is displayed above the block of the associated positions in a card of the goods'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_search_form_title'] = array(
    'type' => 'string',
    'title' => t('Title of the form of search'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_search_form_alt'] = array(
    'type' => 'string',
    'title' => t('The emerging help at prompting the mouse on a floor of input of the search form'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_search_form_up'] = array(
    'type' => 'string',
    'title' => t('The help above a field of input of the search form'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  $variable['site_commerce_search_form_bottom'] = array(
    'type' => 'string',
    'title' => t('The help under a field of input of the search form'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );

  return $variable;
}
