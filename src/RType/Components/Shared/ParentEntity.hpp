/*
** EPITECH PROJECT, 2025
** RType
** File description:
** linked entity component
*/

#pragma once

namespace rtype::components {
    struct ParentEntity {
        unsigned int entity;
        float xOffset = 0.f;
        float yOffset = 0.f;
    };
}