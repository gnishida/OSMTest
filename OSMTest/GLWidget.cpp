#include "GLWidget.h"
#include <QMouseEvent>
#include <iostream>
#include "AssetUtils.h"
#include "OSMImporter.h"
#include <algorithm>
#include <stdlib.h>
#include <iostream>

GLWidget::GLWidget(QWidget *parent)
{
	renderingManager = nullptr;
	eyePosition = QVector3D(0.0f, 3.0f, 500.0f);
	lookAtPosition.setY(3.0f);

	fov = 90.0f;
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

void GLWidget::translateBy(float dx, float dy)
{
	const float theta = glm::radians(rotation.z());
	const float scale = 2 * std::max(10.0f, eyePosition.z()) * std::tan(glm::radians(fov) / 2) / width();
	float tmpx = dx * std::cos(theta) + dy * std::sin(theta);
	float tmpy = -dx * std::sin(theta) + dy * std::cos(theta);
	tmpx *= scale;
	tmpy *= scale;
	translation.setX(translation.x() + tmpx);
	translation.setY(translation.y() + tmpy);
	update();
}

void GLWidget::setCamera(double latitude, double longitude, float orientation, float fov)
{
	glm::vec2 cameraPos = OSMImporter::convertLatLonToUTM(latitude, longitude) - offsetTranslation;
	translation.setX(-cameraPos.x);
	translation.setY(-cameraPos.y);

	eyePosition.setZ(0.1f);
	rotation = QVector3D(-90.0f, 0, orientation);
	this->fov = fov;

	update();
}

void GLWidget::keyPressEvent(QKeyEvent *e) {
	ctrlPressed = false;
	shiftPressed = false;

	if (e->modifiers() & Qt::ControlModifier) {
		ctrlPressed = true;
	}
	if (e->modifiers() & Qt::ShiftModifier) {
		shiftPressed = true;
	}

	switch (e->key()) {
	default:
		break;
	}
}

void GLWidget::keyReleaseEvent(QKeyEvent* e) {
	switch (e->key()) {
	case Qt::Key_Control:
		ctrlPressed = false;
		break;
	case Qt::Key_Shift:
		shiftPressed = false;
		break;
	default:
		break;
	}
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
	model.translate(translation.x(), translation.y(), translation.z());

	QMatrix4x4 view;
	view.lookAt(eyePosition, lookAtPosition, QVector3D(0.0, 1.0, 0.0));

	QMatrix4x4 proj;
	proj.perspective(fov, (float)width() / height(), 0.1, 10000.0);

	QVector4D result = proj * view * model * QVector4D(buildingVertex, 1);
	glm::vec3 result2(result.x() / result.w(), result.y() / result.w(), result.z() / result.w());

	std::cout << (1 + result2.x) * width() / 2 << ", " << (1 - result2.y) * height() / 2 << std::endl;

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
		if (ctrlPressed) {
			rotateBy(QVector3D(dy, 0, dx));
		}
		else {
			translateBy(dx, -dy);
		}
	}

	lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
	float newZ = std::max(1.0f, eyePosition.z() - event->delta() * eyePosition.z() * 0.0006f);
	eyePosition.setZ(newZ);
	update();
}

void GLWidget::loadOSM(const QString& filename)
{
	renderingManager->removeObjects();

	float minX, minY, maxX, maxY;
	std::vector<BuildingParam> buildingParams;
	OSMImporter::import(filename, minX, minY, maxX, maxY, buildingParams);

	offsetTranslation.x = (minX + maxX) / 2;
	offsetTranslation.y = (minY + maxY) / 2;

	for (auto& buildingParam : buildingParams) {
		for (auto& coord : buildingParam.footprint) {
			coord.x = coord.x - offsetTranslation.x;
			coord.y = coord.y - offsetTranslation.y;
		}
	}
	minX -= offsetTranslation.x;
	minY -= offsetTranslation.y;
	maxX -= offsetTranslation.x;
	maxY -= offsetTranslation.y;



	glm::vec2 test = OSMImporter::convertLatLonToUTM(35.645401, 139.929761);
	test -= offsetTranslation;
	int b_ind = -1;
	int v_ind = -1;
	float minimum = std::numeric_limits<float>::max();
	for (int i = 0; i < buildingParams.size(); i++) {
		for (int j = 0; j < buildingParams[i].footprint.size(); j++) {
			float dist = glm::length(buildingParams[i].footprint[j] - test);
			if (dist < minimum) {
				b_ind = i;
				v_ind = j;
				minimum = dist;
			}
		}
	}

	buildingVertex = QVector3D(buildingParams[b_ind].footprint[v_ind].x, buildingParams[b_ind].footprint[v_ind].y, 0);

	renderingManager->addObject("images/shin_urayasu.jpg", AssetUtils::createRectangle(maxX - minX, maxY - minY, 0));

	for (const auto& buildingParam : buildingParams) {
		int random = rand() % 7;
		if (random == 0) {
			renderingManager->addObject("images/facade.jpg", AssetUtils::createPrism(buildingParam.footprint, 0, buildingParam.height));
		}
		else if (random == 1) {
			renderingManager->addObject("images/facade2.jpg", AssetUtils::createPrism(buildingParam.footprint, 0, buildingParam.height));
		}
		else if (random == 2) {
			renderingManager->addObject("images/facade3.jpg", AssetUtils::createPrism(buildingParam.footprint, 0, buildingParam.height));
		}
		else if (random == 3) {
			renderingManager->addObject("images/facade4.jpg", AssetUtils::createPrism(buildingParam.footprint, 0, buildingParam.height));
		}
		else if (random == 4) {
			renderingManager->addObject("images/facade5.jpg", AssetUtils::createPrism(buildingParam.footprint, 0, buildingParam.height));
		}
		else if (random == 5) {
			renderingManager->addObject("images/facade6.jpg", AssetUtils::createPrism(buildingParam.footprint, 0, buildingParam.height));
		}
		else {
			renderingManager->addObject("images/facade7.jpg", AssetUtils::createPrism(buildingParam.footprint, 0, buildingParam.height));
		}

		renderingManager->addObject("images/shin_urayasu.jpg", AssetUtils::createPolygon(buildingParam.footprint, buildingParam.height, minX, minY, maxX, maxY));
	}

	renderingManager->addObject("images/red.jpg", AssetUtils::createSphere(buildingVertex.x(), buildingVertex.y(), buildingVertex.z(), 1));
	

	update();
}