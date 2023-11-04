#include "translations_list_model.hpp"

translations_list_model::translations_list_model(QObject *other) : QAbstractListModel(other) {
}


translations_list_model::translations_list_model(const dictionary &dict)
    : QAbstractListModel(nullptr), dict_{&dict} {
}


int translations_list_model::rowCount([[maybe_unused]] const QModelIndex &parent) const {
  return data_.count();
}


QVariant translations_list_model::data(const QModelIndex &index, int role) const {
  const int row{index.row()};
  if (row >= 0 and row < data_.count()) {
    const auto &trans{data_.at(row)};
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


QHash<int, QByteArray> translations_list_model::roleNames() const {
  static const QHash<int, QByteArray> role_names{
      {source, "source"}, {translation, "translation"}, {category, "category"}};
  return role_names;
}


void translations_list_model::translate(const QString &query, translation_direction direction) {
  if (dict_ == nullptr)
    return;

  clear();
  auto new_query{query};
  new_query.replace("/\\s\\s*/g", " ").replace("/^\\s*/g", "").replace("/\\s*$/g", "");
  const bool a_to_b{direction == translation_direction::lang_a_to_b or
                    direction == translation_direction::bidirectional};
  const bool b_to_a{direction == translation_direction::lang_b_to_a or
                    direction == translation_direction::bidirectional};
  const decltype(dict_->translate_a_to_b(query)) empty;
  auto result_1{a_to_b ? dict_->translate_a_to_b(query) : empty};
  auto result_2{b_to_a ? dict_->translate_b_to_a(query) : empty};
  if (result_1.count() + result_2.count() > 0) {
    beginInsertRows(QModelIndex(), 0, result_1.count() + result_2.count() - 1);
    const auto lang_a_to_b_str{get_translation_direction(translation_direction::lang_a_to_b)};
    const auto lang_b_to_a_str{get_translation_direction(translation_direction::lang_b_to_a)};
    for (const auto &q : result_1)
      data_.append(translation_type{q.first, q.second, lang_a_to_b_str});
    for (const auto &q : result_2)
      data_.append(translation_type{q.first, q.second, lang_b_to_a_str});
    endInsertRows();
    emit countChanged(data_.count());
  }
}


QString translations_list_model::get_translation_direction(translation_direction dir) const {
  switch (dir) {
    case translation_direction::lang_a_to_b:
      return dict_->language_a() + QStringLiteral(" → ") + dict_->language_b();
    case translation_direction::lang_b_to_a:
      return dict_->language_b() + QStringLiteral(" → ") + dict_->language_a();
    case translation_direction::bidirectional:
    default:
      return dict_->language_a() + QStringLiteral(" ↔ ") + dict_->language_b();
  }
}


void translations_list_model::clear() {
  if (!data_.empty()) {
    beginRemoveRows(QModelIndex(), 0, data_.count() - 1);
    data_.clear();
    endRemoveRows();
    emit countChanged(data_.count());
  }
}
