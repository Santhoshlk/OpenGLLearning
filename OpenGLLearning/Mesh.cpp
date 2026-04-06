#include "Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	IndicesCount = 0;
}

void Mesh::CreateMesh(GLfloat vertices[], unsigned int Indices[], unsigned int numberofVertices, unsigned int numberofIndices)
{

	IndicesCount = numberofIndices;

	// now the actual creation of the vertex shader data
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(Indices[0])* IndicesCount,Indices,GL_STATIC_DRAW);

	// now bind the vbo
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices[0])* numberofVertices,vertices,GL_STATIC_DRAW);

	// now Atrribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	std::cout << "Creation is Complete" << std::endl;
}

void Mesh::RenderMesh()
{
	// the actual render part of the loop 
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, IndicesCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::cout << "Rendering is Complete" << std::endl;
}

void Mesh::DeleteMesh()
{
	// now we are deleting the buffer
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	// Delete IBO
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if(VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	IndicesCount = 0;

	std::cout << "Deletion is Complete" << std::endl;
}

Mesh::~Mesh()
{
	DeleteMesh();
	// any pointer release if needed
}
