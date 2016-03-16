<?php
/**
 * @file
 * Template file site-commerce-block-special-offers-items.tpl.php.
 */

/**
 * Доступные переменные:
 *
 * $path_link - ссылка на страницу с полным перечнем позиций по блоку.
 * $items - перечень позиций в виде элементов списка.
 */
?>
<noindex>
<?php if ($path_link): ?>
  <div class="site-commerce-block-link-read-more">
    <?php print l(t('All special offers'), $path_link, array('attributes' => array('rel' => 'nofollow'))); ?>
  </div>
<?php endif; ?>

<?php if ($items): ?>
<ul id="site-commerce-block-special-offers-items" class="site-commerce-block-items">
  <?php print $items; ?>
</ul>
<?php endif; ?>
</noindex>