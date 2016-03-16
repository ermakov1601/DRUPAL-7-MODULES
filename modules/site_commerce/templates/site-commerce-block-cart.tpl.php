<?php

/**
 * @file
 * Реализация темы по умолчанию для рендеринга блока корзины товаров.
 *
 * Этот шаблон используется для вывода блока корзины товаров.
 * Блок должен отображаться на всех старницах сайта.
 *
 * Доступные переменные:
 * - $quantity_order: Количество товаров готовых для заказа в корзине.
 * - $quantity_postponed: Количество отложенных товаров в корзине.
 * - $order_info['cost']: Стоимость заказа в корзине.
 * - $order_info['currency']: Единица измерения количества стоимости.
 *
 * @see template_preprocess_site_commerce_block_cart()
 *
 * @ingroup themeable
 */
?>
<noindex>
<?php if($quantity_order || $quantity_postponed): ?>
<!-- Текст корзины с товарами. -->
<div class="block-site-commerce-cart__item block-site-commerce-cart__item_full">
  <a href="<?php print base_path(); ?>cart" rel="nofollow" class="block-site-commerce-cart__link block-site-commerce-cart__link_full">
    <span class="block-site-commerce-cart__image block-site-commerce-cart__image_full">
      <img src="<?php print base_path(); ?><?php print $directory; ?>/images/shopping-basket-full.png" alt="">
    </span>
    <?php print t('To issue the order'); ?>
  </a>
</div>

<div class="block-site-commerce-cart__item block-site-commerce-cart__item_quantity">
  <?php if($quantity_order): ?>
  <!-- Количество заказанных товаров. -->
  <div class="block-site-commerce-cart__quantity block-site-commerce-cart__quantity_order"><?php print t('Quantity'); ?>&nbsp;&mdash;&nbsp;<?php print $quantity_order; ?></div>
  <?php endif; ?>

  <?php if($quantity_postponed): ?>
  <!-- Количество отложенных товаров. -->
  <div class="block-site-commerce-cart__quantity block-site-commerce-cart__quantity_postponed">(<?php print mb_strtolower(t('The postponed products'), 'UTF-8'); ?>&nbsp;&mdash;&nbsp;<?php print $quantity_postponed; ?>)</div>
  <?php endif; ?>
</div>

<?php if(count($order_info)): ?>
<!-- Стоимость заказа. -->
<div class="block-site-commerce-cart__item block-site-commerce-cart__item_order-info">
  <?php print t('The sum of the order'); ?>:&nbsp;<?php print $order_info['cost']; ?>&nbsp;<?php print $order_info['currency']; ?>
</div>
<?php endif; ?>

<?php else: ?>
<!-- Текст пустой корзины. -->
<div class="block-site-commerce-cart__item block-site-commerce-cart__item_empty">
  <span class="block-site-commerce-cart__image block-site-commerce-cart__image_empty">
    <img src="<?php print base_path(); ?><?php print $directory; ?>/images/shopping-basket.png" alt="">
  </span>
  <?php print variable_get('site_commerce_cart_text_empty'); ?>
</div>
<?php endif; ?>
</noindex>