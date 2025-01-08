/*
** EPITECH PROJECT, 2025
** RType
** File description:
** TODO: add description
*/

#pragma once

#include <string>
#include "IComponent.hpp"

namespace rtype::components {
    struct String : public IComponent
    {
        void create(nlohmann::basic_json<>& value) override {
            s = value;
        };
        std::string s;
    };
}