#include "Windows.hpp"
#include "ResourceManager.hpp"
#include "Keyboard.hpp"
#include "Model.hpp"
#include "LightManager.hpp"
#include "Camera.hpp"
#include <iostream>
#include <cmath>

int main(void)
{
	Windows win(800, 600, "test graphics");
	win.setClearColor(Color::White);

	Camera camera;

	std::shared_ptr<Shader> phong = ResourceManager::getInstance().getShader(0);
	LightManager lights;
	DirectionalLight & light = lights.createDirectionalLight(Color(1.0f, 1.0f, 1.0f, 1.f));
	SpotLight & light3 = lights.createSpotLight(Color(0.0f, 1.0f, 0.0f, 1.f), Vector3(2.f, 3.f, 1.f));
	light.rotateY(40.f);
	light3.setAngles(40.f, 41.f);

	Model model("resources/Trex/TrexByJoel3d.fbx");
	Model ground("resources/cube.obj");
	ground.scale(Vector3(20.f, 2.f, 20.f));
	ground.translate(Vector3(0.f, -5.f, 0.f));

	glfwSetTime(0.f); // TODO make a Time class
	float lastTime = 0.f;
	float dt = 0.f;
	float frameLimit = 1.f / 300.f;
	float timer = 0.f;
	while (win.isOpen())
	{
		// Compute frametime
		float currentTime = glfwGetTime();
		float frametime = (currentTime - lastTime);
		lastTime = currentTime;

		dt += frametime;
		if (dt < frameLimit)
			continue ;

		timer += dt;
		// Inputs
		if (Keyboard::isKeyPressed(GLFW_KEY_ESCAPE))
			win.close();

		// Update
		camera.update(dt);

		//model.rotateY(dt * 40.f);
		light3.rotateX(300.f * frametime);

		while (dt > frameLimit)
			dt -= frameLimit;

		// Draw
		win.clear();

		// set shaders parameters
		phong->setParameter("ProjectionMatrix", camera.getProjectionMatrix());
		phong->setParameter("ViewMatrix", camera.getViewMatrix());
		phong->setParameter("view_position", camera.getPosition());
		phong->setParameter("directional_light_count", lights.getDirectionalLightCount());
		phong->setParameter("directional_lights", lights.getDirectionalLight());
		phong->setParameter("point_light_count", lights.getPointLightCount());
		phong->setParameter("point_lights", lights.getPointLight());
		phong->setParameter("spot_light_count", lights.getSpotLightCount());
		phong->setParameter("spot_lights", lights.getSpotLight());

		// draw models
		ground.draw(*phong);
		model.draw(*phong);

		win.display();
		win.pollEvents();
	}
	return (0);
}
