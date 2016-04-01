<?php

/**
* @file
* Вспомогательный файл модуля site_photogallery.
*
* Содержит в себе функции, которые реализуют токены.
*/

/**
 * Implements hook_token_info().
 */
function site_photogallery_token_info() {
  // Node tokens.
  $node = array();
  if (module_exists('node')) {
    $node['site-photogallery-node-path'] = array(
      'name' => t('Path of photo album'),
      'description' => t('Path to photo album.'),
      'type' => 'node',
    );
  }

  // Taxonomy term and vocabulary tokens.
  $term = array();
  if (module_exists('taxonomy')) {
    $term['site-photogallery-term-path'] = array(
      'name' => t('Category path of photo album'),
      'description' => t('Path to category of photo album.'),
      'type' => 'term',
    );
  }

  return array(
    'tokens' => array(
      'node' => $node,
      'term' => $term,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function site_photogallery_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  // Node tokens.
  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'site-photogallery-node-path':
          $replacements[$original] = site_photogallery_node_path($node);
          break;
      }
    }
  }

  // Taxonomy term tokens.
  if ($type == 'term' && !empty($data['term'])) {
    $term = $data['term'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'site-photogallery-term-path':
          $replacements[$original] = site_photogallery_term_path($term);
          break;
      }
    }
  }

  return $replacements;
}