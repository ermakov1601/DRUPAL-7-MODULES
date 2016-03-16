<?php
/**
 * @file
 * Template file site-commerce-other-positions-item.tpl.php.
 */

/**
 * Доступные переменные:
 *
 * $title - заголовок блока.
 * $image - изображение позиции.
 * $cost - стоимость позиции.
 */
?>
<li class="site-commerce-other-positions-item">
  <div>
  <?php if($image): ?>
    <div class="site-commerce-other-positions-item-image"><?php print $image; ?></div>
  <?php endif; ?>
    <div class="site-commerce-other-positions-item-title"><?php print $title; ?></div>
  <?php if($cost): ?>
    <div class="site-commerce-other-positions-item-cost"><?php print $cost; ?></div>
  <?php endif; ?>
  </div>
</li>