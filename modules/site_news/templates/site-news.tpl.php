<?php
// $Id:
/**
 * @file
 *
 * Доступные переменные:
 */
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix" <?php print $attributes; ?>>

  <!-- Вывод даты создания. -->
  <?php if ($display_submitted): ?>
    <div class="site-news-date">
      <?php print $created; ?>
    </div>
  <?php endif; ?>

  <!-- Вывод главного изображения. -->
  <?php if ($main_image): ?>
    <div class="site-news-main-image-block clearfix">
      <?php print $main_image; ?>
    </div>
  <?php endif; ?>

  <!-- Вывод содержимого. -->
  <?php if ($summary || $body): ?>
    <div class="site-news-content clearfix" <?php print $content_attributes; ?>>
      <?php if ($summary): ?>
        <div class="site-news-summary-content"><?php print $summary; ?></div>
      <?php endif; ?>
      <?php if ($body): ?>
        <?php print $body; ?>
      <?php endif; ?>
    </div>
  <?php endif; ?>

  <!-- Вывод второстепенных изображений. -->
  <?php if ($other_image): ?>
    <div class="site-news-images-block clearfix">
      <?php print $other_image; ?>
    </div>
  <?php endif; ?>

  <!-- Вывод файлов. -->
  <?php if ($files): ?>
    <div class="site-news-files-block clearfix">
      <div class="site-news-files-title"><?php print t('Files for loading'); ?></div>
      <?php print $files; ?>
    </div>
  <?php endif; ?>

  <!-- Вывод тегов. -->
  <?php if ($tags): ?>
    <div class="site-news-tags-block clearfix">
      <?php print $tags; ?>
    </div>
  <?php endif; ?>

  <?php print render($content['links']); ?>

  <?php print render($content['comments']); ?>
</div>