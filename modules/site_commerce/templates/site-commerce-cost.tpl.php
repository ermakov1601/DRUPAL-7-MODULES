<?php

/**
 * @file
 * Реализация темы по умолчанию для рендеринга стоимости товара.
 *
 * Этот шаблон используется для вывода стоимости товара, а так же стоимости с учётом скидок.
 *
 * Доступные переменные:
 * - $global_note_cost: Глобальное примечание к стоимости всех позиций. Выводиться над стоимостью.
 * - $cost: Массив с возможными вариантами представления стоимости (числовые значение стоимости и скидок).
        $cost = array(
          'basic' => 0, // Базовая стоимость.
          'min' => 0,  // Минимальная стоимость.
          'set' => 0,  // Стоимость из набора товаров, базовая не учитывается.
          'special_offer' => array(
            'cost_discount' => 0, // Величина скидки.
            'discount' =>  0, // Величина скидки в процентах.
            'cost' => 0, // Стоимость с учётом скидки или предыдущей стоимости относительно базовой стоимости.
            'old' => 0, // Предыдущая стоимость. Если спецпредложение формируется из разницы предыдущей и текущей стоимости.
          )
        );
 *
 * @see template_preprocess_site_commerce_cost()
 *
 * @ingroup themeable
 */
?>
<div class="site-commerce-cost <?php print $class; ?>">
    <?php
      // Выполняем если выводиться стоимость с учётом спецпредложений.
      if (count($cost['special_offer'])):
    ?>

    <?php // Если спецпредложение формируется с учётом скидки. ?>
    <?php if ($cost['special_offer']['discount']): ?>
    <div class="site-commerce-cost site-commerce-cost_discount">
      <div class="site-commerce-cost__value-old"><?php print $cost['basic']; ?>&nbsp;<?php print $currency; ?><?php print $measure ?></div>
      <div class="site-commerce-cost__value-discount">
        <?php print t('the discount'); ?>:&nbsp;<span class="site-commerce-cost-value-discount"><?php print $cost['special_offer']['discount']; ?>%</span>
      </div>
    </div>
    <div class="site-commerce-cost site-commerce-cost_basic site-commerce-cost_basic-<?php print $type_call; ?>">
      <!-- Стоимость товара с учётом скидки. -->
      <?php print $cost['special_offer']['cost']; ?>&nbsp;<?php print $currency; ?><?php print $measure; ?>
    </div>
    <?php endif; ?>

    <?php // Если спецпредложение формируется с учётом разницы в стоимости старой и текущей. ?>
    <?php if ($cost['special_offer']['old']): ?>
    <div class="site-commerce-cost site-commerce-cost_discount">
      <div class="site-commerce-cost__value-old"><?php print $cost['special_offer']['old']; ?>&nbsp;<?php print $currency; ?><?php print $measure ?></div>
      <div class="site-commerce-cost__value-discount">
        <?php print t('the discount'); ?>&nbsp;<span class="site-commerce-cost-value-discount"><?php print $cost['special_offer']['cost_discount']; ?>&nbsp;<?php print $currency; ?></span>
      </div>
    </div>
    <div class="site-commerce-cost site-commerce-cost_basic site-commerce-cost_basic-<?php print $type_call; ?>">
      <!-- Стоимость товара с учётом скидки. -->
      <?php print $cost['special_offer']['cost']; ?>&nbsp;<?php print $currency; ?><?php print $measure; ?>
    </div>
    <?php endif; ?>

  <?php endif; ?>

  <?php // Если отсутствует скидка или старая цена. ?>
  <?php if (!$cost['special_offer']['discount'] && !$cost['special_offer']['old']): ?>

  <?php if ($cost['basic'] && !$cost['min']): ?>
  <!-- Основная стоимость товара. -->
  <div class="site-commerce-cost site-commerce-cost_basic site-commerce-cost_basic-<?php print $type_call; ?>">
    <?php print $cost['basic']; ?>&nbsp;<?php print $currency; ?>&nbsp;<?php print $measure; ?>
  </div>
  <?php endif; ?>

  <?php if ($cost['basic'] && $cost['min']): ?>
  <!-- Минимальная и максимальная стоимость товара. -->
  <div class="site-commerce-cost site-commerce-cost_basic site-commerce-cost_basic-<?php print $type_call; ?>">
    <?php print t('by'); ?>&nbsp;<?php print $cost['min']; ?>&nbsp;
    <?php print t('before'); ?>&nbsp;<?php print $cost['basic']; ?>&nbsp;
    <?php print $currency; ?><?php print $measure; ?>
  </div>
  <?php endif; ?>

  <?php if (!$cost['basic'] && $cost['min']): ?>
  <!-- Минимальная стоимость товара. -->
  <div class="site-commerce-cost site-commerce-cost_basic site-commerce-cost_basic-<?php print $type_call; ?>">
    <?php print t('by'); ?>&nbsp;<?php print $cost['min']; ?>&nbsp;
    <?php print $currency; ?><?php print $measure; ?>
  </div>
  <?php endif; ?>

  <?php if ($cost['set']): ?>
  <!-- Cтоимость набора товаров. -->
  <?php if ($type_call == 'card'): ?>
  <div class="site-commerce-cost__label"><?php print t('Сost of set'); ?></div>
  <?php endif; ?>
  <div class="site-commerce-cost site-commerce-cost_basic site-commerce-cost_basic-<?php print $type_call; ?>">
    <?php print $cost['set']; ?>&nbsp;<?php print $currency; ?><?php print $measure; ?>
  </div>
  <?php endif; ?>

  <?php endif; ?>
</div>