#include "Zap/Zap.h"
#include "Zap/Rendering/Window.h"
#include "Zap/Rendering/Renderer.h"
#include "Zap/Rendering/PBRenderer.h"
#include "Zap/Scene/Scene.h"
#include "Zap/Scene/Actor.h"
#include "Zap/ModelLoader.h"
#include "Zap/Scene/Transform.h"
#include "Zap/Scene/Material.h"

namespace app {
	Zap::Window* window;
	Zap::Scene* scene;
	Zap::Renderer* renderer;
	Zap::PBRenderer* pbRender;
}

void resize(GLFWwindow* window, int width, int height) {
	app::pbRender->setViewport(width, height, 0, 0);
}

void main() {
	auto base = Zap::Base::createBase("Template Project Name");
	base->init();
	
	app::window = new Zap::Window(1000, 600, "Template Title");
	app::window->init();
	app::window->setResizeCallback(resize);

	app::scene = new Zap::Scene();

	app::renderer = new Zap::Renderer(*app::window);
	app::pbRender = new Zap::PBRenderer(*app::renderer, app::scene);

	Zap::ModelLoader modelLoader = Zap::ModelLoader();
	auto cube = modelLoader.load("Models/Cube.obj");

	char texCol[] = {0xFF, 0xFF, 0xFF, 0xFF};
	modelLoader.loadTexture(&texCol, 1, 1);

	Zap::Actor actor = Zap::Actor();
	app::scene->attachActor(actor);
	actor.addTransform(glm::mat4(1));
	actor.addModel(cube);
	{
		Zap::Material mat{};
		mat.albedoColor = {1, 0.6, 0.1};
		mat.roughness = 0.3;
		mat.metallic = 0;
		actor.cmpModel_setMaterial(mat);
	}

	Zap::Actor light = Zap::Actor();
	app::scene->attachActor(light);
	light.addTransform(glm::mat4(1));
	light.cmpTransform_setPos(1, 2, -3);
	light.addLight({1, 1, 1}, 4);

	Zap::Actor light2 = Zap::Actor();
	app::scene->attachActor(light2);
	light2.addTransform(glm::mat4(1));
	light2.cmpTransform_setPos(1, -4, -2);
	light2.addLight({ 1, 0.6, 0.1 }, 3);

	Zap::Actor camera = Zap::Actor();
	app::scene->attachActor(camera);
	camera.addTransform(glm::mat4(1));
	camera.cmpTransform_setPos(0, 0, -5);
	camera.addCamera();

	app::scene->init();

	app::renderer->addRenderTemplate(app::pbRender);
	app::pbRender->setViewport(1000, 600, 0, 0);
	app::renderer->init();
	app::renderer->beginRecord();
	app::renderer->recRenderTemplate(app::pbRender);
	app::renderer->endRecord();

	app::window->show();
	float deltaTime = 0;
	while (!app::window->shouldClose()) {
		auto startFrame = std::chrono::high_resolution_clock::now();

		actor.cmpTransform_rotate(25 * deltaTime, {1, 1, 1});

		app::pbRender->updateCamera(camera);
		app::scene->update();
		app::renderer->render();

		app::window->pollEvents();

		auto endFrame = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(endFrame - startFrame).count();
	}

	app::renderer->destroy();
	delete app::renderer;
	app::scene->destroy();
	delete app::scene;
	delete app::window;

	base->terminate();
	Zap::Base::releaseBase();

	system("pause");
}