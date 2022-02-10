//
// Created by bened on 08/05/2021.
//

#ifndef GAME_H
#define GAME_H

#include<string>
#include<iostream>
#include<memory>
#include<vector>
#include<map>

#include "player.h"
#include "gamekey.h"

using std::string;

// abstract class
class Game : public std::enable_shared_from_this<Game>
{
private:
    string name;
    std::weak_ptr<Player> host;
    std::map<string, std::weak_ptr<Player>> players;

public:
    Game() = default;
    Game(string name, std::shared_ptr<Player> host);

    virtual ~Game() = default;

    virtual int change(bool) const = 0;

    string get_name() const;
    bool is_allowed(int n) const;
    bool remove_player(const GameKey& gk, std::shared_ptr<Player> p);
    bool add_player(const GameKey& gk, std::shared_ptr<Player> p);
    size_t number_of_players() const;
    std::weak_ptr<Player> best_player() const;
    std::weak_ptr<Player> play(size_t i);

    virtual std::ostream& print(std::ostream& o) const;
};

class RGame : public Game
{
public:
    RGame() = default;

    RGame(string name, std::shared_ptr<Player> host);
    int change(bool won) const override;
    std::ostream& print(std::ostream& o) const override;
};

class UGame : public Game
{
public:
    UGame() = default;

    UGame(string name, std::shared_ptr<Player> host);
    int change(bool) const override;
    std::ostream& print(std::ostream& o) const override;
};

#endif
