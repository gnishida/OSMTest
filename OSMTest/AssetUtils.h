#pragma once

#include "Vertex.h"
#include <vector>
#include <glm/glm.hpp>

class AssetUtils
{
public:
	static std::vector<Vertex> createSphere(float radius);
	static std::vector<Vertex> createPrism(const std::vector<glm::vec2>& polygon, float height);
	static std::vector<Vertex> createRectangle(float width, float length, float height = 0);
	static std::vector<Vertex> createConvexPolygon(const std::vector<glm::vec2>& polygon, float height);
	static std::vector<Vertex> createConvexPolygon(const std::vector<glm::vec2>& polygon, float height, float minX, float minY, float maxX, float maxY);
	static std::vector<Vertex> createPolygon(const std::vector<glm::vec2>& polygon, float height, float minX, float minY, float maxX, float maxY);
};

