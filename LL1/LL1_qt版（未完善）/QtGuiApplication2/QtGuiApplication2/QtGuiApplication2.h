#pragma once



#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplication2.h"
#include<QWidget>
#include<QLineEdit>
#include<qtableview.h>
#include<qstandarditemmodel.h>

class QtGuiApplication2 : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiApplication2(QWidget *parent = Q_NULLPTR);

	Ui::QtGuiApplication2Class ui;
private:
};
