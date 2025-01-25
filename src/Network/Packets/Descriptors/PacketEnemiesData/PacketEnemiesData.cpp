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
            size += sizeof(data.netId);
            size += sizeof(data.pos);
            size += sizeof(data.size);
            size += sizeof(data.vel);
            size += sizeof(data.health);
        }

        std::vector<char> buffer(sizeof(this->_code) + sizeof(int) + size);

        std::memcpy(buffer.data(), &this->_code, sizeof(this->_code));
        std::memcpy(buffer.data() + sizeof(this->_code), &dataCount, sizeof(dataCount));

        size_t currentSize = sizeof(this->_code) + sizeof(dataCount);
        for (const auto &data : datas) {
            std::memcpy(buffer.data() + currentSize, &data.netId, sizeof(data.netId));
            currentSize += sizeof(data.netId);

            std::memcpy(buffer.data() + currentSize, &data.pos, sizeof(data.pos));
            currentSize += sizeof(data.pos);

            std::memcpy(buffer.data() + currentSize, &data.size, sizeof(data.size));
            currentSize += sizeof(data.size);

            std::memcpy(buffer.data() + currentSize, &data.vel, sizeof(data.vel));
            currentSize += sizeof(data.vel);

            std::memcpy(buffer.data() + currentSize, &data.health, sizeof(data.health));
            currentSize += sizeof(data.health);
        }

        return buffer;
    }

    void PacketEnemiesData::fillData(const std::vector<char> &buffer) {
        size_t currentSize = sizeof(this->_code) + sizeof(int);
        int dataCount = 0;
        std::memcpy(&dataCount, buffer.data() + sizeof(this->_code), sizeof(dataCount));

        datas.resize(dataCount);

        for (int i = 0; i < dataCount; ++i) {
            std::memcpy(&datas[i].netId, buffer.data() + currentSize, sizeof(datas[i].netId));
            currentSize += sizeof(datas[i].netId);

            std::memcpy(&datas[i].pos, buffer.data() + currentSize, sizeof(datas[i].pos));
            currentSize += sizeof(datas[i].pos);

            std::memcpy(&datas[i].size, buffer.data() + currentSize, sizeof(datas[i].size));
            currentSize += sizeof(datas[i].size);

            std::memcpy(&datas[i].vel, buffer.data() + currentSize, sizeof(datas[i].vel));
            currentSize += sizeof(datas[i].vel);

            std::memcpy(&datas[i].health, buffer.data() + currentSize, sizeof(datas[i].health));
            currentSize += sizeof(datas[i].health);
        }
    }
}
