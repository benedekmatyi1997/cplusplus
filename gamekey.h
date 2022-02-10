//
// Created by bened on 08/05/2021.
//

#ifndef GAMEKEY_H
#define GAMEKEY_H



#include "player.h"
class Game;
class GameKey {
    GameKey() {} //Private. Implementierung kann auch in GameKey.cpp erfolgen.
    friend bool Player::join_game (std::shared_ptr<Game>);
    friend bool Player::leave_game (std::shared_ptr<Game>);
};



#endif