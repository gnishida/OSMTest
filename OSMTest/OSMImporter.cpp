#include "OSMImporter.h"
#include <unordered_map>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <QFile>
#include <QDomDocument>
#include <string>
#include <iostream>

OSMImporter::OSMImporter() {
}

OSMImporter::~OSMImporter() {
}

void OSMImporter::import(const QString& filename, float& minX, float& minY, float& maxX, float& maxY, std::vector<BuildingParam>& buildingParams) {
	QDomDocument doc;
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file)) return;

	std::unordered_map<unsigned long long, glm::vec2> nodes;
	buildingParams.clear();

	double centerlat = 0;
	double centerlon = 0;

	QDomElement root = doc.documentElement();
	QDomElement element = root.firstChild().toElement();
	while (!element.isNull()) {
		if (element.tagName() == "bounds") {
			double minlat = element.attribute("minlat").toDouble();
			double maxlat = element.attribute("maxlat").toDouble();
			double minlon = element.attribute("minlon").toDouble();
			double maxlon = element.attribute("maxlon").toDouble();

			centerlat = (minlat + maxlat) * 0.5f;
			centerlon = (minlon + maxlon) * 0.5f;

			glm::vec2 minPt = convertLatLonToUTM(minlat, minlon, centerlat, centerlon);
			minX = minPt.x;
			minY = minPt.y;
			glm::vec2 maxPt = convertLatLonToUTM(maxlat, maxlon, centerlat, centerlon);
			maxX = maxPt.x;
			maxY = maxPt.y;
		}
		else if (element.tagName() == "node") {
			unsigned long long id = element.attribute("id").toULongLong();
			double lon = element.attribute("lon").toDouble();
			double lat = element.attribute("lat").toDouble();
			glm::vec2 pos = convertLatLonToUTM(lat, lon, centerlat, centerlon);
			if (minX > maxX) pos.x = -pos.x;
			if (minY > maxY) pos.y = -pos.y;
			nodes[id] = pos;
		}
		else if (element.tagName() == "way") {
			if (!element.hasAttribute("visible") || element.attribute("visible") == "true") {
				std::vector<int> nodeList;
				std::vector<glm::vec2> polygon;
				float height = 8.0f;
				QString name;
				QString type;
				QString subtype;

				QDomElement child_element = element.firstChild().toElement();
				while (!child_element.isNull()) {
					if (child_element.tagName() == "nd") {
						unsigned long long id = child_element.attribute("ref").toULongLong();
						polygon.push_back(nodes[id]);
					}
					else if (child_element.tagName() == "tag") {
						QString k = child_element.attribute("k");
						QString v = child_element.attribute("v");

						if (k == "name") {
							name = v;
						}
						else if (k == "building") {
							type = "building";
						}
						else if (k == "building:levels") {
							height = v.toInt() * 3.5f;
						}
						else if (k == "highway") {
							type = "highway";
						}
						else if (k == "amenity") {
							if (v == "fast_food") {
								subtype = "fast_food";
							}
							else if (v == "parking") {
								subtype = "parking";
							}
						}
						else if (k == "shop") {
							subtype = v;
						}
						else if (k == "leisure") {
							type = v;
						}
					}

					child_element = child_element.nextSiblingElement();
				}

				if (type == "building") {
					if (polygon.size() >= 3) {
						// if the polygon is closed, remove the last point
						if (polygon.front() == polygon.back()) polygon.pop_back();

						buildingParams.emplace_back(name.toUtf8().constData(), polygon, height, subtype.toUtf8().constData());
					}
				}
			}
		}

		element = element.nextSiblingElement();
	}

	// when we convert the longitude/latitude to the UTM, the min/max might be swapped,
	// so we swap them if necessary.
	if (minX > maxX) std::swap(minX, maxX);
	if (minY > maxY) std::swap(minY, maxY);
}

glm::vec2 OSMImporter::convertLatLonToUTM(double lat, double lon, double center_lat, double center_lon) {
	const double radius = 6379000;
	double dlat = (lat - center_lat) / 180 * glm::pi<double>();
	double dlon = (lon - center_lon) / 180 * glm::pi<double>();

	double x = radius * std::cos(center_lat) * dlon;
	double y = radius * dlat;
	return{ x, y };
}