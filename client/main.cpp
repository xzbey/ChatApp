#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "ApplicationController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    ApplicationController controller;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("appController", &controller);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("client", "Auth");

    return app.exec();
}