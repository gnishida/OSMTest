#pragma once

#include <QtWidgets/QMainWindow>
#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QKeyEvent>

#include "GLWidget.h"
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

protected:
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

public slots:
	void onOpen();
	void onCamera();

private:
	Ui::MainWindowClass ui;
	GLWidget glWidget;
};
