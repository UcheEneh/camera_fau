#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQuickView>
#include <QQmlEngine>

#include "todb.h"
#include "crop.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<ToDB>("ToDBBackend", 1, 0, "ToDB");
    qmlRegisterType<crop>("CropBackend", 1, 0, "Crop");
    QGuiApplication app(argc, argv);

    // Original
    /*
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    */

    // From sensor example
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QObject *topLevel = engine.rootObjects().value(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
    if (!window) {
        qWarning("Error: Your root item has to be a Window.");
        return -1;
    }
    window->show();
    // window->showFullScreen();



    /*
    // From declarative camera
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    QObject::connect(view.engine(), &QQmlEngine::quit,
                     qApp, &QGuiApplication::quit);
    view.setSource(QUrl("qrc:/main.qml"));
    view.resize(800, 480);
    view.show();
    */

    return app.exec();
}
