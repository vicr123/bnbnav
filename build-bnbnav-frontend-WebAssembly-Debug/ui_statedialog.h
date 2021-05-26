/********************************************************************************
** Form generated from reading UI file 'statedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATEDIALOG_H
#define UI_STATEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "locationentrybox.h"

QT_BEGIN_NAMESPACE

class Ui_StateDialog
{
public:
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *normalModePage;
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    LocationEntryBox *startLocationBox;
    LocationEntryBox *endLocationBox;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *getDirectionsButton;
    QListView *instructionList;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *goModeButton;
    QWidget *goModePage;
    QVBoxLayout *verticalLayout_4;
    QWidget *currentInstructionWidget;

    void setupUi(QDialog *StateDialog)
    {
        if (StateDialog->objectName().isEmpty())
            StateDialog->setObjectName(QString::fromUtf8("StateDialog"));
        StateDialog->resize(400, 416);
        verticalLayout = new QVBoxLayout(StateDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(StateDialog);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        normalModePage = new QWidget();
        normalModePage->setObjectName(QString::fromUtf8("normalModePage"));
        verticalLayout_3 = new QVBoxLayout(normalModePage);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(normalModePage);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(tab_2);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(15);
        label->setFont(font);

        verticalLayout_2->addWidget(label);

        startLocationBox = new LocationEntryBox(tab_2);
        startLocationBox->setObjectName(QString::fromUtf8("startLocationBox"));

        verticalLayout_2->addWidget(startLocationBox);

        endLocationBox = new LocationEntryBox(tab_2);
        endLocationBox->setObjectName(QString::fromUtf8("endLocationBox"));

        verticalLayout_2->addWidget(endLocationBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, -1, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        getDirectionsButton = new QPushButton(tab_2);
        getDirectionsButton->setObjectName(QString::fromUtf8("getDirectionsButton"));

        horizontalLayout->addWidget(getDirectionsButton);


        verticalLayout_2->addLayout(horizontalLayout);

        instructionList = new QListView(tab_2);
        instructionList->setObjectName(QString::fromUtf8("instructionList"));

        verticalLayout_2->addWidget(instructionList);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        goModeButton = new QPushButton(tab_2);
        goModeButton->setObjectName(QString::fromUtf8("goModeButton"));
        goModeButton->setEnabled(false);

        horizontalLayout_2->addWidget(goModeButton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        tabWidget->addTab(tab_2, QString());

        verticalLayout_3->addWidget(tabWidget);

        stackedWidget->addWidget(normalModePage);
        goModePage = new QWidget();
        goModePage->setObjectName(QString::fromUtf8("goModePage"));
        verticalLayout_4 = new QVBoxLayout(goModePage);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        currentInstructionWidget = new QWidget(goModePage);
        currentInstructionWidget->setObjectName(QString::fromUtf8("currentInstructionWidget"));
        currentInstructionWidget->setAutoFillBackground(true);

        verticalLayout_4->addWidget(currentInstructionWidget);

        stackedWidget->addWidget(goModePage);

        verticalLayout->addWidget(stackedWidget);


        retranslateUi(StateDialog);

        stackedWidget->setCurrentIndex(1);
        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(StateDialog);
    } // setupUi

    void retranslateUi(QDialog *StateDialog)
    {
        StateDialog->setWindowTitle(QCoreApplication::translate("StateDialog", "Dialog", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("StateDialog", "Discover", nullptr));
        label->setText(QCoreApplication::translate("StateDialog", "Get Directions", nullptr));
        startLocationBox->setPlaceholderText(QCoreApplication::translate("StateDialog", "Start Location", nullptr));
        endLocationBox->setPlaceholderText(QCoreApplication::translate("StateDialog", "End Location", nullptr));
        getDirectionsButton->setText(QCoreApplication::translate("StateDialog", "Go", nullptr));
        goModeButton->setText(QCoreApplication::translate("StateDialog", "Start!", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("StateDialog", "Directions", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StateDialog: public Ui_StateDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATEDIALOG_H
