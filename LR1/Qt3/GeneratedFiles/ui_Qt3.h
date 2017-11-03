/********************************************************************************
** Form generated from reading UI file 'Qt3.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT3_H
#define UI_QT3_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Qt3Class
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QListView *normal_list;
    QLineEdit *string_in;
    QPushButton *pushButton;
    QTableView *process_tab;
    QTableView *parse_tab;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Qt3Class)
    {
        if (Qt3Class->objectName().isEmpty())
            Qt3Class->setObjectName(QStringLiteral("Qt3Class"));
        Qt3Class->resize(1388, 749);
        centralWidget = new QWidget(Qt3Class);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        normal_list = new QListView(centralWidget);
        normal_list->setObjectName(QStringLiteral("normal_list"));

        gridLayout->addWidget(normal_list, 0, 0, 2, 1);

        string_in = new QLineEdit(centralWidget);
        string_in->setObjectName(QStringLiteral("string_in"));

        gridLayout->addWidget(string_in, 0, 1, 1, 1);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 0, 2, 1, 1);

        process_tab = new QTableView(centralWidget);
        process_tab->setObjectName(QStringLiteral("process_tab"));

        gridLayout->addWidget(process_tab, 0, 3, 2, 1);

        parse_tab = new QTableView(centralWidget);
        parse_tab->setObjectName(QStringLiteral("parse_tab"));

        gridLayout->addWidget(parse_tab, 1, 1, 1, 2);

        Qt3Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Qt3Class);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1388, 23));
        Qt3Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Qt3Class);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Qt3Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Qt3Class);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Qt3Class->setStatusBar(statusBar);

        retranslateUi(Qt3Class);

        QMetaObject::connectSlotsByName(Qt3Class);
    } // setupUi

    void retranslateUi(QMainWindow *Qt3Class)
    {
        Qt3Class->setWindowTitle(QApplication::translate("Qt3Class", "Qt3", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Qt3Class", "PushButton", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Qt3Class: public Ui_Qt3Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT3_H
