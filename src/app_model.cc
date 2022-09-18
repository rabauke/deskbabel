#include "app_model.hpp"
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent>

app_model::app_model(QObject* parent)
    : QObject{parent}, translations_(new translations_list_model(dict_)) {
}


void app_model::load_dictionary(const QUrl& filename) {
  if (not read_dictionary_future_.isRunning()) {
    dictionary_ready_ = false;
    emit dictionaryReadyChanged();

    auto read_dictionary_worker{[this](const QUrl& filename) {
      try {
        dict_.read(filename.toLocalFile());
      } catch (...) {
        dict_.clear();
      }
    }};

    read_dictionary_future_ = QtConcurrent::run(read_dictionary_worker, filename);
    auto* watcher{new QFutureWatcher<void>()};
    connect(watcher, &QFutureWatcher<void>::finished, [this, watcher]() {
      dictionary_ready_ = true;
      emit dictionaryReadyChanged();
      delete watcher;
    });
    watcher->setFuture(read_dictionary_future_);
  }
}


void app_model::load_default_dictionary() {
  const auto locations{QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)};
  for (const auto& a : locations) {
    const auto filename{a + "/dictionary.txt"};
    if (QFile::exists(filename)) {
      load_dictionary("file://" + filename);
      break;
    }
  }
}


void app_model::set_query_string(const QString& query_string) {
  query_string_ = query_string;
  translations_->translate(query_string_);
  emit translationsChanged();
}
