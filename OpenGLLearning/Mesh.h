#pragma once
#include <iostream>
#include <GL/glew.h>


class Mesh
{
private:
	GLuint VAO, IBO, VBO;
	GLsizei IndicesCount;

public:
	Mesh();
	// Data Functions 
	void CreateMesh(GLfloat vertices[],unsigned int Indices[],unsigned int numberofVertices,unsigned int numberofIndices);
	void RenderMesh();
	void DeleteMesh();

	// destructor
	~Mesh();
};

