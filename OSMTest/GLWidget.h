#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "RenderingManager.h"

#include <QOpenGLWidget>
#include <QVector3D>
#include <glm/glm.hpp>

class GLWidget : public QOpenGLWidget
{
	Q_OBJECT

private:
	QPoint lastPos;
	QVector3D eyePosition;
	QVector3D rotation;
	QVector3D translation;
	glm::vec2 offsetTranslation;
	RenderingManager* renderingManager;
	float fov;

	// key status
	bool shiftPressed;
	bool ctrlPressed;

public:
	explicit GLWidget(QWidget *parent = 0);
	~GLWidget();

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;
	void rotateBy(const QVector3D& rotationAngle);
	void translateBy(float x, float y);
	void setCamera(double latitude, double longitude, float orientation, float fov);

	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

public:
	void loadOSM(const QString& filename);
};

#endif