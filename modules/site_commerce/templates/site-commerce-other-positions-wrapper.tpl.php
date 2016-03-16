<?php
/**
 * @file
 * Template file site-commerce-other-positions-wrapper.tpl.php.
 */

/**
 * Доступные переменные:
 *
 * $items - перечень позиций в виде элементов списка <li></li>.
 * $title - заголовок блока.
 */
?>
<div class="site-commerce-other-positions-wrapper clearfix">
  <div class="site-commerce-other-positions-title"><h3><?php print $title; ?></h3></div>
  <!-- Список прочих позиций -->
  <ul class="site-commerce-other-positions-items slides">
    <?php print $items; ?>
  </ul>
</div>