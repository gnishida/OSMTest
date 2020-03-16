#include "Asset.h"

Asset::Asset(QOpenGLShaderProgram* program) : program(program)
{
	vao = nullptr;
	texture = nullptr;
	dirty = false;
}

Asset::~Asset()
{
	if (texture) {
		texture->destroy();
		texture = nullptr;
	}
	if (vao) {
		vbo.destroy();
		vao->destroy();
		vao = nullptr;
	}
}

void Asset::setTexture(const char* filename)
{
	if (texture) {
		texture->destroy();
		texture = nullptr;
	}
	texture = new QOpenGLTexture(QImage(filename));
}

/**
 * Create vertex array and send the data to GPU.
 * Note that if there is no update to the vertex array (i.e., dirty = false), this function does nothing.
 */
void Asset::createArrays()
{
	if (vao && !dirty) {
		return;
	}

	vao = new QOpenGLVertexArrayObject();
	vao->create();
	vao->bind();

	vbo.create();
	vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo.bind();

	vbo.allocate(vertices.data(), vertices.size() * sizeof(Vertex));

	program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
	program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
	program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, sizeof(Vertex));
	program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, offsetof(Vertex, u), 2, sizeof(Vertex));

	dirty = false;
}

void Asset::bind()
{
	if (vao) {
		vao->bind();
		vbo.bind();
	}
	if (texture) {
		texture->bind();
	}
}

void Asset::release()
{
	if (texture) {
		texture->release();
	}
	if (vao) {
		vbo.release();
		vao->release();
	}
}

void Asset::addVertices(const std::vector<Vertex>& vertices)
{
	this->vertices.insert(this->vertices.end(), vertices.begin(), vertices.end());
	dirty = true;
}
