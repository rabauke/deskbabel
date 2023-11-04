#ifndef APP_MODEL_HPP
#define APP_MODEL_HPP

#include <QObject>
#include <QString>
#include <QScopedPointer>
#include <QStringListModel>
#include <QtQmlIntegration>
#include "dictionary.hpp"
#include "translations_list_model.hpp"


class app_model : public QObject {
  Q_OBJECT

public:
  explicit app_model(QObject* parent = nullptr);

  enum TranslationDirection : int { lang_a_to_b = 1, lang_b_to_a = 2, bidirectional = 3 };

  Q_ENUM(TranslationDirection)

  Q_INVOKABLE void loadDictionary(const QUrl& filename);
  Q_INVOKABLE void loadDefaultDictionary();

private:
  dictionary dict_;

  Q_PROPERTY(QUrl currentFolder READ get_current_folder NOTIFY currentFolderChanged)
  QUrl current_folder_;
  [[nodiscard]] QUrl get_current_folder() const { return current_folder_; }

  Q_PROPERTY(QString queryString READ get_query_string WRITE set_query_string NOTIFY
                 queryStringChanged)
  QString query_string_;
  [[nodiscard]] QString get_query_string() const { return query_string_; }
  void set_query_string(const QString& query_string);

  Q_PROPERTY(const translations_list_model* translations READ get_translations NOTIFY
                 translationsChanged)
  QScopedPointer<translations_list_model> translations_;
  [[nodiscard]] const translations_list_model* get_translations() const {
    return translations_.get();
  }

  Q_PROPERTY(bool dictionaryReady READ get_dictionary_ready NOTIFY dictionaryReadyChanged)
  bool dictionary_ready_{false};
  [[nodiscard]] bool get_dictionary_ready() const { return dictionary_ready_; }

  Q_PROPERTY(qsizetype dictionarySize READ get_dictionary_size NOTIFY dictionarySizeChanged)
  qsizetype dictionary_size_{0};
  [[nodiscard]] qsizetype get_dictionary_size() const { return dictionary_size_; }

  Q_PROPERTY(TranslationDirection translationDirection READ get_translation_direction WRITE
                 set_translation_direction NOTIFY translationDirectionChanged)
  TranslationDirection translation_direction_{TranslationDirection::bidirectional};
  [[nodiscard]] TranslationDirection get_translation_direction() const {
    return translation_direction_;
  }
  void set_translation_direction(TranslationDirection new_translation_direction);

  Q_PROPERTY(QString translationDirection0 READ get_translation_direction_0 NOTIFY dictionaryReadyChanged)
  [[nodiscard]] QString get_translation_direction_0() {
    return translations_->get_translation_direction(translations_list_model::translation_direction::bidirectional);
  }

  Q_PROPERTY(QString translationDirection1 READ get_translation_direction_1 NOTIFY dictionaryReadyChanged);
  [[nodiscard]] QString get_translation_direction_1() {
    return translations_->get_translation_direction(translations_list_model::translation_direction::lang_a_to_b);
  }

  Q_PROPERTY(QString translationDirection2 READ get_translation_direction_2 NOTIFY dictionaryReadyChanged);
  [[nodiscard]] QString get_translation_direction_2() {
    return translations_->get_translation_direction(translations_list_model::translation_direction::lang_b_to_a);
  }

  QML_NAMED_ELEMENT(AppModel)

  QFuture<void> read_dictionary_future_;

signals:
  void currentFolderChanged();
  void queryStringChanged();
  void translationsChanged();
  void dictionaryReadyChanged();
  void dictionarySizeChanged();
  void translationDirectionChanged();
};

#endif
