#ifndef WINDOWS_HPP
# define WINDOWS_HPP

# include <cstddef>
# include <GLFW/glfw3.h>
# include "OpenGL.hpp"
# include "RenderTarget.hpp"

class Color;

class Windows : public RenderTarget
{
public:
	Windows(void);
	Windows(std::size_t width, std::size_t height, char const * title);
	virtual ~Windows(void);

	virtual void bind(void);
	bool isOpen(void) const;
	void display(void) const;
	void pollEvents(void) const;
	void close(void);

	GLFWwindow * getWindow(void) const;

private:
	GLFWwindow *		m_window;
	char const *		m_title;

	void init(void);
	static void errorCallback(int error, char const * description);
	static void resizeCallback(GLFWwindow * window, int width, int height);

};

#endif
