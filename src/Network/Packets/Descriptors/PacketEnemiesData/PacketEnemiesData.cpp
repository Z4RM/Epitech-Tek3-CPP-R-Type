/*
** EPITECH PROJECT, 2025
** RType
** File description:
** PacketEnemiesData.cpp
*/

#include "PacketEnemiesData.hpp"

namespace rtype::network {

    std::vector<char> PacketEnemiesData::bufferize() const {
        size_t size = 0;
        int dataCount = datas.size();

        for (const auto &data : datas) {
            size += sizeof(data.netId.id);
            size += sizeof(data.pos.x) + sizeof(data.pos.y);
            size += sizeof(data.size.width) + sizeof(data.size.height);
            size += sizeof(data.vel.x) + sizeof(data.vel.y);
            size += sizeof(data.health);
            size += sizeof(data.type);
        }

        std::vector<char> buffer(sizeof(this->_code) + sizeof(dataCount) + size);

        std::memcpy(buffer.data(), &this->_code, sizeof(this->_code));
        std::memcpy(buffer.data() + sizeof(this->_code), &dataCount, sizeof(dataCount));

        size_t currentSize = sizeof(this->_code) + sizeof(dataCount);
        for (const auto &data : datas) {
            std::memcpy(buffer.data() + currentSize, &data.netId.id, sizeof(data.netId.id));
            currentSize += sizeof(data.netId.id);

            std::memcpy(buffer.data() + currentSize, &data.pos.x, sizeof(data.pos.x));
            currentSize += sizeof(data.pos.x);
            std::memcpy(buffer.data() + currentSize, &data.pos.y, sizeof(data.pos.y));
            currentSize += sizeof(data.pos.y);

            std::memcpy(buffer.data() + currentSize, &data.size.width, sizeof(data.size.width));
            currentSize += sizeof(data.size.width);
            std::memcpy(buffer.data() + currentSize, &data.size.height, sizeof(data.size.height));
            currentSize += sizeof(data.size.height);

            std::memcpy(buffer.data() + currentSize, &data.vel.x, sizeof(data.vel.x));
            currentSize += sizeof(data.vel.x);
            std::memcpy(buffer.data() + currentSize, &data.vel.y, sizeof(data.vel.y));
            currentSize += sizeof(data.vel.y);

            std::memcpy(buffer.data() + currentSize, &data.health, sizeof(data.health));
            currentSize += sizeof(data.health);

            std::memcpy(buffer.data() + currentSize, &data.type, sizeof(data.type));
            currentSize += sizeof(data.type);
        }

        return buffer;
    }

    void PacketEnemiesData::fillData(const std::vector<char> &buffer) {
        size_t currentSize = sizeof(this->_code) + sizeof(int);
        int dataCount = 0;
        std::memcpy(&dataCount, buffer.data() + sizeof(this->_code), sizeof(dataCount));

        datas.resize(dataCount);

        for (int i = 0; i < dataCount; ++i) {
            std::memcpy(&datas[i].netId.id, buffer.data() + currentSize, sizeof(datas[i].netId.id));
            currentSize += sizeof(datas[i].netId.id);

            std::memcpy(&datas[i].pos.x, buffer.data() + currentSize, sizeof(datas[i].pos.x));
            currentSize += sizeof(datas[i].pos.x);
            std::memcpy(&datas[i].pos.y, buffer.data() + currentSize, sizeof(datas[i].pos.y));
            currentSize += sizeof(datas[i].pos.y);

            std::memcpy(&datas[i].size.width, buffer.data() + currentSize, sizeof(datas[i].size.width));
            currentSize += sizeof(datas[i].size.width);
            std::memcpy(&datas[i].size.height, buffer.data() + currentSize, sizeof(datas[i].size.height));
            currentSize += sizeof(datas[i].size.height);

            std::memcpy(&datas[i].vel.x, buffer.data() + currentSize, sizeof(datas[i].vel.x));
            currentSize += sizeof(datas[i].vel.x);
            std::memcpy(&datas[i].vel.y, buffer.data() + currentSize, sizeof(datas[i].vel.y));
            currentSize += sizeof(datas[i].vel.y);

            std::memcpy(&datas[i].health, buffer.data() + currentSize, sizeof(datas[i].health));
            currentSize += sizeof(datas[i].health);

            std::memcpy(&datas[i].type, buffer.data() + currentSize, sizeof(datas[i].type));
            currentSize += sizeof(datas[i].type);
        }
    }
}
