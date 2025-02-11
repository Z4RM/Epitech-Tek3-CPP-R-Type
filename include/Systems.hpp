/*
** EPITECH PROJECT, 2024
** RType
** File description:
** Systems.hpp
*/

#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "RType/Systems/Movement.hpp"
#include "RType/Systems/AnimationProjectile.hpp"
#include "RType/Systems/LevelRunner.hpp"
#include "RType/Systems/Network/Network.hpp"
#ifdef RTYPE_IS_CLIENT
#include "RType/Systems/RenderWindow.hpp"
#include "RType/Systems/Sound/Sound.hpp"
#endif

#endif //SYSTEMS_HPP
