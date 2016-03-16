<?php

/**
 * @file
 * Реализация темы по умолчанию для рендеринга прочих позиций в карточке товара.
 *
 * Этот шаблон используется для вывода прочих позиций в карточке товара.
 *
 * Доступные переменные:
 *
 * $term - термин таксономии, категория текущего товара.
 *  $term->name - название категории.
 *
 * @see template_preprocess_site_commerce_other_positions()
 *
 * @ingroup themeable
 */
?>
<?php if(count($pids)): ?>
  <div class="site-commerce-other-positions">
    <div class="site-commerce-other-positions__category-title">
      <h3><?php print t('The similar products in category &laquo;@name&raquo;', array('@name' => $term->name)); ?></h3>
    </div>

    <!-- Список прочих позиций -->
    <div class="site-commerce-other-positions__items">
      <?php foreach ($pids as $pid => $position): ?>
      <div class="site-commerce-other-positions__item">
        <div>
          <!-- Изображение товара. -->
          <div class="site-commerce-other-positions site-commerce-other-positions_image">
            <a href="<?php print base_path(); ?><?php print $position['position']->alias; ?>" rel="nofollow" class="site-commerce-other-positions__image-link">
              <img class="site-commerce-other-positions__image" src="<?php print $position['image']['style_url']; ?>" alt="<?php print $position['image']['alt']; ?>" title="<?php print $position['image']['title']; ?>">
            </a>
          </div>

          <!-- Название товара. -->
          <div class="site-commerce-other-positions__title">
            <a class="site-commerce-other-positions__title-link" href="<?php print base_path(); ?><?php print $position['position']->alias; ?>">
              <?php print $position['title']; ?>
            </a>
          </div>

          <?php if($position['cost']): ?>
          <!-- Стоимость товара. -->
          <div class="site-commerce-other-positions__cost"><?php print $position['cost']; ?></div>
          <?php endif; ?>
        </div>
      </div>
      <?php endforeach; ?>
    </div>
  </div>
<?php endif; ?>