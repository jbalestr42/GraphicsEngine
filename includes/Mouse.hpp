#ifndef MOUSE_HPP
# define MOUSE_HPP

# include <map>
# include "Vector2.hpp"
# include "Windows.hpp"

class Mouse
{
public:
	static void callbackButton(GLFWwindow * window, int button, int action, int mods);
	static void callbackPosition(GLFWwindow * window, double posX, double posY);
	static bool isButtonPressed(int button);
	static bool isButtonPress(int button);
	static bool isButtonReleased(int button);
	static Vector2 const & getPosition(void);
	static Vector2 const & getNormalizedPosition(void);

private:
	Mouse(void) = default;
	virtual ~Mouse(void) = default;

	Mouse(Mouse const & mouse);
	Mouse & operator=(Mouse const & mouse);

	static std::map<int, int>	m_buttons;
	static Vector2				m_position;
	static Vector2				m_normalizedPosition;
};

#endif
