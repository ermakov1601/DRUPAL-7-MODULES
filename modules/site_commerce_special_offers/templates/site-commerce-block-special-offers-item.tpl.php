<?php
/**
 * @file
 * Template file site-commerce-block-special-offers-item.tpl.php.
 */

/**
 * Доступные переменные:
 *
 * $pid - идентификатор позиции.
 * $title - заголовок позиций.
 * $image - изображение позиции.
 * $cost - стоимость позиции.
 * $cart - форма добавления в корзину.
 */
?>
<li id="site-commerce-block-special-offers-item-<?php print $pid; ?>" class="site-commerce-block-item">
  <div class="site-commerce-block-item-data">
    <?php if($image): ?>
    <div class="site-commerce-block-item-image"><?php print $image; ?></div>
    <?php endif; ?>
    <div class="site-commerce-block-item-title"><?php print $title; ?></div>
  </div>

  <?php if($cost): ?>
  <div class="site-commerce-block-item-cost"><?php print $cost; ?></div>
  <?php endif; ?>

  <?php if($cart): ?>
  <div class="site-commerce-block-item-cart"><?php print $cart; ?></div>
  <?php endif; ?>
</li>