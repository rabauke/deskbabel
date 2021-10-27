#include "translations_list_model.hpp"

translations_list_model::translations_list_model(const dictionary &dict)
    : QAbstractListModel(nullptr), dict_{dict} {
}


int translations_list_model::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
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
  return QString("not found");
}


QHash<int, QByteArray> translations_list_model::roleNames() const {
  static const QHash<int, QByteArray> role_names{
      {source, "source"}, {translation, "translation"}, {category, "category"}};
  return role_names;
}


int translations_list_model::count() const {
  return data_.count();
}

// QColor translations_list_model::get(int index) {
//   if (index < 0 || index >= data_.count())
//     return {};
//   return data_.at(index);
// }
//
//
// void translations_list_model::insert(int index, const QString &colorValue) {
//   if (index < 0 || index > data_.count())
//     return;
//   QColor color(colorValue);
//   if (!color.isValid())
//     return;
//   // view protocol (begin => manipulate => end]
//   emit beginInsertRows(QModelIndex(), index, index);
//   data_.insert(index, color);
//   emit endInsertRows();
//   emit countChanged(data_.count());
// }
//
//
// void translations_list_model::append(const QString &colorValue) {
//   insert(data_.size(), colorValue);
// }
//
//
// void translations_list_model::remove(int index) {
//   if (index < 0 || index >= data_.count())
//     return;
//   emit beginRemoveRows(QModelIndex(), index, index);
//   data_.removeAt(index);
//   emit endRemoveRows();
//   emit countChanged(data_.count());
// }
//
//
void translations_list_model::translate(const QString &query) {
  clear();
  auto new_query{query};
  new_query.replace("/\\s\\s*/g", " ").replace("/^\\s*/g", "").replace("/\\s*$/g", "");
  auto result_1{dict_.translate_a_to_b(query)};
  auto result_2{dict_.translate_b_to_a(query)};
  if (result_1.count() + result_2.count() > 0) {
    emit beginInsertRows(QModelIndex(), 0, result_1.count() + result_2.count() - 1);
    for (const auto &q : result_1)
      data_.append(translation_type{q.first, q.second, dict_.language_a() + QStringLiteral(" → ") + dict_.language_b()});
    for (const auto &q : result_2)
      data_.append(translation_type{q.first, q.second, dict_.language_b() + QStringLiteral(" → ") + dict_.language_a()});
    emit endInsertRows();
    emit countChanged(data_.count());
  }
}

void translations_list_model::clear() {
  if (!data_.empty()) {
    emit beginRemoveRows(QModelIndex(), 0, data_.count() - 1);
    data_.clear();
    emit endRemoveRows();
    emit countChanged(data_.count());
  }
}
