//
// Created by lpc0503 on 2020/10/14.
//
#include <Game.h>
#include <SFML/Graphics.hpp>
#include <Renderer.h>
//#include <camera.h>
//#include <player.h>

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

    Renderer::Init();

    int lightCount = 1;
    int boxCount   = 1;

    p2 mousePos = {sf::Mouse::getPosition(Game::GetWindow()).x, sf::Mouse::getPosition(Game::GetWindow()).y};
    p4 color    = {1, 0, 0, 1};
    lights.push_back(std::make_shared<Light>(mousePos, color));

    for(int i = 0 ; i < boxCount ; i++) {

        p2 size = {100, 100};
        p2 pos = {0, 0};
        blocks.push_back(std::make_shared<Block>(pos, size));
    }

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

//    for(auto light : lights) {
//
//        for(auto block : blocks) {
//
//            std::vector<p2> vertices = block->getVertices();
//            for(int i = 0 ; i < vertices.size() ; i++) {
//
//                p2 currentVertex  = vertices[i];
//                p2 nextVertex     = vertices[(i+1)%vertices.size()];
//                p2 normal         = p2(nextVertex.y, -nextVertex.x);// 法向量
//                p2 lightToCurrent = currentVertex - light->pos;
//
//                if(glm::dot(normal, lightToCurrent) > 0) {
//
//                    p2 point1 = currentVertex + lightToCurrent;
//                    p2 point2 = nextVertex + (nextVertex - light->pos);
////                    Renderer::DrawQuad(currentVertex, p2());
//                }
//            }
//        }
//    }

//    Renderer::DrawQuad(p2(100, 100), p2(100, 100), p4(1, 1, 1, 1));
    Renderer::DrawQuad(p2(0, 0), p2(0, 100), p2(100, 100), p2(100, 0), p4(1, 1, 1, 1));

    window.display();
}
