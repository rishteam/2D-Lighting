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

    void run();

private:

    float width_, height_;
    sf::RenderWindow window;

    uint32_t textureID;

    std::vector<std::shared_ptr<Light>> lights;
    std::vector<std::shared_ptr<Block>> blocks;
    std::shared_ptr<Framebuffer> fbo;
    std::shared_ptr<Framebuffer> fbo2;
    std::shared_ptr<Framebuffer> fbo3;
    std::shared_ptr<Framebuffer> fbo4;
    int index;
};


#endif //RAY_CASTING_GAME_H
