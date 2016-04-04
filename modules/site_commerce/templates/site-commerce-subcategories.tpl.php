<?php

/**
 * @file
 * Реализация темы по умолчанию для рендеринга подкатегорий.
 *
 * Этот шаблон используется для вывода подкатегорий при
 * просмотре каталога товаров.
 *
 * Доступные переменные:
 * - $term: Термин таксономии, категория товара.
 *
 * @see template_preprocess_site_commerce_subcategories()
 *
 * @ingroup themeable
 */
?>
<div class="clearfix">
  <ul class="site-commerce-subcategories">
    <?php foreach ($terms as $tid => $term): ?>
    <li class="site-commerce-subcategories__item">
      <div class="site-commerce-subcategory">
        <?php if($term->image_url): ?>
        <!-- Изображение категории. -->
        <div class="site-commerce-subcategory__image">
          <div class="site-commerce-subcategory-image">
            <a href="<?php print base_path(); ?><?php print $term->alias; ?>" rel="nofollow" class="site-commerce-subcategory-image__link" title="<?php print $term->name; ?>">
              <img class="site-commerce-subcategory-image__logo" src="<?php print $term->image_url; ?>" alt="<?php print $term->name; ?>">
            </a>
          </div>
        </div>
        <?php endif; ?>

        <!-- Название категории. -->
        <div class="site-commerce-subcategory__title">
          <div class="site-commerce-subcategory-title">
            <a href="<?php print base_path(); ?><?php print $term->alias; ?>" class="site-commerce-subcategory-title__link"><?php print t('@value', array('@value' => $term->name)); ?></a>
            <?php if($term->count): ?>
            <span class="site-commerce-count-in-category">(<?php print $term->count; ?>)</span>
            <?php endif; ?>
          </div>
        </div>
      </div>
    </li>
    <?php endforeach; ?>
  </ul>
</div>