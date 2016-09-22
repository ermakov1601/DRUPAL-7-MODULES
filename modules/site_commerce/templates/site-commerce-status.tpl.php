<?php

/**
 * @file
 * Реализация темы по умолчанию для рендеринга статуса товара.
 *
 * Этот шаблон используется для вывода статуса товара на складе.
 *
 * Доступные переменные:
 *
 * @see template_preprocess_site_commerce_status()
 *
 * @ingroup themeable
 */
?>
<div class="site-commerce-status site-commerce-status_<?php print $status; ?> site-commerce-status_<?php print $type_call; ?> clearfix">
  <span class="site-commerce-status__label site-commerce-status__label_<?php print $status; ?>"><?php print $position_status[$status]; ?></span>
</div>