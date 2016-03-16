<?php

/**
 * @file
 * Site news translatable variables.
 */

/**
 * Implements hook_variable_group_info().
 */
function site_news_variable_group_info() {
  $groups['site_news'] = array(
    'title' => t('Press center'),
    'access' => 'administer site news',
  );

  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function site_news_variable_info($options) {
  $variable['site_news_title'] = array(
    'type' => 'string',
    'title' => t('Heading of page of the press center'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_news',
  );

  return $variable;
}
