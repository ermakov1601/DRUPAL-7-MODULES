<?php

/**
 * @file
 * Реализация темы по умолчанию для рендеринга выбора параметров.
 *
 * Этот шаблон используется для выбора параметров в карточке товара.
 *
 * Доступные переменные:
 * - $pager: Список с элементами переключателями типа пейджер.
 * - $position: Объект текущей позиции товара.
 *
 * @see template_preprocess_site_commerce_select()
 *
 * @ingroup themeable
 */
?>
<noindex>
<div class="site-commerce-select__left">
  <div class="site-commerce-select__category">
    <span class="site-commerce-select__step site-commerce-select__step_1"></span><?php print t('Select category'); ?>
  </div>
  <?php foreach ($terms as $tid => $name): ?>
  <div class="site-commerce-select__term">
    <!-- Ссылка на корневой термин в параметре товара. -->
    <a href="<?php print base_path(); ?>site-commerce-select-data-load/<?php print $tid; ?>/<?php print $pid; ?>/<?php print $param; ?>/nojs" rel="nofollow" class="site-commerce-select__term-link use-ajax" title="<?php print t($name); ?>"><?php print t($name); ?></a>
  </div>
  <?php endforeach; ?>
</div>

<div class="site-commerce-select__right">
  <div id="site-commerce-select-data-<?php print $param; ?>-<?php print $pid; ?>" class="site-commerce-select-data"></div>
  <div id="site-commerce-select-data-more-<?php print $param; ?>-<?php print $pid; ?>" class="site-commerce-select-data-more"></div>
</div>
</noindex>