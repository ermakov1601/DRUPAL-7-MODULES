<?php
/**
 * @file
 * Template file site-commerce-block-novelties-item.tpl.php.
 */

/**
 * Available variables:
 *
 * $pid - идентификатор позиции.
 * $title - заголовок позиций.
 * $image - изображение позиции.
 * $cost - стоимость позиции.
 * $cart - форма добавления в корзину.
 * $status - статус товара на складе.
 * $summary - краткое описание товара.
 * $data - различные примечания, данные о товаре.
 */
?>
<li>
  <!-- Название товара. -->
  <div class="site-commerce-item-title"><?php print $title; ?></div>
</li>
<li id="site-commerce-item-<?php print $pid; ?>" class="site-commerce-item <?php print $classes; ?>">
  <?php if($image): ?>
  <!-- Изображение товара. -->
  <div class="site-commerce-item-image"><?php print $image; ?></div>
  <?php endif; ?>

  <?php if($status || $summary || $data): ?>
  <div class="site-commerce-item-info">
    <?php if($status): ?>
    <!-- Статус товара на складе. -->
    <div class="site-commerce-item-status"><?php print $status; ?></div>
    <?php endif; ?>

    <?php if($summary): ?>
    <!-- Краткое описание товара. -->
    <div class="site-commerce-item-summary"><?php print $summary; ?></div>
    <?php endif; ?>

    <?php if($data): ?>
    <!-- Различные примечания, данные о товаре. -->
    <div class="site-commerce-item-data"><?php print $data; ?></div>
    <?php endif; ?>
  </div>
  <?php endif; ?>

  <?php if($cost || $cart): ?>
  <div class="site-commerce-item-action">
    <?php if($cost): ?>
    <!-- Стоимость товара. -->
    <div class="site-commerce-item-cost"><?php print $cost; ?></div>
    <?php endif; ?>

    <?php if($cart): ?>
    <!-- Купить товар. -->
    <div class="site-commerce-item-cart"><?php print $cart; ?></div>
    <?php endif; ?>
  </div>
  <?php endif; ?>
</li>