<?php

/**
 * @file
 * Реализация темы по умолчанию для рендеринга главного каталога товаров.
 *
 * Этот шаблон используется для вывода главного каталога товаров - категорий и подкатегорий.
 *
 * Доступные переменные:
 * - $changed: Дата и время последнего обновления товаров в каталоге.
 * - $term: Термин таксономии, категория товара.
 *   $term->name: Название категории.
 *   $term->image_url: Путь до изображения категории.
 *   $term->count: Количество позиций в категории (если разрешено их отображать в настройках каталога).
 *
 * @see template_preprocess_site_commerce_catalog()
 *
 * @ingroup themeable
 */
?>
<div class="site-commerce-catalog clearfix">
  <?php if(variable_get('site_commerce_search_form', TRUE) && $search_form = site_commerce_create_search_form('site_commerce')): ?>
    <!-- Форма поиска над каталогом товаров. -->
    <div class="site-commerce-catalog__search-form"><?php print $search_form; ?></div>
  <?php endif; ?>

  <?php if($changed): ?>
    <!-- Дата и время последнего обновления товаров в каталоге. -->
    <div class="site-commerce-catalog__changed"><?php print t('Date of updating'); ?>&nbsp;<?php print $changed; ?></div>
  <?php endif; ?>

  <?php if($catalog_description = variable_get('site_commerce_note_catalog_description', '')): ?>
    <!-- Описание каталога товаров. -->
    <div class="site-commerce-catalog__description"><?php print $catalog_description; ?></div>
  <?php endif; ?>

  <!-- Каталог товаров. -->
  <div class="site-commerce-catalog__categories clearfix">
    <div class="site-commerce-categories">
      <?php foreach ($terms as $tid => $term): ?>
      <div class="site-commerce-categories__item">
        <div class="site-commerce-category">
          <!-- Корневая категория. -->
          <div class="site-commerce-category__item site-commerce-category__item_root clearfix">
            <?php if($term['root']->image_url): ?>
            <!-- Изображение корневой категории. -->
            <div id="site-commerce-category-image-<?php print $term['root']->tid; ?>" class="site-commerce-category-image">
              <a class="site-commerce-category-image__link" href="<?php print base_path(); ?><?php print $term['root']->alias; ?>">
                <img class="site-commerce-category-image__logo" src="<?php print $term['root']->image_style_url; ?>" alt="<?php print $term['root']->name; ?>">
              </a>
            </div>
            <?php endif; ?>

            <div id="site-commerce-category-title-<?php print $term['root']->tid; ?>" class="site-commerce-category-title">
              <a class="site-commerce-category-title__link" href="<?php print base_path(); ?><?php print $term['root']->alias; ?>">
                <!-- Название корневой категории. -->
                <?php print $term['root']->name; ?>
                <!-- Количество позиций. -->
                <?php if($term['root']->count): ?>
                <span class="site-commerce-category-title__count">(<?php print $term['root']->count; ?>)</span>
                <?php endif; ?>
              </a>
            </div>
          </div>

          <?php // Определяем имеются подкатегории. ?>
          <?php if(count($term['childrens'])): ?>
          <div class="site-commerce-category__item_childrens clearfix">

            <div class="site-commerce-category__item site-commerce-category__item_children site-commerce-category__item_children-root clearfix">
              <a class="site-commerce-category-title__link" href="<?php print base_path(); ?><?php print $term['root']->alias; ?>">
                <!-- Название корневой категории. -->
                <?php print $term['root']->name; ?>
                <!-- количество позиций. -->
                <?php if($term['root']->count): ?>
                <span class="site-commerce-category-title__count">(<?php print $term['root']->count; ?>)</span>
                <?php endif; ?>
              </a>
            </div>

            <!-- Подкатегории. -->
            <?php foreach ($term['childrens'] as $tid => $term['children']): ?>
            <div class="site-commerce-category__item site-commerce-category__item_children clearfix">
              <a class="site-commerce-category-children-title__link" href="<?php print base_path(); ?><?php print $term['children']->alias; ?>">
                <?php if($term['children']->image_style_url): ?>
                <!-- Изображение подкатегории. -->
                <span id="site-commerce-category-image-<?php print $term['children']->tid; ?>" class="site-commerce-category-children-image">
                  <img class="site-commerce-category-children-image__logo" src="<?php print $term['children']->image_style_url; ?>" alt="<?php print $term['children']->name; ?>">
                </span>
                <?php endif; ?>
                <!-- Название подкатегории. -->
                <span class="site-commerce-category-children-title__value">
                  <?php print $term['children']->name; ?>
                  <!-- Количество позиций подкатегории. -->
                  <?php if($term['children']->count): ?>
                  <span class="site-commerce-category-children-title__count">(<?php print $term['children']->count; ?>)</span>
                  <?php endif; ?>
                </span>
              </a>
            </div>
            <?php endforeach; ?>
          </div>
          <?php endif; ?>
        </div>
      </div>
      <?php endforeach; ?>
    </div>
  </div>

  <?php if($catalog_description = variable_get('site_commerce_note_catalog_description_bottom', '')): ?>
    <!-- Описание каталога товаров под категориями. -->
    <div class="site-commerce-catalog__description"><?php print $catalog_description; ?></div>
  <?php endif; ?>

</div>