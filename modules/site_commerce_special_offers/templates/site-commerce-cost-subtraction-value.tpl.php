<?php
/**
 * @file
 * Template file site-commerce-cost-subtraction-value.tpl.php.
 * Шаблон для вывода стоимости спецпредложения, когда скидка формируется
 * через разницу старой и новой стоимости.
 */

/**
 * Доступные переменные:
 *
 * $cost - основная стоимость позиции без скидки.
 * $currency - единица измерения стоимости.
 * $measure - единица измерения количества (суффикс стоимости).
 * $discount_cost['old_cost'] - старая стоимость.
 * $discount_cost['cost'] - величина скидки в единицах измерения стоимости.
 */
?>
<div class="site-commerce-cost-discount-block clearfix">
  <div class="site-commerce-cost-value-old-block"><?php print $discount_cost['old_cost']; ?>&nbsp;<?php print $currency; ?><?php print $measure ?></div>
  <div class="site-commerce-cost-value-discount-block">
    <?php print t('The discount'); ?>:&nbsp;<span class="site-commerce-cost-value-discount"><?php print $discount_cost['cost']; ?>&nbsp;<?php print $currency; ?></span>
  </div>
</div>
<div class="site-commerce-cost-value"><?php print $cost; ?>&nbsp;<?php print $currency; ?><?php print $measure ?></div>