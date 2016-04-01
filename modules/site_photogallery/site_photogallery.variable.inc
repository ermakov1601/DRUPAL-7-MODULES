<?php

/**
 * @file
 * Site photogallery translatable variables.
 */

/**
 * Implements hook_variable_group_info().
 */
function site_photogallery_variable_group_info() {
  $groups['site_photogallery'] = array(
    'title' => t('Photo albums'),
    'access' => 'administer site photogallery',
  );

  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function site_photogallery_variable_info($options) {
  $variable['site_photogallery_title'] = array(
    'type' => 'string',
    'title' => t('Heading of page of the photo albums'),
    'description' => '',
    'localize' => TRUE,
    'group' => 'site_photogallery',
  );

  return $variable;
}
