#include "MainWindow.h"
#include "CameraDialog.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	this->setCentralWidget(&glWidget);

	setWindowTitle(tr("OSM Test"));

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionCamera, SIGNAL(triggered()), this, SLOT(onCamera()));
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
	glWidget.keyPressEvent(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent* e) {
	glWidget.keyReleaseEvent(e);
}

void MainWindow::onOpen()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open OSM file..."), "", tr("OSM Files (*.osm)"));
	if (filename.isEmpty()) return;

	glWidget.loadOSM(filename);
}

void MainWindow::onCamera()
{
	CameraDialog dlg;
	dlg.setLatitude(35.64516023062877);
	dlg.setLongitude(139.9296517394975);
	dlg.setOrientation(38.48);
	dlg.setFOV(90.0f);

	if (dlg.exec()) {
		glWidget.setCamera(dlg.getLatitude(), dlg.getLongitude(), dlg.getOrientation(), dlg.getFOV());
	}
}