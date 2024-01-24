#include "TranslationsListModel.hpp"

TranslationsListModel::TranslationsListModel(QObject *other) : QAbstractListModel(other) {
}


TranslationsListModel::TranslationsListModel(const Dictionary &dict)
    : QAbstractListModel(nullptr), m_dict{&dict} {
}


int TranslationsListModel::rowCount([[maybe_unused]] const QModelIndex &parent) const {
  return m_data.count();
}


QVariant TranslationsListModel::data(const QModelIndex &index, int role) const {
  const int row{index.row()};
  if (row >= 0 and row < m_data.count()) {
    const auto &trans{m_data.at(row)};
    switch (role) {
      case source:
        return trans.source;
      case translation:
        return trans.translation;
      case category:
        return trans.category;
    }
  }
  return {};
}


QHash<int, QByteArray> TranslationsListModel::roleNames() const {
  static const QHash<int, QByteArray> role_names{
      {source, "source"}, {translation, "translation"}, {category, "category"}};
  return role_names;
}


void TranslationsListModel::translate(const QString &query, TranslationDirection direction) {
  if (m_dict == nullptr)
    return;

  clear();
  auto new_query{query};
  new_query.replace("/\\s\\s*/g", " ").replace("/^\\s*/g", "").replace("/\\s*$/g", "");
  const bool a_to_b{direction == TranslationDirection::lang_a_to_b or
                    direction == TranslationDirection::bidirectional};
  const bool b_to_a{direction == TranslationDirection::lang_b_to_a or
                    direction == TranslationDirection::bidirectional};
  const decltype(m_dict->translate_a_to_b(query)) empty;
  auto result_1{a_to_b ? m_dict->translate_a_to_b(query) : empty};
  auto result_2{b_to_a ? m_dict->translate_b_to_a(query) : empty};
  if (result_1.count() + result_2.count() > 0) {
    beginInsertRows(QModelIndex(), 0, result_1.count() + result_2.count() - 1);
    const auto lang_a_to_b_str{get_translation_direction(TranslationDirection::lang_a_to_b)};
    const auto lang_b_to_a_str{get_translation_direction(TranslationDirection::lang_b_to_a)};
    for (const auto &q : result_1)
      m_data.append(Translation{q.first, q.second, lang_a_to_b_str});
    for (const auto &q : result_2)
      m_data.append(Translation{q.first, q.second, lang_b_to_a_str});
    endInsertRows();
    emit countChanged(m_data.count());
  }
}


QString TranslationsListModel::get_translation_direction(TranslationDirection dir) const {
  switch (dir) {
    case TranslationDirection::lang_a_to_b:
      return m_dict->language_a() + QStringLiteral(" → ") + m_dict->language_b();
    case TranslationDirection::lang_b_to_a:
      return m_dict->language_b() + QStringLiteral(" → ") + m_dict->language_a();
    case TranslationDirection::bidirectional:
    default:
      return m_dict->language_a() + QStringLiteral(" ↔ ") + m_dict->language_b();
  }
}


void TranslationsListModel::clear() {
  if (!m_data.empty()) {
    beginRemoveRows(QModelIndex(), 0, m_data.count() - 1);
    m_data.clear();
    endRemoveRows();
    emit countChanged(m_data.count());
  }
}
