#include "Zap/Zap.h"
#include "Zap/Window.h"
#include "Zap/PBRenderer.h"
#include "Zap/Scene/Actor.h"
#include "Zap/ModelLoader.h"
#include "Zap/Scene/Transform.h"
#include "Zap/Scene/Component.h"
#include "Zap/Scene/MeshComponent.h"
#include "Zap/Scene/Material.h"

void resize() {
    
}

void main() {
    auto base = Zap::Base::createBase("Template Project Name");
    base->init();
    
    Zap::Window window = Zap::Window(1000, 600, "Template Title");
    window.init();

    Zap::PBRenderer renderer = Zap::PBRenderer(window);

    Zap::ModelLoader modelLoader = Zap::ModelLoader();
    auto cube = modelLoader.load("Models/Cube.obj");

    Zap::Actor actor = Zap::Actor();
    actor.addTransform(glm::mat4(1));
    actor.addMeshes(cube);
    actor.getMeshComponent(0)->m_material.m_AlbedoColor = { 1, 0.5, 0 };

    Zap::Actor light = Zap::Actor();
    light.addTransform(glm::mat4(1));
    light.getTransformComponent()->setPos({1, 2, -3});
    light.addLight({1, 1, 1});

    Zap::Actor camera = Zap::Actor();
    camera.addTransform(glm::mat4(1));
    camera.getTransformComponent()->setPos({ 0, 0, -5 });
    camera.addCamera({ 0, 0, 0 });

    renderer.setViewport(1000, 600, 0, 0);
    renderer.init();

    window.show();
    float deltaTime = 0;
    while (!window.shouldClose()) {
        auto startFrame = std::chrono::high_resolution_clock::now();

        actor.getTransformComponent()->rotate(25 * deltaTime, {1, 1, 1});

        window.clear();

        renderer.render(camera.getComponentIDs(Zap::COMPONENT_TYPE_CAMERA)[0]);

        window.swapBuffers();
        window.pollEvents();

        auto endFrame = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(endFrame - startFrame).count();
    }

    renderer.~PBRenderer();
    window.~Window();

    base->terminate();
    Zap::Base::releaseBase();

    system("pause");
}