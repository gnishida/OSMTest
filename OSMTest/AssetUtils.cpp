#include "AssetUtils.h"
#include <algorithm>
#include <limits>

std::vector<Vertex> AssetUtils::createSphere(float radius) {
	std::vector<Vertex> vertices;

	const int HOR_SLICES = 128;
	const int VERT_SLICES = 64;

	const float M_PI = atan(1) * 4;
	const float r = 0.2;
	for (int j = 0; j < VERT_SLICES; j++) {
		const float z_coord = r * sin(j * M_PI / VERT_SLICES - M_PI / 2);
		const float z2_coord = r * sin((j + 1) * M_PI / VERT_SLICES - M_PI / 2);
		const float new_r = r * cos(j * M_PI / VERT_SLICES - M_PI / 2);
		const float new_r2 = r * cos((j + 1) * M_PI / VERT_SLICES - M_PI / 2);
		for (int i = 0; i < HOR_SLICES; i++) {
			const float x_coord = new_r * cos(i * 2 * M_PI / HOR_SLICES);
			const float y_coord = new_r * sin(i * 2 * M_PI / HOR_SLICES);
			const float x2_coord = new_r * cos((i + 1) * 2 * M_PI / HOR_SLICES);
			const float y2_coord = new_r * sin((i + 1) * 2 * M_PI / HOR_SLICES);
			const float x4_coord = new_r2 * cos(i * 2 * M_PI / HOR_SLICES);
			const float y4_coord = new_r2 * sin(i * 2 * M_PI / HOR_SLICES);
			const float x3_coord = new_r2 * cos((i + 1) * 2 * M_PI / HOR_SLICES);
			const float y3_coord = new_r2 * sin((i + 1) * 2 * M_PI / HOR_SLICES);

			vertices.push_back({ x_coord, y_coord, z_coord, 1 - (float)i / HOR_SLICES, (float)j / VERT_SLICES });
			vertices.push_back({ x2_coord, y2_coord, z_coord, 1 - (float)(i + 1) / HOR_SLICES, (float)j / VERT_SLICES });
			vertices.push_back({ x3_coord, y3_coord, z2_coord, 1 - (float)(i + 1) / HOR_SLICES, (float)(j + 1) / VERT_SLICES });

			vertices.push_back({ x_coord, y_coord, z_coord, 1 - (float)i / HOR_SLICES, (float)j / VERT_SLICES });
			vertices.push_back({ x3_coord, y3_coord, z2_coord, 1 - (float)(i + 1) / HOR_SLICES, (float)(j + 1) / VERT_SLICES });
			vertices.push_back({ x4_coord, y4_coord, z2_coord, 1 - (float)i / HOR_SLICES, (float)(j + 1) / VERT_SLICES });
		}
	}

	return vertices;
}

std::vector<Vertex> AssetUtils::createPrism(const std::vector<glm::vec2>& polygon, float height) {
	std::vector<Vertex> vertices;
	const int NUM_VERTEX = polygon.size();

	for (int i = 0; i < NUM_VERTEX; i++) {
		const float& x1 = polygon[i].x;
		const float& y1 = polygon[i].y;
		const int next = (i + 1) % NUM_VERTEX;
		const float& x2 = polygon[next].x;
		const float& y2 = polygon[next].y;
		const float length = glm::length(glm::vec2(x2 - x1, y2 - y1));

		vertices.push_back({ x1, y1, 0, 0, 0 });
		vertices.push_back({ x2, y2, 0, length / 15.0f, 0 });
		vertices.push_back({ x2, y2, height, length / 15.0f, height / 10.0f });

		vertices.push_back({ x1, y1, 0, 0, 0 });
		vertices.push_back({ x2, y2, height, length / 15.0f, height / 10.0f });
		vertices.push_back({ x1, y1, height, 0, height / 10.0f });
	}

	return vertices;
}

