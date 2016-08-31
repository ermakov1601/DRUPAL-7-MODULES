<?php echo '<?xml version="1.0" encoding="UTF-8"?>'; echo "\r\n"  ?>
<!DOCTYPE yml_catalog SYSTEM "shops.dtd">
<yml_catalog date="<?php echo date('Y-m-d h:i'); ?>">
  <shop>
    <name><?php echo variable_get('site_commerce_yandex_market_name', 'ABC'); ?></name>
    <company><?php echo variable_get('site_commerce_yandex_market_company', 'ABC inc.'); ?></company>
    <url><?php echo $GLOBALS['base_url']; ?>/</url>
    <platform>SiteCommerce на базе Drupal 7</platform>
    <version><?php print $version; ?></version>
    <agency><?php echo variable_get('site_commerce_yandex_market_agency', 'Студия Павла Филинкова'); ?></agency>
    <email><?php echo variable_get('site_commerce_yandex_market_agency_email', variable_get('kvantstudio_admin_mail')); ?></email>
    <currencies>
      <currency id="<?php echo variable_get('site_commerce_yandex_market_сurrency', 'RUR'); ?>" rate="1"/>
    </currencies>
    <categories>
      <?php foreach ($categories as $term): ?>
      <?php $status = array(5, 10, 20, 30); ?>
      <?php if (site_commerce_count_positions($term->tid, $status, 0, TRUE)): ?>
      <category id="<?php echo $term->tid ?>"<?php if($term->parent): ?> parentId="<?php echo $term->parent ?>"<?php endif; ?>><?php echo site_commerce_yml_export_safe($term->name)?></category>
      <?php endif; ?>
      <?php endforeach; ?>
    </categories>
    <offers>
      <?php foreach ($pids as $pid):
        $position = site_commerce_position_load($pid, variable_get('site_commerce_imagecache_first', 'sc_first'));
        $position_summary = theme('site_commerce_body', array('site_commerce' => $position->site_commerce, 'summary_allow' => TRUE, 'value_allow' => TRUE));
        $position_summary = kvantstudio_summary($position_summary, 175, TRUE);

        // Страна производитель.
        $country = "";
        if ($position->field_site_commerce_country) {
          $country = taxonomy_term_load($position->field_site_commerce_country);
        }

        // Статус доступности товара для покупки.
        $available = 'false';
        if ($position->status == 10) {
          $available = 'true';
        }
      ?>
      <?php if ($position->field_site_commerce_cost_min || $position->field_site_commerce_cost): ?>
      <offer id="<?php echo $position->pid; ?>" available="<?php print $available; ?>">
      <url><?php echo url('site-commerce/' . $position->pid, array('absolute' => TRUE)); ?></url>
      <?php if ($position->field_site_commerce_cost_min): ?>
      <price><?php echo site_commerce_cost_format($position->field_site_commerce_cost_min); ?></price>
      <?php endif; ?>
      <?php if (!$position->field_site_commerce_cost_min && $position->field_site_commerce_cost): ?>
      <price><?php echo site_commerce_cost_format($position->field_site_commerce_cost); ?></price>
      <?php endif; ?>
      <currencyId><?php echo variable_get('site_commerce_yandex_market_сurrency', 'RUR'); ?></currencyId>
      <categoryId><?php echo $position->tid; ?></categoryId>
      <?php if (!empty($position->image['url'])): ?><picture><?php echo $position->image['url']; ?></picture><?php endif; ?>
      <store>true</store>
      <delivery>true</delivery>
      <name><?php echo site_commerce_yml_export_safe(check_plain($position->title)) ?></name>
      <description><?php echo site_commerce_yml_export_safe($position_summary) ?></description>
      <sales_notes><?php if ($position->field_site_commerce_cost_min): ?>Окончательная стоимость зависит от объёма заказа.<?php endif; ?></sales_notes>
      <?php if ($country): ?>
      <country_of_origin><?php print site_commerce_yml_export_safe(check_plain($country->name)); ?></country_of_origin>
      <?php endif; ?>
      </offer>
      <?php endif; ?>
      <?php endforeach; ?>
    </offers>
  </shop>
</yml_catalog>