#include "Windows.hpp"
#include "Texture.hpp"
#include "ResourceManager.hpp"
#include "Keyboard.hpp"
#include "Model.hpp"
#include "LightManager.hpp"
#include "Camera.hpp"
#include "ShadowMap.hpp"
#include "DebugDraw.hpp"
#include <iostream>
#include <cmath>

#include "Enums.hpp"
#include "Vector4.hpp"
#include "Math.hpp"
#include <limits>
#include <algorithm>

int main(void)
{
	Windows win(800, 600, "test graphics");
	win.setClearColor(Color::White);

	Camera camera;
	Camera camera2;
	camera2.setFarPlane(1000.0f);
	ShadowMap map;
	map.init();

	std::shared_ptr<Shader> phong = ResourceManager::getInstance().getShader(0);
	std::shared_ptr<Shader> depth = ResourceManager::getInstance().addShader(1, "resources/depth.frag", "resources/depth.vert");
	std::shared_ptr<Shader> screen = ResourceManager::getInstance().addShader(2, "resources/render_depth_map.frag", "resources/render_texture.vert");
	std::shared_ptr<Shader> debug = ResourceManager::getInstance().getShader(-1);

	LightManager lights;
	DirectionalLight & light = lights.createDirectionalLight(Color(1.0f, 1.0f, 1.0f, 1.f));
	light.rotateX(40.f);
	SpotLight & light3 = lights.createSpotLight(Color(0.0f, 1.0f, 0.0f, 1.f), Vector3(3.f, 12.f, 4.f));
	light.rotateY(40.f);
	light3.rotateX(-45.f);
	light3.setAngles(12.5f, 13.5f);

	Model model("resources/Trex/TrexByJoel3d.fbx");
	Model ground("resources/cube.obj");
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

		//		model.rotateY(dt * 40.f);

		while (dt > frameLimit)
			dt -= frameLimit;

		// Draw
		Matrix viewProj;
		Matrix camInv = camera.getViewMatrix().inverse();
			// TODO change the directionnal light direction and the relative stuff in shaders
		Matrix lightView = Matrix::lookAt({0.f, 0.f, 0.f}, -light.getRotatedDirection().normalize(), {0.f, 1.f, 0.f});

		// Retrieve data from IView
		float ar = static_cast<float>(win.getWidth()) / static_cast<float>(win.getHeight());
		float fov = 60.f;
		float near = 0.1f;
		float far = 100.f;
		float cascadeEnd[2] = { near, far };
		std::size_t cascadeCount = 1;

		float halfHeight = std::tan(Deg2Rad * (fov / 2.f));
		float halfWidth = halfHeight * ar;

		for (std::size_t i = 0u; i < cascadeCount; i++)
		{
			float xf = halfWidth * cascadeEnd[i + 1];
			float yf = halfHeight * cascadeEnd[i + 1];

			Vector4 farthestCorner = Vector4(xf, yf, -far, 1.f);

			// Frustum center in camera space
			Vector4 fc = Vector4(0.f, 0.f, -(near + far) / 2.f, 1.f);
			// Frustum center in world space
			Vector4 fcW = camInv * fc;

			// Distance from center to farthest corner used to compute the ortho projection
			float dist = (farthestCorner - fc).length();
			Matrix lightProj = Matrix::orthographicProjection(fcW.x - dist, fcW.x + dist, fcW.y - dist, fcW.y + dist, -fcW.z - dist, -fcW.z + dist);
			viewProj = lightProj * lightView;
		}

		// Draw the shadowmap in a new texture
		map.setViewport();
		map.bind();
		map.clear();

		depth->setParameter("LightViewProjMatrix", viewProj);
		model.draw(*depth);
		ground.draw(*depth);

		// Draw in the window
		win.setViewport();
		win.bind();
		win.clear();

		phong->setParameter("ProjectionMatrix", drawCamera->getProjectionMatrix());
		phong->setParameter("ViewMatrix", drawCamera->getViewMatrix());
		phong->setParameter("view_position", drawCamera->getPosition());
		phong->setParameter("directional_light_count", lights.getDirectionalLightCount());
		phong->setParameter("directional_lights", lights.getDirectionalLight());
		phong->setParameter("point_light_count", lights.getPointLightCount());
		phong->setParameter("point_lights", lights.getPointLight());
		phong->setParameter("spot_light_count", lights.getSpotLightCount());
		phong->setParameter("spot_lights", lights.getSpotLight());

		phong->setParameter("LightViewProjMatrix", viewProj);
		phong->setParameter("shadow_map", ShadowMapIndex);
		map.bindTexture(); // Bind for reading

		//DebugDraw::getInstance().drawTransform(light);
		//DebugDraw::getInstance().drawTransform(cube);

		// draw models
		ground.draw(*phong);
		model.draw(*phong);

		if (Keyboard::isKeyPress(GLFW_KEY_H))
			map.draw(*screen);

		debug->setParameter("ProjectionMatrix", drawCamera->getProjectionMatrix());
		debug->setParameter("ViewMatrix", drawCamera->getViewMatrix());

		DebugDraw::getInstance().drawOrthogonalProjection(viewProj);
		DebugDraw::getInstance().drawFrustum(camera);

		win.display();
		win.pollEvents();
	}
	return (0);
}
