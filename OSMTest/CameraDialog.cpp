#include "CameraDialog.h"

CameraDialog::CameraDialog(QWidget *parent) : QDialog(parent) {
	ui.setupUi(this);

	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(onCancel()));
}

CameraDialog::~CameraDialog() {
}


void CameraDialog::setLatitude(double latitude) {
	ui.lineEditLatitude->setText(QString::number(latitude, 'f', 14));
}

double CameraDialog::getLatitude() {
	return ui.lineEditLatitude->text().toDouble();
}

void CameraDialog::setLongitude(double longitude) {
	ui.lineEditLongitude->setText(QString::number(longitude, 'f', 14));
}

double CameraDialog::getLongitude() {
	return ui.lineEditLongitude->text().toDouble();
}

void CameraDialog::setOrientation(float orientation) {
	ui.lineEditOrientation->setText(QString::number(orientation));
}

float CameraDialog::getOrientation() {
	return ui.lineEditOrientation->text().toFloat();
}

void CameraDialog::setFOV(float fov) {
	ui.lineEditFOV->setText(QString::number(fov));
}

float CameraDialog::getFOV() {
	return ui.lineEditFOV->text().toFloat();
}

void CameraDialog::onOK() {
	accept();
}

void CameraDialog::onCancel() {
	reject();
}