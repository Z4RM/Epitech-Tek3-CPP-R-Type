/*
** EPITECH PROJECT, 2025
** RType
** File description:
** PacketPlayersData.cpp
*/

#include "PacketPlayersData.hpp"

namespace rtype::network {

    std::vector<char> PacketPlayersData::bufferize() const {
        size_t size = 0;
        int dataCount = datas.size();

        for (const auto &data : datas) {
            size += sizeof(data.netId.id);
            size += sizeof(data.pos.x);
            size += sizeof(data.pos.y);
            size += sizeof(data.pos.z);
            size += sizeof(data.size.width);
            size += sizeof(data.size.height);
            size += sizeof(data.vel.x);
            size += sizeof(data.vel.y);
            size += sizeof(data.vel.z);
            size += sizeof(data.health);
            size += sizeof(data.bonuses.size());
            int bonusesCount = data.bonuses.size();
            size += bonusesCount;
            for (auto &bonus : data.bonuses) {
                size += sizeof(bonus);
            }
        }

        std::vector<char> buffer(sizeof(this->_code) + sizeof(int) + size);

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

            std::memcpy(buffer.data() + currentSize, &data.pos.z, sizeof(data.pos.z));
            currentSize += sizeof(data.pos.z);

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

            int bonusCount = data.bonuses.size();
            std::memcpy(buffer.data() + currentSize, &bonusCount, sizeof(bonusCount));
            currentSize += sizeof(bonusCount);

            for (auto &bonus: data.bonuses) {
                std::memcpy(buffer.data() + currentSize, &bonus, sizeof(bonus));
                currentSize += sizeof(bonus);
            }
        }

        return buffer;
    }

    void PacketPlayersData::fillData(const std::vector<char> &buffer) {
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

            std::memcpy(&datas[i].pos.z, buffer.data() + currentSize, sizeof(datas[i].pos.z));
            currentSize += sizeof(datas[i].pos.z);

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

            int bonusCount = 0;
            std::memcpy(&bonusCount, buffer.data() + currentSize, sizeof(bonusCount));
            currentSize += sizeof(bonusCount);
            datas[i].bonuses.resize((bonusCount));

            for (int j = 0; j < bonusCount; j++) {
                std::memcpy(&datas[i].bonuses[j], buffer.data() + currentSize, sizeof(datas[i].bonuses[j]));
                currentSize += sizeof(datas[i].bonuses[j]);
            }
        }
    }
}
