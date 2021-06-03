/********************************************************************************
** Form generated from reading UI file 'splashwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPLASHWIDGET_H
#define UI_SPLASHWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SplashWidget
{
public:
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *splashPage;
    QVBoxLayout *verticalLayout_2;
    QLabel *logoLabel;
    QWidget *disconnectPage;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer_2;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *reconnectButton;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *SplashWidget)
    {
        if (SplashWidget->objectName().isEmpty())
            SplashWidget->setObjectName(QString::fromUtf8("SplashWidget"));
        SplashWidget->resize(400, 300);
        SplashWidget->setWindowTitle(QString::fromUtf8("Form"));
        verticalLayout = new QVBoxLayout(SplashWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        stackedWidget = new QStackedWidget(SplashWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        splashPage = new QWidget();
        splashPage->setObjectName(QString::fromUtf8("splashPage"));
        verticalLayout_2 = new QVBoxLayout(splashPage);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        logoLabel = new QLabel(splashPage);
        logoLabel->setObjectName(QString::fromUtf8("logoLabel"));
        logoLabel->setText(QString::fromUtf8("TextLabel"));
        logoLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(logoLabel);

        stackedWidget->addWidget(splashPage);
        disconnectPage = new QWidget();
        disconnectPage->setObjectName(QString::fromUtf8("disconnectPage"));
        verticalLayout_3 = new QVBoxLayout(disconnectPage);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalSpacer_2 = new QSpacerItem(20, 90, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        label = new QLabel(disconnectPage);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(20);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        reconnectButton = new QPushButton(disconnectPage);
        reconnectButton->setObjectName(QString::fromUtf8("reconnectButton"));

        horizontalLayout->addWidget(reconnectButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 90, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        stackedWidget->addWidget(disconnectPage);

        verticalLayout->addWidget(stackedWidget);


        retranslateUi(SplashWidget);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SplashWidget);
    } // setupUi

    void retranslateUi(QWidget *SplashWidget)
    {
        label->setText(QCoreApplication::translate("SplashWidget", "Disconnected", nullptr));
        reconnectButton->setText(QCoreApplication::translate("SplashWidget", "Reconnect", nullptr));
        (void)SplashWidget;
    } // retranslateUi

};

namespace Ui {
    class SplashWidget: public Ui_SplashWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPLASHWIDGET_H
