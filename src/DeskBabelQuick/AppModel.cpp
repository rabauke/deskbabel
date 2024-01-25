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

  const QVariant auto_load_dictionary{settings.value("autoLoadDictionary")};
  if (auto_load_dictionary.canConvert<bool>())
    m_auto_load_dictionary = auto_load_dictionary.toBool();
}


AppModel::~AppModel() {
  QSettings settings;
  settings.setValue("dictionary", m_current_dictionary_file);
  settings.setValue("autoLoadDictionary", m_auto_load_dictionary);
}


void AppModel::set_auto_load_dictionary(bool auto_load_dictionary) {
  if (auto_load_dictionary != m_auto_load_dictionary) {
    m_auto_load_dictionary = auto_load_dictionary;
    emit autoLoadDictionaryChanged();
  }
}


void AppModel::loadDictionary(const QUrl& filename) {
  if (not m_read_dictionary_future.isRunning()) {
    m_dictionary_state = DictionaryState::loading;
    m_current_folder = filename.adjusted(QUrl::RemoveFilename);
    m_current_dictionary_file = filename;
    emit dictionaryStateChanged();
    emit currentFolderChanged();
    emit currentDictionaryFileChanged();

    connect(&m_dictionary, &Dictionary::entrieLoaded, this, [this](qsizetype n) {
      m_dictionary_size = n;
      emit dictionarySizeChanged();
    });

    auto read_dictionary_worker{[this](const QUrl& filename) {
      try {
        m_dictionary.read(filename.toLocalFile());
      } catch (...) {
        m_dictionary.clear();
        return false;
      }
      return true;
    }};

    m_read_dictionary_future = QtConcurrent::run(read_dictionary_worker, filename);
    auto* watcher{new QFutureWatcher<bool>()};
    connect(watcher, &QFutureWatcher<bool>::finished, this, [this, watcher]() {
      disconnect(&m_dictionary, nullptr, this, nullptr);
      m_dictionary_state = watcher->result() ? DictionaryState::ready : DictionaryState::error;
      m_dictionary_size = m_dictionary.size();
      emit dictionaryStateChanged();
      emit dictionarySizeChanged();
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
