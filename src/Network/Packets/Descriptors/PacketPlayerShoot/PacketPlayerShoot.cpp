/*
** EPITECH PROJECT, 2025
** RType
** File description:
** packet playershoot
*/

#include "PacketPlayerShoot.hpp"
namespace rtype::network {
    std::vector<char> PacketPlayerShoot::bufferize() const {
        // Calcul de la taille totale du buffer à envoyer
        size_t size = sizeof(this->_code) + sizeof(netId) + sizeof(bool) +
                      sizeof(eventId) + sizeof(pos.x) + sizeof(pos.y) + sizeof(isPlayer);

        std::vector<char> buffer(size);
        size_t currentSize = 0;

        // Sérialisation des données dans le buffer
        std::memcpy(buffer.data() + currentSize, &this->_code, sizeof(this->_code));
        currentSize += sizeof(this->_code);

        std::memcpy(buffer.data() + currentSize, &this->netId, sizeof(this->netId));
        currentSize += sizeof(this->netId);

        std::memcpy(buffer.data() + currentSize, &this->isSuperProjectile, sizeof(bool));
        currentSize += sizeof(bool);

        std::memcpy(buffer.data() + currentSize, &this->eventId, sizeof(this->eventId));
        currentSize += sizeof(this->eventId);

        std::memcpy(buffer.data() + currentSize, &this->pos.x, sizeof(this->pos.x));
        currentSize += sizeof(this->pos.x);

        std::memcpy(buffer.data() + currentSize, &this->pos.y, sizeof(this->pos.y));
        currentSize += sizeof(this->pos.y);

        std::memcpy(buffer.data() + currentSize, &this->isPlayer, sizeof(this->isPlayer));

        return buffer;
    }

    void PacketPlayerShoot::fillData(const std::vector<char> &buffer) {
        size_t currentSize = 0;

        currentSize += sizeof(this->_code);

        std::memcpy(&this->netId, buffer.data() + currentSize, sizeof(this->netId));
        currentSize += sizeof(this->netId);

        std::memcpy(&this->isSuperProjectile, buffer.data() + currentSize, sizeof(bool));
        currentSize += sizeof(bool);

        std::memcpy(&this->eventId, buffer.data() + currentSize, sizeof(this->eventId));
        currentSize += sizeof(this->eventId);

        std::memcpy(&this->pos.x, buffer.data() + currentSize, sizeof(this->pos.x));
        currentSize += sizeof(this->pos.x);

        std::memcpy(&this->pos.y, buffer.data() + currentSize, sizeof(this->pos.y));
        currentSize += sizeof(this->pos.y);

        std::memcpy(&this->isPlayer, buffer.data() + currentSize, sizeof(this->isPlayer));
    }
}