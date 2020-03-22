#include "AssetUtils.h"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Partition_traits_2.h>
#include <CGAL/partition_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/random_polygon_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/create_offset_polygons_2.h>

#include <algorithm>
#include <limits>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Partition_traits_2<K> Traits;

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

std::vector<Vertex> AssetUtils::createRectangle(float width, float length, float height) {
	std::vector<Vertex> vertices;
	const float x1 = -width / 2;
	const float x2 = width / 2;
	const float y1 = -length / 2;
	const float y2 = length / 2;
	vertices.push_back({ x1, y1, height, 0, 0 });
	vertices.push_back({ x2, y1, height, 1, 0 });
	vertices.push_back({ x2, y2, height, 1, 1 });

	vertices.push_back({ x1, y1, height, 0, 0 });
	vertices.push_back({ x2, y2, height, 1, 1 });
	vertices.push_back({ x1, y2, height, 0, 1 });

	return vertices;
}

std::vector<Vertex> AssetUtils::createConvexPolygon(const std::vector<glm::vec2>& polygon, float height) {
	if (polygon.size() < 3) return {};

	std::vector<Vertex> vertices;

	const float& x0 = polygon[0].x;
	const float& y0 = polygon[0].y;

	for (int i = 0; i < polygon.size(); i++) {
		const float& x1 = polygon[i].x;
		const float& y1 = polygon[i].y;
		const int next = (i + 1) % polygon.size();
		const float& x2 = polygon[next].x;
		const float& y2 = polygon[next].y;

		vertices.push_back({ x0, y0, height, 0, 0 });
		vertices.push_back({ x1, y1, height, 1, 0.2 });
		vertices.push_back({ x2, y2, height, 0.4, 1 });
	}

	return vertices;
}

std::vector<Vertex> AssetUtils::createConvexPolygon(const std::vector<glm::vec2>& polygon, float height, float minX, float minY, float maxX, float maxY) {
	if (polygon.size() < 3) return {};

	std::vector<Vertex> vertices;

	const float& x0 = polygon[0].x;
	const float& y0 = polygon[0].y;

	float u0 = (polygon[0].x - minX) / (maxX - minX);
	float v0 = (polygon[0].y - minY) / (maxY - minY);

	for (int i = 0; i < polygon.size(); i++) {
		const float& x1 = polygon[i].x;
		const float& y1 = polygon[i].y;
		float u1 = (x1 - minX) / (maxX - minX);
		float v1 = (y1 - minY) / (maxY - minY);

		const int next = (i + 1) % polygon.size();
		const float& x2 = polygon[next].x;
		const float& y2 = polygon[next].y;
		float u2 = (x2 - minX) / (maxX - minX);
		float v2 = (y2 - minY) / (maxY - minY);

		vertices.push_back({ x0, y0, height, u0, v0 });
		vertices.push_back({ x1, y1, height, u1, v1 });
		vertices.push_back({ x2, y2, height, u2, v2 });
	}

	return vertices;
}

std::vector<Vertex> AssetUtils::createPolygon(const std::vector<glm::vec2>& polygon, float height, float minX, float minY, float maxX, float maxY) {
	if (polygon.size() < 3) return {};

	Traits::Polygon_2 cga_polygon;
	for (int i = 0; i < polygon.size(); ++i) {
		cga_polygon.push_back(Traits::Point_2(polygon[i].x, polygon[i].y));
	}

	if (cga_polygon.is_clockwise_oriented()) {
		cga_polygon.reverse_orientation();
	}

	// tesselate the concave polygon
	std::list<Traits::Polygon_2> partition_polys;
	Traits partition_traits;
	CGAL::greene_approx_convex_partition_2(cga_polygon.vertices_begin(), cga_polygon.vertices_end(), std::back_inserter(partition_polys), partition_traits);

	std::vector<Vertex> vertices;

	for (auto fit = partition_polys.begin(); fit != partition_polys.end(); ++fit) {
		std::vector<glm::vec2> convexPolygon;
		for (auto vit = fit->vertices_begin(); vit != fit->vertices_end(); ++vit) {
			convexPolygon.push_back({ (float)vit->x(), (float)vit->y() });
		}
		std::vector<Vertex> subVertices = createConvexPolygon(convexPolygon, height, minX, minY, maxX, maxY);
		vertices.insert(vertices.end(), subVertices.begin(), subVertices.end());
	}

	return vertices;
}