#pragma once

#include "BuildingParam.h"
#include <QString>

class OSMImporter {
public:
	OSMImporter();
	~OSMImporter();

	static void import(const QString& filename, std::vector<BuildingParam>& buildingParams);
	static glm::vec2 convertLatLonToUTM(double lat, double lon, double center_lat, double center_lon);
};

