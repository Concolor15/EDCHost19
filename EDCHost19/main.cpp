#include "stdafx.h"
#include "EDCHost19.h"
#include <QtWidgets/QApplication>

#include "HighResCam.h"

int main(int argc, char *argv[])
{
	QApplication theApp(argc, argv);

	//Load Stylesheets
	QFile myStylesheets("MyUI.css");
	myStylesheets.open(QFile::ReadOnly);
	QString theStyle = myStylesheets.readAll();
	theApp.setStyleSheet(theStyle);
	myStylesheets.close();

	EDCHost19 theMainWindow;
	theMainWindow.show();
	return theApp.exec();
}
