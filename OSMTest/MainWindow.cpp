#include "MainWindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	glWidget.rotateBy(QVector3D(42, 42, -21));
	this->setCentralWidget(&glWidget);

	setWindowTitle(tr("OSM Test"));

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
}

void MainWindow::onOpen()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open OSM file..."), "", tr("OSM Files (*.osm)"));
	if (filename.isEmpty()) return;

	glWidget.loadOSM(filename);
}