#include "GLWidget.h"
#include <QMouseEvent>
#include <iostream>
#include "AssetUtils.h"
#include "OSMImporter.h"
#include <algorithm>
#include <stdlib.h>

GLWidget::GLWidget(QWidget *parent)
{
	renderingManager = nullptr;
	eyePosition = QVector3D(0, 0, 100);
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
	proj.perspective(60.0, width() / height(), 0.1, 10000.0);

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
		rotateBy(QVector3D(dy, 0, dx));
	}

	lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
	eyePosition.setZ(eyePosition.z() - event->delta() * 0.1);
	update();
}

void GLWidget::loadOSM(const QString& filename)
{
	renderingManager->removeObjects();

	float minX, minY, maxX, maxY;
	std::vector<BuildingParam> buildingParams;
	OSMImporter::import(filename, minX, minY, maxX, maxY, buildingParams);

	float translate_x = (minX + maxX) / 2;
	float translate_y = (minY + maxY) / 2;

	for (auto& buildingParam : buildingParams) {
		for (auto& coord : buildingParam.footprint) {
			coord.x = coord.x - translate_x;
			coord.y = coord.y - translate_y;
		}
	}
	minX -= translate_x;
	minY -= translate_y;
	maxX -= translate_x;
	maxY -= translate_y;

	renderingManager->addObject("images/shin_urayasu.jpg", AssetUtils::createRectangle(maxX - minX, maxY - minY));

	for (const auto& buildingParam : buildingParams) {
		int random = rand() % 7;
		if (random == 0) {
			renderingManager->addObject("images/facade.jpg", AssetUtils::createPrism(buildingParam.footprint, buildingParam.height));
		}
		else if (random == 1) {
			renderingManager->addObject("images/facade2.jpg", AssetUtils::createPrism(buildingParam.footprint, buildingParam.height));
		}
		else if (random == 2) {
			renderingManager->addObject("images/facade3.jpg", AssetUtils::createPrism(buildingParam.footprint, buildingParam.height));
		}
		else if (random == 3) {
			renderingManager->addObject("images/facade4.jpg", AssetUtils::createPrism(buildingParam.footprint, buildingParam.height));
		}
		else if (random == 4) {
			renderingManager->addObject("images/facade5.jpg", AssetUtils::createPrism(buildingParam.footprint, buildingParam.height));
		}
		else if (random == 5) {
			renderingManager->addObject("images/facade6.jpg", AssetUtils::createPrism(buildingParam.footprint, buildingParam.height));
		}
		else {
			renderingManager->addObject("images/facade3.jpg", AssetUtils::createPrism(buildingParam.footprint, buildingParam.height));
		}

		renderingManager->addObject("images/shin_urayasu.jpg", AssetUtils::createPolygon(buildingParam.footprint, buildingParam.height, minX, minY, maxX, maxY));
	}
	update();
}