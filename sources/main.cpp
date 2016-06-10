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
		bindTexture();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, getWidth(), getHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		createFrameBuffer(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};

int main(void)
{
	Windows win(800, 600, "test graphics");
	win.setClearColor(Color::White);

	Camera camera;
	ShadowMap map;

	LightManager lights;
	DirectionalLight & light = lights.createDirectionalLight(Color(1.0f, 0.0f, 1.0f, 1.f));
	PointLight & light2 = lights.createPointLight(Color(1.0f, 0.0f, 0.0f, 1.f), Vector3(2.f, 1.f, 1.f));
	light.rotateY(40.f);

	Model model("resources/Trex/TrexByJoel3d.fbx");

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

		// Update (first lights and view)
		lights.update();
		camera.update(dt);

		model.rotateY(dt * 40.f);
		light2.translate(Vector3(0.05f, 0.f, 0.f) * std::cos(timer));
		//light.rotateX(10.f * dt);
		//light.rotateZ(50.f * dt);
		//light.rotateY(-50.f * dt);

		while (dt > frameLimit)
			dt -= frameLimit;

		// Draw
		//glViewport(0, 0, map.getWidth(), map.getHeight());
		//map.bindFrameBuffer(); // for each directionnal light
		//glClear(GL_DEPTH_BUFFER_BIT);
		////set View and projection matrices
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glViewport(0, 0, win.getWidth(), win.getHeight());
		win.clear();
		camera.setViewProjectionMatrices();
		model.draw();

		win.display();
		win.pollEvents();
	}
	return (0);
}
