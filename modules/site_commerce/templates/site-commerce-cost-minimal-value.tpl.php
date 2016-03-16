<?php
/**
 * @file
 * Template file site-commerce-cost-minimal-cost.tpl.php.
 * Шаблон для вывода минимальной стоимости.
 */

/**
 * Доступные переменные:
 *
 * $cost - основная стоимость позиции без скидки.
 * $currency - единица измерения стоимости.
 * $measure - единица измерения количества (суффикс стоимости).
 * $discount - скидка в % (если позиция привязана к спецпредложению).
 */
?>
<span class="site-commerce-cost-value"><?php print $cost; ?>&nbsp;<?php print $currency; ?><?php print $measure ?></span>
<?php if ($discount): ?>
<div class="site-commerce-cost-minimal-cost-discount-block">
  <span class="site-commerce-cost-minimal-cost-discount-block-title"><?php print t('Buy now and receive the discount'); ?></span>:
  <span class="site-commerce-cost-minimal-cost-discount-block-value"><?php print $discount ?>%</span>
</div>
<?php endif; ?>