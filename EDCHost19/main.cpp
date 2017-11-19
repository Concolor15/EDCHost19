#include "stdafx.h"
#include "config.h"

#include <QtCore>
#include <QtQuick>
#include <QtQuickControls2/QQuickStyle>
#include <QtQml>

#include "imgproc.h"
#include "MyCamera.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    //qputenv( "QSG_RENDER_LOOP", "threaded");
    QQuickStyle::setStyle("Material");

    QGuiApplication app(argc, argv);

    Controller::static_Init();

    GetController()->getMatchWindow()->show();
    return QGuiApplication::exec();
}
