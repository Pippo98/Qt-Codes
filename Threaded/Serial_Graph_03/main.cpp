#include <QGuiApplication>
#include <QtQml/QQmlContext>
#include <QQmlApplicationEngine>
#include <QtWidgets/QApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QDebug>
#include <QObject>

#include "graph.h"
#include "serial.h"
#include "backend.h"

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QQmlContext *ctx = engine.rootContext();

    backend _backend;
    serial  _serial;
    graph   _graph;

    ctx->setContextProperty("serial", &_serial);
    ctx->setContextProperty("backend", &_backend);
    ctx->setContextProperty("graph", &_graph);

    _serial.start(QThread::Priority::HighestPriority);
    _graph.start(QThread::Priority::HighPriority);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;


    return app.exec();
}
