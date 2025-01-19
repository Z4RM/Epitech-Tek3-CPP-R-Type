/*
** EPITECH PROJECT, 2025
** RType
** File description:
** Counter
*/

#pragma once

#include <atomic>
#include <string>
#include <SFML/Graphics/Text.hpp>
#ifndef RTYPE_IS_CLIENT
    namespace rtype::components {
        struct Counter {
            std::shared_ptr<std::atomic<int>> count = 0;
            int max = 0;
            std::string name;

            Counter(int iniCount, int max, std::string &name) : count(std::make_shared<std::atomic<int>>(iniCount)), max(max), name
            (name) {

            }

            int getCount() {
                return count->load();
            }
            void update(int newCount) {
                count->store(newCount);
            }
        };
    }
#else
#include "RType/Components/Client/SfText.hpp"

namespace rtype::components {
        struct Counter {
            int count = 0;
            int max = 0;
            std::string name;
            components::SfText text;

            Counter(int iniCount, int max, std::string &name, const sf::Vector2f pos, int charSize) : count
            (iniCount),
             max
            (max), name
            (name),
            text(
            (std::to_string(iniCount) + "/" + std::to_string(max) + " " + name),
                "./assets/fonts/Starborn.ttf", sf::Color::White, charSize, pos) {
            }

            int getCount() {
                return count;
            }

            void update(int newCount) {
                count = newCount;
                const std::string newStr = std::to_string(count) + "/" + std::to_string(max) + " " + name;
                this->text.text.setString(newStr);
            }
        };
    }
#endif