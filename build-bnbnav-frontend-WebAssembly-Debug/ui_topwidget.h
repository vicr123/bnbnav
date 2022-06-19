/********************************************************************************
** Form generated from reading UI file 'topwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOPWIDGET_H
#define UI_TOPWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TopWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *panLabel;
    QSpacerItem *horizontalSpacer;
    QPushButton *spyButton;
    QPushButton *editModeButton;
    QPushButton *nightModeButton;
    QPushButton *loginButton;

    void setupUi(QWidget *TopWidget)
    {
        if (TopWidget->objectName().isEmpty())
            TopWidget->setObjectName(QString::fromUtf8("TopWidget"));
        TopWidget->resize(509, 94);
        TopWidget->setWindowTitle(QString::fromUtf8("Form"));
        horizontalLayout = new QHBoxLayout(TopWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        panLabel = new QLabel(TopWidget);
        panLabel->setObjectName(QString::fromUtf8("panLabel"));
        panLabel->setText(QString::fromUtf8("x: 0   y: 0"));

        horizontalLayout->addWidget(panLabel);

        horizontalSpacer = new QSpacerItem(230, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        spyButton = new QPushButton(TopWidget);
        spyButton->setObjectName(QString::fromUtf8("spyButton"));

        horizontalLayout->addWidget(spyButton);

        editModeButton = new QPushButton(TopWidget);
        editModeButton->setObjectName(QString::fromUtf8("editModeButton"));
        editModeButton->setCheckable(true);

        horizontalLayout->addWidget(editModeButton);

        nightModeButton = new QPushButton(TopWidget);
        nightModeButton->setObjectName(QString::fromUtf8("nightModeButton"));
        nightModeButton->setCheckable(true);

        horizontalLayout->addWidget(nightModeButton);

        loginButton = new QPushButton(TopWidget);
        loginButton->setObjectName(QString::fromUtf8("loginButton"));
        loginButton->setText(QString::fromUtf8("Log In"));

        horizontalLayout->addWidget(loginButton);


        retranslateUi(TopWidget);

        QMetaObject::connectSlotsByName(TopWidget);
    } // setupUi

    void retranslateUi(QWidget *TopWidget)
    {
        spyButton->setText(QCoreApplication::translate("TopWidget", "Spy", nullptr));
        editModeButton->setText(QCoreApplication::translate("TopWidget", "Edit Mode", nullptr));
        nightModeButton->setText(QCoreApplication::translate("TopWidget", "Night Mode", nullptr));
        (void)TopWidget;
    } // retranslateUi

};

namespace Ui {
    class TopWidget: public Ui_TopWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOPWIDGET_H
