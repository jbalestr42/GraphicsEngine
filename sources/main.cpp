#include "Windows.hpp"
#include "Texture.hpp"
#include "ResourceManager.hpp"
#include "Keyboard.hpp"
#include "Model.hpp"
#include "LightManager.hpp"
#include "Camera.hpp"
#include <iostream>
#include <cmath>

# include "FrameBuffer.hpp"
class ShadowMap : public FrameBuffer
{
public:
	ShadowMap(void) :
		FrameBuffer(1024u, 1024u)
	{}

	virtual ~ShadowMap(void) = default;

	virtual void initTextureParam(void)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, getWidth(), getHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		createFrameBuffer(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
};

#include "Vector4.hpp"
#include <limits>
#include "Math.hpp"
int main(void)
{
	Windows win(800, 600, "test graphics");
	win.setClearColor(Color::White);

	Camera camera;
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

	glfwSetTime(0.f);
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

		model.rotateY(dt * 40.f);
		light2.translate(Vector3(0.05f, 0.f, 0.f) * std::cos(timer));

		while (dt > frameLimit)
			dt -= frameLimit;

		// Draw
		Matrix viewProj;
		Matrix cam = camera.getViewMatrix();
		Matrix camInv = cam.inverse();

		float ar = 600.f / 800.f;
		float fov = 60.f;
		float near = 0.1f;
		float far = 100.f;
		float cascadeEnd[2] = { near, far };
		std::size_t cascadeCount = 1;

		float halfWidth = tanf(Deg2Rad * fov / 2.f);
		float halfHeight = tanf(Deg2Rad * fov * ar / 2.f);

		//std::cout << "test " << Vector4(0.5f, 0.1f, 0.5f, 1.f) * cam << std::endl;
		//std::cout << cam << std::endl;
		//std::cout << camInv << std::endl;
		//std::cout << camera.getPosition() << std::endl;

		Matrix lightView;

		for (std::size_t i = 0u; i < cascadeCount; i++)
		{
			float xn = halfWidth * cascadeEnd[i];
			float xf = halfWidth * cascadeEnd[i + 1];
			float yn = halfHeight * cascadeEnd[i];
			float yf = halfHeight * cascadeEnd[i + 1];
			std::size_t NUM_FRUSTUM_CORNERS = 8;

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

			lightView = Matrix::lookAt(Vector3(0.f, 0.f, 0.f), light.getRotatedDirection(), {0.f, 1.f, 0.f});

			Vector4 frustumCornersL[NUM_FRUSTUM_CORNERS];

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
			for (std::size_t j = 0 ; j < NUM_FRUSTUM_CORNERS ; j++)
			{
				// Transform the frustum coordinate from view to world space
				Vector4 vW = frustumCorners[j] * camInv;
				//std::cout << j << " - " << frustumCorners[j] << std::endl;
				//std::cout << " - " << vW << std::endl;

				// Transform the frustum coordinate from world to light space
				frustumCornersL[j] = vW * lightView;
				//std::cout << " - " << frustumCornersL[j] << std::endl;

				minX = std::min(minX, frustumCornersL[j].x);
				maxX = std::max(maxX, frustumCornersL[j].x);
				minY = std::min(minY, frustumCornersL[j].y);
				maxY = std::max(maxY, frustumCornersL[j].y);
				minZ = std::min(minZ, frustumCornersL[j].z);
				maxZ = std::max(maxZ, frustumCornersL[j].z);
			}
			Matrix lightProj = Matrix::orthographicProjection(minX, maxX, minY, maxY, minZ, maxZ);
			viewProj = lightProj * lightView;
		}

		//Matrix lightProj = Matrix::orthographicProjection(-10.f, 10.f, -10.f, 10.f, 1.f, 100.f);
		//Matrix lightView = Matrix::lookAt(light.getPosition(), light.getDirection(), {0.f, 1.f, 0.f});
		//Matrix viewProj = lightProj * lightView;

		glViewport(0, 0, map.getWidth(), map.getHeight());
		map.bindFrameBuffer(); // Bind for wirting
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);

		depth->setParameter("LightViewProjMatrix", viewProj);
		model.draw(*depth);
		cube.draw(*depth);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glViewport(0, 0, win.getWidth(), win.getHeight());
		win.clear();
		// set shaders parameters
		phong->setParameter("ProjectionMatrix", camera.getProjectionMatrix());
		phong->setParameter("ViewMatrix", camera.getViewMatrix());
		phong->setParameter("view_position", camera.getPosition());
		phong->setParameter("directional_light_count", lights.getDirectionalLightCount());
		phong->setParameter("directional_lights", lights.getDirectionalLight());
		phong->setParameter("point_light_count", lights.getPointLightCount());
		phong->setParameter("point_lights", lights.getPointLight());

		phong->setParameter("LightMatrix", viewProj);

		phong->setParameter("shadow_map", 2);
		glActiveTexture(GL_TEXTURE0 + 2);
		map.bindTexture(); // Bind for reading

		// draw models
		model.draw(*phong);
		cube.draw(*phong);

		if (Keyboard::isKeyPress(GLFW_KEY_H))
		{
			screen->bind();
			map.draw(*screen);
		}

		win.display();
		win.pollEvents();
	}
	return (0);
}
