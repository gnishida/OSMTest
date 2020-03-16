#include "AssetUtils.h"

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

/*
std::vector<Vertex> AssetUtils::createCube(float edge) {
	static const GLfloat coords[6][4][3] = {
		{ { 0.2f, -0.2f, -0.2f }, { -0.2f, -0.2f, -0.2f }, { -0.2f, 0.2f, -0.2f }, { 0.2f, 0.2f, -0.2f } },
		{ { 0.2f, 0.2f, -0.2f }, { -0.2f, 0.2f, -0.2f }, { -0.2f, 0.2f, 0.2f }, { 0.2f, 0.2f, 0.2f } },
		{ { 0.2f, -0.2f, 0.2f }, { 0.2f, -0.2f, -0.2f }, { 0.2f, 0.2f, -0.2f }, { 0.2f, 0.2f, 0.2f } },
		{ { -0.2f, -0.2f, -0.2f }, { -0.2f, -0.2f, 0.2f }, { -0.2f, 0.2f, 0.2f }, { -0.2f, 0.2f, -0.2f } },
		{ { 0.2f, -0.2f, 0.2f }, { -0.2f, -0.2f, 0.2f }, { -0.2f, -0.2f, -0.2f }, { 0.2f, -0.2f, -0.2f } },
		{ { -0.2f, -0.2f, 0.2f }, { 0.2f, -0.2f, 0.2f }, { 0.2f, 0.2f, 0.2f }, { -0.2f, 0.2f, 0.2f } }
	};

	for (int i = 0; i < 6; i++) {
		assets[i] = new Asset(program);
		assets[i]->setTexture(QString("images/side%1.png").arg(i + 1).toUtf8().constData());

		assets[i]->addVertex(coords[i][0][0], coords[i][0][1], coords[i][0][2], 0, 0);
		assets[i]->addVertex(coords[i][1][0], coords[i][1][1], coords[i][1][2], 1, 0);
		assets[i]->addVertex(coords[i][2][0], coords[i][2][1], coords[i][2][2], 1, 1);

		assets[i]->addVertex(coords[i][0][0], coords[i][0][1], coords[i][0][2], 0, 0);
		assets[i]->addVertex(coords[i][2][0], coords[i][2][1], coords[i][2][2], 1, 1);
		assets[i]->addVertex(coords[i][3][0], coords[i][3][1], coords[i][3][2], 0, 1);
	}
}
*/

std::vector<Vertex> AssetUtils::createRectangle(float width, float length) {
	std::vector<Vertex> vertices;
	const float x1 = -width / 2;
	const float x2 = width / 2;
	const float y1 = -length / 2;
	const float y2 = length / 2;
	vertices.push_back({ x1, y1, 0, 1, 0 });
	vertices.push_back({ x2, y1, 0, 0, 0 });
	vertices.push_back({ x2, y2, 0, 0, 1 });

	vertices.push_back({ x1, y1, 0, 1, 0 });
	vertices.push_back({ x2, y2, 0, 0, 1 });
	vertices.push_back({ x1, y2, 0, 1, 1 });

	return vertices;
}