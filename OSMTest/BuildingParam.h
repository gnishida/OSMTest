#pragma once

#include <vector>
#include <glm/glm.hpp>

class BuildingParam {
public:
	std::string name;
	std::vector<glm::vec2> footprint;
	float height;
	std::string type;

public:
	BuildingParam(const std::string& name, const std::vector<glm::vec2>& footprint, float height, const std::string& type);
	~BuildingParam();
};

