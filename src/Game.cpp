//
// Created by lpc0503 on 2020/10/14.
//
#include <Game.h>
#include <SFML/Graphics.hpp>
#include <Renderer.h>

float r() {

    return rand() / (RAND_MAX + 1.0);
}

float ri() {

    return rand() % (100) + 100;
}

Game *Game::s_instance = new Game(1280, 720);

Game::Game(float width, float height) : width_(width), height_(height), ambientMask(p4(1, 1, 1, 0.5)){

    window.create(sf::VideoMode(width, height), "Ray-Casting");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    if (!gladLoadGL())
    {
        printf("Something went wrong!\n");
        exit(-1);
    }
}

uint32_t Game::loadTexture(std::string path) {

    sf::Image img;
    img.loadFromFile(path);
    img.flipVertically();

    uint32_t t;
    glGenTextures(1, &t);
    glBindTexture(GL_TEXTURE_2D, t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)img.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    return t;
}

p2 Game::rotate(float angle) {

    p2 dir = {1, 0};
    glm::mat4 trans{1.f};
    trans = glm::rotate(trans, glm::radians(angle), p3(0.f, 0.f, 1.f));

    dir = trans * glm::vec4(dir, 0, 0);
    return dir;
}

p2 Game::cast(p2 lightPos, p2 dir, p2 pos1, p2 pos2) {

    float r_px = lightPos.x;
    float r_py = lightPos.y;
    float r_dx = dir.x;
    float r_dy = dir.y;

    float s_px = pos1.x;
    float s_py = pos1.y;
    float s_dx = pos2.x-pos1.x;
    float s_dy = pos2.y-pos1.y;

    //is parallel
    float r_mag = sqrt(r_dx*r_dx+r_dy*r_dy);
    float s_mag = sqrt(s_dx*s_dx+s_dy*s_dy);
    if(r_dx/r_mag==s_dx/s_mag && r_dy/r_mag==s_dy/s_mag)
        return {-100000, -100000};

    float Tb = (r_dx*(s_py-r_py) + r_dy*(r_px-s_px))/(s_dx*r_dy - s_dy*r_dx);
    float Ta = (s_px+s_dx*Tb-r_px)/r_dx;

    if( Ta < 0 )
        return {-100000, -100000};
    if( Tb < 0 || Tb > 1 )
        return {-100000, -100000};

    return {r_px+r_dx*Ta,r_py+r_dy*Ta};
}


void Game::init() {

    camera = std::make_shared<OrthographicCamera>(-1, 1, -1, 1);

    viewPortCoord.push_back(std::make_pair(p2{0, 0}, p2{0, height_}));
    viewPortCoord.push_back(std::make_pair(p2{0, height_}, p2{width_, height_}));
    viewPortCoord.push_back(std::make_pair(p2{width_, height_}, p2{width_, 0}));
    viewPortCoord.push_back(std::make_pair(p2{width_, 0}, p2{0, 0}));

    Renderer::Init(camera);

    srand(time(NULL));

    index = 0;
    int lightCount = 1;

    p2 mousePos = {0, 0};
    p4 color    = {1, 1, 1, 1};
    lights.push_back(std::make_shared<Light>(mousePos, color, ri()));

//    lights.push_back(std::make_shared<Light>(p2(600, 300), p4(1, 1, 1, 1), ri()));


    p2 size{0.3, 0.3};
//    blocks.push_back(std::make_shared<Block>(p2{100, 300}, size));
//    blocks.push_back(std::make_shared<Block>(p2{300, 300}, size));
//    blocks.push_back(std::make_shared<Block>(p2{500, 300}, size));
    blocks.push_back(std::make_shared<Block>(p2{-1, 0.7}, size));
    blocks.push_back(std::make_shared<Block>(p2{-1, -1}, size));
    blocks.push_back(std::make_shared<Block>(p2{0.7, 0.7}, size));
    blocks.push_back(std::make_shared<Block>(p2{0.7, -1}, size));

    lightFBO = Framebuffer::Create(FramebufferSpecification{(uint32_t)Game::Get().GetWidth(), (uint32_t)Game::Get().GetHeight()});
    worldFBO = Framebuffer::Create(FramebufferSpecification{(uint32_t)Game::Get().GetWidth(), (uint32_t)Game::Get().GetHeight()});
    fbo3 = Framebuffer::Create(FramebufferSpecification{(uint32_t)Game::Get().GetWidth(), (uint32_t)Game::Get().GetHeight()});

    textureID = loadTexture("assets/1.jpg");
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
    p2 mousePos = {sf::Mouse::getPosition(Game::Get().GetWindow()).x, sf::Mouse::getPosition(Game::Get().GetWindow()).y};

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

        if(!flag) {

            flag = true;
            for(auto light: lights) {

                if(light->traceMouse) {

                    light->traceMouse = false;
                }
            }

            for(auto block : blocks) {

                if(block->mouseTrace) {

                    block->mouseTrace = false;
                }
            }
        }
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {

        if(!flag) {

            flag = true;
            lights.push_back(std::make_shared<Light>(mousePos, p4{1, 1, 1, 1}, ri()));
        }
    }
    else flag = false;
}

void Game::update(float dt) {

    for(auto light : lights) {

        light->onUpdate(dt);
    }

    for(auto block : blocks) {

        block->onUpdate(dt);
    }
}

