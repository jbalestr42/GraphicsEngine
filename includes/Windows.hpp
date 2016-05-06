#ifndef WINDOWS_HPP
# define WINDOWS_HPP

# include <GL/glew.h>
# include <GLFW/glfw3.h>
# include <memory>
# include "Color.hpp"

class IDrawable;
class IView;

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

	void draw(IDrawable & drawable);

	GLFWwindow * getWindow(void) const;
	IView const & getView(void) const;

	void setClearColor(Color const & color);
	void setView(std::shared_ptr<IView> view);

private:
	GLFWwindow *			m_window;
	int						m_width;
	int						m_height;
	char const *			m_title;
	std::shared_ptr<IView>	m_view;

	Windows(Windows const & windows);
	Windows & operator=(Windows const & windows);

	void init(void);
	static void errorCallback(int error, char const * description);
	static void resizeCallback(GLFWwindow * window, int width, int height);

};

#endif
