/********************************************************************************
** Form generated from reading UI file 'newroaddialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWROADDIALOG_H
#define UI_NEWROADDIALOG_H

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

class Ui_NewRoadDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *okButton;
    QLineEdit *nameBox;
    QLabel *label_3;
    QComboBox *typeBox;

    void setupUi(QDialog *NewRoadDialog)
    {
        if (NewRoadDialog->objectName().isEmpty())
            NewRoadDialog->setObjectName(QString::fromUtf8("NewRoadDialog"));
        NewRoadDialog->resize(400, 136);
        gridLayout = new QGridLayout(NewRoadDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(NewRoadDialog);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(15);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 2);

        label_2 = new QLabel(NewRoadDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancelButton = new QPushButton(NewRoadDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout->addWidget(cancelButton);

        okButton = new QPushButton(NewRoadDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        horizontalLayout->addWidget(okButton);


        gridLayout->addLayout(horizontalLayout, 3, 0, 1, 2);

        nameBox = new QLineEdit(NewRoadDialog);
        nameBox->setObjectName(QString::fromUtf8("nameBox"));

        gridLayout->addWidget(nameBox, 1, 1, 1, 1);

        label_3 = new QLabel(NewRoadDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        typeBox = new QComboBox(NewRoadDialog);
        typeBox->setObjectName(QString::fromUtf8("typeBox"));

        gridLayout->addWidget(typeBox, 2, 1, 1, 1);


        retranslateUi(NewRoadDialog);

        QMetaObject::connectSlotsByName(NewRoadDialog);
    } // setupUi

    void retranslateUi(QDialog *NewRoadDialog)
    {
        NewRoadDialog->setWindowTitle(QCoreApplication::translate("NewRoadDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("NewRoadDialog", "New Road", nullptr));
        label_2->setText(QCoreApplication::translate("NewRoadDialog", "Name", nullptr));
        cancelButton->setText(QCoreApplication::translate("NewRoadDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("NewRoadDialog", "OK", nullptr));
        label_3->setText(QCoreApplication::translate("NewRoadDialog", "Type", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewRoadDialog: public Ui_NewRoadDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWROADDIALOG_H
