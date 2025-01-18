/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Interface for entities
*/

#pragma once

namespace rtype::ecs {
    class IEntity {
    public:
        virtual ~IEntity() = default;
        virtual unsigned int getId() = 0;
    };
}