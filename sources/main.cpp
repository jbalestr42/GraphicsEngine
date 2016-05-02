#include "Windows.hpp"
#include "ResourceManager.hpp"
#include "Shader.hpp"
#include "Keyboard.hpp"
#include <iostream>

int main(void)
{
	Windows win(800, 600, "test graphics");
	win.setClearColor(Color::White);

	Shader shader("resources/default.frag" ,"resources/default.vert");
	Matrix m_view;
	Matrix m_projection;
	m_view.translate(Vector3(0.f, 0.f, 0.0f));
	shader.setParameter("ViewMatrix", m_view);
	m_projection.perspectiveProjection(60.f, 800.f / 600.f, 0.1f, 100.f);
	shader.setParameter("ProjectionMatrix", m_projection);

	std::shared_ptr<Mesh> mesh = ResourceManager::getInstance().getMesh("resources/chaise.obj");

	glfwSetTime(0.f);
	float lastTime = 0.f;
	while (win.isOpen())
	{
		// Compute frametime
		float currentTime = glfwGetTime();
		float frametime = (currentTime - lastTime);
		(void)frametime;
		lastTime = currentTime;

		// Inputs
		if (Keyboard::isKeyPressed(GLFW_KEY_ESCAPE))
			win.close();

		// Update

		// Draw
		win.clear();
		if (glGetError()) // TODO remove
			std::cout << "error" << std::endl;

		win.display();
		win.pollEvents();
	}
	return (0);
}
