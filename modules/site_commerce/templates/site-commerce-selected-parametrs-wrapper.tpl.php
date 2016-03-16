<?php
/**
 * @file
 * Template file site-commerce-selected-parametrs-wrapper.tpl.php.
 * Не изменяйте id блока <div id="site-commerce-selected-parametrs-items-<?php print $pid; ?>"></div>.
 */

/**
 * Available variables:
 *
 * $pid - идентификатор позиции.
 * $parametrs - параметры, которые формируются через шаблон site-commerce-selected-parametr-item.tpl.php.
 * $title - заголовок блока с параметрами (например, Выбранные параметры).
 */
?>
<div id="site-commerce-selected-parametrs-title-<?php print $pid; ?>" class="site-commerce-selected-parametrs-title clearfix"><?php print $title; ?></div>
<div id="site-commerce-selected-parametrs-items-<?php print $pid; ?>"><?php print $parametrs; ?></div>