#include <iostream>
// include the OPENGL Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint Width = 800, Height = 800;

int main()
{
	// first initialize glfw

	if (!glfwInit())
	{
		// the initialization failed
		std::cout << "The Glfw Initialization Failed" << std::endl;
		glfwTerminate();
		return 1;
	}

	// set the Properties of the window before creating
	//1) OpenGl Version for the context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// 2) setup to no deprecating value
	//3)Only Allow Forward Features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the actual window
	GLFWwindow* TestMainWindow = glfwCreateWindow(Width, Height, "Test Main Window", NULL, NULL);

	if (!TestMainWindow)
	{
		// window Initialization failed
		std::cout << "Window Initialization Failed " << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(TestMainWindow);

	// before u can create ur GLEW u need to get the Buffer size to actually know where to draw the pixels
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(TestMainWindow, &BufferWidth, &BufferHeight);

	// u can Create ur viiewPort 

	glViewport(0, 0, BufferWidth, BufferHeight);

	

	// setup the code so that u can use the extra features
	glewExperimental = GL_TRUE;

	// now u can Initialize ur GLEW
	if (glewInit() != GLEW_OK)
	{
		// ur GLEW Initialization Failed
		std::cout << "GLEW Initialization Failed" << std::endl;
		glfwDestroyWindow(TestMainWindow);
		glfwTerminate();
		return 1;

	}


	// the main loop for the rendered

	while (!glfwWindowShouldClose(TestMainWindow))
	{
		// to get all events 
		glfwPollEvents();
       
		// at nothing u can st atlest the clear window 
		glClearColor(0.32f, 0.12f, 0.25f, 1.0f);
		// set the color data to be zero
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(TestMainWindow);
		
	}

	return 0;
}