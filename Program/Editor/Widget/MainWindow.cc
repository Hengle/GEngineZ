#include "MainWindow.h"
#include "D3DWidget.h"

#include "../QtUI/ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow) {
	ui->setupUi(this);


	z::D3DWidget* tab_2 = new z::D3DWidget(this, 1);
	tab_2->setObjectName(QString::fromUtf8("tab_2"));
	ui->tabWidget->addTab(tab_2, QString());

	//setWidget
	z::D3DWidget* tab_3 = new z::D3DWidget(this, 2);
	tab_3->setObjectName(QString::fromUtf8("tab_3"));
	ui->tabWidget->addTab(tab_3, QString());

}

MainWindow::~MainWindow()
{
	delete ui;
}