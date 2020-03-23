#pragma once

#include "BuildingParam.h"
#include <QDomDocument>
#include <QString>

class OSMImporter {
public:
	OSMImporter();
	~OSMImporter();

	static void import(const QString& filename, float& minX, float& minY, float& maxX, float& maxY, std::vector<BuildingParam>& buildingParams);
	static void readTag(const QDomElement& element, QString& name, QString& type, QString& subtype, float& height);
	static glm::vec2 convertLatLonToUTM(double lat, double lon);
};

