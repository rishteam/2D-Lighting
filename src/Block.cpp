//
// Created by lpc0503 on 2020/10/22.
//
#include <Block.h>

Block::Block(p2 pos, float width, float height) : pos(pos), width(width), height(height) {

}

std::vector<p2> Block::getVertices() {

    return std::vector<p2> {pos, {pos.x, pos.y+height}, {pos.x + width, pos.y+height}, {pos.x + width, pos.y}};
}
