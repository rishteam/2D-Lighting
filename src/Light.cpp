//
// Created by icejj on 2020/10/15.
//
#include <Light.h>
#include <Game.h>

int Light::id = 0;

Light::Light(p2 pos, p4 color, float r, float constant, float linear, float quadratic) : pos(pos), color(color), radius(r), constant(constant), linear(linear), quadratic(quadratic){

    tag = "Light " + std::to_string(id++);
}

void Light::onUpdate(float dt) {


    if(traceMouse)
        pos = {sf::Mouse::getPosition(Game::Get().GetWindow()).x, sf::Mouse::getPosition(Game::Get().GetWindow()).y};
}

