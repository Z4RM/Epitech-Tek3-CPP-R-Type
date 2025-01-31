/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Counter
*/

#pragma once

#include <atomic>
#include <string>

#ifdef RTYPE_IS_CLIENT
#include <optional>
#include "RType/Components/Client/SfText.hpp"
#endif

//TODO: maybe delete this and make this component only a client component or use this in server instead of the MenuState variable ?

namespace rtype::components {
    struct Counter {
        std::shared_ptr<std::atomic<int>> count = nullptr;
        int max = 0;
        std::string name;
#ifdef RTYPE_IS_CLIENT
        std::optional<SfText> text;
#endif

        Counter(int iniCount, int max, std::string &name)
                : count(std::make_shared<std::atomic<int>>(iniCount)), max(max), name(name) {}

#ifdef RTYPE_IS_CLIENT

        Counter(int iniCount, int max, std::string &name, const sf::Vector2f pos, int charSize)
                : count(std::make_shared<std::atomic<int>>(iniCount)),
                  max(max),
                  name(name),
                  text(std::make_optional<SfText>(
                          (name + ": " + std::to_string(iniCount) + "/" + std::to_string(max)),
                          "./assets/fonts/Starborn.ttf",
                          sf::Color::White,
                          charSize,
                          pos
                  )) {}

#endif

        int getCount() {
            return count->load();
        }

        void update(int newCount) {
            count->store(newCount);

#ifdef RTYPE_IS_CLIENT
            if (text.has_value())
                text->text.setString(name + ": " + std::to_string(count->load()) + "/" + std::to_string(max));
#endif
        }
    };
}
