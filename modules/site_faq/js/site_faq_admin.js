if (Drupal.jsEnabled) {
  /**
   * Функция: Смена статуса заявки от пользователя.
   */
  function update_status_note(id, status) {

    // Проверка входных параметров функции
    if ((id == null) || (id == 0)) return;
    if ((status == null)) return;

    if (status == 0) {

      if (!confirm("Удалить?")) return;

      // Отображаем прогресс выполнения
      $('#delete_button-' + id + '').html('<img src="/sites/all/modules/kvantstudio/images/ajax-loader-round.gif" alt="Прогресс" />');

    } else {

      // Отображаем прогресс выполнения
      $('#save_button-' + id + '').html('<img src="/sites/all/modules/kvantstudio/images/ajax-loader-round.gif" alt="Прогресс" />');

    }

    // Функция обработчик запроса ajax
    var success_result = function (data) {
      if (data.update_status_note_result) {
        $('#tr-' + id + '').hide();
        $('#tr-details-' + id + '').hide();
      } else {
        alert("Ошибка выполнения: success_result. Напишите в службу поддержки Вашего сайта указав ошибку.");
      }
    }

    // Функция обработчик ошибки ajax
    var error_result = function (data) {
      alert("Ошибка выполнения: update_status_note. Напишите в службу поддержки Вашего сайта указав ошибку. " + data);
    }

    // AJAX.
    $.ajax({
    type: 'POST',
    timeout: 5000,
    url: this.href,
    dataType: 'json',
    success: success_result,
    error: error_result,
    data: 'js=update_status_note&id=' + id + '&status=' + status + '',
    });

    // Предотврашаем перезагрузку браузера
    return false;

  }

  /**
   * Функция: сохранение ответа на вопрос пользователя.
   */
  function create_answer(id, status) {

    // Сброс параметров
    $('#result_action-' + id + '').empty();

    // Проверка входных параметров функции
    if ((id == null) || (id == 0)) return;
    if ((status == null)) return;

    if (status == 0) {

      if (!confirm("Вы действительно хотите удалить вопрос?")) return;

      // Отображаем прогресс выполнения
      $('#delete_button-' + id + '').html('<img src="/sites/all/modules/kvantstudio/images/ajax-loader-round.gif" alt="Прогресс" />');

    } else {

      var text = $('#text-' + id + '').val();
      if ((text == null) || (text == '')) {
        $('#result_action-' + id + '').html('<span class="error">Вы не указали текст соообщения</span>');
        return;
      }

      // Отображаем прогресс выполнения
      $('#save_button-' + id + '').html('<img src="/sites/all/modules/kvantstudio/images/ajax-loader-line.gif" alt="Прогресс" />');

    }



    // Функция обработчик запроса ajax
    var success_result = function (data) {
      if (data.create_answer_result) {
        $('#tr-' + id + '').hide();
        $('#tr-details-' + id + '').hide();
      } else {
        alert("Ошибка выполнения: success_result. Напишите в службу поддержки Вашего сайта указав ошибку.");
      }
    }

    // Функция обработчик ошибки ajax
    var error_result = function (data) {
      alert("Ошибка выполнения: create_answer. Напишите в службу поддержки Вашего сайта указав ошибку.");
    }

    // AJAX.
    $.ajax({
    type: 'POST',
    timeout: 5000,
    url: this.href,
    dataType: 'json',
    success: success_result,
    error: error_result,
    data: 'js=create_answer&id=' + id + '&status=' + status + '&text=' + text + '',
    });

    // Предотврашаем перезагрузку браузера
    return false;

  }

}