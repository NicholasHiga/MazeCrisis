#include "OpenGLRenderer.h"

#include <Windows.h>
#include <GL/glew.h>
#include <gl/gl.h>
#include <gl/GLU.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Skybox.h"
#include "Material.h"
#include "SceneNode.h"
#include "SceneGraph.h"
#include "GameObject.h"
#include "EffectedModel.h"
#include "ShaderProgram.h"
#include "ShaderVariable.h"

using glm::vec3;
using glm::mat4;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

unique_ptr<OpenGLRenderer> OpenGLRenderer::myInstance = nullptr;

OpenGLRenderer*
OpenGLRenderer::getInstance()
{
	if (myInstance == nullptr)
		myInstance = unique_ptr<OpenGLRenderer>(new OpenGLRenderer());

	return myInstance.get();
}

OpenGLRenderer::OpenGLRenderer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LEQUAL);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
}

void
OpenGLRenderer::renderFixedPrimitve(PrimitiveType prim)
{
	GLuint vao, vbo;
	GLfloat points[] = {
		0.0f, 0.0f,		// Dot
		-0.5f, 0.0f,	// Line
		0.5f, 0.0f,
		0.0f, 0.5f,		// Triangle
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,		// Square
		0.5f, -0.5f,
		-0.5f, -0.5f,
		-0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f
	};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	if (prim == PrimitiveType::PRIM_POINT) {
		glDrawArrays(GL_POINTS, 0, 1);
	}
	else if (prim == PrimitiveType::PRIM_LINE) {
		glDrawArrays(GL_LINES, 1, 2);
	}
	else if (prim == PrimitiveType::PRIM_TRIANGLE) {
		glDrawArrays(GL_TRIANGLES, 3, 3);
	}
	else if (prim == PrimitiveType::PRIM_QUAD) {
		glDrawArrays(GL_TRIANGLES, 6, 6);
	}
}

/*void
OpenGLRenderer::renderMesh(MeshType &meshType)
{*/
	/*if (typeid(meshType) == typeid(SingleMesh))
		renderSingleMesh(dynamic_cast<SingleMesh&>(meshType));*/
	//else
		//renderMesh(dynamic_cast<PolyMesh&>(meshType));
/*
	if (SingleMesh* mesh = dynamic_cast<SingleMesh*>(&meshType))
		renderSingleMesh(dynamic_cast<SingleMesh&>(meshType));
}*/

