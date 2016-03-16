<?php
/**
 * @file
 * Template file site-commerce-block-categories-item.tpl.php.
 */

/**
 * Available variables:
 *
 * $title - название категории.
 * $image - изображение категории.
 * $alias - url путь к категории.
 */
?>
<?php if($image): ?>
<li><a href="/<?php print $alias; ?>" title="<?php print $title; ?>"><span class="site-commerce-block-category-image"><?php print $image; ?></span><?php print $title; ?></a></li>
<?php else: ?>
<li><a href="/<?php print $alias; ?>" title="<?php print $title; ?>"><?php print $title; ?></a></li>
<?php endif; ?>