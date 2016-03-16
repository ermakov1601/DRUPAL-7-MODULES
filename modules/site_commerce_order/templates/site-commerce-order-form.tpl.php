<?php

/**
 * @file
 * Реализация темы по умолчанию для рендеринга формы оформления заказа.
 *
 * Этот шаблон используется для вывода всех доступных полей формы оформления заказа.
 *
 * Доступные переменные:
 *
 * @see template_preprocess_site_commerce_order_form()
 *
 * @ingroup themeable
 */
?>
<!-- Заголовок формы оформления заказа. -->
<h2><?php print t('Registration of the order'); ?></h2>
<div class="site-commerce-order-title-required"><sup>*</sup> &mdash; <?php print t('required to be filled'); ?></div>

<div class="site-commerce-order-information clearfix">
  <div class="site-commerce-order-information__personal">
    <!-- Персональная информация. -->
    <h3><?php print t('Personal information'); ?></h3>
    <?php foreach ($order_form['personal_information'] as $key => $value): ?>
      <?php print $value['element']; ?>
    <?php endforeach; ?>
  </div>
  <div class="site-commerce-order-information__delivery">
    <?php if($order_form['delivery_information']['delivery_type']['element']): ?>
      <!-- Информация о доставке. -->
      <h3><?php print t('The information for delivery'); ?></h3>
      <?php foreach ($order_form['delivery_information'] as $key => $value): ?>
        <?php print $value['element']; ?>
      <?php endforeach; ?>
    <?php endif; ?>

    <?php if(isset($order_form['datetime']['date'])): ?>
      <!-- Дата. -->
      <h3><?php print t('Desirable date'); ?></h3>
      <?php print $order_form['datetime']['date']; ?>
    <?php endif; ?>

    <?php if(isset($order_form['datetime']['time'])): ?>
      <!-- Время. -->
      <h3><?php print t('Desirable time'); ?></h3>
      <?php print $order_form['datetime']['time']; ?>
    <?php endif; ?>

    <?php if(isset($order_form['datetime']['date_time'])): ?>
      <!-- Дата и время. -->
      <h3><?php print t('Desirable date and time'); ?></h3>
      <?php print $order_form['datetime']['date_time']; ?>
    <?php endif; ?>
  </div>
</div>

<?php if(isset($order_form['comment'])): ?>
  <!-- Комментарий к заказу. -->
  <h3><?php print t('Additional information'); ?></h3>
  <?php print $order_form['comment']; ?>
<?php endif; ?>

<!-- Кнопка оформления заказа. -->
<?php print $render_children; ?>