std::vector<Vertex> AssetUtils::createRectangle(float width, float length) {
	std::vector<Vertex> vertices;
	const float x1 = -width / 2;
	const float x2 = width / 2;
	const float y1 = -length / 2;
	const float y2 = length / 2;
	vertices.push_back({ x1, y1, 0, 0, 0 });
	vertices.push_back({ x2, y1, 0, 1, 0 });
	vertices.push_back({ x2, y2, 0, 1, 1 });

	vertices.push_back({ x1, y1, 0, 0, 0 });
	vertices.push_back({ x2, y2, 0, 1, 1 });
	vertices.push_back({ x1, y2, 0, 0, 1 });

	return vertices;
}

std::vector<Vertex> AssetUtils::createPolygon(const std::vector<glm::vec2>& polygon, float height) {
	std::vector<Vertex> vertices;
	const int NUM_VERTEX = polygon.size();

	float x_max = -std::numeric_limits<float>::max();;
	float x_min = std::numeric_limits<float>::max();;
	float y_max = -std::numeric_limits<float>::max();;
	float y_min = std::numeric_limits<float>::max();;
	for (int i = 0; i < NUM_VERTEX; i++) {
		x_max = std::max(x_max, polygon[i].x);
		x_min = std::min(x_min, polygon[i].x);
		y_max = std::max(y_max, polygon[i].y);
		y_min = std::min(y_min, polygon[i].y);
	}
	float x_average = (x_max + x_min) / 2;
	float y_average = (y_max + y_min) / 2;

	for (int i = 0; i < NUM_VERTEX; i++) {
		const float& x1 = polygon[i].x;
		const float& y1 = polygon[i].y;
		const int next = (i + 1) % NUM_VERTEX;
		const float& x2 = polygon[next].x;
		const float& y2 = polygon[next].y;
		const float length = glm::length(glm::vec2(x2 - x1, y2 - y1));

		vertices.push_back({ x1, y1, height, 0, 0 });
		vertices.push_back({ x2, y2, height, 1, 0.2 });
		vertices.push_back({ x_average, y_average, height, 0.4, 1 });
	}

	return vertices;
}

std::vector<Vertex> AssetUtils::createPolygon2(const std::vector<glm::vec2>& polygon, float height, float minX, float minY, float maxX, float maxY) {
	std::vector<Vertex> vertices;
	const int NUM_VERTEX = polygon.size();

	float x_max = -std::numeric_limits<float>::max();;
	float x_min = std::numeric_limits<float>::max();;
	float y_max = -std::numeric_limits<float>::max();;
	float y_min = std::numeric_limits<float>::max();;
	for (int i = 0; i < NUM_VERTEX; i++) {
		x_max = std::max(x_max, polygon[i].x);
		x_min = std::min(x_min, polygon[i].x);
		y_max = std::max(y_max, polygon[i].y);
		y_min = std::min(y_min, polygon[i].y);
	}
	float x_average = (x_max + x_min) / 2;
	float y_average = (y_max + y_min) / 2;

	float u3 = (x_average - minX) / (maxX - minX);
	float v3 = (y_average - minY) / (maxY - minY);

	for (int i = 0; i < NUM_VERTEX; i++) {
		const float& x1 = polygon[i].x;
		const float& y1 = polygon[i].y;
		const int next = (i + 1) % NUM_VERTEX;
		const float& x2 = polygon[next].x;
		const float& y2 = polygon[next].y;
		const float length = glm::length(glm::vec2(x2 - x1, y2 - y1));
		float u1 = (x1 - minX) / (maxX - minX);
		float v1 = (y1 - minY) / (maxY - minY);
		float u2 = (x2 - minX) / (maxX - minX);
		float v2 = (y2 - minY) / (maxY - minY);
		vertices.push_back({ x1, y1, height, u1, v1 });
		vertices.push_back({ x2, y2, height, u2, v2 });
		vertices.push_back({ x_average, y_average, height, u3, v3 });
	}

	return vertices;
}