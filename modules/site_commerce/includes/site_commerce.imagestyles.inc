<?php

/**
 * @file
 * Provides default Image styles presets that can be overridden by site administrators.
 */

/*
 * Implementation of hook_image_default_styles().
 */
function site_commerce_image_default_styles() {
  $styles = array();

  $styles['sc_first'] = array(
    'effects' => array(
      array(
        'name' => 'image_scale',
        'data' => array('width' => 600, 'height' => 500, 'upscale' => 0),
        'weight' => 0,
      ),
      array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
           'RGB' => array('HEX' => '#FFFFFF'),
           'under' => 1,
           'upscale' => 0,
           'exact' => array('width' => 600, 'height' => 500, 'xpos' => 'center', 'ypos' => 'center'),
           'relative' => array('leftdiff' => 0, 'rightdiff' => 0, 'bottomdiff' => 0),
        ),
        'weight' => 0,
      ),
    ),
  );

  $styles['sc_thumbnail'] = array(
    'effects' => array(
      array(
        'name' => 'image_scale',
        'data' => array('width' => 100, 'height' => 80, 'upscale' => 0),
        'weight' => 0,
      ),
      array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
           'RGB' => array('HEX' => '#FFFFFF'),
           'under' => 1,
           'upscale' => 0,
           'exact' => array('width' => 100, 'height' => 80, 'xpos' => 'center', 'ypos' => 'center'),
           'relative' => array('leftdiff' => 0, 'rightdiff' => 0, 'bottomdiff' => 0),
        ),
        'weight' => 0,
      ),
    ),
  );

  $styles['sc_full'] = array(
    'effects' => array(
      array(
        'name' => 'image_scale',
        'data' => array('width' => 1024, 'height' => '', 'upscale' => 0),
        'weight' => 0,
      ),
    ),
  );

  $styles['sc_block'] = array(
    'effects' => array(
      array(
        'name' => 'image_scale',
        'data' => array('width' => 300, 'height' => 200, 'upscale' => 0),
        'weight' => 0,
      ),
      array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
           'RGB' => array('HEX' => '#FFFFFF'),
           'under' => 1,
           'upscale' => 0,
           'exact' => array('width' => 300, 'height' => 200, 'xpos' => 'center', 'ypos' => 'center'),
           'relative' => array('leftdiff' => 0, 'rightdiff' => 0, 'bottomdiff' => 0),
        ),
        'weight' => 0,
      ),
    ),
  );

  $styles['sc_taxonomy_block'] = array(
    'effects' => array(
      array(
        'name' => 'image_scale',
        'data' => array('width' => 40, 'height' => 40, 'upscale' => 0),
        'weight' => 0,
      ),
      array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
           'RGB' => array('HEX' => '#FFFFFF'),
           'under' => 1,
           'upscale' => 0,
           'exact' => array('width' => 40, 'height' => 40, 'xpos' => 'center', 'ypos' => 'center'),
           'relative' => array('leftdiff' => 0, 'rightdiff' => 0, 'bottomdiff' => 0),
        ),
        'weight' => 0,
      ),
    ),
  );

  $styles['sc_taxonomy'] = array(
    'effects' => array(
      array(
        'name' => 'image_scale',
        'data' => array('width' => 300, 'height' => 300, 'upscale' => 0),
        'weight' => 0,
      ),
      array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
           'RGB' => array('HEX' => '#FFFFFF'),
           'under' => 1,
           'upscale' => 0,
           'exact' => array('width' => 300, 'height' => 300, 'xpos' => 'center', 'ypos' => 'center'),
           'relative' => array('leftdiff' => 0, 'rightdiff' => 0, 'bottomdiff' => 0),
        ),
        'weight' => 0,
      ),
    ),
  );

  $styles['sc_subtaxonomy'] = array(
    'effects' => array(
      array(
        'name' => 'image_scale',
        'data' => array('width' => 120, 'height' => 120, 'upscale' => 0),
        'weight' => 0,
      ),
      array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
           'RGB' => array('HEX' => '#FFFFFF'),
           'under' => 1,
           'upscale' => 0,
           'exact' => array('width' => 140, 'height' => 140, 'xpos' => 'center', 'ypos' => 'center'),
           'relative' => array('leftdiff' => 0, 'rightdiff' => 0, 'bottomdiff' => 0),
        ),
        'weight' => 0,
      ),
    ),
  );

  $styles['sc_taxonomy_term'] = array(
    'effects' => array(
      array(
        'name' => 'image_scale',
        'data' => array('width' => 300, 'height' => 200, 'upscale' => 0),
        'weight' => 0,
      ),
      array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
           'RGB' => array('HEX' => '#FFFFFF'),
           'under' => 1,
           'upscale' => 0,
           'exact' => array('width' => 300, 'height' => 200, 'xpos' => 'center', 'ypos' => 'center'),
           'relative' => array('leftdiff' => 0, 'rightdiff' => 0, 'bottomdiff' => 0),
        ),
        'weight' => 0,
      ),
    ),
  );

  $styles['sc_parametr_cart'] = array(
    'effects' => array(
      array(
        'name' => 'image_scale',
        'data' => array('width' => 80, 'height' => 80, 'upscale' => 0),
        'weight' => 0,
      ),
      array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
           'RGB' => array('HEX' => '#FFFFFF'),
           'under' => 1,
           'upscale' => 0,
           'exact' => array('width' => 80, 'height' => 80, 'xpos' => 'center', 'ypos' => 'center'),
           'relative' => array('leftdiff' => 0, 'rightdiff' => 0, 'bottomdiff' => 0),
        ),
        'weight' => 0,
      ),
    ),
  );

  $styles['sc_select'] = array(
    'effects' => array(
      array(
        'name' => 'image_scale',
        'data' => array('width' => 80, 'height' => 80, 'upscale' => 0),
        'weight' => 0,
      ),
    ),
  );

  $styles['sc_select_load'] = array(
    'effects' => array(
      array(
        'name' => 'image_scale',
        'data' => array('width' => 480, 'height' => 400, 'upscale' => 0),
        'weight' => 0,
      ),
      array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
           'RGB' => array('HEX' => '#FFFFFF'),
           'under' => 1,
           'upscale' => 0,
           'exact' => array('width' => 480, 'height' => 400, 'xpos' => 'center', 'ypos' => 'center'),
           'relative' => array('leftdiff' => 0, 'rightdiff' => 0, 'bottomdiff' => 0),
        ),
        'weight' => 0,
      ),
    ),
  );

  $styles['sc_select_card'] = array(
    'effects' => array(
      array(
        'name' => 'image_scale_and_crop',
        'data' => array('width' => 60, 'height' => 60, 'upscale' => 0),
        'weight' => 0,
      ),
    ),
  );

  $styles['sc_children_image'] = array(
    'effects' => array(
      array(
        'name' => 'image_scale',
        'data' => array('width' => 40, 'height' => 40, 'upscale' => 0),
        'weight' => 0,
      ),
      array(
        'name' => 'canvasactions_definecanvas',
        'data' => array(
           'RGB' => array('HEX' => '#FFFFFF'),
           'under' => 1,
           'upscale' => 0,
           'exact' => array('width' => 40, 'height' => 40, 'xpos' => 'center', 'ypos' => 'center'),
           'relative' => array('leftdiff' => 0, 'rightdiff' => 0, 'bottomdiff' => 0),
        ),
        'weight' => 0,
      ),
    ),
  );
  return $styles;
}