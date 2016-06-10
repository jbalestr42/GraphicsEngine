#include "Windows.hpp"
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
	light.rotateY(40.f);
	light.rotateX(40.f);
	light.setPosition(light.getRotation());

	Model model("resources/Trex/TrexByJoel3d.fbx");
	Model cube("resources/cube.obj");
	cube.translate({0.f, -1.f, 0.f});
	cube.scale({20.f, 0.5f, 20.f});

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
		glViewport(0, 0, map.getWidth(), map.getHeight());
		map.bindFrameBuffer(); // for each directionnal light
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Matrix lightProj = Matrix::orthographicProjection(-10.f, 10.f, -10.f, 10.f, 1.f, 100.f);
		Matrix lightView = Matrix::lookAt(light.getPosition(), {0.f, 0.f, 1.f}, {0.f, 1.f, 0.f});
		Matrix viewProj = lightProj * lightView;
		depth->setParameter("LightViewProjMatrix", viewProj);
		model.draw(*depth);
		cube.draw(*depth);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, win.getWidth(), win.getHeight());
		win.clear();
		screen->bind();
		map.draw();

		//// set shaders parameters
		//phong->setParameter("ProjectionMatrix", camera.getProjectionMatrix());
		//phong->setParameter("ViewMatrix", camera.getViewMatrix());
		//phong->setParameter("view_position", camera.getPosition());
		//phong->setParameter("directional_light_count", lights.getDirectionalLightCount());
		//phong->setParameter("directional_lights", lights.getDirectionalLight());
		//phong->setParameter("point_light_count", lights.getPointLightCount());
		//phong->setParameter("point_lights", lights.getPointLight());

		//// draw models
		//model.draw(*phong);
		//cube.draw(*phong);

		win.display();
		win.pollEvents();
	}
	return (0);
}
