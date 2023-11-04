#include "app_model.hpp"
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QSettings>
#include <QStandardPaths>


app_model::app_model(QObject* parent)
    : QObject{parent}, translations_(new translations_list_model(dict_)) {
  QSettings settings;
  const QVariant dictionary{settings.value("dictionary")};
  if (dictionary.canConvert<QUrl>())
    current_folder_ = dictionary.toUrl().adjusted(QUrl::RemoveFilename);
  else {
    const QStringList folders{
        QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)};
    if (not folders.isEmpty())
      current_folder_ = QUrl::fromLocalFile(folders[0]);
  }
}


void app_model::loadDictionary(const QUrl& filename) {
  if (not read_dictionary_future_.isRunning()) {
    dictionary_ready_ = false;
    emit dictionaryReadyChanged();

    auto read_dictionary_worker{[this](const QUrl& filename) {
      try {
        dict_.read(filename.toLocalFile());
        QSettings settings;
        settings.setValue("dictionary", filename);
        current_folder_ = filename.adjusted(QUrl::RemoveFilename);
      } catch (...) {
        dict_.clear();
      }
    }};

    read_dictionary_future_ = QtConcurrent::run(read_dictionary_worker, filename);
    auto* watcher{new QFutureWatcher<void>()};
    connect(watcher, &QFutureWatcher<void>::finished, this, [this, watcher]() {
      dictionary_ready_ = true;
      dictionary_size_ = dict_.size();
      emit dictionaryReadyChanged();
      emit dictionarySizeChanged();
      emit currentFolderChanged();
      delete watcher;
    });
    watcher->setFuture(read_dictionary_future_);
  }
}


void app_model::loadDefaultDictionary() {
  QSettings settings;
  const QVariant dictionary{settings.value("dictionary")};
  if (dictionary.canConvert<QUrl>())
    loadDictionary(dictionary.toUrl());
  else {
    dictionary_ready_ = true;
    dictionary_size_ = dict_.size();
    emit dictionaryReadyChanged();
    emit dictionarySizeChanged();
  }
}


void app_model::set_query_string(const QString& query_string) {
  if (query_string_ != query_string) {
    query_string_ = query_string;
    translations_list_model::translation_direction dir(
        static_cast<translations_list_model::translation_direction>(translation_direction_));
    translations_->translate(query_string_, dir);
    emit translationsChanged();
  }
}


void app_model::set_translation_direction(
    app_model::TranslationDirection new_translation_direction) {
  if (translation_direction_ != new_translation_direction) {
    translation_direction_ = new_translation_direction;
    translations_list_model::translation_direction dir(
        static_cast<translations_list_model::translation_direction>(translation_direction_));
    translations_->translate(query_string_, dir);
    emit translationsChanged();
    emit translationDirectionChanged();
  }
}
