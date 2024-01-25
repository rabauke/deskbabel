#pragma once

#include <QObject>
#include <QString>
#include <QScopedPointer>
#include <QStringListModel>
#include <QtQmlIntegration>
#include "Dictionary.hpp"
#include "TranslationsListModel.hpp"


class AppModel : public QObject {
  Q_OBJECT
  QML_NAMED_ELEMENT(AppModel)

public:
  explicit AppModel(QObject* parent = nullptr);
  ~AppModel();

  enum TranslationDirection : int { lang_a_to_b = 1, lang_b_to_a = 2, bidirectional = 3 };
  Q_ENUM(TranslationDirection)

  enum DictionaryState : int { not_loaded = 1, loading = 2, error = 3, ready = 4 };
  Q_ENUM(DictionaryState)

  Q_INVOKABLE void loadDictionary(const QUrl& filename);
  Q_INVOKABLE void loadDefaultDictionary();

  Q_PROPERTY(QUrl currentDictionaryFile READ get_current_dictionary_file NOTIFY
                 currentDictionaryFileChanged)
  Q_PROPERTY(QString currentDictionaryFileName READ get_current_dictionary_file_name NOTIFY
                 currentDictionaryFileChanged)
  Q_PROPERTY(QUrl currentFolder READ get_current_folder NOTIFY currentFolderChanged)
  Q_PROPERTY(bool autoLoadDictionary READ get_auto_load_dictionary WRITE
                 set_auto_load_dictionary NOTIFY autoLoadDictionaryChanged)
  Q_PROPERTY(QString queryString READ get_query_string WRITE set_query_string NOTIFY
                 queryStringChanged)
  Q_PROPERTY(const TranslationsListModel* translations READ get_translations NOTIFY
                 translationsChanged)
  Q_PROPERTY(bool dictionaryLoading READ get_dictionary_loading NOTIFY dictionaryStateChanged)
  Q_PROPERTY(bool dictionaryError READ get_dictionary_error NOTIFY dictionaryStateChanged)
  Q_PROPERTY(bool dictionaryReady READ get_dictionary_ready NOTIFY dictionaryStateChanged)
  Q_PROPERTY(qsizetype dictionarySize READ get_dictionary_size NOTIFY dictionarySizeChanged)
  Q_PROPERTY(TranslationDirection translationDirection READ get_translation_direction WRITE
                 set_translation_direction NOTIFY translationDirectionChanged)
  Q_PROPERTY(QString translationDirection0 READ get_translation_direction_0 NOTIFY
                 dictionaryStateChanged)
  Q_PROPERTY(QString translationDirection1 READ get_translation_direction_1 NOTIFY
                 dictionaryStateChanged)
  Q_PROPERTY(QString translationDirection2 READ get_translation_direction_2 NOTIFY
                 dictionaryStateChanged)

private:
  Dictionary m_dictionary;

  QUrl m_current_dictionary_file;
  [[nodiscard]] QUrl get_current_dictionary_file() const { return m_current_dictionary_file; }
  [[nodiscard]] QString get_current_dictionary_file_name() const {
    return m_current_dictionary_file.fileName();
  }

  QUrl m_current_folder;
  [[nodiscard]] QUrl get_current_folder() const { return m_current_folder; }

  bool m_auto_load_dictionary{false};
  [[nodiscard]] bool get_auto_load_dictionary() const { return m_auto_load_dictionary; }
  void set_auto_load_dictionary(bool auto_load_dictionary);

  QString m_query_string;
  [[nodiscard]] QString get_query_string() const { return m_query_string; }
  void set_query_string(const QString& query_string);

  QScopedPointer<TranslationsListModel> m_translations;
  [[nodiscard]] const TranslationsListModel* get_translations() const {
    return m_translations.get();
  }

  DictionaryState m_dictionary_state{DictionaryState::not_loaded};
  [[nodiscard]] bool get_dictionary_loading() const { return m_dictionary_state == DictionaryState::loading; }
  [[nodiscard]] bool get_dictionary_error() const { return m_dictionary_state == DictionaryState::error; }
  [[nodiscard]] bool get_dictionary_ready() const { return m_dictionary_state == DictionaryState::ready; }

  qsizetype m_dictionary_size{0};
  [[nodiscard]] qsizetype get_dictionary_size() const { return m_dictionary_size; }

  TranslationDirection m_translation_direction{TranslationDirection::bidirectional};
  [[nodiscard]] TranslationDirection get_translation_direction() const {
    return m_translation_direction;
  }
  void set_translation_direction(TranslationDirection new_translation_direction);

  [[nodiscard]] QString get_translation_direction_0() {
    return m_translations->get_translation_direction(
        TranslationsListModel::TranslationDirection::bidirectional);
  }

  [[nodiscard]] QString get_translation_direction_1() {
    return m_translations->get_translation_direction(
        TranslationsListModel::TranslationDirection::lang_a_to_b);
  }

  [[nodiscard]] QString get_translation_direction_2() {
    return m_translations->get_translation_direction(
        TranslationsListModel::TranslationDirection::lang_b_to_a);
  }

  QFuture<bool> m_read_dictionary_future;

signals:
  void currentFolderChanged();
  void currentDictionaryFileChanged();
  void autoLoadDictionaryChanged();
  void queryStringChanged();
  void translationsChanged();
  void dictionaryStateChanged();
  void dictionarySizeChanged();
  void translationDirectionChanged();
};


Q_DECLARE_METATYPE(AppModel::TranslationDirection)
Q_DECLARE_METATYPE(AppModel::DictionaryState)
