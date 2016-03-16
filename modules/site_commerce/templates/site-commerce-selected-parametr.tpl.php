<?php

/**
 * @file
 * Template file site-commerce-selected-parametr-item.tpl.php.
 */

/**
 * Доступные переменные:
 *
 * $cpid - идентификатор параметра в корзине.
 * $parametr_name - название параметра.
 * $parametr_value - значение параметра.
 * $parametr_cost - стоимость параметра.
 * $parametr_currency - единица измерения стоимости.
 * $parametr_image - изображение параметра.
 * $parametr_is_cart - признак отображения параметра в корзине. При этом параметре изменяется способ отображения параметров.
 */
?>
<?php
  $class = "site-commerce-selected-parametr-item clearfix";
  if ($parametr_is_cart) {
    $class = "site-commerce-selected-parametr-cart-item";
  }
?>
<div id="site-commerce-selected-parametr-item-<?php print $cpid; ?>" class="<?php print $class; ?>">
  <?php if ($parametr_image): ?>
    <div class="site-commerce-selected-parametr-image"><?php print $parametr_image; ?></div>
  <?php endif; ?>
  <div class="site-commerce-selected-parametr-data">
    <div class="site-commerce-selected-parametr-value"><?php print $parametr_value; ?></div>
    <div class="site-commerce-selected-parametr-name"><?php print $parametr_name; ?></div>
    <?php if ($parametr_cost): ?>
    <div class="site-commerce-selected-parametr-cost"><?php print $parametr_cost; ?>&nbsp;<?php print $parametr_currency; ?></div>
    <?php endif; ?>
    <div class="site-commerce-selected-parametr-links">
      <?php print $parametr_link_edit; ?>&nbsp;<?php print $parametr_link_delete; ?>
    </div>
  </div>
</div>