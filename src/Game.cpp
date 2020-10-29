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

    return rand() % (100) + 100;
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
    p4 color    = {1, 1, 1, 1};
    lights.push_back(std::make_shared<Light>(mousePos, color, ri()));
    p2 size{100, 100};
    blocks.push_back(std::make_shared<Block>(p2{100, 300}, size));
    blocks.push_back(std::make_shared<Block>(p2{300, 300}, size));
    blocks.push_back(std::make_shared<Block>(p2{500, 300}, size));
    blocks.push_back(std::make_shared<Block>(p2{700, 300}, size));
    blocks.push_back(std::make_shared<Block>(p2{900, 300}, size));
    blocks.push_back(std::make_shared<Block>(p2{1100, 300}, size));

    fbo = Framebuffer::Create(FramebufferSpecification{(uint32_t)Game::Get().GetWidth(), (uint32_t)Game::Get().GetHeight()});
    fbo2 = Framebuffer::Create(FramebufferSpecification{(uint32_t)Game::Get().GetWidth(), (uint32_t)Game::Get().GetHeight()});
    fbo3 = Framebuffer::Create(FramebufferSpecification{(uint32_t)Game::Get().GetWidth(), (uint32_t)Game::Get().GetHeight()});
    fbo4 = Framebuffer::Create(FramebufferSpecification{(uint32_t)Game::Get().GetWidth(), (uint32_t)Game::Get().GetHeight()});

    textureID = loadTexture("assets/1.png");
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
        window.clear(sf::Color(0, 0, 0));
        auto clock_end = std::chrono::steady_clock::now();
        std::chrono::duration<float> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(clock_end - clock_prev);
        dt = time_span.count();
        clock_prev = clock_end;

        processInput(dt);
        update(dt);
        render();
        ImGui::SFML::Update(window, deltaClock.restart());
        onImGuiRender(dt);
        ImGui::SFML::Render(window);

        window.display();
    }
}

void Game::processInput(float dt) {

    static bool flag = false;

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

        if(!flag) {

            flag = true;
            for(auto light: lights) {

                if(light->traceMouse) {

                    light->traceMouse = false;
                }
            }
        }
    }
    else {

        flag = false;
    }
}

uint32_t Game::loadTexture(std::string path) {

    sf::Image img;
    img.loadFromFile(path);
    img.flipVertically();

    uint32_t texture;
    glGenTextures(1, &texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)img.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture;
}


void Game::update(float dt) {

    for(auto light : lights) {

        light->onUpdate(dt);
    }
}

void Game::onImGuiRender(float dt) {

    ImGui::Begin("Light Attribute");

    if(ImGui::CollapsingHeader("Lights")) {

        int id = 1;
        for(auto light : lights) {

            ImGui::PushID(id++);
            if(ImGui::TreeNode(light->tag.c_str())) {

                ImGui::Checkbox("Follow Mouse", &light->traceMouse);

                if(!light->traceMouse) {

                    ImGui::PushItemWidth(100);

                    ImGui::DragFloat("##PosX", &light->pos.x, 0.1f); ImGui::SameLine();
                    ImGui::DragFloat("##PosY", &light->pos.y, 0.1f); ImGui::SameLine();
                    ImGui::Text("Light Position");
                    ImGui::PopItemWidth();
                }

                ImGui::ColorEdit4("##Color", glm::value_ptr(light->color));

                ImGui::DragFloat("##Constant", &light->constant, 0.0001, 0.f, 1.f, "%.06f");
                ImGui::DragFloat("##Linear", &light->linear, 0.00001, 0.f, 0.1f, "%.06f");
                ImGui::DragFloat("##Quardratic", &light->quadratic, 0.000001, 0.f, 0.1f, "%.06f");
                ImGui::TreePop();
            }

            ImGui::PopID();
        }

    }

    ImGui::End();
}

void Game::render() {

    // Shadow

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    fbo->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    fbo->unbind();

    fbo->bind();
    for(auto light : lights) {

        glColorMask(false, false, false, false);
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

                if(glm::dot(normal, lightToCurrent) < 0)
                {
                    p2 point1 = (currentVertex + lightToCurrent * 800.f) ;
                    p2 point2 = (nextVertex + (nextVertex - light->pos)* 800.f) ;
                    Renderer::DrawQuad(currentVertex, point1, point2, nextVertex, p4(0.0, 0.0, 0.0, 1.));
                }
            }
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glStencilFunc(GL_EQUAL, 0, 1);
        glColorMask(true, true, true, true);
        Renderer::DrawLight(p2(0, 0), p2(0, Game::Get().GetHeight()), p2(Game::Get().GetWidth(), Game::Get().GetHeight()), p2(Game::Get().GetWidth(), 0), *light);
        glClear(GL_STENCIL_BUFFER_BIT);
        glDisable(GL_BLEND);
    }
    fbo->unbind();
    fbo2->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//    glStencilFunc(GL_ALWAYS, 1, 1);
//    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    Renderer::DrawQuad(p2(0, 0), p2(0, Game::Get().GetHeight()), p2(Game::Get().GetWidth(), Game::Get().GetHeight()), p2(Game::Get().GetWidth(), 0), p4(1, 1, 1, 1));
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
    glClear(GL_STENCIL_BUFFER_BIT);
    for(auto block : blocks) {
        std::vector<p2> vertices = block->getVertices();
        Renderer::DrawQuad(vertices[0], vertices[1], vertices[2], vertices[3], p4(1, 0, 0, 1));
    }
    fbo2->unbind();


    glBlendFunc(GL_ONE, GL_ONE);
    Renderer::DrawTexture(fbo->getColorAttachmentRendererID(), fbo2->getColorAttachmentRendererID(), fbo3->getColorAttachmentRendererID());
//    Renderer::DrawSingleTexture(fbo2->getColorAttachmentRendererID());
}