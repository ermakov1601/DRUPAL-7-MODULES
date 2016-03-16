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
 */
?>
<ul id="site-commerce-block-novelties-item-<?php print $pid; ?>" class="site-commerce-block-item site-commerce-block-novelties-item">
<?php if($image): ?>
  <li class="site-commerce-block-item-image"><?php print $image; ?></li>
<?php endif; ?>
  <li class="site-commerce-block-item-title"><?php print $title; ?></li>
<?php if($cost): ?>
  <li class="site-commerce-block-item-cost"><?php print $cost; ?></li>
<?php endif; ?>
<?php if($cart): ?>
  <li class="site-commerce-block-item-cart"><?php print $cart; ?></li>
<?php endif; ?>
</ul>