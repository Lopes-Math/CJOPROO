/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *usernameLabel;
    QLineEdit *usernameLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QLabel *errorLabel;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(400, 250);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName("actionQuit");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        usernameLabel = new QLabel(centralwidget);
        usernameLabel->setObjectName("usernameLabel");

        verticalLayout->addWidget(usernameLabel);

        usernameLineEdit = new QLineEdit(centralwidget);
        usernameLineEdit->setObjectName("usernameLineEdit");

        verticalLayout->addWidget(usernameLineEdit);

        passwordLabel = new QLabel(centralwidget);
        passwordLabel->setObjectName("passwordLabel");

        verticalLayout->addWidget(passwordLabel);

        passwordLineEdit = new QLineEdit(centralwidget);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setEchoMode(QLineEdit::EchoMode::Password);

        verticalLayout->addWidget(passwordLineEdit);

        loginButton = new QPushButton(centralwidget);
        loginButton->setObjectName("loginButton");

        verticalLayout->addWidget(loginButton);

        errorLabel = new QLabel(centralwidget);
        errorLabel->setObjectName("errorLabel");

        verticalLayout->addWidget(errorLabel);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Login", nullptr));
        usernameLabel->setText(QCoreApplication::translate("MainWindow", "Usu\303\241rio", nullptr));
        usernameLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"         QLabel {\n"
"           font-size: 18px;\n"
"           font-weight: bold;\n"
"           color: #FF66B2;  \n"
"         }\n"
"       ", nullptr));
        usernameLineEdit->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"         QLineEdit {\n"
"           border: 2px solid #FF66B2;\n"
"           border-radius: 10px;\n"
"           padding: 10px;\n"
"           font-size: 16px;  \n"
"           color: #FF66B2;   \n"
"         }\n"
"         QLineEdit:focus {\n"
"           border: 2px solid #FF3399;\n"
"         }\n"
"       ", nullptr));
        passwordLabel->setText(QCoreApplication::translate("MainWindow", "Senha", nullptr));
        passwordLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"         QLabel {\n"
"           font-size: 18px;\n"
"           font-weight: bold;\n"
"           color: #FF66B2;  \n"
"         }\n"
"       ", nullptr));
        passwordLineEdit->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"         QLineEdit {\n"
"           border: 2px solid #FF66B2;\n"
"           border-radius: 10px;\n"
"           padding: 10px;\n"
"           font-size: 16px;  \n"
"           color: #FF66B2;   \n"
"         }\n"
"         QLineEdit:focus {\n"
"           border: 2px solid #FF3399;\n"
"         }\n"
"       ", nullptr));
        loginButton->setText(QCoreApplication::translate("MainWindow", "Entrar", nullptr));
        loginButton->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"         QPushButton {\n"
"           background-color: #FF66B2;\n"
"           color: white;\n"
"           border-radius: 15px;\n"
"           border: 2px solid #FF3399;\n"
"           padding: 10px 20px;\n"
"           font-size: 16px;\n"
"         }\n"
"         QPushButton:hover {\n"
"           background-color: #FF3399;\n"
"           border-color: #FF66B2;\n"
"         }\n"
"       ", nullptr));
        errorLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"         QLabel {\n"
"           color: red;\n"
"           font-size: 16px;   \n"
"           font-weight: bold;\n"
"         }\n"
"       ", nullptr));
        errorLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
