#pragma once

#include <QtWidgets/QMainWindow>
#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>

#include "GLWidget.h"
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

private:
	Ui::MainWindowClass ui;
	GLWidget glWidget;
};
