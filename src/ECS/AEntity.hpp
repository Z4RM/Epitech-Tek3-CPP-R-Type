/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Abstract entity class
*/

#pragma once
#include "IEntity.hpp"
#include "EntityManager.hpp"

namespace rtype::ecs {
    class AEntity : public IEntity {
    public:
        ~AEntity() override = default;
        explicit AEntity(EntityManager &entityManager) { this->_id = entityManager.createEntity(); }
        unsigned int getId() final { return this->_id; };
    protected:
        size_t _id = 0;
    };
}
