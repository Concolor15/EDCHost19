#include "stdafx.h"
#include "config.h"

#include <QtCore>
#include <QtQuick>
#include <QtQuickControls2/QQuickStyle>
#include <QtQml>

#include "HighResCam.h"
#include "MyCamera.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    //qputenv( "QSG_RENDER_LOOP", "threaded");
    QQuickStyle::setStyle("Material");

	QApplication theApp(argc, argv);

	//Load Stylesheets
    QFile myStylesheets(":/MyUI.css");
	myStylesheets.open(QFile::ReadOnly);
	QString theStyle = myStylesheets.readAll();
    theApp.setStyleSheet(theStyle);
	myStylesheets.close();

    Controller::Init();

    //EDCHost19 theMainWindow;
    //theMainWindow.show();
	return theApp.exec();
}
