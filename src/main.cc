#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStandardPaths>
#include <QLocale>
#include <QTranslator>
#include <QQmlContext>

#include "app_model.hpp"
#include "translations_list_model.hpp"


int main(int argc, char *argv[]) {
  QCoreApplication::setApplicationName(QStringLiteral("DeskBabel"));

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
  QGuiApplication app(argc, argv);

  QTranslator translator;
  const QStringList ui_languages{QLocale::system().uiLanguages()};
  for (const QString &locale : ui_languages) {
    const QString base_name{"DeskBabel_" + QLocale(locale).name()};
    if (translator.load(":/i18n/" + base_name)) {
      app.installTranslator(&translator);
      break;
    }
  }

  // https://doc.qt.io/qt-6/qtqml-modules-cppplugins.html
  // https://github.com/sailfishos-mirror/qtdeclarative/tree/dev/tests/auto/qml/qmlbasicapp
  qmlRegisterType<app_model>("DeskBabel.QmlComponents", 1, 0, "AppModel");
  qmlRegisterType<translations_list_model>("DeskBabel.QmlComponents", 1, 0, "DataEntryModel");

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
