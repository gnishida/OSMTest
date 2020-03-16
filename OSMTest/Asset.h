#pragma once

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include "Vertex.h"

class Asset
{
public:
	enum { PROGRAM_VERTEX_ATTRIBUTE = 0, PROGRAM_TEXCOORD_ATTRIBUTE = 1 };

private:
	QOpenGLShaderProgram *program;
	QOpenGLVertexArrayObject* vao;
	QOpenGLBuffer vbo;
	QOpenGLTexture* texture;
	std::vector<Vertex> vertices;
	bool dirty;

public:
	Asset(QOpenGLShaderProgram* program);
	~Asset();

	void setTexture(const char* filename);
	void createArrays();
	void bind();
	void release();
	void addVertices(const std::vector<Vertex>& vertices);
	int numVertices() { return vertices.size(); }
};
