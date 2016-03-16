if (Drupal.jsEnabled) {
  /**
   * Функция: сохранение ответа на вопрос пользователя.
   */
  function open_location(p1, p2) {
    var id = '';
    if (p2) id = $('#' + p2 + '').val();
    var link = '/' + p1 + '/' + id;
    document.location.replace(link);
  }
}