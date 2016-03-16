<?php

/**
 * @file
 * Реализация темы по умолчанию для рендеринга формы поиска товаров.
 *
 * Этот шаблон используется для вывода формы поиска товаров. Дополнительно позволяет
 * отображать текст над формой поиска и под формой поиска.
 *
 * Доступные переменные:
 * $search_form - форма поиска.
 * $search_form_up - содержимое над формой поиска.
 * $search_form_bottom - содержимое под формой поиска.
 * $search_form_in_header - bool если TRUE, форма поиска выводиться в шапке сайта в header.
 * @see template_preprocess_site_commerce_create_search_form()
 *
 * @ingroup themeable
 */
?>
<div class="site-commerce-search">
<?php if ($search_form_up): ?>
  <div class="site-commerce-search__up"><?php print $search_form_up; ?></div>
<?php endif; ?>
<div class="site-commerce-search__form-wrapper container-inline">
  <?php print $search_form; ?>
</div>
<?php if ($search_form_bottom): ?>
  <div class="site-commerce-search__bottom"><?php print $search_form_bottom; ?></div>
<?php endif; ?>
</div>