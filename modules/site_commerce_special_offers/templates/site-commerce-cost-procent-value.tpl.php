<?php
/**
 * @file
 * Template file site-commerce-cost-procent-value.tpl.php.
 * Шаблон для вывода стоимости спецпредложения по умолчанию, когда скидка формируется
 * через процентное соотношение от основной стоимости.
 */

/**
 * Доступные переменные:
 *
 * $cost - основная стоимость позиции без скидки.
 * $currency - единица измерения стоимости.
 * $measure - единица измерения количества (суффикс стоимости).
 * $discount_cost['discount_cost'] - стоимость с учётом скидки.
 * $discount_cost['discount'] - величина скидки в процентах.
 * $discount_cost['cost'] - величина скидки в единицах измерения стоимости.
 */
?>
<div class="site-commerce-cost-discount-block clearfix">
  <div class="site-commerce-cost-value-old-block"><?php print $cost; ?>&nbsp;<?php print $currency; ?><?php print $measure ?></div>
  <div class="site-commerce-cost-value-discount-block">
    <?php print t('The discount'); ?>:&nbsp;<span class="site-commerce-cost-value-discount"><?php print $discount_cost['cost']; ?>&nbsp;<?php print $currency; ?></span>
  </div>
</div>
<div class="site-commerce-cost-value"><?php print $cost; ?>&nbsp;<?php print $currency; ?><?php print $measure ?></div>
