//
// Created by lpc0503 on 2020/10/22.
//
#include <Block.h>
#include <Game.h>

int Block::id = 0;

Block::Block(p2 pos, p2 size, p4 color) : pos(pos), size(size), color(color) ,mouseTrace(false) {

    tag = "Block" + std::to_string(id++);
}

void Block::onUpdate(float dt) {

    if(mouseTrace)
        pos = {sf::Mouse::getPosition(Game::Get().GetWindow()).x, sf::Mouse::getPosition(Game::Get().GetWindow()).y};
}


std::vector<p2> Block::getVertices() {

    return std::vector<p2> {pos, {pos.x, pos.y+size.y}, {pos.x + size.x, pos.y+size.y}, {pos.x + size.x, pos.y}};
}
