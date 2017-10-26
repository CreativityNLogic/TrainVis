#include "AnimatedMesh.h"

#include "../Glad/glad.h"

AnimatedMesh::AnimatedMesh() :
	mWireframe(false)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(0);
}

AnimatedMesh::AnimatedMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<VertexBone> boneData) :
	mAnimatedVertices(vertices),
	mIndices(indices),
	mBoneData(mBoneData),
	mWireframe(false)
{
	mRootJoint.CalculateInverseBindTransform(glm::mat4());
	setupMesh();
}


AnimatedMesh::~AnimatedMesh()
{
}

void AnimatedMesh::Draw()
{
	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
}

std::vector<Vertex> AnimatedMesh::GetAnimatedVertices() const
{
	return mAnimatedVertices;
}

std::vector<unsigned int> AnimatedMesh::GetIndices() const
{
	return mIndices;
}

void AnimatedMesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, mAnimatedVertices.size() * sizeof(Vertex), &mAnimatedVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, BBO);
	glBufferData(GL_ARRAY_BUFFER, mBoneData.size() * sizeof(unsigned int), &mBoneData[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex tangents
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, BiTangent));
	// vertex texture coords
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex colour
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Colour));
	// bone ids
	glEnableVertexAttribArray(6);
	glVertexAttribIPointer(6, NUM_BONES_PER_VEREX, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, BoneIDs));
	// vertex weights
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, NUM_BONES_PER_VEREX, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, VertexWeights));
	
	glBindVertexArray(0);
}

void AnimatedMesh::PlayAnimation(Animation animation)
{

}

void AnimatedMesh::Update()
{

}

std::vector<glm::mat4> AnimatedMesh::GetJointTransforms()
{
	std::vector<glm::mat4> jointMatricies;
	AddJointToArray(mRootJoint, jointMatricies);
	return jointMatricies;
}

void AnimatedMesh::AddJointToArray(Joint headJoint, std::vector<glm::mat4> &jointMatrices)
{
	jointMatrices.push_back(headJoint.GetTransform());
	for (auto &joint : headJoint.GetChildren())
		AddJointToArray(joint, jointMatrices);
}

void AnimatedMesh::SetRootJoint(Joint joint)
{
	mRootJoint = joint;
}

Joint AnimatedMesh::GetRootJoint() const
{
	return mRootJoint;
}