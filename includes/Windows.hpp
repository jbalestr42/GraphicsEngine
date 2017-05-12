#ifndef WINDOWS_HPP
# define WINDOWS_HPP

# if defined(__APPLE__) || defined(MACOSX)
#  include <OpenGL/gl3.h>
# else
#  include <GL/glew.h>
# endif
# include <GLFW/glfw3.h>
# include <memory>

class Color;

class Windows
{
public:
	Windows(void);
	Windows(int width, int height, char const * title);
	virtual ~Windows(void);

	bool isOpen(void) const;
	void clear(void) const;
	void display(void) const;
	void pollEvents(void) const;
	void close(void);

	GLFWwindow * getWindow(void) const;

	void setClearColor(Color const & color);

private:
	GLFWwindow *			m_window;
	int						m_width;
	int						m_height;
	char const *			m_title;

	Windows(Windows const & windows);
	Windows & operator=(Windows const & windows);

	void init(void);
	static void errorCallback(int error, char const * description);
	static void resizeCallback(GLFWwindow * window, int width, int height);

};

#endif
