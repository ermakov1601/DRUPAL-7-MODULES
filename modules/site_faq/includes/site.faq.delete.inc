<?php
// $Id:

/**
 * Функция: формирование перечня вопросов и ответов.
 */
function site_faq_delete($nid) {

  // Обрабатываем входные параметры
  if ($nid && !is_numeric($nid)) return;

  node_delete($nid);
  db_query("DELETE FROM {site_faq_questions} WHERE nid=%d", $nid);
  db_query("DELETE FROM {site_faq_answers} WHERE nid=%d", $nid);

  drupal_goto("site-faq/questions");

}