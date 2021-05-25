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
    QPushButton *editModeButton;
    QPushButton *loginButton;

    void setupUi(QWidget *TopWidget)
    {
        if (TopWidget->objectName().isEmpty())
            TopWidget->setObjectName(QString::fromUtf8("TopWidget"));
        TopWidget->resize(400, 94);
        horizontalLayout = new QHBoxLayout(TopWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        panLabel = new QLabel(TopWidget);
        panLabel->setObjectName(QString::fromUtf8("panLabel"));
        panLabel->setText(QString::fromUtf8("x: 0   y: 0"));

        horizontalLayout->addWidget(panLabel);

        horizontalSpacer = new QSpacerItem(230, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        editModeButton = new QPushButton(TopWidget);
        editModeButton->setObjectName(QString::fromUtf8("editModeButton"));
        editModeButton->setCheckable(true);

        horizontalLayout->addWidget(editModeButton);

        loginButton = new QPushButton(TopWidget);
        loginButton->setObjectName(QString::fromUtf8("loginButton"));
        loginButton->setText(QString::fromUtf8("Log In"));

        horizontalLayout->addWidget(loginButton);


        retranslateUi(TopWidget);

        QMetaObject::connectSlotsByName(TopWidget);
    } // setupUi

    void retranslateUi(QWidget *TopWidget)
    {
        TopWidget->setWindowTitle(QCoreApplication::translate("TopWidget", "Form", nullptr));
        editModeButton->setText(QCoreApplication::translate("TopWidget", "Edit Mode", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TopWidget: public Ui_TopWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOPWIDGET_H
