#include "Keyboard.hpp"
#include "RenderManager.hpp"
#include "Camera.hpp"
#include "Model.hpp"
#include "DebugDraw.hpp"
#include <cmath>

int main(void)
{
	Windows win(800, 600, "test graphics");
	win.setClearColor(Color::White);

	Camera camera;
	Camera camera2;
	camera2.setFarPlane(1000.0f);

	std::shared_ptr<Shader> phong = ResourceManager::getInstance().getShader(ShaderId::Phong);
	std::shared_ptr<Shader> depth = ResourceManager::getInstance().addShader(ShaderId::Depth, "resources/depth.frag", "resources/depth.vert");
	std::shared_ptr<Shader> screen = ResourceManager::getInstance().addShader(ShaderId::Screen, "resources/render_depth_map.frag", "resources/render_texture.vert");
	std::shared_ptr<Shader> debug = ResourceManager::getInstance().getShader(ShaderId::Debug);

	RenderManager render;
	DirectionalLight & light = render.createDirectionalLight(Color(1.0f, 1.0f, 1.0f, 1.f));
	light.rotateX(40.f);
	SpotLight & light3 = render.createSpotLight(Color(0.0f, 1.0f, 0.0f, 1.f), Vector3(0.f, 10.f, 0.f));
	light.rotateY(40.f);
	light3.rotateX(-90.f);
	light3.setAngles(12.5f, 13.5f);

	Model model("resources/Trex/TrexByJoel3d.fbx");
	Model model1("resources/Trex/TrexByJoel3d.fbx");
	Model ground("resources/cube.obj");
	model.translate({0.f, -4.0f, 0.f});
	model1.translate({-15.f, 15.f, 15.f});
	ground.scale(Vector3(40.f, 2.f, 40.f));
	ground.translate(Vector3(0.f, -5.f, 0.f));

	glfwSetTime(0.f); // TODO make a Time class
	float lastTime = 0.f;
	float dt = 0.f;
	float frameLimit = 1.f / 300.f;
	float timer = 0.f;
	Camera * drawCamera = &camera;
	while (win.isOpen())
	{
		// Compute frametime
		float currentTime = static_cast<float>(glfwGetTime());
		float frametime = (currentTime - lastTime);
		lastTime = currentTime;

		dt += frametime;
		if (dt < frameLimit)
			continue ;

		timer += dt;
		// Inputs
		if (Keyboard::isKeyPressed(GLFW_KEY_ESCAPE))
			win.close();

		if (Keyboard::isKeyPressed(GLFW_KEY_1))
			drawCamera = &camera;
		else if (Keyboard::isKeyPressed(GLFW_KEY_2))
			drawCamera = &camera2;

		// Update
		drawCamera->update(dt);

		//model.rotateY(dt * 40.f);
		light3.translate({std::cos(timer) * 0.1f, 0.f, 0.f});

		while (dt > frameLimit)
			dt -= frameLimit;

		render.draw(ground, *phong);
		render.draw(model, *phong);
		render.draw(model1, *phong);

		render.display(win, *drawCamera);

		//DebugDraw::getInstance().drawTransform(light3);
		//DebugDraw::getInstance().drawTransform(model);
		//DebugDraw::getInstance().drawFrustum(camera);
		debug->setParameter("ProjectionMatrix", drawCamera->getProjectionMatrix());
		debug->setParameter("ViewMatrix", drawCamera->getViewMatrix());

		win.display();
		win.pollEvents();
	}
	return (0);
}
