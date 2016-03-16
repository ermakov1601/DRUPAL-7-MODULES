<?php

/**
 * @file
 * Реализация темы по умолчанию для рендеринга корзины.
 *
 * Этот шаблон используется для вывода корзины товаров, которые готовы к заказу или отложены.
 *
 * Доступные переменные:
 *
 * @see template_preprocess_site_commerce_cart()
 *
 * @ingroup themeable
 */
?>
<div class="site-commerce-cart">
  <!-- Примечание, если товаров в корзине нет. -->
  <div class="site-commerce-cart__empty <?php print $cart_empty_class; ?>"><?php print t('Your cart is empty! Choose the goods interesting you, push a button «To buy» and pass in a basket for registration of the order.'); ?></div>

  <div class="site-commerce-cart__ready clearfix">
    <?php foreach ($cart[1] as $cid => $cart): ?>
      <div id="site-commerce-cart__item-<?php print $cid; ?>" class="site-commerce-cart__item clearfix">
        <!-- Изображение и заголовок позиции. -->

          <div class="site-commerce-cart__item-image">
            <img class="site-commerce-cart__image" src="<?php print $cart['position']->image['style_url']; ?>" alt="<?php print $cart['position']->image['alt']; ?>" title="<?php print $cart['position']->image['title']; ?>">
          </div>
          <div class="site-commerce-cart__item-title">
            <a href="<?php print $cart['position']->alias; ?>" class="site-commerce-cart__link">
              <?php print $cart['position']->title; ?>
            </a>
          </div>

      </div>
      <!-- Блок с кнопками действий: количество, удаление, перенос в отложенные товары. -->
      <div id="site-commerce-cart__item-action-<?php print $cid; ?>" class="site-commerce-cart__item-action clearfix">
        <!-- Текущая стоимость с учётом количества. Не изменяйте имя id элемента. -->
        <span id="site-commerce-cart__item-cost-<?php print $cid; ?>" class="site-commerce-cart__item-cost"><?php print $cart['cost_all']; ?></span> <?php print $cart['position']->currency; ?>
        <?php print t('per') ?>
        <!--<a class="site-commerce-cart__link site-commerce-cart__link_minus" сid="<?php print $cid; ?>" id="minus-<?php print $cid; ?>" href="javascript:void(0);"><img src="/<?php print $directory; ?>/images/minus.png" alt=""></a>-->
        <input type="number" class="site-commerce-cart__item-quantity" id="quantity-<?php print $cid; ?>" сid="<?php print $cid; ?>" min="1" max="1000" value="<?php print $cart['quantity']; ?>" step="1" cart_value="<?php print $cart['quantity']; ?>">
        <!--<a class="site-commerce-cart__link site-commerce-cart__link_plus" сid="<?php print $cid; ?>" id="plus-<?php print $cid; ?>" href="javascript:void(0);"><img src="/<?php print $directory; ?>/images/plus.png" alt=""></a>-->
        <?php print t('piece') ?>
        <a class="site-commerce-cart__link site-commerce-cart__link_delete" сid="<?php print $cid; ?>" id="delete-<?php print $cid; ?>" href="javascript:void(0);">
          <span class="link-js"><?php print mb_strtolower(t('Delete'), 'UTF-8'); ?></span>
        </a>
      </div>
      <?php if($cart['cost_changed']): ?>
      <!-- Примечание, если была обновлена стоимость в корзине -->
        <div id="site-commerce-cart__note-<?php print $cid; ?>" class="site-commerce-cart__note site-commerce-cart__note_warning">
          <?php print t("Attention! Cost of this goods has just changed."); ?>
        </div>
      <?php endif; ?>
    <?php endforeach; ?>

    <?php // Блок отображения итоговой стоимости товаров в корзине. ?>
    <?php foreach ($cart_result as $status => $cart): ?>
      <?php if($cart['status'] === 1): ?>
        <?php if($cart['cost_discount']): ?>
        <!-- Стоимость с учетом скидки за объем заказанного товара. -->
        <div class="site-commerce-cart__item-discount">
          <?php print t('We give you the discount @discount%', array('@discount' => $cart['discount'])); ?> (<?php print $cart['cost_discount']; ?> <?php print $currency; ?>)
        </div>
        <?php endif; ?>

        <?php if($cart['cost_result']): ?>
        <!-- Итоговая стоимость. Не изменяйте имя класса site-commerce-cart__cost-result. -->
        <div class="site-commerce-cart__item-cost-result">
          <?php print t('Total, without delivery'); ?> <span class="site-commerce-cart__cost-result"><?php print $cart['cost_result']; ?></span> <?php print $currency; ?>
        </div>
        <?php endif; ?>

        <?php if($cart['cost_result_description']): ?>
        <!-- Примечание если итоговая стоимость равна нулю или хотя бы у одной из позиций в корзине стоимость равна нулю. -->
        <div class="site-commerce-cart__item-cost-result-description">
          <?php print $cart['cost_result_description']; ?>
        </div>
        <?php endif; ?>
      <?php endif; ?>
    <?php endforeach; ?>
  </div>
</div>

<?php if($order_form): ?>
  <!-- Форма оформления заказа. Не убирайте название класса site-commerce-cart-order-form. -->
  <div class="site-commerce-cart-order-form">
    <?php print $order_form; ?>
  </div>
<?php endif; ?>