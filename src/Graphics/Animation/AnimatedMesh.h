#ifndef ANIMATEDMESH_H
#define ANIMATEDMESH_H

#include <vector>

#include "../VertexData.h"
#include "Animation.h"
#include "Animator.h"
#include "Joint.h"

class AnimatedMesh
{
public:
	/*  Functions  */
	AnimatedMesh();
	AnimatedMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<VertexBone> boneData);
	~AnimatedMesh();

	void Draw();

	std::vector<Vertex> GetAnimatedVertices() const;
	std::vector<unsigned int> GetIndices() const;

	void PlayAnimation(Animation animation);
	void Update();

	void SetRootJoint(Joint joint);
	Joint GetRootJoint() const;

	std::vector<glm::mat4> GetJointTransforms();
	void AddJointToArray(Joint headJoint, std::vector<glm::mat4> &jointMatrices);

private:
	/*  Functions    */
	void setupMesh();

private:
	/*  Render data  */
	unsigned int VAO, VBO, EBO, BBO;

	Joint mRootJoint;
	unsigned int mJointCount;

	bool mWireframe;

	/*  Mesh Data  */
	std::vector<Vertex> mAnimatedVertices;
	std::vector<unsigned int> mIndices;
	std::vector<VertexBone> mBoneData;
};

#endif // ANIMATEDMESH_H