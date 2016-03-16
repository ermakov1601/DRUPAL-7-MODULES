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
<div class="site-commerce-status site-commerce-status_<?php print $status; ?> site-commerce-status_<?php print $type_call; ?>">
  <div class="site-commerce-status__label"><?php print $position_status[$status]; ?></div>
</div>