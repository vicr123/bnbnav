/********************************************************************************
** Form generated from reading UI file 'turnrestrictioneditor.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TURNRESTRICTIONEDITOR_H
#define UI_TURNRESTRICTIONEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_TurnRestrictionEditor
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *editOtherEdgeButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *applyButton;
    QPushButton *clearTurnRestrictionsButton;

    void setupUi(QDialog *TurnRestrictionEditor)
    {
        if (TurnRestrictionEditor->objectName().isEmpty())
            TurnRestrictionEditor->setObjectName(QString::fromUtf8("TurnRestrictionEditor"));
        TurnRestrictionEditor->resize(400, 300);
        gridLayout = new QGridLayout(TurnRestrictionEditor);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 0, 1, 1);

        editOtherEdgeButton = new QPushButton(TurnRestrictionEditor);
        editOtherEdgeButton->setObjectName(QString::fromUtf8("editOtherEdgeButton"));

        gridLayout->addWidget(editOtherEdgeButton, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        applyButton = new QPushButton(TurnRestrictionEditor);
        applyButton->setObjectName(QString::fromUtf8("applyButton"));

        gridLayout->addWidget(applyButton, 2, 2, 1, 1);

        clearTurnRestrictionsButton = new QPushButton(TurnRestrictionEditor);
        clearTurnRestrictionsButton->setObjectName(QString::fromUtf8("clearTurnRestrictionsButton"));

        gridLayout->addWidget(clearTurnRestrictionsButton, 2, 0, 1, 1);


        retranslateUi(TurnRestrictionEditor);

        QMetaObject::connectSlotsByName(TurnRestrictionEditor);
    } // setupUi

    void retranslateUi(QDialog *TurnRestrictionEditor)
    {
        TurnRestrictionEditor->setWindowTitle(QCoreApplication::translate("TurnRestrictionEditor", "Turn Restrictions", nullptr));
        editOtherEdgeButton->setText(QCoreApplication::translate("TurnRestrictionEditor", "Edit a different edge", nullptr));
        applyButton->setText(QCoreApplication::translate("TurnRestrictionEditor", "Apply Changes", nullptr));
        clearTurnRestrictionsButton->setText(QCoreApplication::translate("TurnRestrictionEditor", "Clear All", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TurnRestrictionEditor: public Ui_TurnRestrictionEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TURNRESTRICTIONEDITOR_H