void
OpenGLRenderer::renderEffectedModel(EffectedModel &model, BoundingBox *boundingBox, shared_ptr<SceneNode> node, Camera &camera)
{
	Material *material = model.getMaterial();
	vector<ShaderVariable> samplerVars = material->getShader()->getSamplerVars();

	mat4 modelM = node.get()->getModelMatrix();
	matrixStack.pushModelStack(modelM);

	if (camera.getFrustum()->isBoxInFrustum(*boundingBox))
	{
		glUseProgram(material->getShader()->getProgramID());

		if (boundingBox->getIsVisible())
		{
			
		}

		// Only diffuse texture.
		if (!samplerVars.empty())
		{
			glActiveTexture(GL_TEXTURE0);
			glUniform1i(samplerVars[0].getUniformLocation(), 0); // Might be buggy with multiple textures.
			glBindTexture(GL_TEXTURE_2D, material->getDiffuseTexture()->textureID);
		}

		glUniformMatrix4fv(material->getShader()->getModelViewMatrixID(),
			1, GL_FALSE, glm::value_ptr(matrixStack.getViewMatrix() * matrixStack.getModelMatrix() ));

		glUniformMatrix4fv(material->getShader()->getProjectionID(),
			1, GL_FALSE, glm::value_ptr(matrixStack.getProjectionMatrix()));

		SingleMesh *m = model.getMesh();
		glBindVertexArray(m->getVAO());

		if (!m->getIndices().empty())
			glDrawElements(GL_TRIANGLES, m->getIndices().size(), GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, m->getVertices().size());

		glBindVertexArray(0);
		node.get()->isBeingRendered = true;
		glUseProgram(0);
	}
	else
		node.get()->isBeingRendered = false;

	matrixStack.popModelStack();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void 
OpenGLRenderer::renderSkybox(Skybox &skybox, std::shared_ptr<SceneNode> node)
{
	vector<ShaderVariable> samplerVars = skybox.getShader()->getSamplerVars();

	glUseProgram(skybox.getShader()->getProgramID());
	glUniformMatrix4fv(skybox.getShader()->getModelViewMatrixID(),
		1, GL_FALSE, glm::value_ptr(matrixStack.getUntranslatedViewMatrix()));
	glUniformMatrix4fv(skybox.getShader()->getProjectionID(),
		1, GL_FALSE, glm::value_ptr(matrixStack.getProjectionMatrix()));

	SingleMesh *m = skybox.getMesh();
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(samplerVars[0].getUniformLocation(), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTexture()->textureID);

	glDepthMask(GL_FALSE);
	glBindVertexArray(m->getVAO());
	glDrawArrays(GL_TRIANGLES, 0, m->getVertices().size());

	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}

/*
void
OpenGLRenderer::renderMesh(Mesh &mesh)
{
	for (int i = 0; i < mesh.getNumPrimitives(); i++)
		renderPrimitiveShape(mesh.getPrimitives()[i]);
}

void
OpenGLRenderer::renderMesh(PolyMesh &polymesh)
{
	for (int i = 0; i < polymesh.getNumMeshes(); i++)
		renderMesh(*((*polymesh.getMeshes())[i]));
}*/

void 
OpenGLRenderer::setWireframeMode(bool isOn)
{ 
	isWireframe = isOn; 

	if (isOn) 
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void
OpenGLRenderer::renderSceneGraph(SceneGraph &graph, Camera &camera)
{
	prepareToRender(camera);
	graph.doSomethingEachElement(graph.getRootSceneNode(),
		[this, &camera](shared_ptr<SceneNode> n)
	{ 
		renderSceneNode(n, camera);
	});
}

void
OpenGLRenderer::renderSceneNode(shared_ptr<SceneNode> node, Camera &camera)
{
	vector<Renderable*> *renderables = node.get()->getRenderables();
	for (size_t i = 0; i < (*renderables).size(); i++)
	{
		if (GameObject *go = dynamic_cast<GameObject*>((*renderables)[i]))
		{
			if (go->getIsVisible())
				renderEffectedModel(*go->getModel(), go->getBoundingBoxes()[i], node, camera);
		}
		else if (Skybox *sky = dynamic_cast<Skybox*>((*renderables)[i]))
		{
			renderSkybox(*sky, node);
		}
	}
}

void
OpenGLRenderer::setWindowSize(int width, int height, Camera *cam)
{
	winHeight = height;
	winWidth = width;
	if (winHeight == 0)
	{
		// avoid zerodivide
		winHeight = 1;
	}
	glViewport(0, 0, winWidth, winHeight);			// set Viewport size

	mat4 projection = glm::perspective(45.0f, (GLfloat)winWidth / (GLfloat)winHeight, 0.1f, 1000.0f);
	matrixStack.setProjectionMatrix(projection);

	if (cam)
		cam->getFrustum()->setWindowSize(45.0f, (GLfloat)winWidth / (GLfloat)winHeight, 0.1f, 1000.0f);
}

void OpenGLRenderer::prepareToRender(Camera &camera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); wireframe
	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/
	glEnable(GL_DEPTH_TEST);
	//glFrontFace(GL_CW);
	//glEnable(GL_CULL_FACE);
	vec3 pos, look, up;
	pos = camera.getCameraPosition();
	look = camera.getSceneCenter();
	up = camera.getCameraUp();
	matrixStack.setViewMatrix(glm::lookAt(pos, look, up));
	//matrixStack.setUntranslatedViewMatrix(glm::lookAt(vec3(0, 0, 0), look, up));
}