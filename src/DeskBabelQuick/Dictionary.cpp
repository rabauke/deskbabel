#include "Dictionary.hpp"
#include <QFile>
#include <QSet>
#include <QRegularExpression>


void Dictionary::read(const QString &filename) {
  clear();
  QFile file(filename);
  if (not file.open(QIODevice::ReadOnly | QIODevice::Text))
    throw std::runtime_error("cannot read file");
  while (!file.atEnd()) {
    const QString line{file.readLine()};
    if (line.startsWith('#')) {
      QRegularExpression rx("^# ([A-Z][A-Z])-([A-Z][A-Z]) ");
      auto match{rx.match(line)};
      if (match.hasMatch()) {
        m_language_a = match.captured(1);
        m_language_b = match.captured(2);
      }
      continue;
    }
    const auto line_split{line.split('\t')};
    if (line_split.size() < 2)
      continue;
    QString entry_a{line_split[0]};
    QString entry_b{line_split[1]};
    if (entry_a.startsWith("to "))
      entry_a.remove(0, 3);
    if (entry_b.startsWith("to "))
      entry_b.remove(0, 3);
    m_dict_a.push_back(entry_a.toUtf8());
    m_dict_a.back().squeeze();
    m_dict_b.push_back(entry_b.toUtf8());
    m_dict_b.back().squeeze();
    const QString entry_plain_a{purify(entry_a)};
    const QString entry_plain_b{purify(entry_b)};
    for (const auto &v : entry_plain_a.split(' ', Qt::SkipEmptyParts)) {
      QByteArray word{v.toUtf8()};
      word.squeeze();
      m_map_a.insert(word, m_dict_a.size() - 1);
    }
    for (const auto &v : entry_plain_b.split(' ', Qt::SkipEmptyParts)) {
      QByteArray word{v.toUtf8()};
      word.squeeze();
      m_map_b.insert(word, m_dict_b.size() - 1);
    }
  }
  m_dict_a.squeeze();
  m_dict_b.squeeze();
  if (m_dict_a.empty() or m_dict_b.empty())
    throw std::runtime_error("empty dictionary");
}


qsizetype Dictionary::size() const {
  return m_dict_a.size();
}


void Dictionary::clear() {
  m_dict_a.clear();
  m_dict_b.clear();
  m_map_a.clear();
  m_map_b.clear();
  m_language_a.clear();
  m_language_b.clear();
}


QList<QPair<QString, QString>> Dictionary::translate_a_to_b(const QString &query) const {
  return translate(query, m_dict_a, m_dict_b, m_map_a);
}


QList<QPair<QString, QString>> Dictionary::translate_b_to_a(const QString &query) const {
  return translate(query, m_dict_b, m_dict_a, m_map_b);
}


QList<QPair<QString, QString>> Dictionary::translate(
    const QString &query, const QVector<QByteArray> &dict_a, const QVector<QByteArray> &dict_b,
    const QMultiMap<QByteArray, int> &map_a) const {
  // remove non-letters from query and split into single words
  QStringList query_list{purify(query).split(' ', Qt::SkipEmptyParts)};
  // no results if no words in query
  if (query_list.empty())
    return {};
  // construct intersection of all matches for each single query word
  QSet<int> results;
  {
    auto i{map_a.lowerBound(query_list[0].toUtf8())};
    while (i != map_a.end() and
           (i.key() == query_list[0] or QString(i.key()).startsWith(QString(query_list[0])))) {
      results.insert(*i);
      ++i;
      if (results.size() >= 4 * m_max_num_results)
        break;
    }
  }
  for (int k{1}; k < query_list.size(); ++k) {
    QSet<int> further_results;
    auto i{map_a.lowerBound(query_list[k].toUtf8())};
    while (i != map_a.end() and
           (i.key() == query_list[k] or QString(i.key()).startsWith(QString(query_list[k])))) {
      further_results.insert(*i);
      ++i;
    }
    results.intersect(further_results);
    if (results.size() >= 4 * m_max_num_results)
      break;
  }
  // calculate scores for each match
  QVector<int> hits(results.begin(), results.end());
  QVector<int> scores(results.size(), 0);
  // combine query words successfully into a string and check if
  // match contains this string, if yes increase score, in particular,
  // when match starts with this string
  for (int i{0}; i < hits.size(); ++i) {
    QString plain{purify(dict_a[hits[i]])};
    QString prefix{query_list[0]};
    if (plain.startsWith(prefix)) {
      scores[i] += 6;
      if (QString(dict_a[hits[i]])
              .toCaseFolded()
              .contains(QRegularExpression("^" + prefix + "\\S")))
        scores[i] -= 2;
    } else if (plain.contains(prefix))
      scores[i] += 3;
    for (int k{1}; k < query_list.size(); ++k) {
      prefix += " ";
      prefix += query_list[k];
      if (plain.startsWith(prefix))
        scores[i] += 6;
      else if (plain.contains(prefix))
        scores[i] += 3;
    }
    // additional points if there is an exact match
    if (plain == prefix)
      scores[i] += 2;
    // prefer short results
    scores[i] -= plain.count(" ");
  }
  // indirect sort according to scores
  QVector<int> indices(results.size());
  std::iota(indices.begin(), indices.end(), 0);
  std::stable_sort(indices.begin(), indices.end(), [&](const int a, const int b) -> bool {
    return (scores[a] == scores[b])
               ? (QString::compare(QString(dict_a[hits[a]]), QString(dict_a[hits[b]]),
                                   Qt::CaseInsensitive) < 0)
               : (scores[a] > scores[b]);
  });
  // generate sorted results
  QList<QPair<QString, QString>> result;
  for (auto i : indices) {
    result.append({dict_a[hits[i]], dict_b[hits[i]]});
    if (result.size() == m_max_num_results)
      break;
  }
  return result;
}


QString Dictionary::purify(const QString &entry) {
  QString plain;
  plain.reserve(entry.size());
  bool in_word_mode{true};
  QChar waiting_for;
  for (auto l : entry) {
    if (in_word_mode) {
      if (l.isLetter()) {
        plain.append(l.toCaseFolded());
        continue;
      }
      if (l == '-')
        l = ' ';
      if (l.isSpace() and (not plain.endsWith(' ')) and (not plain.isEmpty())) {
        plain.append(l);
        continue;
      }
      if (l == '(') {
        waiting_for = ')';
        in_word_mode = false;
        continue;
      }
      if (l == '[') {
        waiting_for = ']';
        in_word_mode = false;
        continue;
      }
      if (l == '{') {
        waiting_for = '}';
        in_word_mode = false;
        continue;
      }
      if (l == '<') {
        waiting_for = '>';
        in_word_mode = false;
        continue;
      }
    } else {
      if (l == waiting_for)
        in_word_mode = true;
    }
  }
  if (plain.endsWith(' '))
    plain.chop(1);
  return plain;
}
