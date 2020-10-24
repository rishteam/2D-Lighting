//
// Created by icejj on 2020/10/15.
//
#include <Light.h>
#include <Game.h>

Light::Light(p2 pos, p4 color, float r) : pos(pos), color(color), radius(r) {

}

void Light::onUpdate(float dt) {

    pos = {sf::Mouse::getPosition(Game::Get().GetWindow()).x, sf::Mouse::getPosition(Game::Get().GetWindow()).y};
}

