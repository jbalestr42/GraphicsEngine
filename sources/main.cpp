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
#include <limits>
#include "Math.hpp"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(void)
{
	Windows win(800, 600, "test graphics");
	win.setClearColor(Color::White);

	Camera camera;
	Camera camera2;
	ShadowMap map;
	map.init();

	std::shared_ptr<Shader> phong = ResourceManager::getInstance().getShader(0);
	std::shared_ptr<Shader> depth = ResourceManager::getInstance().addShader(1, "resources/depth.frag", "resources/depth.vert");
	std::shared_ptr<Shader> screen = ResourceManager::getInstance().addShader(2, "resources/render_depth_map.frag", "resources/render_texture.vert");

	LightManager lights;
	DirectionalLight & light = lights.createDirectionalLight(Color(1.0f, 1.0f, 1.0f, 0.2f));
	PointLight & light2 = lights.createPointLight(Color(1.0f, 0.0f, 0.0f, 1.f), Vector3(2.f, 1.f, 1.f));
	light2.translate({0.f, 1.f, 0.f});
	light.rotateX(40.f);
	light.setPosition(light.getRotation());

	Model model("resources/Trex/TrexByJoel3d.fbx");
	Model cube("resources/cube.obj");
	cube.translate({0.f, -1.f, 0.f});
	cube.scale({20.f, 0.5f, 20.f});

	camera.setPosition({0.f, -10.f, -20.f});
	camera2.setPosition({0.f, -10.f, -20.f});

	glfwSetTime(0.f);
	float lastTime = 0.f;
	float dt = 0.f;
	float frameLimit = 1.f / 300.f;
	float timer = 0.f;
	Camera * drawCamera = &camera;
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

		if (Keyboard::isKeyPressed(GLFW_KEY_1))
			drawCamera = &camera;
		else if (Keyboard::isKeyPressed(GLFW_KEY_2))
			drawCamera = &camera2;

		// Update
		drawCamera->update(dt);

		model.rotateY(dt * 40.f);
		light2.translate(Vector3(0.05f, 0.f, 0.f) * std::cos(timer));

		while (dt > frameLimit)
			dt -= frameLimit;

		// Draw
		Matrix viewProj;
		Matrix camInv = camera.getViewMatrix().inverse();

		// Retrieve data from IView
		float ar = win.getWidth() / win.getHeight();
		float fov = 60.f;
		float near = 0.1f;
		float far = 100.f;
		float cascadeEnd[2] = { near, far };
		std::size_t cascadeCount = 1;

		float halfWidth = tanf(Deg2Rad * (fov / 2.f));
		float halfHeight = tanf(Deg2Rad * ((fov * ar) / 2.f));

		//std::cout << "test " << Vector4(0.5f, 0.1f, 0.5f, 1.f) * cam << std::endl;
		//std::cout << cam << std::endl;
		//std::cout << camInv << std::endl;
		//std::cout << camera.getPosition() << std::endl;

		for (std::size_t i = 0u; i < cascadeCount; i++)
		{
			float xn = halfWidth * cascadeEnd[i];
			float xf = halfWidth * cascadeEnd[i + 1];
			float yn = halfHeight * cascadeEnd[i];
			float yf = halfHeight * cascadeEnd[i + 1];
			std::size_t NUM_FRUSTUM_CORNERS = 8;

			// Calculate the 8 corners of the view frustum in world space
			Vector4 frustumCorners[NUM_FRUSTUM_CORNERS] =
			{
				// near face
				{xn, yn, cascadeEnd[i], 1.f},
				{-xn, yn, cascadeEnd[i], 1.f},
				{xn, -yn, cascadeEnd[i], 1.f},
				{-xn, -yn, cascadeEnd[i], 1.f},

				// far face
				{xf, yf, cascadeEnd[i + 1], 1.f},
				{-xf, yf, cascadeEnd[i + 1], 1.f},
				{xf, -yf, cascadeEnd[i + 1], 1.f},
				{-xf, -yf, cascadeEnd[i + 1], 1.f},
			};

			glm::vec4 fC[NUM_FRUSTUM_CORNERS] =
			{
				// near face
				{xn, yn, cascadeEnd[i], 1.f},
				{-xn, yn, cascadeEnd[i], 1.f},
				{xn, -yn, cascadeEnd[i], 1.f},
				{-xn, -yn, cascadeEnd[i], 1.f},

				// far face
				{xf, yf, cascadeEnd[i + 1], 1.f},
				{-xf, yf, cascadeEnd[i + 1], 1.f},
				{xf, -yf, cascadeEnd[i + 1], 1.f},
				{-xf, -yf, cascadeEnd[i + 1], 1.f},
			};

			Matrix lightView = Matrix::lookAt({0.f, 0.f, 0.f}, light.getRotatedDirection().normalize(), {0.f, 1.f, 0.f});
			glm::vec3 dir;
			dir.x = light.getRotatedDirection().normalize().x;
			dir.y = light.getRotatedDirection().normalize().y;
			dir.z = light.getRotatedDirection().normalize().z;
			//glm::mat4 lV = glm::lookAt(glm::vec3(0.f, 0.f, 0.f), dir, glm::vec3(0.f, 1.f, 0.f));
			glm::mat4 lV = glm::make_mat4(lightView.getMatrix());

			Vector4 frustumCornersL[NUM_FRUSTUM_CORNERS];
			glm::vec4 fCL[NUM_FRUSTUM_CORNERS];

			float minX = std::numeric_limits<float>::max();
			float maxX = std::numeric_limits<float>::min();
			float minY = std::numeric_limits<float>::max();
			float maxY = std::numeric_limits<float>::min();
			float minZ = std::numeric_limits<float>::max();
			float maxZ = std::numeric_limits<float>::min();

			//std::cout << camera.getPosition() << std::endl;
			//std::cout << camera.getRotation() << std::endl;
			//std::cout << camera.getViewMatrix() << std::endl;
			//std::cout << camera.getViewMatrix().inverse() << std::endl;
			//std::cout << camInv << std::endl;
			glm::mat4 camI = glm::make_mat4(camInv.getMatrix());
			for (std::size_t j = 0 ; j < NUM_FRUSTUM_CORNERS ; j++)
			{
				// Transform the frustum coordinate from view to world space
				Vector4 vW = camInv * frustumCorners[j];
				glm::vec4 v = camI * fC[j];
				//std::cout << j << " - " << frustumCorners[j] << std::endl;
				//std::cout << " - " << vW << " --- " << frustumCorners[j]<<std::endl;

				// Transform the frustum coordinate from world to light space
				frustumCornersL[j] = lightView * vW;
				fCL[j] = lV * v;
				//std::cout << " - " <<  frustumCornersL[j] << std::endl;

				minX = std::min(minX, frustumCornersL[j].x);
				maxX = std::max(maxX, frustumCornersL[j].x);
				minY = std::min(minY, frustumCornersL[j].y);
				maxY = std::max(maxY, frustumCornersL[j].y);
				minZ = std::min(minZ, frustumCornersL[j].z);
				maxZ = std::max(maxZ, frustumCornersL[j].z);
			}

			Matrix lightProj = Matrix::orthographicProjection(minX, maxX, minY, maxY, minZ, maxZ);
			DebugDraw::getInstance().drawOrthogonalProjection(minX, maxX, minY, maxY, minZ, maxZ);
			viewProj = lightProj * lightView;
		}

		// Draw the shadowmap in a new texture
		map.setViewport();
		map.bind(); // Bind for wirting
		map.clear();

		depth->setParameter("LightViewProjMatrix", viewProj);
		model.draw(*depth);
		cube.draw(*depth);

		// Draw in the window
		win.setViewport();
		win.bind();
		win.clear();

		// set shaders parameters
		phong->setParameter("ProjectionMatrix", drawCamera->getProjectionMatrix());
		phong->setParameter("ViewMatrix", drawCamera->getViewMatrix());
		phong->setParameter("view_position", drawCamera->getPosition());
		phong->setParameter("directional_light_count", lights.getDirectionalLightCount());
		phong->setParameter("directional_lights", lights.getDirectionalLight());
		phong->setParameter("point_light_count", lights.getPointLightCount());
		phong->setParameter("point_lights", lights.getPointLight());

		phong->setParameter("LightViewProjMatrix", viewProj);
		phong->setParameter("shadow_map", ShadowMapIndex);
		map.bindTexture(); // Bind for reading

		DebugDraw::getInstance().drawTransform(light);
		DebugDraw::getInstance().drawTransform(light2);
		DebugDraw::getInstance().drawTransform(model);
		DebugDraw::getInstance().drawTransform(cube);
		// draw models
		model.draw(*phong);
		cube.draw(*phong);

		if (Keyboard::isKeyPress(GLFW_KEY_H))
			map.draw(*screen);

		ResourceManager::getInstance().getShader(3)->setParameter("ProjectionMatrix", camera.getProjectionMatrix());
		ResourceManager::getInstance().getShader(3)->setParameter("ViewMatrix", camera.getViewMatrix());
		DebugDraw::getInstance().draw();

		win.display();
		win.pollEvents();
	}
	return (0);
}
