/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once
#include "IComponent.hpp"

namespace rtype::components {
    struct Created : public IComponent
    {
        void create(nlohmann::basic_json<> &value) override {
            isCreate = value["isCreate"];
        }
        bool isCreate = false;
    };
}