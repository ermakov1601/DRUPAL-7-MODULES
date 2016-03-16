<?php
/**
 * @file
 * Template file site-commerce-block-novelties-items.tpl.php.
 */

/**
 * Available variables:
 *
 * $path_link - ссылка на страницу с полным перечнем позиций по блоку.
 * $items - перечень позиций.
 */
?>
<?php if ($path_link): ?>
<div class="site-commerce-block-path-link"><?php print l(t('All novelties'), $path_link); ?></div>
<?php endif; ?>
<?php print $items; ?>