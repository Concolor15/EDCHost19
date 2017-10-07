/********************************************************************************
** Form generated from reading UI file 'MatchMain.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATCHMAIN_H
#define UI_MATCHMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_MatchMain
{
public:
    QLabel *lblCamera;
    QLabel *lblMatchBGN;
    QPushButton *btnBeginEnd;
    QLabel *lblScoreA;
    QLabel *lblEvilA;
    QLabel *lblEvilB;
    QLabel *lblScoreB;
    QLabel *lblSAVal;
    QLabel *lblEAVal;
    QLabel *lblEBVal;
    QLabel *lblSBVal;
    QPushButton *btnPauseResume;
    QPushButton *btnSOA;
    QPushButton *btnPenaltyB;
    QPushButton *btnPlusB;
    QPushButton *btnPenaltyA;
    QPushButton *btnPlusA;
    QLabel *lblTime;
    QLabel *lblTVal;
    QLabel *lblHaltA;
    QLabel *lblHaltB;
    QLabel *lblHAVal;
    QLabel *lblHBVal;
    QPushButton *btnSOB;

    void setupUi(QDialog *MatchMain)
    {
        if (MatchMain->objectName().isEmpty())
            MatchMain->setObjectName(QStringLiteral("MatchMain"));
        MatchMain->resize(1000, 800);
        MatchMain->setMinimumSize(QSize(1000, 800));
        MatchMain->setMaximumSize(QSize(1000, 800));
        lblCamera = new QLabel(MatchMain);
        lblCamera->setObjectName(QStringLiteral("lblCamera"));
        lblCamera->setGeometry(QRect(25, 175, 800, 600));
        lblMatchBGN = new QLabel(MatchMain);
        lblMatchBGN->setObjectName(QStringLiteral("lblMatchBGN"));
        lblMatchBGN->setGeometry(QRect(0, 0, 1000, 800));
        btnBeginEnd = new QPushButton(MatchMain);
        btnBeginEnd->setObjectName(QStringLiteral("btnBeginEnd"));
        btnBeginEnd->setGeometry(QRect(850, 550, 100, 40));
        lblScoreA = new QLabel(MatchMain);
        lblScoreA->setObjectName(QStringLiteral("lblScoreA"));
        lblScoreA->setGeometry(QRect(25, 25, 125, 25));
        lblScoreA->setAlignment(Qt::AlignCenter);
        lblEvilA = new QLabel(MatchMain);
        lblEvilA->setObjectName(QStringLiteral("lblEvilA"));
        lblEvilA->setGeometry(QRect(175, 25, 125, 25));
        lblEvilA->setAlignment(Qt::AlignCenter);
        lblEvilB = new QLabel(MatchMain);
        lblEvilB->setObjectName(QStringLiteral("lblEvilB"));
        lblEvilB->setGeometry(QRect(700, 25, 125, 25));
        lblEvilB->setAlignment(Qt::AlignCenter);
        lblScoreB = new QLabel(MatchMain);
        lblScoreB->setObjectName(QStringLiteral("lblScoreB"));
        lblScoreB->setGeometry(QRect(850, 25, 125, 25));
        lblScoreB->setAlignment(Qt::AlignCenter);
        lblSAVal = new QLabel(MatchMain);
        lblSAVal->setObjectName(QStringLiteral("lblSAVal"));
        lblSAVal->setGeometry(QRect(25, 75, 125, 50));
        lblSAVal->setAlignment(Qt::AlignCenter);
        lblEAVal = new QLabel(MatchMain);
        lblEAVal->setObjectName(QStringLiteral("lblEAVal"));
        lblEAVal->setGeometry(QRect(175, 75, 125, 50));
        lblEAVal->setAlignment(Qt::AlignCenter);
        lblEBVal = new QLabel(MatchMain);
        lblEBVal->setObjectName(QStringLiteral("lblEBVal"));
        lblEBVal->setGeometry(QRect(700, 75, 125, 50));
        lblEBVal->setAlignment(Qt::AlignCenter);
        lblSBVal = new QLabel(MatchMain);
        lblSBVal->setObjectName(QStringLiteral("lblSBVal"));
        lblSBVal->setGeometry(QRect(850, 75, 125, 50));
        lblSBVal->setAlignment(Qt::AlignCenter);
        btnPauseResume = new QPushButton(MatchMain);
        btnPauseResume->setObjectName(QStringLiteral("btnPauseResume"));
        btnPauseResume->setGeometry(QRect(850, 475, 100, 40));
        btnSOA = new QPushButton(MatchMain);
        btnSOA->setObjectName(QStringLiteral("btnSOA"));
        btnSOA->setGeometry(QRect(850, 625, 100, 40));
        btnPenaltyB = new QPushButton(MatchMain);
        btnPenaltyB->setObjectName(QStringLiteral("btnPenaltyB"));
        btnPenaltyB->setGeometry(QRect(850, 400, 100, 40));
        btnPlusB = new QPushButton(MatchMain);
        btnPlusB->setObjectName(QStringLiteral("btnPlusB"));
        btnPlusB->setGeometry(QRect(850, 325, 100, 40));
        btnPenaltyA = new QPushButton(MatchMain);
        btnPenaltyA->setObjectName(QStringLiteral("btnPenaltyA"));
        btnPenaltyA->setGeometry(QRect(850, 250, 100, 40));
        btnPlusA = new QPushButton(MatchMain);
        btnPlusA->setObjectName(QStringLiteral("btnPlusA"));
        btnPlusA->setGeometry(QRect(850, 175, 100, 40));
        lblTime = new QLabel(MatchMain);
        lblTime->setObjectName(QStringLiteral("lblTime"));
        lblTime->setGeometry(QRect(450, 25, 100, 25));
        lblTime->setAlignment(Qt::AlignCenter);
        lblTVal = new QLabel(MatchMain);
        lblTVal->setObjectName(QStringLiteral("lblTVal"));
        lblTVal->setGeometry(QRect(450, 75, 100, 50));
        lblTVal->setAlignment(Qt::AlignCenter);
        lblHaltA = new QLabel(MatchMain);
        lblHaltA->setObjectName(QStringLiteral("lblHaltA"));
        lblHaltA->setGeometry(QRect(325, 25, 100, 25));
        lblHaltA->setAlignment(Qt::AlignCenter);
        lblHaltB = new QLabel(MatchMain);
        lblHaltB->setObjectName(QStringLiteral("lblHaltB"));
        lblHaltB->setGeometry(QRect(575, 25, 100, 25));
        lblHaltB->setAlignment(Qt::AlignCenter);
        lblHAVal = new QLabel(MatchMain);
        lblHAVal->setObjectName(QStringLiteral("lblHAVal"));
        lblHAVal->setGeometry(QRect(325, 75, 100, 50));
        lblHAVal->setAlignment(Qt::AlignCenter);
        lblHBVal = new QLabel(MatchMain);
        lblHBVal->setObjectName(QStringLiteral("lblHBVal"));
        lblHBVal->setGeometry(QRect(575, 75, 100, 50));
        lblHBVal->setAlignment(Qt::AlignCenter);
        btnSOB = new QPushButton(MatchMain);
        btnSOB->setObjectName(QStringLiteral("btnSOB"));
        btnSOB->setGeometry(QRect(850, 700, 100, 40));
        lblMatchBGN->raise();
        lblCamera->raise();
        btnBeginEnd->raise();
        lblScoreA->raise();
        lblEvilA->raise();
        lblEvilB->raise();
        lblScoreB->raise();
        lblSAVal->raise();
        lblEAVal->raise();
        lblEBVal->raise();
        lblSBVal->raise();
        btnPauseResume->raise();
        btnSOA->raise();
        btnPenaltyB->raise();
        btnPlusB->raise();
        btnPenaltyA->raise();
        btnPlusA->raise();
        lblTime->raise();
        lblTVal->raise();
        lblHaltA->raise();
        lblHaltB->raise();
        lblHAVal->raise();
        lblHBVal->raise();
        btnSOB->raise();

        retranslateUi(MatchMain);
        QObject::connect(btnPlusA, SIGNAL(clicked()), MatchMain, SLOT(OnPlusA()));
        QObject::connect(btnPlusB, SIGNAL(clicked()), MatchMain, SLOT(OnPlusB()));
        QObject::connect(btnPenaltyA, SIGNAL(clicked()), MatchMain, SLOT(OnPenaltyA()));
        QObject::connect(btnPenaltyB, SIGNAL(clicked()), MatchMain, SLOT(OnPenaltyB()));
        QObject::connect(btnPauseResume, SIGNAL(clicked()), MatchMain, SLOT(OnPR()));
        QObject::connect(btnBeginEnd, SIGNAL(clicked()), MatchMain, SLOT(OnBE()));
        QObject::connect(btnSOA, SIGNAL(clicked()), MatchMain, SLOT(OnSOA()));
        QObject::connect(btnSOB, SIGNAL(clicked()), MatchMain, SLOT(OnSOB()));

        QMetaObject::connectSlotsByName(MatchMain);
    } // setupUi

    void retranslateUi(QDialog *MatchMain)
    {
        MatchMain->setWindowTitle(QApplication::translate("MatchMain", "\350\277\233\350\241\214\346\257\224\350\265\233", Q_NULLPTR));
        lblCamera->setText(QString());
        lblMatchBGN->setText(QString());
        btnBeginEnd->setText(QString());
        lblScoreA->setText(QApplication::translate("MatchMain", "A\346\226\271\345\276\227\345\210\206", Q_NULLPTR));
        lblEvilA->setText(QApplication::translate("MatchMain", "A\346\226\271\351\202\252\346\201\266\345\200\274", Q_NULLPTR));
        lblEvilB->setText(QApplication::translate("MatchMain", "B\346\226\271\351\202\252\346\201\266\345\200\274", Q_NULLPTR));
        lblScoreB->setText(QApplication::translate("MatchMain", "B\346\226\271\345\276\227\345\210\206", Q_NULLPTR));
        lblSAVal->setText(QString());
        lblEAVal->setText(QString());
        lblEBVal->setText(QString());
        lblSBVal->setText(QString());
        btnPauseResume->setText(QString());
        btnSOA->setText(QApplication::translate("MatchMain", "\347\202\271\347\220\203A", Q_NULLPTR));
        btnPenaltyB->setText(QApplication::translate("MatchMain", "B\346\226\271\347\212\257\350\247\204", Q_NULLPTR));
        btnPlusB->setText(QApplication::translate("MatchMain", "B\346\226\271\345\212\240\345\210\206", Q_NULLPTR));
        btnPenaltyA->setText(QApplication::translate("MatchMain", "A\346\226\271\347\212\257\350\247\204", Q_NULLPTR));
        btnPlusA->setText(QApplication::translate("MatchMain", "A\346\226\271\345\212\240\345\210\206", Q_NULLPTR));
        lblTime->setText(QApplication::translate("MatchMain", "\346\257\224\350\265\233\346\227\266\351\227\264", Q_NULLPTR));
        lblTVal->setText(QString());
        lblHaltA->setText(QApplication::translate("MatchMain", "A\345\201\234\346\255\242\346\254\241\346\225\260", Q_NULLPTR));
        lblHaltB->setText(QApplication::translate("MatchMain", "B\345\201\234\346\255\242\346\254\241\346\225\260", Q_NULLPTR));
        lblHAVal->setText(QString());
        lblHBVal->setText(QString());
        btnSOB->setText(QApplication::translate("MatchMain", "\347\202\271\347\220\203B", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MatchMain: public Ui_MatchMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATCHMAIN_H
