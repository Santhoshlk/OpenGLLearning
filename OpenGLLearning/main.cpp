#include <iostream>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int Width = 800, Height = 800;
const float toRadians = glm::pi<float>() / 180;

GLuint VAO, VBO, EBO, Shader;

GLint Uniform_model , Uniform_Projection;

bool direction = true;
float trOffset = 0.0f;
float trMaxOffset = 0.5f;
float trIncrement = 0.005f;


bool ScaleDirection = true;
float ScaleOffset = 0.4;
float ScaleMin = 0.1f;
float scaleMax = 0.8f;
float ScaleIncrement = 0.01f;


float RotOffset = 0;
float RotIncrement = 0.5f;



static const char* VertexShader = "       \n\
#version 330                              \n\
                                           \n\
layout (location = 0) in vec3 pos;          \n\
                                             \n\
 uniform  mat4 model;                             \n\
 uniform  mat4 projection;                             \n\
 out vec4 VertexColor;                                     \n\
                                              \n\
void main()                                   \n\
{                                              \n\
   gl_Position =  projection * model * vec4(pos,1.0); \n\
   VertexColor = vec4(clamp(pos,0.0f,1.0f),1.0f);                                       \n\
}";

static const char* FragmentShader = "       \n\
#version 330                                 \n\
                                             \n\
 out vec4 color;                             \n\
 in  vec4 VertexColor;                                     \n\
void main()                                   \n\
{                                              \n\
   color = VertexColor;                \n\
}";

void CreatePenatgon()
{
	// first u give all the vertices 
	GLfloat Vertices[] =
	{
	   0.0f,  0.0f, 1.0f,  // 0 - apex (top)
	 0.5f,  0.0f, 0.0f,  // 1 - right
	 0.1545f, 0.4755f, 0.0f, // 2 - top right
	-0.4045f, 0.2939f, 0.0f, // 3 - top left
	-0.4045f,-0.2939f, 0.0f, // 4 - bottom left
	 0.1545f,-0.4755f, 0.0f  // 5 - bottom right
	};

	// indices the way they are taken should be defined in the start
	unsigned int Indices[] =
	{
		// pentagon base
     1, 2, 3,
     1, 3, 4,
     1, 4, 5,
    // pyramid sides
     0, 1, 2,
     0, 2, 3,
     0, 3, 4,
     0, 4, 5,
     0, 5, 1
	};

	// now u gen 1 VAO , 1 VBO, 1 EBO
  
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// make EBO next VBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	// make the vbo
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	// u are gonna say how to traverse the vbo that dosent change
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// now u can unbind top down hierarchy
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
}

void AddShader(GLuint Program, const char* CurrentShader, GLenum ShaderType)
{
	GLuint ShaderID = glCreateShader(ShaderType);

	// u need to store the shader in  a array
	const GLchar* ShaderCode[1];
	ShaderCode[0] = CurrentShader;

	GLint ShaderLength[1];
	ShaderLength[0] = strlen(ShaderCode[0]);

	glShaderSource(ShaderID, 1, ShaderCode, ShaderLength);
    
	//complie the shader
	glCompileShader(ShaderID);
	//erreor Handling
	// error handling
	GLint Result = 0;
	GLchar ErrorLog[1024] = { 0 };

	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
	if (!Result)
	{
		std::cout << "the link Status is a failure" << std::endl;
		glGetShaderInfoLog(ShaderID, sizeof(ErrorLog), 0, ErrorLog);
		std::cout << "The Error Log :" << "\n" << ErrorLog << std::endl;
		return;
	}

	// u acn add the shader to the Program 
	glAttachShader(Program, ShaderID);
	return;

}


