<?php

/**
 * @file
 * Реализация темы по умолчанию для рендеринга каталога товарова в блоке.
 *
 * Этот шаблон используется для вывода каталога, категорий товаров в блоках.
 *
 * Доступные переменные:
 *
 * @see template_preprocess_site_commerce_block_categories()
 *
 * @ingroup themeable
 */
?>
<noindex>
<div class="site-commerce-block-categories">
  <?php foreach ($terms as $tid => $term): ?>
  <div class="site-commerce-block-categories__item">
    <a href="<?php print base_path(); ?><?php print $term->alias; ?>" rel="nofollow" class="site-commerce-block-categories__link">
      <!-- Изображение категории. -->
      <span id="site-commerce-block-categories__image-<?php print $term->tid; ?>" class="site-commerce-block-categories__image">
        <img src="<?php print $term->image_style_url; ?>" alt="">
      </span>
      <!-- Название категории. -->
      <?php print $term->name; ?>
    </a>
  </div>
  <?php endforeach; ?>
</div>
</noindex>