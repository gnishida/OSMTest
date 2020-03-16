#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	glWidget.rotateBy(QVector3D(42, 42, -21));
	this->setCentralWidget(&glWidget);

	setWindowTitle(tr("OSM Test"));

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
}