void Game::onImGuiRender(float dt) {

    ImGui::Begin("Light Attribute");
    {
        ImGui::ColorEdit4("Ambient", glm::value_ptr(ambientMask), ImGuiColorEditFlags_Float);

        if(ImGui::CollapsingHeader("Lights")) {

            int id = 1;
            for(auto light : lights) {

                ImGui::PushID(id++);
                if(ImGui::TreeNode(light->tag.c_str())) {

                    ImGui::DragFloat("LightMul", &light->lightMul, 0.1f, 0, FLT_MAX);

                    ImGui::PushItemWidth(100);

                    ImGui::DragFloat("##PosX", &light->pos.x, 0.1f); ImGui::SameLine();
                    ImGui::DragFloat("##PosY", &light->pos.y, 0.1f); ImGui::SameLine();
                    ImGui::Text("Light Position");
                    ImGui::PopItemWidth();
//                ImGui::Checkbox("Follow Mouse", &light->traceMouse);
//                if(!light->traceMouse) {
//
//
//                }
                    ImGui::ColorEdit4("##Color", glm::value_ptr(light->color));

                    ImGui::DragFloat("##Constant", &light->constant, 0.0001, 0.f, 1.f, "%.06f");
                    ImGui::DragFloat("##Linear", &light->linear, 0.00001, 0.f, 0.1f, "%.06f");
                    ImGui::DragFloat("##Quardratic", &light->quadratic, 0.000001, 0.f, 0.1f, "%.06f");
                    ImGui::TreePop();
                }

                ImGui::PopID();
            }
        }
    }
    ImGui::End();

    ImGui::Begin("Block");
    {
        if(ImGui::CollapsingHeader("Blocks")) {

            int id = 1;
            for(auto block : blocks) {

                ImGui::PushID(id++);

                if(ImGui::TreeNode(block->tag.c_str())) {

                    ImGui::Checkbox("Follow Mouse##block", &block->mouseTrace);
                    if(!block->mouseTrace) {

                        ImGui::PushItemWidth(100);

                        ImGui::DragFloat("##PosX##block", &block->pos.x, 0.1f); ImGui::SameLine();
                        ImGui::DragFloat("##PosY##block", &block->pos.y, 0.1f); ImGui::SameLine();
                        ImGui::Text("Block Position##block");
                        ImGui::PopItemWidth();
                    }

                    ImGui::ColorEdit4("##Color##block", glm::value_ptr(block->color));
                    ImGui::TreePop();
                }
                ImGui::PopID();
            }
        }
    }
    ImGui::End();

//    ImGui::Begin("Camera");
//    {
//        ImGui::Text("", glm::value_ptr(camera->getProjectionMatrix()));
//    }
//    ImGui::End();
}

void Game::render() {

    // Shadow
    glEnable(GL_BLEND);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    lightFBO->bind();
    lightFBO->unbind();

    lightFBO->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    Renderer::DrawQuad(p2(-1, -1), p2(-1, 1), p2(1, 1), p2(1, -1), ambientMask);
//    Renderer::DrawQuad(p2(0, 0), p2(0, 100), p2(100, 100), p2(Game::Get().GetWidth(), 0), ambientMask);
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
                    p2 point1 = (currentVertex + lightToCurrent * 10.f) ;
                    p2 point2 = (nextVertex + (nextVertex - light->pos)* 10.f) ;
                    Renderer::DrawQuad(currentVertex, point1, point2, nextVertex, p4(0, 0, 0, 0.5));
                }
            }
        }

        p2 dir1 = rotate(light->angle);
        p2 dir2 = rotate(-light->angle);
        p2 point1;
        p2 point2;
        for(int i = 0 ; i < viewPortCoord.size() ; i++) {

            p2 tmp = cast(light->pos, dir1, viewPortCoord[i].first, viewPortCoord[i].second);
            if(tmp != p2(-100000, -100000)) {

                point1 = tmp;
            }

            tmp = cast(light->pos, dir2, viewPortCoord[i].first, viewPortCoord[i].second);
            if(tmp != p2(-100000, -100000)) {

                point2 = tmp;
            }
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glStencilFunc(GL_EQUAL, 0, 1);
        glColorMask(true, true, true, true);
        Renderer::DrawLight({-1, -1}, {-1, 1}, {1, 1}, {1, -1}, *light);
//        Renderer::DrawLight(light->pos, point1, point1, point2, *light);
//        Renderer::DrawLight(p2(0, 0), p2(0, Game::Get().GetHeight()), p2(Game::Get().GetWidth(), Game::Get().GetHeight()), p2(Game::Get().GetWidth(), 0), *light);
        glClear(GL_STENCIL_BUFFER_BIT);
        glDisable(GL_BLEND);
    }
    lightFBO->unbind();

    worldFBO->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    Renderer::DrawSingleTexture(textureID); // bg
    for(auto block : blocks) {
        std::vector<p2> vertices = block->getVertices();
        Renderer::DrawQuad(vertices[0], vertices[1], vertices[2], vertices[3], block->color);
    }
    worldFBO->unbind();

    glBlendFunc(GL_ONE, GL_ONE);
    Renderer::DrawSingleTexture(worldFBO->getColorAttachmentRendererID());
//    Renderer::DrawTexture(lightFBO->getColorAttachmentRendererID(), worldFBO->getColorAttachmentRendererID());
}
