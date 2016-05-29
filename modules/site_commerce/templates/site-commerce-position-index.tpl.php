<?php

/**
 * @file
 * Реализация темы по умолчанию для рендеринга карточки товара.
 *
 * Этот шаблон используется для вывода параметров карточки товара в полном виде.
 *
 * Доступные переменные:
 *
 * @see template_preprocess_site_commerce_position_index()
 *
 * @ingroup themeable
 */
?>
<noindex>
<?php if($variables['position_index']): ?>
<div class="site-commerce-position-index">
  <h3><?php print t('This product is part of'); ?>:</h3>

  <?php foreach ($variables['position_index'] as $pid => $position): ?>
  <div class="site-commerce-position-index__item">
    <a href="<?php print base_path(); ?><?php print $position->alias; ?>" target="_blank" rel="nofollow" class="site-commerce-position-index__link"><?php print $position->title; ?></a>
  </div>
  <?php endforeach; ?>
</div>
<?php endif; ?>
</noindex>