#pragma once

#include "BuildingParam.h"
#include <QString>

class OSMImporter {
public:
	OSMImporter();
	~OSMImporter();

	static void import(const QString& filename, float& minX, float& minY, float& maxX, float& maxY, std::vector<BuildingParam>& buildingParams);
	static glm::vec2 convertLatLonToUTM(double lat, double lon, double center_lat, double center_lon);
};

