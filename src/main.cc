#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLocale>
#include <QTranslator>
#include <QQmlContext>
#include <QString>

#include "version.hpp"
#include "translations_list_model.hpp"


int main(int argc, char *argv[]) {
  const QVersionNumber applicationVersion(version_major, version_minor, version_patch);

  QGuiApplication app(argc, argv);
  app.setApplicationName(u"DeskBabel"_qs);
  app.setOrganizationName(u"Heiko Bauke"_qs);
  app.setOrganizationDomain(u"heiko.bauke"_qs);
  app.setApplicationVersion(applicationVersion.toString());

  QTranslator translator;
  const QStringList ui_languages{QLocale::system().uiLanguages()};
  for (const QString &locale : ui_languages) {
    const QString base_name{"DeskBabel_" + QLocale(locale).name()};
    if (translator.load(":/i18n/" + base_name)) {
      app.installTranslator(&translator);
      break;
    }
  }

  QQmlApplicationEngine engine;
  const QUrl url{QStringLiteral("qrc:/qml/main.qml")};
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &obj_url) {
        if (obj == nullptr and url == obj_url)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
