<ul class="site-commerce-prices clearfix">
    <?php foreach ($variables['files'] as $fid => $value): ?>
        <li class="site-commerce-prices__item">
            <a class="site-commerce-prices__link" href="<?php print $value['url']; ?>" target="_blank"><?php print $value['description']; ?></a>
        </li>
    <?php endforeach; ?>
</ul>