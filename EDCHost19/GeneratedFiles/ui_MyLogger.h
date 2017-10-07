/********************************************************************************
** Form generated from reading UI file 'MyLogger.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYLOGGER_H
#define UI_MYLOGGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_MyLogger
{
public:
    QTextEdit *teCamera;
    QTextEdit *teSerial;
    QLabel *lblDebugBGN;

    void setupUi(QDialog *MyLogger)
    {
        if (MyLogger->objectName().isEmpty())
            MyLogger->setObjectName(QStringLiteral("MyLogger"));
        MyLogger->resize(750, 600);
        MyLogger->setMinimumSize(QSize(750, 600));
        MyLogger->setMaximumSize(QSize(750, 600));
        teCamera = new QTextEdit(MyLogger);
        teCamera->setObjectName(QStringLiteral("teCamera"));
        teCamera->setGeometry(QRect(25, 175, 325, 400));
        teCamera->setReadOnly(true);
        teSerial = new QTextEdit(MyLogger);
        teSerial->setObjectName(QStringLiteral("teSerial"));
        teSerial->setGeometry(QRect(400, 175, 325, 400));
        teSerial->setReadOnly(true);
        lblDebugBGN = new QLabel(MyLogger);
        lblDebugBGN->setObjectName(QStringLiteral("lblDebugBGN"));
        lblDebugBGN->setGeometry(QRect(0, 0, 750, 600));
        lblDebugBGN->raise();
        teCamera->raise();
        teSerial->raise();

        retranslateUi(MyLogger);

        QMetaObject::connectSlotsByName(MyLogger);
    } // setupUi

    void retranslateUi(QDialog *MyLogger)
    {
        MyLogger->setWindowTitle(QApplication::translate("MyLogger", "\350\260\203\350\257\225\344\277\241\346\201\257", Q_NULLPTR));
        lblDebugBGN->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MyLogger: public Ui_MyLogger {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYLOGGER_H
