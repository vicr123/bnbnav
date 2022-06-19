/********************************************************************************
** Form generated from reading UI file 'spywidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPYWIDGET_H
#define UI_SPYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SpyWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QRadioButton *disableSpyButton;
    QRadioButton *spyOnTurnRestrictionButton;

    void setupUi(QWidget *SpyWidget)
    {
        if (SpyWidget->objectName().isEmpty())
            SpyWidget->setObjectName(QString::fromUtf8("SpyWidget"));
        SpyWidget->resize(400, 98);
        SpyWidget->setAutoFillBackground(true);
        verticalLayout = new QVBoxLayout(SpyWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(SpyWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(15);
        label->setFont(font);

        verticalLayout->addWidget(label);

        disableSpyButton = new QRadioButton(SpyWidget);
        disableSpyButton->setObjectName(QString::fromUtf8("disableSpyButton"));

        verticalLayout->addWidget(disableSpyButton);

        spyOnTurnRestrictionButton = new QRadioButton(SpyWidget);
        spyOnTurnRestrictionButton->setObjectName(QString::fromUtf8("spyOnTurnRestrictionButton"));

        verticalLayout->addWidget(spyOnTurnRestrictionButton);


        retranslateUi(SpyWidget);

        QMetaObject::connectSlotsByName(SpyWidget);
    } // setupUi

    void retranslateUi(QWidget *SpyWidget)
    {
        SpyWidget->setWindowTitle(QCoreApplication::translate("SpyWidget", "Form", nullptr));
        label->setText(QCoreApplication::translate("SpyWidget", "Spy", nullptr));
        disableSpyButton->setText(QCoreApplication::translate("SpyWidget", "Disable Spy", nullptr));
        spyOnTurnRestrictionButton->setText(QCoreApplication::translate("SpyWidget", "Spy on nodes with turn restrictions", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SpyWidget: public Ui_SpyWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPYWIDGET_H
