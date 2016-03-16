<?php

/**
* @file
* Вспомогательный файл модуля site_commerce.
*
* Содержит в себе функции, которые реализуют токены.
*/

/**
 * Implements hook_token_info().
 */
function site_commerce_token_info() {
  // SiteCommerce tokens.
  $types['site_commerce'] = array(
    'name' => t('Products'),
    'description' => t('Tokens related to individual products.'),
    'needs-data' => 'site_commerce',
  );

  $site_commerce = array();
  $site_commerce['title'] = array(
    'name' => t('Title'),
    'description' => t('Title of product.'),
  );
  $site_commerce['product-path'] = array(
    'name' => t('Product path'),
    'description' => t('Path to product.'),
  );

  $term = array();
  // Taxonomy term and vocabulary tokens.
  if (module_exists('taxonomy')) {
    $term['category-path'] = array(
      'name' => t('Category path of products'),
      'description' => t('Path to category of product.'),
      'type' => 'term',
    );
  }

  return array(
    'types' => $types,
    'tokens' => array(
      'site_commerce' => $site_commerce,
      'term' => $term,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function site_commerce_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'site_commerce' && !empty($data['site_commerce'])) {
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'title':
          $replacements[$original] = $data['site_commerce']->title;
          break;

        case 'product-path':
          $replacements[$original] = site_commerce_product_path($data['site_commerce']);
          break;
      }
    }
  }

  // Taxonomy term tokens.
  if ($type == 'term' && !empty($data['term'])) {
    $term = $data['term'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'category-path':
          $replacements[$original] = site_commerce_category_path($term);
          break;
      }
    }
  }

  return $replacements;
}