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

public:
  explicit AppModel(QObject* parent = nullptr);

  enum TranslationDirection : int { lang_a_to_b = 1, lang_b_to_a = 2, bidirectional = 3 };

  Q_ENUM(TranslationDirection)

  Q_INVOKABLE void loadDictionary(const QUrl& filename);
  Q_INVOKABLE void loadDefaultDictionary();

private:
  Dictionary m_dictionary;

  Q_PROPERTY(QUrl currentFolder READ get_current_folder NOTIFY currentFolderChanged)
  QUrl m_current_folder;
  [[nodiscard]] QUrl get_current_folder() const { return m_current_folder; }

  Q_PROPERTY(QUrl currentDictionaryFile READ get_current_dictionary_file NOTIFY
                 currentDictionaryFileChanged)
  QUrl m_current_dictionary_file;
  [[nodiscard]] QUrl get_current_dictionary_file() const { return m_current_dictionary_file; }

  Q_PROPERTY(QString queryString READ get_query_string WRITE set_query_string NOTIFY
                 queryStringChanged)
  QString m_query_string;
  [[nodiscard]] QString get_query_string() const { return m_query_string; }
  void set_query_string(const QString& query_string);

  Q_PROPERTY(const TranslationsListModel* translations READ get_translations NOTIFY
                 translationsChanged)
  QScopedPointer<TranslationsListModel> m_translations;
  [[nodiscard]] const TranslationsListModel* get_translations() const {
    return m_translations.get();
  }

  Q_PROPERTY(bool dictionaryReady READ get_dictionary_ready NOTIFY dictionaryReadyChanged)
  bool m_dictionary_ready{false};
  [[nodiscard]] bool get_dictionary_ready() const { return m_dictionary_ready; }

  Q_PROPERTY(qsizetype dictionarySize READ get_dictionary_size NOTIFY dictionarySizeChanged)
  qsizetype m_dictionary_size{0};
  [[nodiscard]] qsizetype get_dictionary_size() const { return m_dictionary_size; }

  Q_PROPERTY(TranslationDirection translationDirection READ get_translation_direction WRITE
                 set_translation_direction NOTIFY translationDirectionChanged)
  TranslationDirection m_translation_direction{TranslationDirection::bidirectional};
  [[nodiscard]] TranslationDirection get_translation_direction() const {
    return m_translation_direction;
  }
  void set_translation_direction(TranslationDirection new_translation_direction);

  Q_PROPERTY(QString translationDirection0 READ get_translation_direction_0 NOTIFY
                 dictionaryReadyChanged)
  [[nodiscard]] QString get_translation_direction_0() {
    return m_translations->get_translation_direction(
        TranslationsListModel::TranslationDirection::bidirectional);
  }

  Q_PROPERTY(QString translationDirection1 READ get_translation_direction_1 NOTIFY
                 dictionaryReadyChanged);
  [[nodiscard]] QString get_translation_direction_1() {
    return m_translations->get_translation_direction(
        TranslationsListModel::TranslationDirection::lang_a_to_b);
  }

  Q_PROPERTY(QString translationDirection2 READ get_translation_direction_2 NOTIFY
                 dictionaryReadyChanged);
  [[nodiscard]] QString get_translation_direction_2() {
    return m_translations->get_translation_direction(
        TranslationsListModel::TranslationDirection::lang_b_to_a);
  }

  QML_NAMED_ELEMENT(AppModel)

  QFuture<void> m_read_dictionary_future;

signals:
  void currentFolderChanged();
  void currentDictionaryFileChanged();
  void queryStringChanged();
  void translationsChanged();
  void dictionaryReadyChanged();
  void dictionarySizeChanged();
  void translationDirectionChanged();
};
