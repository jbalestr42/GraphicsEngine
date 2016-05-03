#include "Windows.hpp"
#include "ResourceManager.hpp"
#include "Shader.hpp"
#include "Keyboard.hpp"
#include "Mesh.hpp"
#include "DirectionalLight.hpp"
#include <iostream>

int main(void)
{
	Windows win(800, 600, "test graphics");
	win.setClearColor(Color::White);

	Shader shader("resources/ambientlight.frag" ,"resources/ambientlight.vert");
	Matrix m_view;
	Matrix m_projection;
	m_view.translate(Vector3(0.f, 0.f, -4.f));
	shader.setParameter("ViewMatrix", m_view);
	m_projection.perspectiveProjection(60.f, 800.f / 600.f, 0.1f, 100.f);
	shader.setParameter("ProjectionMatrix", m_projection);

	Mesh mesh("resources/corridor.obj");
	std::vector<DirectionalLight> dirLights;
	dirLights.emplace_back(DirectionalLight(Color(0.5f, 0.2f, 0.5f, 1.f), 1.f));
	shader.setParameter("directional_light_count", dirLights.size());
	shader.setParameter("directional_lights", dirLights);

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
		mesh.rotateY(frametime * 40.f);

		// Draw
		win.clear();
		mesh.draw(shader);

		win.display();
		win.pollEvents();
	}
	return (0);
}
