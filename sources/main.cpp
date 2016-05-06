#include "Windows.hpp"
#include "ResourceManager.hpp"
#include "Shader.hpp"
#include "Keyboard.hpp"
#include "Model.hpp"
#include "DirectionalLight.hpp"
#include "Camera.hpp"
#include <iostream>

int main(void)
{
	Windows win(800, 600, "test graphics");
	win.setClearColor(Color::White);

	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	win.setView(camera);

	Shader shader("resources/ambientlight.frag" ,"resources/ambientlight.vert");
	Model model("resources/lego/lego_poeople_obj.obj");

	std::vector<DirectionalLight> dirLights;
	dirLights.emplace_back(Color(1.0f, 1.0f, 1.0f, 1.f), 0.2f, 1.f);

	glfwSetTime(0.f);
	float lastTime = 0.f;
	float dt = 0.f;
	float frameLimit = 1.f / 300.f;
	while (win.isOpen())
	{
		// Compute frametime
		float currentTime = glfwGetTime();
		float frametime = (currentTime - lastTime);
		(void)frametime;
		lastTime = currentTime;

		dt += frametime;
		if (dt < frameLimit)
			continue ;

		// Inputs
		if (Keyboard::isKeyPressed(GLFW_KEY_ESCAPE))
			win.close();

		// Update
		camera->update(dt);

		//model.rotateY(dt * 40.f);
		//dirLights[0].rotateX(10.f * dt);
		//dirLights[0].rotateZ(50.f * dt);
		//dirLights[0].rotateY(-50.f * dt);
		//shader.setParameter("directional_light_count", dirLights.size());
		//shader.setParameter("directional_lights", dirLights);
		//shader.setParameter("directional_lights", dirLights);

		while (dt > frameLimit)
			dt -= frameLimit;

		// Draw
		win.clear();
		win.draw(model);

		win.display();
		win.pollEvents();
	}
	return (0);
}
