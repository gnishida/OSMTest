#include "BuildingParam.h"

BuildingParam::BuildingParam(const std::string& name, const std::vector<glm::vec2>& footprint, float height, const std::string& type) : name(name), footprint(footprint), height(height), type(type) {
}

BuildingParam::~BuildingParam() {
}
