//
// Created by lpc0503 on 2020/10/14.
//
#pragma once

#ifndef RAY_CASTING_GAME_H
#define RAY_CASTING_GAME_H

#include <pch.h>
#include <Block.h>
#include <Light.h>
#include <Framebuffer.h>
#include <OrthographicCamera.h>

class Game {

public:

    static Game& Get() { return *s_instance; }
    static Game *s_instance;
    sf::RenderWindow& GetWindow() { return window; }
    float GetWidth() { return width_; }
    float GetHeight() { return height_; }
    sf::Vector2u GetWindowSize() {return window.getSize(); }

    std::vector<std::shared_ptr<Block>> GetBlocks() { return blocks; }
    std::vector<std::shared_ptr<Light>> GetLights() { return lights; }

    Game(float width, float height);

    ~Game() = default;

    void init();

    void processInput(float dt);

    void update(float dt);

    void render();

    void onImGuiRender(float dt);

    uint32_t loadTexture(std::string path);

    p2 rotate(float angle);

    p2 cast(p2 lightPos, p2 dir, p2 pos1, p2 pos2);

    void run();

private:


    std::vector<std::pair<p2, p2>> viewPortCoord;

    std::shared_ptr<OrthographicCamera> camera;

    float width_, height_;
    sf::RenderWindow window;

    uint32_t textureID;

    std::vector<std::shared_ptr<Light>> lights;
    std::vector<std::shared_ptr<Block>> blocks;
    std::shared_ptr<Framebuffer> lightFBO;
    std::shared_ptr<Framebuffer> worldFBO;
    std::shared_ptr<Framebuffer> fbo3;
    int index;
    p4 ambientMask;
};


#endif //RAY_CASTING_GAME_H
