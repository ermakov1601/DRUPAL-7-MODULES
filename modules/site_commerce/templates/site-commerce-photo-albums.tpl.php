<?php

/**
 * @file
 * Template file site-commerce-photo-albums.tpl.php.
 */

/**
 * Доступные переменные:
 *
 * $variables[''].
 */
?>
<div class="site-commerce-photo-albums clearfix">
  <h2><?php print variable_get('site_photogallery_title', t('Photo albums')); ?></h2>

  <?php foreach ($variables['data'] as $tid => $item): ?>
    <a rel="nofollow" href="/<?php print $item['alias'] ?>" class="site-commerce-photo-albums__link"><?php print $item['name'] ?></a>
  <?php endforeach; ?>
</div>
