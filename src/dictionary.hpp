#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QByteArray>
#include <QString>
#include <QVector>
#include <QMultiHash>
#include <QVariantList>

class dictionary;

class dictionary {
  QVector<QByteArray> dict_a_;
  QVector<QByteArray> dict_b_;
  QMultiHash<QByteArray, int> map_a_;
  QMultiHash<QByteArray, int> map_b_;
  QString language_a_;
  QString language_b_;
  const int max_num_results_{200};

  friend class dictionary_loader;

public:
  dictionary() = default;
  void read(const QString &filename);
  [[nodiscard]] int size() const;
  void clear();
  [[nodiscard]] bool dict_empty() const { return dict_a_.empty() or dict_b_.empty(); }
  [[nodiscard]] QList<QPair<QString, QString>> translate_a_to_b(const QString &query) const;
  [[nodiscard]] QList<QPair<QString, QString>> translate_b_to_a(const QString &query) const;
  [[nodiscard]] QString language_a() const { return language_a_; }
  [[nodiscard]] QString language_b() const { return language_b_; }

private:
  [[nodiscard]] QList<QPair<QString, QString>> translate(
      const QString &query, const QVector<QByteArray> &dict_a,
      const QVector<QByteArray> &dict_b, const QMultiHash<QByteArray, int> &map_a) const;

  static QString purify(const QString &entry);
};

#endif  // DICTIONARY_HPP
