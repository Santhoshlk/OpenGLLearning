#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint Width = 800, Height = 800;

// these are the variables assigned to the Parts of Shader Pipeline
GLuint Vao, Vbo, Shader;

// Create the Shaders
static const char* VertexShader = "   \n\
#version 330                           \n\
                                        \n\
// now create the input to the shader    \n\
layout (location = 0) in vec3 pos;  \n\
                                          \n\
void main()                                \n\
{                                           \n\
   gl_Position =  vec4( pos.x*0.5,pos.y*0.5,pos.z*0.5,1.0); \n\
}";

static const char* FragmentShader = "   \n\
#version 330                           \n\
                                        \n\
// now create out color                  \n\
 out vec4 color;                         \n\
                                          \n\
void main()                                \n\
{                                           \n\
   color = vec4(0.0,1.0,0.0,1.0); \n\
}";



void CreateTraingle()
{
	// Create the Points for the vertices obviously
	GLfloat vertices[9] = {
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f
	};

	// Generate the Vertex Array required
	glGenVertexArrays(1, &Vao);
	glBindVertexArray(Vao);

	// Generate the Buffer Bind the Buffer and Add the Data to the Buffer
	glGenBuffers(1, &Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Make the Attribute Pointer and Enable the value of VertexArray
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(0);


	// as Buffer is Completed u need to unbind it
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AddShader(GLuint Program, const char* CurrentShader, GLenum ShaderType)
{
	// get the shader code
	GLuint ShaderID = glCreateShader(ShaderType);

	// store the shader length and the shadere as we need it
	const GLchar* shaderCode[1];
	shaderCode[0] = CurrentShader;


	 GLint ShaderSize[1];
	ShaderSize[0] = strlen(shaderCode[0]);

	glShaderSource(ShaderID, 1, shaderCode, ShaderSize);
	

	// attach the shader 
	
	// u have to do the Compiling
	glCompileShader(ShaderID);
	glAttachShader(Program, ShaderID);
	// error handling
	GLint result = 0;
	GLchar ErrorLog[1024] = { 0 };
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		std::cout << "the Shader Compiling  process is not done properly" << std::endl;
		glGetShaderInfoLog(ShaderID, sizeof(ErrorLog), 0, ErrorLog);
		return;
	}
	return;
}

void CompileShaders()
{
	Shader = glCreateProgram();
	if (!Shader)
	{
		std::cout << "The Program Has Not Been Created " << std::endl;
		return;
	}

	// add the shaders to the Program
	AddShader(Shader, VertexShader, GL_VERTEX_SHADER);
	AddShader(Shader, FragmentShader, GL_FRAGMENT_SHADER);

	// u have to handle the linking and error handling
	GLint result = 0;
	GLchar ErrorLog[1024] = { 0 };

	// u can do the actual linking
	glLinkProgram(Shader);
	glGetProgramiv(Shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		std::cout << "the linking process is not done properly" << std::endl;
		glGetProgramInfoLog(Shader, sizeof(ErrorLog), 0, ErrorLog);
		return;
	}
	glValidateProgram(Shader);
	glGetProgramiv(Shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		std::cout << "the Validation  process is not done properly" << std::endl;
		glGetProgramInfoLog(Shader, sizeof(ErrorLog), 0, ErrorLog);
		return;
	}
}






int main()
{
	//1)GLFW initialization
	if (!glfwInit())
	{
		std::cout << "Glfw Initialization has Failed" << std::endl;
		glfwTerminate();
		return 1;

	}

	//2) set all the Context Prequistics
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//3) create the window and make the context
	GLFWwindow* TestMainWindow = glfwCreateWindow(Width, Height, "Test Main Window ", NULL, NULL);
	if (!TestMainWindow)
	{
		// u can print that the window creation gone wrong
		std::cout << "The window is not suceesfully created" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(TestMainWindow);

	//4) crreate viewport
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(TestMainWindow, &BufferWidth, &BufferHeight);
	glViewport(0, 0, BufferWidth, BufferHeight);

	// 5) glew initialization
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew didn't initilaize succesfully" << std::endl;
		glfwDestroyWindow(TestMainWindow);
		glfwTerminate();
		return 1;
	}


	// after everything is initialized now u can do the actual graphics part
	CreateTraingle();
	CompileShaders();


	// now the actual loop logic
	while (!glfwWindowShouldClose(TestMainWindow))
	{
		// to get all events 
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// do the actual logic of implimenting the shaders
		glUseProgram(Shader);
		glBindVertexArray(Vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);
		// for no flicker 
		glfwSwapBuffers(TestMainWindow);
	}

	glfwDestroyWindow(TestMainWindow);
	glfwTerminate();
	return 0;
}