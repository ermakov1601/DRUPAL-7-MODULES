<?php
/**
 * @file
 * Template file site-commerce-cost-wrapper.tpl.php.
 * Шаблон для вывода минимальной стоимости.
 */

/**
 * Доступные переменные:
 *
 * $items - HTML содержимое представления стоимости.
 * $global_note_cost - глобальное примечание к стоимости всех позиций.
 */
?>
<?php print $items; ?>
<?php if ($global_note_cost): ?>
<div class="site-commerce-global-note-cost"><?php print $global_note_cost; ?></div>
<?php endif; ?>