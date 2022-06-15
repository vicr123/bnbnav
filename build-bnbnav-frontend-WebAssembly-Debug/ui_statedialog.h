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
#include <QtWidgets/QToolButton>
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
    QWidget *discoverTab;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_3;
    LocationEntryBox *searchBox;
    QPushButton *searchButton;
    QStackedWidget *discoverStack;
    QWidget *noLandmarkPage;
    QVBoxLayout *verticalLayout_6;
    QWidget *landmarkPage;
    QVBoxLayout *verticalLayout_7;
    QLabel *landmarkName;
    QLabel *landmarkCoordinates;
    QPushButton *directionsToLandmarkButton;
    QSpacerItem *verticalSpacer;
    QWidget *directionsTab;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    LocationEntryBox *startLocationBox;
    LocationEntryBox *endLocationBox;
    QHBoxLayout *horizontalLayout;
    QToolButton *routeOptionsButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *getDirectionsButton;
    QLabel *errorLabel;
    QListView *instructionList;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *goModeButton;
    QWidget *goModePage;
    QVBoxLayout *verticalLayout_4;
    QWidget *currentInstructionWidget;
    QWidget *currentRouteWidget;
    QHBoxLayout *horizontalLayout_4;
    QLabel *routeInformationLabel;
    QWidget *thenWidget;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_2;
    QLabel *thenIcon;

    void setupUi(QDialog *StateDialog)
    {
        if (StateDialog->objectName().isEmpty())
            StateDialog->setObjectName(QString::fromUtf8("StateDialog"));
        StateDialog->resize(400, 432);
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
        discoverTab = new QWidget();
        discoverTab->setObjectName(QString::fromUtf8("discoverTab"));
        verticalLayout_5 = new QVBoxLayout(discoverTab);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        searchBox = new LocationEntryBox(discoverTab);
        searchBox->setObjectName(QString::fromUtf8("searchBox"));

        horizontalLayout_3->addWidget(searchBox);

        searchButton = new QPushButton(discoverTab);
        searchButton->setObjectName(QString::fromUtf8("searchButton"));

        horizontalLayout_3->addWidget(searchButton);


        verticalLayout_5->addLayout(horizontalLayout_3);

        discoverStack = new QStackedWidget(discoverTab);
        discoverStack->setObjectName(QString::fromUtf8("discoverStack"));
        noLandmarkPage = new QWidget();
        noLandmarkPage->setObjectName(QString::fromUtf8("noLandmarkPage"));
        verticalLayout_6 = new QVBoxLayout(noLandmarkPage);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        discoverStack->addWidget(noLandmarkPage);
        landmarkPage = new QWidget();
        landmarkPage->setObjectName(QString::fromUtf8("landmarkPage"));
        verticalLayout_7 = new QVBoxLayout(landmarkPage);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        landmarkName = new QLabel(landmarkPage);
        landmarkName->setObjectName(QString::fromUtf8("landmarkName"));
        QFont font;
        font.setPointSize(20);
        landmarkName->setFont(font);

        verticalLayout_7->addWidget(landmarkName);

        landmarkCoordinates = new QLabel(landmarkPage);
        landmarkCoordinates->setObjectName(QString::fromUtf8("landmarkCoordinates"));

        verticalLayout_7->addWidget(landmarkCoordinates);

        directionsToLandmarkButton = new QPushButton(landmarkPage);
        directionsToLandmarkButton->setObjectName(QString::fromUtf8("directionsToLandmarkButton"));

        verticalLayout_7->addWidget(directionsToLandmarkButton);

        verticalSpacer = new QSpacerItem(20, 240, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer);

        discoverStack->addWidget(landmarkPage);

        verticalLayout_5->addWidget(discoverStack);

        tabWidget->addTab(discoverTab, QString());
        directionsTab = new QWidget();
        directionsTab->setObjectName(QString::fromUtf8("directionsTab"));
        verticalLayout_2 = new QVBoxLayout(directionsTab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(directionsTab);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font1;
        font1.setPointSize(15);
        label->setFont(font1);

        verticalLayout_2->addWidget(label);

        startLocationBox = new LocationEntryBox(directionsTab);
        startLocationBox->setObjectName(QString::fromUtf8("startLocationBox"));

        verticalLayout_2->addWidget(startLocationBox);

        endLocationBox = new LocationEntryBox(directionsTab);
        endLocationBox->setObjectName(QString::fromUtf8("endLocationBox"));

        verticalLayout_2->addWidget(endLocationBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, -1, 0);
        routeOptionsButton = new QToolButton(directionsTab);
        routeOptionsButton->setObjectName(QString::fromUtf8("routeOptionsButton"));
        routeOptionsButton->setPopupMode(QToolButton::InstantPopup);
        routeOptionsButton->setArrowType(Qt::NoArrow);

        horizontalLayout->addWidget(routeOptionsButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        getDirectionsButton = new QPushButton(directionsTab);
        getDirectionsButton->setObjectName(QString::fromUtf8("getDirectionsButton"));

        horizontalLayout->addWidget(getDirectionsButton);


        verticalLayout_2->addLayout(horizontalLayout);

        errorLabel = new QLabel(directionsTab);
        errorLabel->setObjectName(QString::fromUtf8("errorLabel"));
        errorLabel->setEnabled(false);
        errorLabel->setWordWrap(true);

        verticalLayout_2->addWidget(errorLabel);

        instructionList = new QListView(directionsTab);
        instructionList->setObjectName(QString::fromUtf8("instructionList"));

        verticalLayout_2->addWidget(instructionList);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        goModeButton = new QPushButton(directionsTab);
        goModeButton->setObjectName(QString::fromUtf8("goModeButton"));
        goModeButton->setEnabled(false);

        horizontalLayout_2->addWidget(goModeButton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        tabWidget->addTab(directionsTab, QString());

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

        currentRouteWidget = new QWidget(goModePage);
        currentRouteWidget->setObjectName(QString::fromUtf8("currentRouteWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(currentRouteWidget->sizePolicy().hasHeightForWidth());
        currentRouteWidget->setSizePolicy(sizePolicy);
        horizontalLayout_4 = new QHBoxLayout(currentRouteWidget);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        routeInformationLabel = new QLabel(currentRouteWidget);
        routeInformationLabel->setObjectName(QString::fromUtf8("routeInformationLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(routeInformationLabel->sizePolicy().hasHeightForWidth());
        routeInformationLabel->setSizePolicy(sizePolicy1);
        routeInformationLabel->setMargin(9);

        horizontalLayout_4->addWidget(routeInformationLabel);

        thenWidget = new QWidget(currentRouteWidget);
        thenWidget->setObjectName(QString::fromUtf8("thenWidget"));
        thenWidget->setAutoFillBackground(true);
        horizontalLayout_5 = new QHBoxLayout(thenWidget);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_2 = new QLabel(thenWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_5->addWidget(label_2);

        thenIcon = new QLabel(thenWidget);
        thenIcon->setObjectName(QString::fromUtf8("thenIcon"));
        thenIcon->setText(QString::fromUtf8(""));

        horizontalLayout_5->addWidget(thenIcon);


        horizontalLayout_4->addWidget(thenWidget);


        verticalLayout_4->addWidget(currentRouteWidget);

        stackedWidget->addWidget(goModePage);

        verticalLayout->addWidget(stackedWidget);


        retranslateUi(StateDialog);
        QObject::connect(startLocationBox, SIGNAL(go()), endLocationBox, SLOT(setFocus()));
        QObject::connect(endLocationBox, SIGNAL(go()), getDirectionsButton, SLOT(click()));
        QObject::connect(searchBox, SIGNAL(go()), searchButton, SLOT(click()));

        stackedWidget->setCurrentIndex(0);
        tabWidget->setCurrentIndex(1);
        discoverStack->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(StateDialog);
    } // setupUi

    void retranslateUi(QDialog *StateDialog)
    {
        StateDialog->setWindowTitle(QCoreApplication::translate("StateDialog", "bnbnav", nullptr));
        searchBox->setPlaceholderText(QCoreApplication::translate("StateDialog", "Search", nullptr));
        searchButton->setText(QCoreApplication::translate("StateDialog", "Search", nullptr));
        landmarkName->setText(QCoreApplication::translate("StateDialog", "TextLabel", nullptr));
        landmarkCoordinates->setText(QCoreApplication::translate("StateDialog", "TextLabel", nullptr));
        directionsToLandmarkButton->setText(QCoreApplication::translate("StateDialog", "Directions to here", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(discoverTab), QCoreApplication::translate("StateDialog", "Discover", nullptr));
        label->setText(QCoreApplication::translate("StateDialog", "Get Directions", nullptr));
        startLocationBox->setPlaceholderText(QCoreApplication::translate("StateDialog", "Start Location", nullptr));
        endLocationBox->setPlaceholderText(QCoreApplication::translate("StateDialog", "End Location", nullptr));
        routeOptionsButton->setText(QCoreApplication::translate("StateDialog", "Route Options", nullptr));
        getDirectionsButton->setText(QCoreApplication::translate("StateDialog", "Go", nullptr));
        errorLabel->setText(QCoreApplication::translate("StateDialog", "We can't find that location. Please enter a valid location.", nullptr));
        goModeButton->setText(QCoreApplication::translate("StateDialog", "Start!", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(directionsTab), QCoreApplication::translate("StateDialog", "Directions", nullptr));
        routeInformationLabel->setText(QCoreApplication::translate("StateDialog", "Calculating...", nullptr));
        label_2->setText(QCoreApplication::translate("StateDialog", "then", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StateDialog: public Ui_StateDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATEDIALOG_H
