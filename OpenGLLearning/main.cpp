#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>

const int Width = 800, Height = 800;

GLuint VAO1,VAO2,VAO3, VBO1, VBO2, VBO3, Shader;

static const char* VertexShader = "       \n\
#version 330                              \n\
                                           \n\
layout (location = 0) in vec3 pos;          \n\
void main()                                   \n\
{                                              \n\
   gl_Position = vec4(pos.x,pos.y,pos.z,1.0);    \n\
}";

static const char* FragmentShader = "       \n\
#version 330                                 \n\
                                             \n\
 out vec4 color;                             \n\
void main()                                   \n\
{                                              \n\
   color = vec4(0.0,1.0,1.0,1.0);                \n\
}";

void CreatePenatgon()
{
	GLfloat Triangle1[9] = {
		0.5f,0.0f,0.0f,
		0.1545f,0.4755f,0.0f,
		-0.4045f,0.2939f,0.0f

	};
	GLfloat Triangle2[9] = {
		0.5f,0.0f,0.0f,
		-0.4045f,0.2939f,0.0f,
		-0.4045f,-0.2939f,0.0f

	};
	GLfloat Triangle3[9] = {
		0.5f,0.0f,0.0f,
		-0.4045f,-0.2939f,0.0f,
		0.1545f,-0.4755f,0.0f
	};

	// now here we need 3 vao and 3 vbo 
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);

	// Now lets create bind and add data to buffers
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER,VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle1), Triangle1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// now unbind the first one
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	// now here we need 3 vao and 3 vbo 
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	// Now lets create bind and add data to buffers
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle2), Triangle2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// now unbind the first one
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// now here we need 3 vao and 3 vbo 
	glGenVertexArrays(1, &VAO3);
	glBindVertexArray(VAO3);

	// Now lets create bind and add data to buffers
	glGenBuffers(1, &VBO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle3), Triangle3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// now unbind the first one
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return;
	
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


void complieShaders()
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
	  complieShaders();

	// the actual call loop
	while (!glfwWindowShouldClose(MainWindow))
	{
		glfwPollEvents();

		// u can give a clear color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// do the actual program work
		glUseProgram(Shader);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(VAO3);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);
		glfwSwapBuffers(MainWindow);
	}
	glfwDestroyWindow(MainWindow);
	glfwTerminate();

	std::cin.get();
	return 0;

}
