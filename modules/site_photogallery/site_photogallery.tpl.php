<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix" <?php print $attributes; ?>>

  <?php if ($display_submitted): ?>
    <div class="site-photogallery-album-annotation">
      <?php print format_date($created, 'short'); ?>
    </div>
  <?php endif; ?>

  <?php
    // Скрываем элементы, которые мы выведем позже.
    hide($content['comments']);
    hide($content['links']);
    hide($content['field_site_photogallery_file']);
    hide($content['field_site_photogallery_image']);
    hide($content['field_site_photogallery_taxonomy']);
    hide($content['field_tags']);
  ?>

  <div class="content clearfix" <?php print $content_attributes; ?>>
    <?php if ($body[0]['summary']): ?>
    <!-- Краткое описание фотоальбома. -->
    <div class="site-photogallery-summary-content">
      <?php print render($body[0]['summary']); ?>
    </div>
    <?php endif; ?>

    <?php if (!empty($field_site_photogallery_image) && count($field_site_photogallery_image)): ?>
    <!-- Изображения. -->
    <div class="site-photogallery-wrapper clearfix">
      <?php print theme('site_photogallery_images', array('node' => $node));?>
    </div>
    <?php endif; ?>

    <?php if ($body[0]['value']): ?>
    <!-- Полное описание фотоальбома. -->
    <div class="site-photogallery-value-content">
      <?php print render($body[0]['value']); ?>
    </div>
    <?php endif; ?>
  </div>

  <?php if (!empty($node->field_tags)): ?>
  <!-- Вывод тегов node. -->
  <div class="site-photogallery-tags-block clearfix">
    <?php
      $count = 0;
      $tags = '';
      $tag_name = '';
      $language = 'und';
        foreach ($node->field_tags[$language] as $value) {
          $tag_name = $node->field_tags[$language][$count]['taxonomy_term']->name;
          $tag_url = 'taxonomy/term/' . $node->field_tags[$language][$count]['taxonomy_term']->tid;
          $tags .= l($tag_name, $tag_url) . ' ';
          $count ++;
        }
        print '<span class="site-photogallery-tags">' . t('Tags') . ':</span> ' . $tags;
    ?>
  </div>
  <?php endif; ?>

  <?php
    if ($teaser || !empty($content['comments']['comment_form'])) {
      unset($content['links']['comment']['#links']['comment-add']);
    }
    $links = render($content['links']);
    if ($links):
  ?>
  <div class="link-wrapper">
    <?php print $links; ?>
  </div>
  <?php endif; ?>

  <?php
    unset($content['comments']);
    print render($content['comments']);
  ?>

</div>