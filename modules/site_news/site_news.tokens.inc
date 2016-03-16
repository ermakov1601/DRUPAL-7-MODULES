<?php

/**
* @file
* Вспомогательный файл модуля site_news.
*
* Содержит в себе функции, которые реализуют токены.
*/

/**
 * Implements hook_token_info().
 */
function site_news_token_info() {
  // Node tokens.
  $node = array();
  if (module_exists('node')) {
    $node['site-news-node-path'] = array(
      'name' => t('Path of material'),
      'description' => t('Path to material of press center.'),
      'type' => 'node',
    );
  }

  // Taxonomy term and vocabulary tokens.
  $term = array();
  if (module_exists('taxonomy')) {
    $term['site-news-term-path'] = array(
      'name' => t('Category path of material'),
      'description' => t('Path to category of material of press center.'),
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
function site_news_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  // Node tokens.
  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'site-news-node-path':
          $replacements[$original] = site_news_node_path($node);
          break;
      }
    }
  }

  // Taxonomy term tokens.
  if ($type == 'term' && !empty($data['term'])) {
    $term = $data['term'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'site-news-term-path':
          $replacements[$original] = site_news_term_path($term);
          break;
      }
    }
  }

  return $replacements;
}