/********************************************************************************
** Form generated from reading UI file 'newlabeldialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWLABELDIALOG_H
#define UI_NEWLABELDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_NewLabelDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_3;
    QLineEdit *nameBox;
    QLabel *label_2;
    QComboBox *typeBox;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *NewLabelDialog)
    {
        if (NewLabelDialog->objectName().isEmpty())
            NewLabelDialog->setObjectName(QString::fromUtf8("NewLabelDialog"));
        NewLabelDialog->resize(400, 133);
        gridLayout = new QGridLayout(NewLabelDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(NewLabelDialog);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(15);
        label->setFont(font);

        gridLayout->addWidget(label, 1, 1, 1, 2);

        label_3 = new QLabel(NewLabelDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 3, 1, 1, 1);

        nameBox = new QLineEdit(NewLabelDialog);
        nameBox->setObjectName(QString::fromUtf8("nameBox"));

        gridLayout->addWidget(nameBox, 2, 2, 1, 1);

        label_2 = new QLabel(NewLabelDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 1, 1, 1);

        typeBox = new QComboBox(NewLabelDialog);
        typeBox->setObjectName(QString::fromUtf8("typeBox"));

        gridLayout->addWidget(typeBox, 3, 2, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        cancelButton = new QPushButton(NewLabelDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout_2->addWidget(cancelButton);

        okButton = new QPushButton(NewLabelDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        horizontalLayout_2->addWidget(okButton);


        gridLayout->addLayout(horizontalLayout_2, 4, 1, 1, 2);


        retranslateUi(NewLabelDialog);

        okButton->setDefault(true);


        QMetaObject::connectSlotsByName(NewLabelDialog);
    } // setupUi

    void retranslateUi(QDialog *NewLabelDialog)
    {
        NewLabelDialog->setWindowTitle(QCoreApplication::translate("NewLabelDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("NewLabelDialog", "New Label", nullptr));
        label_3->setText(QCoreApplication::translate("NewLabelDialog", "Type", nullptr));
        label_2->setText(QCoreApplication::translate("NewLabelDialog", "Label", nullptr));
        cancelButton->setText(QCoreApplication::translate("NewLabelDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("NewLabelDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewLabelDialog: public Ui_NewLabelDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWLABELDIALOG_H
