#pragma once

#include "Vertex.h"
#include <vector>

class AssetUtils
{
public:
	static std::vector<Vertex> createSphere(float radius);
	static std::vector<Vertex> createPrism(std::vector<std::pair<float, float>> polygon, float height);
	static std::vector<Vertex> createRectangle(float width, float length);
};

