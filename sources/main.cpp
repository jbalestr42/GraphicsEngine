#include "Windows.hpp"
#include "ResourceManager.hpp"
#include "Keyboard.hpp"
#include "Model.hpp"
#include "LightManager.hpp"
#include "Camera.hpp"
#include <iostream>

int main(void)
{
	Windows win(800, 600, "test graphics");
	win.setClearColor(Color::White);

	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	win.setView(camera);

	LightManager lights;
	DirectionalLight & light = lights.createDirectionalLight(Color(1.0f, 0.0f, 1.0f, 1.f), 0.2f, 1.f);
	DirectionalLight & light2 = lights.createDirectionalLight(Color(0.2f, 1.0f, 1.0f, 1.f), 0.2f, 1.f);
	light.rotateY(40.f);
	light2.rotateY(40.f);
	light2.rotateZ(40.f);

	Model model("resources/Trex/TrexByJoel3d.fbx");

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
		lights.update();
		camera->update(dt);

		model.rotateY(dt * 40.f);
		//light.rotateX(10.f * dt);
		//light.rotateZ(50.f * dt);
		//light.rotateY(-50.f * dt);

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
