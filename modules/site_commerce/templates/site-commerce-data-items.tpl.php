<?php
/**
 * @file
 * Template file site-commerce-data-template-items.tpl.php.
 */

/**
 * Available variables:
 *
 * $pid - идентификатор позиции.
 * $items - массив с названиями и значениями полей карточки товара.
 */
?>
<div class="site-commerce-data">
  <ul id="site-commerce-data-items-<?php print $pid; ?>" class="site-commerce-data-items">
    <?php if ($items['field_site_commerce_code']['field_value']): ?>
      <!-- Артикул -->
      <li class="site-commerce-data-item">
        <span class="site-commerce-data-item-label"><?php print $items['field_site_commerce_code']['field_label'] ?>:</span>
        <span class="site-commerce-data-item-value"><?php print $items['field_site_commerce_code']['field_value'] ?></span>
      </li>
    <?php endif; ?>
    <?php if ($items['field_site_commerce_country']['field_value']): ?>
      <!-- Страна производитель -->
      <li class="site-commerce-data-item">
        <span class="site-commerce-data-item-label"><?php print $items['field_site_commerce_country']['field_label'] ?>:</span>
        <span class="site-commerce-data-item-value"><?php print $items['field_site_commerce_country']['field_value'] ?></span>
      </li>
    <?php endif; ?>
    <?php if ($items['field_site_commerce_manufacture']['field_value']): ?>
      <!-- Производитель -->
      <li class="site-commerce-data-item">
        <span class="site-commerce-data-item-label"><?php print $items['field_site_commerce_manufacture']['field_label'] ?>:</span>
        <span class="site-commerce-data-item-value"><?php print $items['field_site_commerce_manufacture']['field_value'] ?></span>
      </li>
    <?php endif; ?>
    <?php if ($items['field_site_commerce_brand']['field_value']): ?>
      <!-- Торговая марка -->
      <li class="site-commerce-data-item">
        <span class="site-commerce-data-item-label"><?php print $items['field_site_commerce_brand']['field_label'] ?>:</span>
        <span class="site-commerce-data-item-value"><?php print $items['field_site_commerce_brand']['field_value'] ?></span>
      </li>
    <?php endif; ?>
  </ul>
</div>