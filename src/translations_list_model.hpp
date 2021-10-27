#ifndef TRANSLATIONS_LIST_MODEL_HPP
#define TRANSLATIONS_LIST_MODEL_HPP

#include <QtCore>
#include <QtGui>
#include "dictionary.hpp"


class translations_list_model : public QAbstractListModel {
  Q_OBJECT
private:
  static constexpr int source = Qt::UserRole;
  static constexpr int translation = Qt::UserRole + 1;
  static constexpr int category = Qt::UserRole + 2;

public:
  translations_list_model(const dictionary &dict);
  ~translations_list_model() = default;

  int rowCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;

  // gives the size of the model
  Q_PROPERTY(int count READ count NOTIFY countChanged)

  Q_INVOKABLE void translate(const QString &query);
  Q_INVOKABLE void clear();

signals:
  void countChanged(int);

protected:
  QHash<int, QByteArray> roleNames() const override;

private:
  int count() const;

  struct translation_type {
    QString source;
    QString translation;
    QString category;
  };

  const dictionary &dict_;
  QList<translation_type> data_;
};

#endif
