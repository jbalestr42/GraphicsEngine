#include "Mouse.hpp"

std::map<int, int>	Mouse::m_buttons;
Vector2				Mouse::m_position;
Vector2				Mouse::m_normalizedPosition;

void Mouse::callbackButton(GLFWwindow *, int button, int action, int)
{
	if (action != GLFW_REPEAT)
		m_buttons[button] = action;
}

void Mouse::callbackPosition(GLFWwindow * window, double posX, double posY)
{
	int width;
	int height;
	glfwGetWindowSize(window, &width, &height);
	m_position.x = posX;
	m_position.y = posY;
	m_normalizedPosition.x = posX / static_cast<float>(width);
	m_normalizedPosition.y = posY / static_cast<float>(height);
}

bool Mouse::isButtonPressed(int button)
{
	if (m_buttons.find(button) != m_buttons.end())
	{
		if (m_buttons[button] == GLFW_PRESS)
		{
			m_buttons[button] = GLFW_REPEAT;
			return true;
		}
	}
	return false;
}

bool Mouse::isButtonPress(int button)
{
	if (m_buttons.find(button) != m_buttons.end())
	{
		if (m_buttons[button] == GLFW_PRESS)
			m_buttons[button] = GLFW_REPEAT;
		if (m_buttons[button] == GLFW_REPEAT)
			return true;
	}
	return false;
}

bool Mouse::isButtonReleased(int button)
{
	if (m_buttons.find(button) != m_buttons.end())
	{
		if (m_buttons[button] == GLFW_RELEASE)
		{
			m_buttons[button] = -1;
			return true;
		}
	}
	return false;
}

Vector2 const & Mouse::getPosition(void)
{
	return (m_position);
}
