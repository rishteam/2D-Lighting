//
// Created by lpc0503 on 2020/10/14.
//
#include <Game.h>
#include <SFML/Graphics.hpp>
#include <Renderer.h>
//#include <camera.h>
//#include <player.h>

float r() {

    return rand() / (RAND_MAX + 1.0);
}

float ri() {

    return rand() % (250) + 1;
}

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

    srand(time(NULL));

    index = 0;
    int lightCount = 1;

    p2 mousePos = {20, 20};
    p4 color    = {r(), r(), r(), r()};
    lights.push_back(std::make_shared<Light>(mousePos, color, ri()));
    p2 size{100, 100};
    blocks.push_back(std::make_shared<Block>(p2{100, 300}, size));
    blocks.push_back(std::make_shared<Block>(p2{300, 300}, size));
    blocks.push_back(std::make_shared<Block>(p2{500, 300}, size));
    blocks.push_back(std::make_shared<Block>(p2{700, 300}, size));
    blocks.push_back(std::make_shared<Block>(p2{900, 300}, size));
    blocks.push_back(std::make_shared<Block>(p2{1100, 300}, size));
}

void Game::run() {

    bool running = true;

    std::chrono::steady_clock::time_point clock_prev = std::chrono::steady_clock::now();
    float dt;
    sf::Clock deltaClock;
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    while(running) {

        sf::Event event;
        while(window.pollEvent((event))) {

            ImGui::SFML::ProcessEvent(event);

            if(event.type == sf::Event::Closed) running = false;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//        glClearColor(1, 1, 1, 1);

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

    static bool flag = false;


    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

        if(!flag) {

            flag = true;
            lights.push_back(std::make_shared<Light>(p2{100, 100}, p4{r(), r(), r(), 1}, ri()));
            index++;
        }
    }
    else {

        flag = false;
    }
}

void Game::update(float dt) {

    lights[index]->onUpdate(dt);
}

void Game::onImGuiRender(float dt) {

    window.clear(sf::Color(1, 1, 1));
    ImGui::SFML::Render(window);
}

void Game::render() {

//    glEnable(GL_BLEND);
//    glBlendFunc(GL_ONE, GL_ONE);
    for(auto light : lights) {

//        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
//        glStencilFunc(GL_EQUAL, 0, 1);
//        glColorMask(false, false, false, false);
        glStencilFunc(GL_ALWAYS, 1, 1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        for(auto block : blocks) {

            std::vector<p2> vertices = block->getVertices();
            for(int i = 0 ; i < vertices.size() ; i++) {

                p2 currentVertex  = vertices[i];
                p2 nextVertex     = vertices[(i+1)%vertices.size()];
                p2 edge           = nextVertex - currentVertex;
                p2 normal         = p2(edge.y, -edge.x);
                p2 lightToCurrent = currentVertex - light->pos;

                if(glm::dot(normal, lightToCurrent) > 0) {

                    p2 point1 = (currentVertex + lightToCurrent * 800.f) ;
                    p2 point2 = (nextVertex + (nextVertex - light->pos)* 800.f) ;
                    Renderer::DrawQuad(currentVertex, point1, point2, nextVertex, p4(0, 0.0, 0.0, 1));
                }
            }
        }
    }

    for(auto block : blocks) {

        std::vector<p2> vertices = block->getVertices();
        Renderer::DrawQuad(vertices[0], vertices[1], vertices[2], vertices[3], p4(1, 0, 0, 1.));
    }

    glColorMask(true, true, true, true);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    for(auto light : lights) {
        // p2(0, 0), p2(0, Game::Get().GetHeight()), p2(Game::Get().GetWidth(), Game::Get().GetHeight()), p2(Game::Get().GetWidth(), 0)
        // light->pos, p2(light->pos.x, light->pos.y + 100), p2(light->pos.x + 100, light->pos.y + 100)
        Renderer::DrawLight(p2(0, 0), p2(0, Game::Get().GetHeight()), p2(Game::Get().GetWidth(), Game::Get().GetHeight()), p2(Game::Get().GetWidth(), 0), *light);
    }
    glDisable(GL_BLEND);


    window.display();
}
