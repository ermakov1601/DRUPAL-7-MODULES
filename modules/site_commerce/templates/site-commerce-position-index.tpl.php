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
  <div class="site-commerce-position-index__title">
    <?php print t('This product is part of'); ?>
  </div>

  <?php foreach ($variables['position_index'] as $pid => $position): ?>
  <div class="site-commerce-position-index__item">
    <a href="<?php print base_path(); ?><?php print $position->alias; ?>" rel="nofollow" class="site-commerce-position-index__link"><?php print $position->title; ?></a>
  </div>
  <?php endforeach; ?>
</div>
<?php endif; ?>
</noindex>