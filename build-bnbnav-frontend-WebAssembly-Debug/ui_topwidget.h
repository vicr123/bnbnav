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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TopWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *editModeButton;

    void setupUi(QWidget *TopWidget)
    {
        if (TopWidget->objectName().isEmpty())
            TopWidget->setObjectName(QString::fromUtf8("TopWidget"));
        TopWidget->resize(400, 94);
        horizontalLayout = new QHBoxLayout(TopWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(293, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        editModeButton = new QPushButton(TopWidget);
        editModeButton->setObjectName(QString::fromUtf8("editModeButton"));
        editModeButton->setCheckable(true);

        horizontalLayout->addWidget(editModeButton);


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