void ProcessingShaders()
{
	Shader = glCreateProgram();

	if (!Shader)
	{
		std::cout << "Shader Program Creation is not done Properly" << std::endl;
		return;
	}
	// u add the shadets to the program
	AddShader(Shader, VertexShader, GL_VERTEX_SHADER);
	AddShader(Shader, FragmentShader, GL_FRAGMENT_SHADER);

	// link the shaders to the Program
	glLinkProgram(Shader);

	// error handling
	GLint Result = 0;
	GLchar ErrorLog[1024] = { 0 };

	glGetProgramiv(Shader, GL_LINK_STATUS, &Result);
	if (!Result)
	{
		std::cout << "the link Status is a failure" << std::endl;
		glGetProgramInfoLog(Shader, sizeof(ErrorLog), 0, ErrorLog);
		std::cout << "The Error Log :" << "\n" << ErrorLog << std::endl;
		return;
	}

	// now u validate
	glValidateProgram(Shader);

	
	glGetProgramiv(Shader, GL_VALIDATE_STATUS, &Result);
	if (!Result)
	{
		std::cout << "the Validate Status is a failure" << std::endl;
		glGetProgramInfoLog(Shader, sizeof(ErrorLog), 0, ErrorLog);
		std::cout << "The Error Log :" << "\n" << ErrorLog << std::endl;
		return;
	}

	// now u can get the value of the uniform variables
	Uniform_model = glGetUniformLocation(Shader,"model");
	Uniform_Projection = glGetUniformLocation(Shader, "projection");

}



int main()
{
	//glfw init
	if (!glfwInit())
	{
		std::cout << "The Glfw Initialization is failed" << std::endl;
		glfwTerminate();
		return 1;
	}

	// window prequistics
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// winow creation
	GLFWwindow* MainWindow = glfwCreateWindow(Width, Height, "Main Window", NULL, NULL);

	if (!MainWindow)
	{
		std::cout << "The Main Window Initialization Failed" << std::endl;
		glfwTerminate();
		return 1;

	}
	// Make the winddow as the current context
	glfwMakeContextCurrent(MainWindow);

	// get the buffer size
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(MainWindow, &BufferWidth, &BufferHeight);

	// here u need to enable the depth test as we are doing a 3d object
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, BufferWidth, BufferHeight);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew UInitialization is failed" << std::endl;
		glfwDestroyWindow(MainWindow);
		glfwTerminate();
		return 1;
	}

	  CreatePenatgon();
	 ProcessingShaders();

	 // make the projection matrix
	 glm::mat4 projectionMatrix(1.0f);
	 projectionMatrix = glm::perspective(45.0f,(GLfloat)BufferWidth/(GLfloat)BufferHeight,0.1f,100.f);




	// the renderer loop
	while (!glfwWindowShouldClose(MainWindow))
	{
		glfwPollEvents();

		// logic
		if (direction)
		{
			// check and increment
			if (trOffset >= trMaxOffset)
			{
				direction = false;
			}
			trOffset += trIncrement;
		}
		else
		{
			if (trOffset <= -1 * (trMaxOffset))
			{
				direction = true;
			}
			trOffset -= trIncrement;
		}
		
		if (ScaleDirection)
		{

			if (ScaleOffset >= scaleMax)
			{
				ScaleDirection = false;
			}
			ScaleOffset += ScaleIncrement;
		}
		else
		{
			if (ScaleOffset <=ScaleMin)
			{
				ScaleDirection = true;
			}
			ScaleOffset -= ScaleIncrement;
		}

		if(RotOffset <= 360.f)
		{  
			RotOffset += RotIncrement;
		}
		else
		{
			RotOffset -= 360.f;
		}


		// u can give a clear color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// u need to clear the color as well as depth test bit values in the pixels 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// do the actual program work
		glm::mat4 model(1.0f); 

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::rotate(model, RotOffset* toRadians,glm::vec3(0.0f,1.0f,0.0f));
		// finally u can scale
		model = glm::scale(model, glm::vec3(1.0f,1.0f, 1.0f));
		glUseProgram(Shader);
		
		// now actually send the value into the vertex shader
		glUniformMatrix4fv(Uniform_model, 1, GL_FALSE, value_ptr(model));
		glUniformMatrix4fv(Uniform_Projection, 1, GL_FALSE, value_ptr(projectionMatrix));
	    // u need to do this before u bind the vertex array as it uses vertex shader
		// u only need to do vao as it stores all the data no need to bind others
		glBindVertexArray(VAO);
		// now draw the Elements
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		// atlast u can just sawp the Buffers
		glfwSwapBuffers(MainWindow);
	}
	glfwDestroyWindow(MainWindow);
	glfwTerminate();

	std::cin.get();
	return 0;

}
