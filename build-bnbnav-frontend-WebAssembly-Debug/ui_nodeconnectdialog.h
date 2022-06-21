/********************************************************************************
** Form generated from reading UI file 'nodeconnectdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NODECONNECTDIALOG_H
#define UI_NODECONNECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_NodeConnectDialog
{
public:
    QGridLayout *gridLayout;
    QComboBox *roadSelectionBox;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QLabel *addDirective;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *okButton;
    QLabel *label;
    QPushButton *newRoadButton;
    QCheckBox *twoWayBox;

    void setupUi(QDialog *NodeConnectDialog)
    {
        if (NodeConnectDialog->objectName().isEmpty())
            NodeConnectDialog->setObjectName(QString::fromUtf8("NodeConnectDialog"));
        NodeConnectDialog->resize(400, 175);
        NodeConnectDialog->setWindowTitle(QString::fromUtf8("New Edge"));
        gridLayout = new QGridLayout(NodeConnectDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        roadSelectionBox = new QComboBox(NodeConnectDialog);
        roadSelectionBox->setObjectName(QString::fromUtf8("roadSelectionBox"));

        gridLayout->addWidget(roadSelectionBox, 1, 1, 1, 1);

        label_2 = new QLabel(NodeConnectDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        addDirective = new QLabel(NodeConnectDialog);
        addDirective->setObjectName(QString::fromUtf8("addDirective"));

        horizontalLayout->addWidget(addDirective);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancelButton = new QPushButton(NodeConnectDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setText(QString::fromUtf8("Cancel"));

        horizontalLayout->addWidget(cancelButton);

        okButton = new QPushButton(NodeConnectDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setText(QString::fromUtf8("OK"));

        horizontalLayout->addWidget(okButton);


        gridLayout->addLayout(horizontalLayout, 4, 0, 1, 2);

        label = new QLabel(NodeConnectDialog);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(15);
        label->setFont(font);
        label->setText(QString::fromUtf8("New Edge"));

        gridLayout->addWidget(label, 0, 0, 1, 2);

        newRoadButton = new QPushButton(NodeConnectDialog);
        newRoadButton->setObjectName(QString::fromUtf8("newRoadButton"));

        gridLayout->addWidget(newRoadButton, 2, 1, 1, 1);

        twoWayBox = new QCheckBox(NodeConnectDialog);
        twoWayBox->setObjectName(QString::fromUtf8("twoWayBox"));
        twoWayBox->setChecked(true);

        gridLayout->addWidget(twoWayBox, 3, 0, 1, 2);


        retranslateUi(NodeConnectDialog);

        okButton->setDefault(true);


        QMetaObject::connectSlotsByName(NodeConnectDialog);
    } // setupUi

    void retranslateUi(QDialog *NodeConnectDialog)
    {
        label_2->setText(QCoreApplication::translate("NodeConnectDialog", "Road", nullptr));
        addDirective->setText(QCoreApplication::translate("NodeConnectDialog", "Use /addroadnode to add edges", nullptr));
        newRoadButton->setText(QCoreApplication::translate("NodeConnectDialog", "Create New Road", nullptr));
        twoWayBox->setText(QCoreApplication::translate("NodeConnectDialog", "Two Way", nullptr));
        (void)NodeConnectDialog;
    } // retranslateUi

};

namespace Ui {
    class NodeConnectDialog: public Ui_NodeConnectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NODECONNECTDIALOG_H
