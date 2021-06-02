/********************************************************************************
** Form generated from reading UI file 'newlandmarkdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWLANDMARKDIALOG_H
#define UI_NEWLANDMARKDIALOG_H

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

class Ui_NewLandmarkDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *nameBox;
    QLabel *label_3;
    QComboBox *typeBox;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *NewLandmarkDialog)
    {
        if (NewLandmarkDialog->objectName().isEmpty())
            NewLandmarkDialog->setObjectName(QString::fromUtf8("NewLandmarkDialog"));
        NewLandmarkDialog->resize(400, 144);
        gridLayout = new QGridLayout(NewLandmarkDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(NewLandmarkDialog);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(15);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 2);

        label_2 = new QLabel(NewLandmarkDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        nameBox = new QLineEdit(NewLandmarkDialog);
        nameBox->setObjectName(QString::fromUtf8("nameBox"));

        gridLayout->addWidget(nameBox, 1, 1, 1, 1);

        label_3 = new QLabel(NewLandmarkDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        typeBox = new QComboBox(NewLandmarkDialog);
        typeBox->setObjectName(QString::fromUtf8("typeBox"));

        gridLayout->addWidget(typeBox, 2, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancelButton = new QPushButton(NewLandmarkDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout->addWidget(cancelButton);

        okButton = new QPushButton(NewLandmarkDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        horizontalLayout->addWidget(okButton);


        gridLayout->addLayout(horizontalLayout, 3, 0, 1, 2);


        retranslateUi(NewLandmarkDialog);

        okButton->setDefault(true);


        QMetaObject::connectSlotsByName(NewLandmarkDialog);
    } // setupUi

    void retranslateUi(QDialog *NewLandmarkDialog)
    {
        NewLandmarkDialog->setWindowTitle(QCoreApplication::translate("NewLandmarkDialog", "New Landmark", nullptr));
        label->setText(QCoreApplication::translate("NewLandmarkDialog", "New Landmark", nullptr));
        label_2->setText(QCoreApplication::translate("NewLandmarkDialog", "Name", nullptr));
        label_3->setText(QCoreApplication::translate("NewLandmarkDialog", "Type", nullptr));
        cancelButton->setText(QCoreApplication::translate("NewLandmarkDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("NewLandmarkDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewLandmarkDialog: public Ui_NewLandmarkDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWLANDMARKDIALOG_H
