<?php

/**
 * @file
 * Template file site-commerce-data.tpl.php.
 */

/**
 * Доступные переменные:
 *
 * $variables['data'] - массив с параметрами для вывода.
 */
?>
<ul class="site-commerce-data clearfix">
  <?php foreach ($variables['data'] as $field_name => $item): ?>
    <?php if($item['field_value']): ?>
    <li class="site-commerce-data__item">
      <span class="site-commerce-data__label"><?php print $item['field_label']; ?>:
      </span><span class="site-commerce-data__value"><?php print $item['field_value']; ?>.</span>
    </li>
    <?php endif; ?>
  <?php endforeach; ?>
</ul>