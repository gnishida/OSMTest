#include "GLWidget.h"
#include <QMouseEvent>
#include <iostream>
#include "AssetUtils.h"

GLWidget::GLWidget(QWidget *parent)
{
	renderingManager = nullptr;
	eyePosition = QVector3D(0, 0, 3);
}

GLWidget::~GLWidget()
{
	makeCurrent();
	if (renderingManager) {
		delete renderingManager;
		renderingManager = nullptr;
	}
	doneCurrent();
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
	return QSize(200, 200);
}

void GLWidget::rotateBy(const QVector3D& rotationAngle)
{
	rotation += rotationAngle;
	update();
}

void GLWidget::initializeGL()
{
	renderingManager = new RenderingManager();
	makeObject();
}

void GLWidget::paintGL()
{
	QMatrix4x4 model;
	model.rotate(rotation.x(), 1.0f, 0.0f, 0.0f);
	model.rotate(rotation.y(), 0.0f, 1.0f, 0.0f);
	model.rotate(rotation.z(), 0.0f, 0.0f, 1.0f);

	QMatrix4x4 view;
	view.lookAt(eyePosition, QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 1.0, 0.0));

	QMatrix4x4 proj;
	proj.perspective(60.0, width() / height(), 0.1, 100.0);

	renderingManager->render(proj * view * model);
}

void GLWidget::resizeGL(int width, int height)
{
	renderingManager->setViewport(width, height);
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
	lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		rotateBy(QVector3D(dy, dx, 0));
	}
	else if (event->buttons() & Qt::RightButton) {
		rotateBy(QVector3D(dy, 0, dx));
	}
	lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
	eyePosition.setZ(eyePosition.z() - event->delta() * 0.001);
	update();
}

void GLWidget::makeObject()
{
	renderingManager->addObject("images/side1.png", AssetUtils::createRectangle(0.5, 0.5));
	renderingManager->addObject("images/earth.jpg", AssetUtils::createSphere(0.35));
}