/********************************************************************************
** Form generated from reading UI file 'Perspective.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PERSPECTIVE_H
#define UI_PERSPECTIVE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Perspective
{
public:
    QLabel *lblPSPBGN;
    QLabel *lblPicture;
    QLabel *lblPosLU;
    QLineEdit *lePosLU;
    QLabel *lblPosRU;
    QLineEdit *lePosRU;
    QLabel *lblPosLD;
    QLineEdit *lePosLD;
    QLabel *lblPosRD;
    QLineEdit *lePosRD;
    QPushButton *btnConfirm;
    QPushButton *btnRevoke;

    void setupUi(QDialog *Perspective)
    {
        if (Perspective->objectName().isEmpty())
            Perspective->setObjectName(QStringLiteral("Perspective"));
        Perspective->resize(1500, 950);
        Perspective->setMinimumSize(QSize(1500, 950));
        Perspective->setMaximumSize(QSize(1500, 950));
        lblPSPBGN = new QLabel(Perspective);
        lblPSPBGN->setObjectName(QStringLiteral("lblPSPBGN"));
        lblPSPBGN->setGeometry(QRect(0, 0, 1500, 950));
        lblPicture = new QLabel(Perspective);
        lblPicture->setObjectName(QStringLiteral("lblPicture"));
        lblPicture->setGeometry(QRect(25, 175, 1280, 720));
        lblPosLU = new QLabel(Perspective);
        lblPosLU->setObjectName(QStringLiteral("lblPosLU"));
        lblPosLU->setGeometry(QRect(1350, 175, 100, 25));
        lePosLU = new QLineEdit(Perspective);
        lePosLU->setObjectName(QStringLiteral("lePosLU"));
        lePosLU->setGeometry(QRect(1350, 225, 100, 25));
        lePosLU->setFrame(true);
        lePosLU->setAlignment(Qt::AlignCenter);
        lePosLU->setReadOnly(true);
        lblPosRU = new QLabel(Perspective);
        lblPosRU->setObjectName(QStringLiteral("lblPosRU"));
        lblPosRU->setGeometry(QRect(1350, 275, 100, 25));
        lePosRU = new QLineEdit(Perspective);
        lePosRU->setObjectName(QStringLiteral("lePosRU"));
        lePosRU->setGeometry(QRect(1350, 325, 100, 25));
        lePosRU->setFrame(true);
        lePosRU->setAlignment(Qt::AlignCenter);
        lePosRU->setReadOnly(true);
        lblPosLD = new QLabel(Perspective);
        lblPosLD->setObjectName(QStringLiteral("lblPosLD"));
        lblPosLD->setGeometry(QRect(1350, 375, 100, 25));
        lePosLD = new QLineEdit(Perspective);
        lePosLD->setObjectName(QStringLiteral("lePosLD"));
        lePosLD->setGeometry(QRect(1350, 425, 100, 25));
        lePosLD->setFrame(true);
        lePosLD->setAlignment(Qt::AlignCenter);
        lePosLD->setReadOnly(true);
        lblPosRD = new QLabel(Perspective);
        lblPosRD->setObjectName(QStringLiteral("lblPosRD"));
        lblPosRD->setGeometry(QRect(1350, 475, 100, 25));
        lePosRD = new QLineEdit(Perspective);
        lePosRD->setObjectName(QStringLiteral("lePosRD"));
        lePosRD->setGeometry(QRect(1350, 525, 100, 25));
        lePosRD->setFrame(true);
        lePosRD->setAlignment(Qt::AlignCenter);
        lePosRD->setReadOnly(true);
        btnConfirm = new QPushButton(Perspective);
        btnConfirm->setObjectName(QStringLiteral("btnConfirm"));
        btnConfirm->setGeometry(QRect(1350, 675, 100, 40));
        btnRevoke = new QPushButton(Perspective);
        btnRevoke->setObjectName(QStringLiteral("btnRevoke"));
        btnRevoke->setGeometry(QRect(1350, 600, 100, 40));

        retranslateUi(Perspective);
        QObject::connect(btnRevoke, SIGNAL(clicked()), Perspective, SLOT(OnRevoke()));
        QObject::connect(btnConfirm, SIGNAL(clicked()), Perspective, SLOT(OnConfirm()));

        QMetaObject::connectSlotsByName(Perspective);
    } // setupUi

    void retranslateUi(QDialog *Perspective)
    {
        Perspective->setWindowTitle(QApplication::translate("Perspective", "\350\256\276\347\275\256\351\200\217\350\247\206", Q_NULLPTR));
        lblPSPBGN->setText(QString());
        lblPicture->setText(QString());
        lblPosLU->setText(QApplication::translate("Perspective", "\345\267\246\344\270\212\345\235\220\346\240\207\357\274\232", Q_NULLPTR));
        lblPosRU->setText(QApplication::translate("Perspective", "\345\217\263\344\270\212\345\235\220\346\240\207\357\274\232", Q_NULLPTR));
        lblPosLD->setText(QApplication::translate("Perspective", "\345\267\246\344\270\213\345\235\220\346\240\207\357\274\232", Q_NULLPTR));
        lblPosRD->setText(QApplication::translate("Perspective", "\345\217\263\344\270\213\345\235\220\346\240\207\357\274\232", Q_NULLPTR));
        btnConfirm->setText(QApplication::translate("Perspective", "\347\241\256\350\256\244\345\235\220\346\240\207", Q_NULLPTR));
        btnRevoke->setText(QApplication::translate("Perspective", "\346\270\205\351\231\244\345\235\220\346\240\207", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Perspective: public Ui_Perspective {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PERSPECTIVE_H
