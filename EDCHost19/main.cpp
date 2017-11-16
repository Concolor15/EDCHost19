#include "stdafx.h"
#include "config.h"

#include "EDCHost19.h"
#include <QtWidgets/QApplication>
#include <QtQuick>
#include <QtQml>

#include "HighResCam.h"
#include "MyCamera.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    qputenv( "QSG_RENDER_LOOP", "threaded");

	QApplication theApp(argc, argv);

	//Load Stylesheets
    QFile myStylesheets(":/MyUI.css");
	myStylesheets.open(QFile::ReadOnly);
	QString theStyle = myStylesheets.readAll();
    theApp.setStyleSheet(theStyle);
	myStylesheets.close();

    Controller::Init();

    qmlRegisterSingletonType<Controller>("my.uri", 1, 0, "Ctrl", [](QQmlEngine*, QJSEngine*)->QObject*{return GetController();});
    qmlRegisterType<MyFilter>("my.uri", 1, 0, "Filter");

    QQmlEngine engine;
    engine.rootContext()->setContextProperty("logic", &GetController()->GetLogic());

    QQmlComponent comp1(&engine, QUrl("qrc:/MatchUI.qml"));

    qInfo() << comp1.errors();

    QQuickWindow* window = qobject_cast<QQuickWindow*>(comp1.create());
    window->show();

    //EDCHost19 theMainWindow;
    //theMainWindow.show();
	return theApp.exec();
}
