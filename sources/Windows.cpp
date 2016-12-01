#include "Windows.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Color.hpp"
#include <iostream>

Windows::Windows(void) :
	Windows(800u, 600u, "OpenGL Window")
{ }

Windows::Windows(std::size_t width, std::size_t height, char const * title) :
	RenderTarget(width, height),
	m_window(nullptr),
	m_title(title)
{
	RenderTarget::setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	init();
}

Windows::~Windows(void)
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Windows::init(void)
{
	glfwSetErrorCallback(errorCallback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_SAMPLES, 4); // antialiasing
#if defined(__APPLE__) || defined(MACOSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#else
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
#endif
	glfwWindowHint(GLFW_DEPTH_BITS, 16);

	glfwWindowHint(GLFW_RESIZABLE, true);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// if debug
	// glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	m_window = glfwCreateWindow(getWidth(), getHeight(), m_title, NULL, NULL);
	if (!m_window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(m_window, Keyboard::callback);
	glfwSetMouseButtonCallback(m_window, Mouse::callbackButton);
	glfwSetCursorPosCallback(m_window, Mouse::callbackPosition);
	glfwSetFramebufferSizeCallback(m_window, resizeCallback);

	glfwMakeContextCurrent(m_window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	glGetError(); // Avoid first error coming from nowhere
	glfwSwapInterval(1); // vsync
	int width = 0;
	int height = 0;
	glfwGetFramebufferSize(m_window, &width, &height);
	setWidth(width);
	setHeight(height);
	resizeCallback(m_window, getWidth(), getHeight());
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Windows::bind(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool Windows::isOpen(void) const
{
	return (!glfwWindowShouldClose(m_window));
}

void Windows::display(void) const
{
	glfwSwapBuffers(m_window);
}

void Windows::pollEvents(void) const
{
	glfwPollEvents();
}

void Windows::close(void)
{
	glfwSetWindowShouldClose(m_window, true);
}

GLFWwindow * Windows::getWindow(void) const
{
	return m_window;
}

void Windows::errorCallback(int error, char const * description)
{
	std::cerr << "ERROR " << error << " : " << description << std::endl;
}

void Windows::resizeCallback(GLFWwindow * window, int width, int height)
{
	(void)window;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}
