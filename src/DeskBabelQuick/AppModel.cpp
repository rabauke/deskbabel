#include "AppModel.hpp"
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QSettings>
#include <QStandardPaths>


AppModel::AppModel(QObject* parent)
    : QObject{parent}, m_translations{new TranslationsListModel(m_dictionary)} {
  QSettings settings;
  const QVariant dictionary{settings.value("dictionary")};
  if (dictionary.canConvert<QUrl>())
    m_current_folder = dictionary.toUrl().adjusted(QUrl::RemoveFilename);
  else {
    const QStringList folders{
        QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)};
    if (not folders.isEmpty())
      m_current_folder = QUrl::fromLocalFile(folders[0]);
  }
}


void AppModel::loadDictionary(const QUrl& filename) {
  if (not m_read_dictionary_future.isRunning()) {
    m_dictionary_ready = false;
    emit dictionaryReadyChanged();

    auto read_dictionary_worker{[this](const QUrl& filename) {
      try {
        m_dictionary.read(filename.toLocalFile());
        QSettings settings;
        settings.setValue("dictionary", filename);
        m_current_folder = filename.adjusted(QUrl::RemoveFilename);
        m_current_dictionary_file = filename;
      } catch (...) {
        m_dictionary.clear();
      }
    }};

    m_read_dictionary_future = QtConcurrent::run(read_dictionary_worker, filename);
    auto* watcher{new QFutureWatcher<void>()};
    connect(watcher, &QFutureWatcher<void>::finished, this, [this, watcher]() {
      m_dictionary_ready = true;
      m_dictionary_size = m_dictionary.size();
      emit dictionaryReadyChanged();
      emit dictionarySizeChanged();
      emit currentFolderChanged();
      emit currentDictionaryFileChanged();
      delete watcher;
    });
    watcher->setFuture(m_read_dictionary_future);
  }
}


void AppModel::loadDefaultDictionary() {
  QSettings settings;
  const QVariant dictionary{settings.value("dictionary")};
  if (dictionary.canConvert<QUrl>())
    loadDictionary(dictionary.toUrl());
  else {
    m_dictionary_ready = true;
    m_dictionary_size = m_dictionary.size();
    emit dictionaryReadyChanged();
    emit dictionarySizeChanged();
  }
}


void AppModel::set_query_string(const QString& query_string) {
  if (m_query_string != query_string) {
    m_query_string = query_string;
    TranslationsListModel::TranslationDirection dir(
        static_cast<TranslationsListModel::TranslationDirection>(m_translation_direction));
    m_translations->translate(m_query_string, dir);
    emit translationsChanged();
  }
}


void AppModel::set_translation_direction(
    AppModel::TranslationDirection new_translation_direction) {
  if (m_translation_direction != new_translation_direction) {
    m_translation_direction = new_translation_direction;
    TranslationsListModel::TranslationDirection dir(
        static_cast<TranslationsListModel::TranslationDirection>(m_translation_direction));
    m_translations->translate(m_query_string, dir);
    emit translationsChanged();
    emit translationDirectionChanged();
  }
}
