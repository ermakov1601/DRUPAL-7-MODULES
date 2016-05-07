<?php

/**
 * @file
 * Реализация темы по умолчанию для рендеринга карточки товара.
 *
 * Этот шаблон используется для вывода параметров карточки товара в полном виде.
 *
 * Доступные переменные:
 *
 * @see template_preprocess_site_commerce()
 *
 * @ingroup themeable
 */
?>
<?php render($page['content']['metatags']); ?>
<div id="site-commerce-<?php print $pid; ?>" class="<?php print $classes; ?> clearfix" <?php print $attributes; ?>>
  <div class="site-commerce__header clearfix">
    <!-- Блок с изображениями товара. -->
    <div class="site-commerce__header-images">
      <!-- Артикул товара. -->
      <div class="site-commerce__header-code">
        <?php print t('Part number'); ?>: <?php print $identifier; ?>
      </div>

      <?php if($site_commerce_images): ?>
        <!-- Изображения товара, которые формируются сторонним форматером. Задается в настройках поля. -->
        <?php print $site_commerce_images; ?>
      <?php else: ?>
        <?php if(count($images['main'])): ?>
          <!-- Главное изображение товара. -->
          <div class="site-commerce__header-image-main">
            <a class="site-commerce__link site-commerce__link_image litebox zoom" href="<?php print $images['main']['url']; ?>" rel="site-commerce-images" data-litebox-group="site-commerce-litebox-images" data-litebox-text="<?php print $images['main']['title']; ?>">
              <img class="site-commerce__image site-commerce__image_main" src="<?php print $images['main']['style_url']; ?>" alt="<?php print $images['main']['alt']; ?>" title="<?php print $images['main']['title']; ?>">
            </a>
          </div>
        <?php endif; ?>
        <?php if(count($images['other'])): ?>
          <!-- Прочие изображения товара. -->
          <div class="site-commerce__header-images-other">
            <?php foreach ($images['other'] as $key => $image): ?>
              <a class="site-commerce__link site-commerce__link_image litebox" href="<?php print $image['url']; ?>" rel="site-commerce-images" data-litebox-group="site-commerce-litebox-images" data-litebox-text="<?php print $image['title']; ?>">
                <img class="site-commerce__image site-commerce__image_other" src="<?php print $image['style_url']; ?>" alt="<?php print $image['alt']; ?>" title="<?php print $image['title']; ?>">
              </a>
            <?php endforeach; ?>
          </div>
        <?php endif; ?>
      <?php endif; ?>
    </div>

    <div class="site-commerce__header-actions">
      <?php if($site_commerce_status): ?>
        <!-- Статус позиции. -->
        <?php print $site_commerce_status; ?>
      <?php endif; ?>

      <?php
        // Содержимое переменной $site_commerce_cost формируется шаблоном site-commerce-cost.tpl.php.
        if($site_commerce_cost):
      ?>
        <!-- Стоимость товара. -->
        <?php print $site_commerce_cost; ?>
      <?php endif; ?>

      <?php if($site_commerce_cost_none_exist): ?>
        <!-- Примечание, которое выводиться если базовая стоимость равна нулю. -->
        <div class="site-commerce-content__note site-commerce-content__note_cost-none-exist clearfix">
          <?php print $site_commerce_cost_none_exist; ?>
        </div>
      <?php endif; ?>

      <?php if($site_commerce_cost_minimal_exist): ?>
        <!-- Примечание, которое выводиться если существует минимальная стоимость. -->
        <div class="site-commerce-content__note site-commerce-content__note_cost-minimal-exist clearfix">
          <?php print $site_commerce_cost_minimal_exist; ?>
        </div>
      <?php endif; ?>

      <?php if($site_commerce_cart): ?>
        <!-- Добавить в корзину, купить в 1 клик. -->
        <?php print $site_commerce_cart; ?>
      <?php endif; ?>
    </div>
  </div>

  <?php if($site_commerce_admin_actions_form): ?>
    <!-- Ссылки администрирования. -->
    <?php print $site_commerce_admin_actions_form; ?>
  <?php endif; ?>

  <div class="site-commerce__content clearfix">
    <?php
      // Содержимое переменной $site_commerce_position_index формируется шаблоном site-commerce-position-index.tpl.php.
      if($site_commerce_position_index):
    ?>
    <!-- Блок для вывода позиций, в которые данный товар входит. -->
    <?php print $site_commerce_position_index; ?>
    <?php endif; ?>

    <?php if($site_commerce_summary): ?>
    <!-- Краткое описание товара. -->
    <div class="site-commerce-content__description">
      <h2 class="site-commerce-content__description-title"><?php print t('Description'); ?></h2>
      <div class="site-commerce-content__switcher">
        <span class="site-commerce-content__switcher-item"><a class="site-commerce-content__switcher-link site-commerce-content__switcher-link_briefly site-commerce-content__switcher-link_active" href="javascript:void(0);">Краткое</a></span><span class="site-commerce-content__switcher-item"><a class="site-commerce-content__switcher-link site-commerce-content__switcher-link_detail" href="javascript:void(0);">Полное</a></span>
      </div>
    </div>

    <div class="site-commerce-content__summary-text">
      <?php print $site_commerce_summary; ?>
    </div>
    <?php endif; ?>

    <?php
      // Содержимое переменной $site_commerce_data формируется шаблоном site-commerce-data.tpl.php.
      if($site_commerce_data):
    ?>
    <!-- Характеристики товара. -->
    <?php print $site_commerce_data; ?>
    <?php endif; ?>
  </div>

  <?php if($site_commerce_body): ?>
    <!-- Подробное описание товара -->
    <?php if($site_commerce_summary): ?>
      <div class="site-commerce-content__body-title">
        <a class="site-commerce-content__switcher-link_detail" href="javascript:void(0);"><span class="link-js"><?php print t('Show full description'); ?></span></a>
      </div>
      <div class="site-commerce-content__body element-invisible"><?php print $site_commerce_body; ?></div>
    <?php else: ?>
      <?php print $site_commerce_body; ?>
    <?php endif; ?>
  <?php endif; ?>

  <?php if($site_commerce_position_index_content): ?>
    <!-- Товары, которые можно купить вместе с текущим товаров, входят в его состав. -->
    <div id="site-commerce-position-index-content-<?php print $pid; ?>" class="site-commerce-position-index-content clearfix">
      <?php print $site_commerce_position_index_content; ?>
    </div>
  <?php endif; ?>

  <?php if($site_commerce_other_positions): ?>
    <!-- Прочие товары, которые относятся к той же категории, что и текущий товар. -->
    <?php print $site_commerce_other_positions; ?>
  <?php endif; ?>
</div>