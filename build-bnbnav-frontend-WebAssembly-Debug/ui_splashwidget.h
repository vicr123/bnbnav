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
#include <QtWidgets/QLabel>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SplashWidget
{
public:
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout_2;
    QLabel *logoLabel;
    QWidget *page_2;

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
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayout_2 = new QVBoxLayout(page);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        logoLabel = new QLabel(page);
        logoLabel->setObjectName(QString::fromUtf8("logoLabel"));
        logoLabel->setText(QString::fromUtf8("TextLabel"));
        logoLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(logoLabel);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        stackedWidget->addWidget(page_2);

        verticalLayout->addWidget(stackedWidget);


        retranslateUi(SplashWidget);

        QMetaObject::connectSlotsByName(SplashWidget);
    } // setupUi

    void retranslateUi(QWidget *SplashWidget)
    {
        (void)SplashWidget;
    } // retranslateUi

};

namespace Ui {
    class SplashWidget: public Ui_SplashWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPLASHWIDGET_H
