<?php

/**
 * @file
 * Реализация темы по умолчанию для рендеринга товаров при просмотре в каталоге.
 *
 * Этот шаблон используется для вывода параметров карточки товара в упрощенном виде при
 * просмотре товаров по той или иной категории.
 *
 * Доступные переменные:
 * - $pager: Список с элементами переключателями типа пейджер.
 * - $position: Объект текущей позиции товара.
 *
 * @see template_preprocess_site_commerce_term()
 *
 * @ingroup themeable
 */
?>
<?php print $pager; ?>

<?php foreach ($pids as $pid => $position): ?>
<div class="site-commerce-term <?php print $position->classes; ?> clearfix">
  <!-- Название товара. -->
  <div class="site-commerce-term__title clearfix">
    <a href="<?php print base_path(); ?><?php print $position->alias; ?>" class="site-commerce-term__link site-commerce-term__link_title">
      <?php print $position->title; ?>
    </a>
  </div>

  <div class="site-commerce-term__content clearfix">
    <!-- Изображение товара. -->
    <div class="site-commerce-term__side site-commerce-term__side_left">
      <a href="<?php print base_path(); ?><?php print $position->alias; ?>" rel="nofollow" class="site-commerce-term__link">
        <img class="site-commerce-term__image" src="<?php print $position->image_style_url; ?>" alt="<?php print $position->image_alt; ?>" title="<?php print $position->image_title; ?>">
      </a>
    </div>

    <div class="site-commerce-term__side site-commerce-term__side_right">
      <div class="site-commerce-term__side-right-left">
        <?php if($position->status): ?>
        <!-- Статус товара на складе. -->
        <div class="site-commerce-term__status"><?php print $position->status; ?></div>
        <?php endif; ?>

        <?php if($position->cost): ?>
        <!-- Стоимость товара. -->
        <div class="site-commerce-term__cost"><?php print $position->cost; ?></div>
        <?php endif; ?>
      </div>

      <div class="site-commerce-term__side-right-right">
        <?php if($position->cart): ?>
        <!-- Купить товар. -->
        <div class="site-commerce-term__cart"><?php print $position->cart; ?></div>
        <?php endif; ?>
      </div>
    </div>
  </div>

  <?php if($position->status || $position->summary || $position->data): ?>
  <?php if($position->summary): ?>
  <!-- Краткое описание товара. -->
  <div class="site-commerce-term__summary clearfix"><?php print $position->summary; ?></div>
  <?php endif; ?>

  <?php if($position): ?>
  <!-- Различные примечания, данные о товаре. -->
  <div class="site-commerce-term__data clearfix"><?php print $position->data; ?></div>
  <?php endif; ?>
  <?php endif; ?>

  <div class="site-commerce-term__link-read-more clearfix">
    <!-- Ссылка на карточку товара. -->
    <a href="<?php print base_path(); ?><?php print $position->alias; ?>" rel="nofollow" class="site-commerce-term__link site-commerce-term__link_read-more">
      <?php print t('read more'); ?>
    </a>
  </div>

</div>

<noindex>
<?php if($position->form_admin): ?>
<!-- Панель администрирования. -->
<div class="site-commerce-term__form-admin"><?php print $position->form_admin; ?></div>
<?php endif; ?>
</noindex>

<?php endforeach; ?>

<?php print $pager; ?>