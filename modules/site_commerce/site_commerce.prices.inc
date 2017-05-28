<?php

/**
 * Page callback: displays prices.
 */
function site_commerce_prices() {
  $files = [];
  $result = db_query('SELECT n.field_site_commerce_file_fid, n.field_site_commerce_file_description FROM {field_data_field_site_commerce_file} n ORDER BY n.field_site_commerce_file_description');
  foreach ($result as $row) {
    $fid = $row->field_site_commerce_file_fid;
    $file = file_load($fid);
    $files[$fid] = [
      'description' => $row->field_site_commerce_file_description,
      'url' => file_create_url($file->uri),
    ];

  }
  return theme('site_commerce_prices', array('files' => $files));
}

/**
 * Returns HTML for a list of available node types for node creation.
 *
 * @param $variables
 *   An associative array containing:
 *   - content: An array of content types.
 *
 * @ingroup themeable
 */
function theme_site_commerce_prices($variables) {
  $files = $variables['files'];
}