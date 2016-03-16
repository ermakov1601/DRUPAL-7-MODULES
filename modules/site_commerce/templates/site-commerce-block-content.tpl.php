<?php

/**
 * @file
 * Реализация темы по умолчанию для рендеринга позиций в блоках.
 *
 * Этот шаблон используется для вывода параметров карточки товара в полном виде.
 *
 * Доступные переменные:
 * - $path: ссылка на страницу с полным перечнем позиций по блоку.
 *
 * @see template_preprocess_site_commerce_block_content()
 *
 * @ingroup themeable
 */
?>
<noindex>
<div class="site-commerce-block-content">
  <?php if ($path): ?>
  <div class="site-commerce-block-content__read-more">
    <a href="<?php print base_path(); ?><?php print $path; ?>" rel="nofollow" class="site-commerce-block-content__read-more-link">
      <?php print t('read more'); ?>
    </a>
  </div>
  <?php endif; ?>

  <div class="site-commerce-block-content__items">
    <?php foreach ($pids as $pid => $position): ?>
    <div class="site-commerce-block-content__item clearfix">
      <div class="site-commerce-block-content__data clearfix">
        <!-- Изображение товара. -->
        <div class="site-commerce-block-content__data-image">
          <a href="<?php print base_path(); ?><?php print $position['position']->alias; ?>" rel="nofollow" class="site-commerce-block-content__image-link">
            <img class="site-commerce-block-content__image" src="<?php print $position['image']['style_url']; ?>" alt="<?php print $position['image']['alt']; ?>" title="<?php print $position['image']['title']; ?>">
          </a>
        </div>

        <!-- Название товара. -->
        <div class="site-commerce-block-content__data-title">
          <a href="<?php print base_path(); ?><?php print $position['position']->alias; ?>" rel="nofollow" class="site-commerce-other-positions__title-link">
            <?php print $position['title']; ?>
          </a>
        </div>
      </div>

      <?php if($position['cost']): ?>
      <!-- Стоимость товара. -->
      <div class="site-commerce-block-content__cost clearfix"><?php print $position['cost']; ?></div>
      <?php endif; ?>

      <?php if($position['cart']): ?>
      <!-- Кнопка добавить в корзину. -->
      <div class="site-commerce-block-content__cart clearfix"><?php print $position['cart']; ?></div>
      <?php endif; ?>
    </div>
    <?php endforeach; ?>
  </div>
</div>
</noindex>