#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

#include <bullet/LinearMath/btIDebugDraw.h>
#include "../Graphics/Mesh.h"
#include "../Graphics/Material.h"
#include "../Graphics/Camera.h"

class DebugDraw : public btIDebugDraw
{
public:
	DebugDraw() :
		mDebugMesh(),
		mDebugMaterial("../../assets/shaders/debug.vs", "../../assets/shaders/debug.fs")
	{
		mDebugMesh.SetWireframe(true);
	}

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		Vertex vertex;
		vertex.Position = glm::vec3(from.x(), from.y(), from.z());
		vertex.Colour = glm::vec3(color.x(), color.y(), color.z());

		mDebugMesh.AddVertices(vertex);

		vertex.Position = glm::vec3(to.x(), to.y(), to.z());

		mDebugMesh.AddVertices(vertex);
	}

	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
	{

	}

	virtual void reportErrorWarning(const char* warningString)
	{
		std::cout << warningString << std::endl;
	}

	virtual void draw3dText(const btVector3& location, const char* textString)
	{

	}

	virtual void setDebugMode(int debugMode)
	{
		mDebugMode = debugMode;
	}

	virtual int	getDebugMode() const
	{
		return mDebugMode;
	}

	void setCamera(Camera *camera)
	{
		mCamera = camera;
	}

	void draw()
	{
		if (mCamera != nullptr)
		{
			mDebugMaterial.setProjection(mCamera->GetProjectionMatrix());
			mDebugMaterial.setView(mCamera->GetViewMatrix());
			mDebugMaterial.setViewPosition(mCamera->GetPosition());
		}

		mDebugMaterial.Bind();
		mDebugMesh.Draw();
	}

private:
	int mDebugMode;
	Camera *mCamera;
	Material mDebugMaterial;
	Mesh mDebugMesh;
};

#endif // DEBUG_DRAW_H