#ifndef MESH_H
#define MESH_H

#include <vector>

#include "VertexData.h"

class Mesh 
{
public:
	/*  Functions  */
	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Mesh();

	void Draw();
	
	void AddVertices(Vertex vertex);

	void SetVertices(std::vector<Vertex> vertices);
	void SetIndicies(std::vector<unsigned int> indices);

	std::vector<Vertex> GetVertices() const;
	std::vector<unsigned int> GetIndices() const;

	void Flush();

	void SetWireframe(bool wireframe);

private:
	/*  Functions    */
	void setupMesh();

private:
	/*  Render data  */
	unsigned int VAO, VBO, EBO;

	bool mWireframe;

	/*  Mesh Data  */
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
};

#endif // MESH_H