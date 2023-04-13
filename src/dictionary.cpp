#include "dictionary.hpp"
#include <QFile>
#include <QSet>
#include <QRegularExpression>


void dictionary::read(const QString &filename) {
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
        language_a_ = match.captured(1);
        language_b_ = match.captured(2);
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
    dict_a_.push_back(entry_a.toUtf8());
    dict_a_.back().squeeze();
    dict_b_.push_back(entry_b.toUtf8());
    dict_b_.back().squeeze();
    const QString entry_plain_a{purify(entry_a)};
    const QString entry_plain_b{purify(entry_b)};
    for (const auto &v : entry_plain_a.split(' ', Qt::SkipEmptyParts)) {
      QByteArray word{v.toUtf8()};
      word.squeeze();
      map_a_.insert(word, dict_a_.size() - 1);
    }
    for (const auto &v : entry_plain_b.split(' ', Qt::SkipEmptyParts)) {
      QByteArray word{v.toUtf8()};
      word.squeeze();
      map_b_.insert(word, dict_b_.size() - 1);
    }
  }
  dict_a_.squeeze();
  dict_b_.squeeze();
  if (dict_a_.empty() or dict_b_.empty())
    throw std::runtime_error("empty dictionary");
}


qsizetype dictionary::size() const {
  return dict_a_.size();
}


void dictionary::clear() {
  dict_a_.clear();
  dict_b_.clear();
  map_a_.clear();
  map_b_.clear();
  language_a_.clear();
  language_b_.clear();
}


QList<QPair<QString, QString>> dictionary::translate_a_to_b(const QString &query) const {
  return translate(query, dict_a_, dict_b_, map_a_);
}


QList<QPair<QString, QString>> dictionary::translate_b_to_a(const QString &query) const {
  return translate(query, dict_b_, dict_a_, map_b_);
}


QList<QPair<QString, QString>> dictionary::translate(
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
      if (results.size() >= 4 * max_num_results_)
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
    if (results.size() >= 4 * max_num_results_)
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
                                   Qt::CaseInsensitive) <= 0)
               : (scores[a] > scores[b]);
  });
  // generate sorted results
  QList<QPair<QString, QString>> result;
  for (auto i : indices) {
    result.append({dict_a[hits[i]], dict_b[hits[i]]});
    if (result.size() == max_num_results_)
      break;
  }
  return result;
}


QString dictionary::purify(const QString &entry) {
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
