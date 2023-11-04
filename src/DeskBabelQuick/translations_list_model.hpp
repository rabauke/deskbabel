#ifndef TRANSLATIONS_LIST_MODEL_HPP
#define TRANSLATIONS_LIST_MODEL_HPP

#include <QtCore>
#include <QtGui>
#include <QtQmlIntegration>
#include "dictionary.hpp"


class translations_list_model : public QAbstractListModel {
  Q_OBJECT
private:
  static constexpr int source = Qt::UserRole;
  static constexpr int translation = Qt::UserRole + 1;
  static constexpr int category = Qt::UserRole + 2;

public:
  enum class translation_direction : int {
    lang_a_to_b = 1,
    lang_b_to_a = 2,
    bidirectional = 3
  };

  explicit translations_list_model(QObject *other = nullptr);
  explicit translations_list_model(const dictionary &dict);
  virtual ~translations_list_model() = default;

  [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
  [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

  void translate(const QString &query, translation_direction direction);
  [[nodiscard]] QString get_translation_direction(translation_direction dir) const;
  void clear();

signals:
  void countChanged(int);

protected:
  QHash<int, QByteArray> roleNames() const override;

private:
  struct translation_type {
    QString source;
    QString translation;
    QString category;
  };

  const dictionary *dict_{nullptr};
  QList<translation_type> data_;
};

#endif
