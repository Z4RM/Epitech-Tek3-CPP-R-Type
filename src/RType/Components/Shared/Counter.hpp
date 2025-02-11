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

//TODO: maybe delete this and make this component only a client component or use this in server instead of the MenuState
// variable ? and maybe change atomic variable by a normal because new component manager is thread safe

namespace rtype::components {
    struct Counter {
        std::shared_ptr<std::atomic<int>> count = nullptr;
        int max = 0;
        std::string name;
#ifdef RTYPE_IS_CLIENT
        std::optional<SfText> text;
        bool displayMax = false;
#endif

        Counter(int iniCount, int max, std::string &name)
                : count(std::make_shared<std::atomic<int>>(iniCount)), max(max), name(name) {}

#ifdef RTYPE_IS_CLIENT

        Counter(int iniCount, int max, std::string &name, const sf::Vector2f pos, int charSize, bool displayMax = true)
                : count(std::make_shared<std::atomic<int>>(iniCount)),
                  max(max),
                  name(name),
                  displayMax(displayMax) {
            std::string defaultText;

            if (displayMax) {
               defaultText = name + ": " + std::to_string(iniCount) + "/" + std::to_string(max);
            } else {
                defaultText = name + ": " + std::to_string(iniCount);
            }
            this->text = std::make_optional<SfText>(
                          defaultText,
                          "./assets/fonts/Starborn.ttf",
                          sf::Color::White,
                          charSize,
                          pos
            );
        }

#endif

        int getCount() {
            return count->load();
        }

        void update(int newCount) {
            count->store(newCount);

#ifdef RTYPE_IS_CLIENT
            std::string newStr;

            if (this->displayMax) {
                newStr = name + ": " + std::to_string(count->load()) + "/" + std::to_string(max);
            } else {
                newStr = name + ": " + std::to_string(count->load());
            }
            if (text.has_value())
                text->text.setString(newStr);
#endif
        }
    };
}
