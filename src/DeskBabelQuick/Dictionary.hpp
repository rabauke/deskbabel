#pragma once

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QByteArray>
#include <QString>
#include <QVector>
#include <QMultiMap>
#include <QVariantList>


class Dictionary : public QObject {
  Q_OBJECT

  enum class Language : int {
    unknown,
    en  // English
  };

  QVector<QByteArray> m_dict_a;
  QVector<QByteArray> m_dict_b;
  QMultiMap<QByteArray, int> m_map_a;
  QMultiMap<QByteArray, int> m_map_b;
  QString m_language_a;
  QString m_language_b;
  Language m_language_kind_a{Language::unknown};
  Language m_language_kind_b{Language::unknown};
  static constexpr int m_max_num_results = 200;

  friend class DictionaryLoader;

public:
  Dictionary() = default;
  void read(const QString &filename);
  [[nodiscard]] qsizetype size() const;
  void clear();
  [[nodiscard]] bool dict_empty() const { return m_dict_a.empty() or m_dict_b.empty(); }
  [[nodiscard]] QList<QPair<QString, QString>> translate_a_to_b(const QString &query) const;
  [[nodiscard]] QList<QPair<QString, QString>> translate_b_to_a(const QString &query) const;
  [[nodiscard]] QString language_a() const { return m_language_a; }
  [[nodiscard]] QString language_b() const { return m_language_b; }

signals:
  void entrieLoaded(qsizetype number_of_entries);

private:
  [[nodiscard]] QList<QPair<QString, QString>> translate(
      const QString &query, const QVector<QByteArray> &dict_a,
      const QVector<QByteArray> &dict_b, const QMultiMap<QByteArray, int> &map_a,
      Language language_kind_a) const;

  static QString purify(QString entry, Language language);
};
