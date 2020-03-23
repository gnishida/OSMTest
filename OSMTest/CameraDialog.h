#pragma once

#include <QDialog>
#include "ui_CameraDialog.h"

class CameraDialog : public QDialog {
	Q_OBJECT

private:
	Ui::CameraDialog ui;

public:
	CameraDialog(QWidget *parent = 0);
	~CameraDialog();
	
	void setLatitude(double latitude);
	double getLatitude();
	void setLongitude(double longitude);
	double getLongitude();
	void setOrientation(float orientation);
	float getOrientation();
	void setFOV(float fov);
	float getFOV();

public slots:
	void onOK();
	void onCancel();
};

