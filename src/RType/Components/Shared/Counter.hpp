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
#include <SFML/Graphics/Text.hpp>
#endif

//TODO: maybe delete this and make this component only a client component or use this in server instead of the MenuState variable ?
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
            (name + ": " + std::to_string(iniCount) + "/" + std::to_string(max)),
                "./assets/fonts/Starborn.ttf", sf::Color::White, charSize, pos) {
            }

            int getCount() {
                return count;
            }

            void update(int newCount) {
                count = newCount;
                const std::string newStr = name + ": " + std::to_string(count) + "/" + std::to_string(max);
                this->text.text.setString(newStr);
            }
        };
    }
#endif