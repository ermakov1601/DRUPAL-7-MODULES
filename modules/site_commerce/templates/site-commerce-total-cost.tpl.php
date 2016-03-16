<?php
/**
 * @file
 * Template file site-commerce-total-cost.tpl.php.
 */

/**
 * Доступные переменные:
 *
 * $pid - идентификатор позиции.
 * $total_cost - итоговая стоимость.
 * $currency - единица измерения стоимости.
 * $title - заголовок блока с параметрами (например, Выбранные параметры).
 */
?>
<div id="site-commerce-total-cost-title-<?php print $pid; ?>" class="site-commerce-total-cost-title clearfix"><?php print $title; ?></div>
<div id="site-commerce-total-cost-value-<?php print $pid; ?>" class="site-commerce-total-cost-value clearfix"><?php print $total_cost; ?>&nbsp;<?php print $currency; ?></div>