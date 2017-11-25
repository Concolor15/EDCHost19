#include "stdafx.h"

#include <QtCore>
#include <QtQuick>
#include <QtQuickControls2/QQuickStyle>
#include <QtQml>

#include "imgproc.h"
#include "camera.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    //qputenv( "QSG_RENDER_LOOP", "threaded");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QQuickStyle::setStyle("Universal");

    QGuiApplication app(argc, argv);

    Controller::static_Init();

    GetController()->getMatchWindow()->show();
    return QGuiApplication::exec();
}
