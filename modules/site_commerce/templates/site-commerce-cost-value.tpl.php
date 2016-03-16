<?php
/**
 * @file
 * Template file site-commerce-cost-value.tpl.php.
 */

/**
 * Доступные переменные:
 *
 * $cost - основная стоимость позиции без скидки.
 * $currency - единица измерения стоимости.
 * $measure - единица измерения количества (суффикс стоимости).
 */
?>
<div class="site-commerce-cost-value"><?php print $cost; ?>&nbsp;<?php print $currency; ?><?php print $measure ?></div>