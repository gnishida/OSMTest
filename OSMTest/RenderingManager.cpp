#include "RenderingManager.h"

RenderingManager::RenderingManager()
{
	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);

	program = new QOpenGLShaderProgram;
	program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/vert.glsl");
	program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/frag.glsl");
	program->bindAttributeLocation("vertex", Asset::PROGRAM_VERTEX_ATTRIBUTE);
	program->bindAttributeLocation("texCoord", Asset::PROGRAM_TEXCOORD_ATTRIBUTE);
	program->link();

	program->bind();
	program->setUniformValue("texture", 0);
}

RenderingManager::~RenderingManager()
{
	for (auto it = assets.begin(); it != assets.end(); it++) {
		Asset* asset = it->second;
		delete asset;
		asset = nullptr;
	}
	assets.clear();
	delete program;
	program = nullptr;
}

void RenderingManager::setViewport(int width, int height)
{
	glViewport(0, 0, width, height);
}

void RenderingManager::addObject(const char* filename, const std::vector<Vertex>& vertices)
{
	Asset* asset = nullptr;

	if (assets.find(filename) == assets.end()) {
		asset = new Asset(program);
		asset->setTexture(filename);
		assets[filename] = asset;
	}
	else {
		asset = assets[filename];
	}

	asset->addVertices(vertices);
}

void RenderingManager::render(const QMatrix4x4& cameraMatrix)
{
	glClearColor(0, 0.1, 0.1, 1);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program->setUniformValue("matrix", cameraMatrix);

	for (auto it = assets.begin(); it != assets.end(); it++) {
		Asset* asset = it->second;
		asset->createArrays();
		asset->bind();
		glDrawArrays(GL_TRIANGLES, 0, asset->numVertices());
	}
}
