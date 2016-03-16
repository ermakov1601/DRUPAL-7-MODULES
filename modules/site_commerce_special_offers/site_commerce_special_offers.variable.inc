<?php

/**
 * @file
 * Site commerce translatable variables.
 */

/**
 * Implements hook_variable_info().
 */
function site_commerce_special_offers_variable_info($options) {
  $variable['site_commerce_special_offers_title'] = array(
    'type' => 'string',
    'title' => t('Heading of page of special offers'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_commerce',
  );
  return $variable;
}
