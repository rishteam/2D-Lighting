//
// Created by lpc0503 on 2020/10/22.
//
#include <Block.h>

Block::Block(p2 pos, p2 size) : pos(pos), size(size) {

}

std::vector<p2> Block::getVertices() {

    return std::vector<p2> {pos, {pos.x, pos.y+size.y}, {pos.x + size.x, pos.y+size.y}, {pos.x + size.x, pos.y}};
}
