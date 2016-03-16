<?php
// $Id: site_faq.tpl.php,v 1.2 2010/12/01 00:18:15 webchick Exp $

/**
 * @file
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type, i.e., "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode, e.g. 'full', 'teaser'...
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined, e.g. $node->body becomes $body. When needing to access
 * a field's raw values, developers/themers are strongly encouraged to use these
 * variables. Otherwise they will have to explicitly specify the desired field
 * language, e.g. $node->body['en'], thus overriding any language negotiation
 * rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 */
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix" <?php print $attributes; ?>>

  <?php print render($title_prefix); ?>
  <?php if (!$page): ?>
    <h1<?php print $title_attributes; ?>>
      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
    </h1>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <?php if ($display_submitted):
    // Автор публикации.
    if (!empty($node->field_site_faq_creator)) {
      $creator = t('Creator') . ': ' . $node->field_site_faq_creator['und'][0]['value'];
    }
    else {
      $creator = t('Creator') . ': ' . t('is not specified');
    }
  ?>
    <div class="site-faq-date">
      <?php print format_date($created, 'short'); ?> | <?php print $creator; ?>
    </div>
  <?php endif; ?>

  <?php
    // Скрываем элементы, которые мы выведем позже.
    hide($content['comments']);
    hide($content['links']);
    hide($content['field_site_faq_file']);
    hide($content['field_site_faq_image']);
    hide($content['field_site_faq_taxonomy']);
    hide($content['field_tags']);
  ?>

  <div class="site-faq-main-image-block clearfix">
    <?php
      // Вывод главноего изображения.
      $count = 0;
      $language = 'und';
      if (!empty($field_site_faq_image)) {
        $imagecache = variable_get('site_faq_default_main_imagecache_preset', 'large');
        $variables = array(
          'style_name' => $imagecache,
          'path' => $field_site_faq_image[$count]['uri'],
          'alt' => $field_site_faq_image[$count]['alt'],
          'title' => $field_site_faq_image[$count]['title'],
          'width' => $field_site_faq_image[$count]['width'],
          'height' => $field_site_faq_image[$count]['height'],
          'attributes' => array('class' => 'site-faq-image'),
        );

        print theme_image_style($variables);
      }
    ?>
  </div>

  <div class="content clearfix" <?php print $content_attributes; ?>>
    <div class="site-faq-summary-content"><?php print render($body[0]['summary']); ?></div>
    <?php print render(site_faq_node_summary($node)); ?>
  </div>

  <?php if (!empty($field_site_faq_image) && count($field_site_faq_image) > 1): ?>
    <div class="site-faq-images-block clearfix">
      <?php
        // Вывод изображений прикрепленных к node.
        $count = 0;
        $language = 'und';
        foreach ($field_site_faq_image as $value) {
          if ($count > 0) {
            $imagecache = variable_get('site_faq_default_imagecache_preset', 'thumbnail');
            $title = $field_site_faq_image[$count]['title'];
            $uri = $field_site_faq_image[$count]['uri'];
            $variables = array(
              'style_name' => $imagecache,
              'path' => $uri,
              'alt' => $field_site_faq_image[$count]['alt'],
              'title' => $title,
              'width' => $field_site_faq_image[$count]['width'],
              'height' => $field_site_faq_image[$count]['height'],
              'attributes' => array('class' => 'site-faq-image'),
            );
            print '<a class="colorbox" title="' . $title . '" href="' . file_create_url($uri) . '">' . theme_image_style($variables) . '</a>';
          }
          $count ++;
        }
      ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($node->field_tags)): ?>
    <div class="site-faq-tags-block clearfix">
      <?php
        // Вывод тегов node.
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
          print '<span class="site-faq-tags">' . t('Tags') . ':</span> ' . $tags;
      ?>
    </div>
  <?php endif; ?>

  <?php
    // Отображение кнопок Яндекс на ссылки в социальные сети.
    if (variable_get('site_faq_yandex_buttons')):
  ?>
  <div class="yashare-auto-init" data-yashareL10n="ru" data-yashareType="icon" data-yashareQuickServices="vkontakte,facebook,twitter,odnoklassniki,moimir,lj,gplus"></div>
  <?php endif; ?>

  <?php
    // Remove the "Add new comment" link on the teaser page or if the comment
    // form is being displayed on the same page.
    if ($teaser || !empty($content['comments']['comment_form'])) {
      unset($content['links']['comment']['#links']['comment-add']);
    }
    // Only display the wrapper div if there are links.
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