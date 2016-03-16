<?php echo '<?xml version="1.0" encoding="UTF-8"?>'; echo "\r\n"  ?>
<!DOCTYPE yml_catalog SYSTEM "shops.dtd">
<yml_catalog date="<?php echo date('Y-m-d h:i'); ?>">
  <shop>
    <name><?php echo variable_get('site_name', 'Drupal'); ?></name>
    <company><?php echo variable_get('site_name', 'Drupal'); ?></company>
    <url><?php echo $GLOBALS['base_url']; ?>/</url>
    <platform>CMS Drupal SiteCommerce</platform>
    <version><?php echo $version; ?></version>
    <agency>kvantstudio.ru</agency>
    <email>info@kvantstudio.ru</email>
    <currencies>
      <currency id="<?php echo variable_get('site_commerce_yml_export_currency_code', 'RUR'); ?>" rate="1"/>
    </currencies>
    <categories>
      <?php foreach ($categories as $term): ?>
      <category id="<?php echo $term->tid ?>"<?php if($term->parent): ?> parentId="<?php echo $term->parent ?>"<?php endif; ?>><?php echo site_commerce_yml_export_safe($term->name)?></category>
      <?php endforeach; ?>
    </categories>
    <offers>
      <?php foreach ($pids as $pid):
        $position = site_commerce_position_load($pid, variable_get('site_commerce_imagecache_first', 'sc_first'));
        $position_summary = theme('site_commerce_body', array('site_commerce' => $position->site_commerce, 'summary_allow' => TRUE, 'value_allow' => FALSE));
        $position_summary = kvantstudio_summary($position_summary, 255, TRUE);

        // Страна производитель.
        $country = "";
        if ($position->field_site_commerce_country) {
          $country = taxonomy_term_load($position->field_site_commerce_country);
        }
      ?>
      <?php if ($position->field_site_commerce_cost): ?>
      <offer id="<?php echo $position->pid; ?>" available="false">
      <url><?php echo url('site-commerce/' . $position->pid, array('absolute' => TRUE)); ?></url>
      <price><?php echo site_commerce_cost_format($position->field_site_commerce_cost); ?></price>
      <currencyId><?php echo variable_get('site_commerce_yml_export_currency_code', 'RUR'); ?></currencyId>
      <categoryId><?php echo $position->tid; ?></categoryId>
      <?php if (!empty($position->image['url'])): ?><picture><?php echo $position->image['url']; ?></picture><?php endif; ?>
      <store>false</store>
      <delivery>true</delivery>
      <name><?php echo site_commerce_yml_export_safe(check_plain($position->title)) ?></name>
      <description><?php echo site_commerce_yml_export_safe($position_summary) ?></description>
      <sales_notes><?php if ($position->field_site_commerce_cost_min): ?>Минимальная сумма заказа <?php echo site_commerce_cost_format($position->field_site_commerce_cost_min); ?> <?php echo variable_get('site_commerce_default_currency'); ?><?php endif; ?></sales_notes>
      <?php if ($country): ?>
      <country_of_origin><?php print site_commerce_yml_export_safe(check_plain($country->name)); ?></country_of_origin>
      <?php endif; ?>
      </offer>
      <?php endif; ?>
      <?php endforeach; ?>
    </offers>
  </shop>
</yml_catalog>