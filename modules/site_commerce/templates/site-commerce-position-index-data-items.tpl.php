<?php
/**
 * @file
 * Template file site-commerce-position-index-data-items.tpl.php.
 */

/**
 * Available variables:
 *
 * $items - темизированный перечень позиций через site-commerce-position-index-data-item.tpl.php.
 * $pid - идентификатор позиции.
 */
?>
<div id="site-commerce-position-index-data-items-<?php print $pid; ?>" class="site-commerce-position-index-data-items">
  <div id="site-commerce-position-index-title-<?php print $pid; ?>" class="site-commerce-position-index-title">
    <?php print t('This product is part of'); ?>:
  </div>
  <?php print $items; ?>
</div>