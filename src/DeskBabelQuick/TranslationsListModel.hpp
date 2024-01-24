#pragma once

#include <QtCore>
#include <QtGui>
#include <QtQmlIntegration>
#include "Dictionary.hpp"


class TranslationsListModel : public QAbstractListModel {
  Q_OBJECT
public:
  enum class TranslationDirection : int {
    lang_a_to_b = 1,
    lang_b_to_a = 2,
    bidirectional = 3
  };

  explicit TranslationsListModel(QObject *other = nullptr);
  explicit TranslationsListModel(const Dictionary &dict);
  virtual ~TranslationsListModel() = default;

  [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
  [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

  void translate(const QString &query, TranslationDirection direction);
  [[nodiscard]] QString get_translation_direction(TranslationDirection dir) const;
  void clear();

signals:
  void countChanged(int);

protected:
  QHash<int, QByteArray> roleNames() const override;

private:
  struct Translation {
    QString source;
    QString translation;
    QString category;
  };

  const Dictionary *m_dict{nullptr};
  QList<Translation> m_data;

  static constexpr int source = Qt::UserRole;
  static constexpr int translation = Qt::UserRole + 1;
  static constexpr int category = Qt::UserRole + 2;
};
