/*
** EPITECH PROJECT, 2025
** RType
** File description:
** On click component
*/

#pragma once
#include <functional>

namespace rtype::components {
    struct OnClick {
        std::function<void()> fn;
    };
}