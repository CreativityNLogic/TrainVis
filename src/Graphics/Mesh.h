#ifndef MESH_H
#define MESH_H

#include <vector>

#include "VertexData.h"

class Mesh 
{
public:
	/*  Functions  */
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Mesh();

	void Draw();

	std::vector<Vertex> GetVertices() const;
	std::vector<unsigned int> GetIndices() const;

private:
	/*  Functions    */
	void setupMesh();

private:
	/*  Render data  */
	unsigned int VAO, VBO, EBO;

	/*  Mesh Data  */
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
};

#endif // MESH_H