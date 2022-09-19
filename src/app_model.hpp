#ifndef APP_MODEL_HPP
#define APP_MODEL_HPP

#include <QObject>
#include <QString>
#include <QScopedPointer>
#include <QStringListModel>
#include "dictionary.hpp"
#include "translations_list_model.hpp"


class app_model : public QObject {
  Q_OBJECT

public:
  explicit app_model(QObject* parent = nullptr);

  Q_INVOKABLE void load_dictionary(const QUrl& filename);
  Q_INVOKABLE void load_default_dictionary();

private:
  dictionary dict_;

  Q_PROPERTY(QString queryString READ get_query_string WRITE set_query_string)
  QString query_string_;
  [[nodiscard]] QString get_query_string() const { return query_string_; }
  void set_query_string(const QString& query_string);

  Q_PROPERTY(
      const translations_list_model* translations READ get_translations NOTIFY translationsChanged)
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

  QFuture<void> read_dictionary_future_;

signals:
  void translationsChanged();
  void dictionaryReadyChanged();
  void dictionarySizeChanged();
};

#endif
