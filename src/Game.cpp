//
// Created by lpc0503 on 2020/10/14.
//
#include <Game.h>
#include <SFML/Graphics.hpp>
#include <Renderer.h>
#include <camera.h>
#include <player.h>

Game *Game::s_instance = new Game(1280, 720);

Game::Game(float width, float height) : width_(width), height_(height){

    window.create(sf::VideoMode(width, height), "Ray-Casting");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    if (!gladLoadGL())
    {
        printf("Something went wrong!\n");
        exit(-1);
    }

}

void Game::init() {

    std::cout << "game init\n";
    Renderer::Init();
}

void Game::run() {

    bool running = true;

    std::chrono::steady_clock::time_point clock_prev = std::chrono::steady_clock::now();
    float dt;
    sf::Clock deltaClock;
    while(running) {

        sf::Event event;
        while(window.pollEvent((event))) {

            ImGui::SFML::ProcessEvent(event);

            if(event.type == sf::Event::Closed) running = false;
        }
        window.clear();

        auto clock_end = std::chrono::steady_clock::now();
        std::chrono::duration<float> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(clock_end - clock_prev);
        dt = time_span.count();
        clock_prev = clock_end;

        processInput(dt);
        update(dt);
        ImGui::SFML::Update(window, deltaClock.restart());
        onImGuiRender(dt);
        render();
    }
}

void Game::processInput(float dt) {

}

void Game::update(float dt) {

}

void Game::onImGuiRender(float dt) {

    window.clear();
    ImGui::SFML::Render(window);
}

void Game::render() {

    Renderer::DrawQuad(p2(0, 0), p2(0, 0), p4(1, 0, 0, 1));
    window.display();
}
