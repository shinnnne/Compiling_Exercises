#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Qt3.h"
#include<QLineEdit>
#include<qtableview.h>
#include<qstandarditemmodel.h>

class Qt3 : public QMainWindow
{
	Q_OBJECT

public:
	Qt3(QWidget *parent = Q_NULLPTR);
	Ui::Qt3Class ui;
private:
	
};
