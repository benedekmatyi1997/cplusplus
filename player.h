#ifndef PLAYER_H
#define PLAYER_H

#include<string>
#include<iostream>
#include<memory>
#include<vector>
#include<map>

#include "player.h"

using std::string;

class Game;

enum class Mode{Ranked,Unranked};

class Player : public std::enable_shared_from_this<Player>
{
private:
    string name;
    int mmr; // Matchmakingrating
    std::shared_ptr<Game> hosted_game;
    std::map<string, std::weak_ptr<Game>> games;
public:
    Player(string name, int mmr);
    string get_name() const;
    int get_mmr() const;

    std::shared_ptr<Game> get_hosted_game() const;
    void change_mmr(int n);
    bool host_game(string s, Mode m);
    bool join_game(std::shared_ptr<Game> g);
    bool leave_game(std::shared_ptr<Game> g);
    std::vector<std::weak_ptr<Player>> invite_players(const std::vector<std::weak_ptr<Player>>& v);
    bool close_game();
    std::ostream& print(std::ostream& o) const;
};

#endif
