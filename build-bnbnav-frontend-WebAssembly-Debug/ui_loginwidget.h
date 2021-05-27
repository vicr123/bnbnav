/********************************************************************************
** Form generated from reading UI file 'loginwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWIDGET_H
#define UI_LOGINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWidget
{
public:
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *loginPage;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *loginUsernameBox;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *loginButton;
    QWidget *userPage;
    QVBoxLayout *verticalLayout_2;
    QLabel *userPageTitle;
    QPushButton *followMeButton;
    QPushButton *logOutButton;

    void setupUi(QWidget *LoginWidget)
    {
        if (LoginWidget->objectName().isEmpty())
            LoginWidget->setObjectName(QString::fromUtf8("LoginWidget"));
        LoginWidget->resize(400, 111);
        LoginWidget->setWindowTitle(QString::fromUtf8("Form"));
        LoginWidget->setAutoFillBackground(true);
        verticalLayout = new QVBoxLayout(LoginWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(LoginWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        loginPage = new QWidget();
        loginPage->setObjectName(QString::fromUtf8("loginPage"));
        gridLayout = new QGridLayout(loginPage);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(loginPage);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(15);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 2);

        loginUsernameBox = new QLineEdit(loginPage);
        loginUsernameBox->setObjectName(QString::fromUtf8("loginUsernameBox"));

        gridLayout->addWidget(loginUsernameBox, 1, 1, 1, 1);

        label_2 = new QLabel(loginPage);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        loginButton = new QPushButton(loginPage);
        loginButton->setObjectName(QString::fromUtf8("loginButton"));

        horizontalLayout->addWidget(loginButton);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 2);

        stackedWidget->addWidget(loginPage);
        userPage = new QWidget();
        userPage->setObjectName(QString::fromUtf8("userPage"));
        verticalLayout_2 = new QVBoxLayout(userPage);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        userPageTitle = new QLabel(userPage);
        userPageTitle->setObjectName(QString::fromUtf8("userPageTitle"));
        userPageTitle->setFont(font);

        verticalLayout_2->addWidget(userPageTitle);

        followMeButton = new QPushButton(userPage);
        followMeButton->setObjectName(QString::fromUtf8("followMeButton"));
        followMeButton->setCheckable(true);

        verticalLayout_2->addWidget(followMeButton);

        logOutButton = new QPushButton(userPage);
        logOutButton->setObjectName(QString::fromUtf8("logOutButton"));

        verticalLayout_2->addWidget(logOutButton);

        stackedWidget->addWidget(userPage);

        verticalLayout->addWidget(stackedWidget);


        retranslateUi(LoginWidget);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(LoginWidget);
    } // setupUi

    void retranslateUi(QWidget *LoginWidget)
    {
        label->setText(QCoreApplication::translate("LoginWidget", "Log In", nullptr));
        label_2->setText(QCoreApplication::translate("LoginWidget", "Minecraft Username", nullptr));
        loginButton->setText(QCoreApplication::translate("LoginWidget", "Log In", nullptr));
        userPageTitle->setText(QCoreApplication::translate("LoginWidget", "Log In", nullptr));
        followMeButton->setText(QCoreApplication::translate("LoginWidget", "Follow Me", nullptr));
        logOutButton->setText(QCoreApplication::translate("LoginWidget", "Log Out", nullptr));
        (void)LoginWidget;
    } // retranslateUi

};

namespace Ui {
    class LoginWidget: public Ui_LoginWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWIDGET_H
