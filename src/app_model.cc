#include "app_model.hpp"

app_model::app_model(QObject* parent)
    : QObject{parent}, translations_(new translations_list_model(dict_)) {
  const auto locations{QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)};
  for (const auto& a : locations) {
    const auto filename{a + "/dictionary.txt"};
    if (QFile::exists(filename)) {
      dictionary_ready_ = false;
      try {
        dict_.read(filename);
      } catch (...) {
        dict_.clear();
      }
      dictionary_ready_ = true;
      break;
    }
  }
}


void app_model::load(const QUrl& filename) {
  try {
    dict_.read(filename.toLocalFile());
  } catch (...) {
    dict_.clear();
  }
  query_string_ = "";
  emit queryStringChanged();
  set_query_string("");
}


void app_model::set_query_string(const QString& query_string) {
  query_string_ = query_string;
  translations_->translate(query_string_);
  emit translationsChanged();
}
