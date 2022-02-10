//
// Created by bened on 08/05/2021.
//

#include<string>
#include<iostream>
#include<memory>
#include<algorithm>
#include<vector>
#include<map>

#include "player.h"
#include "game.h"
#include "gamekey.h"

Player::Player(string name, int mmr) : name{name}, mmr{mmr}
{
    if(name.size()==0 || mmr<0 || mmr>9999)
    {
        throw std::runtime_error("Falsche Angaben");
    }
}

string Player::get_name() const
{
    return this->name;
}
int Player::get_mmr() const
{
    return this->mmr;
}

std::shared_ptr<Game> Player::get_hosted_game() const
{
    return this->hosted_game;
}

void Player::change_mmr(int n)
{
    if((this->mmr+n)>0 && (this->mmr+n)<9999)
    {
        this->mmr+=n;
    }
}

bool Player::host_game(string s, Mode m)
{
    if(s.size()==0)
    {
        throw std::runtime_error("");
    }
    if(this->get_hosted_game()== nullptr)
    {
        if(m == Mode::Ranked)
        {
            hosted_game = std::make_shared<RGame>(s,shared_from_this());
            return true;
        }
        if(m==Mode::Unranked)
        {
            hosted_game = std::make_shared<UGame>(s,shared_from_this());
            return true;
        }
    }
    return false;
}

bool Player::join_game(std::shared_ptr<Game> g)
{
    if(g->add_player(GameKey(), shared_from_this()) == false)
    {
        return false;
    }

    games[g->get_name()] = std::weak_ptr<Game>(g);
    return true;
}

bool Player::leave_game(std::shared_ptr<Game> g)
{
    bool a = false;
    bool b = false;
    for(auto it = games.begin(); it != games.end(); ++it)
    {
        if(it->second.lock() == g)
        {
            a=true;
            games.erase(it);
        }
    }
    if(g->remove_player(GameKey(), shared_from_this()))
    {
        b=true;
    }
    if(a && b)
    {
        return true;
    }
        return false;

}

std::vector<std::weak_ptr<Player>> Player::invite_players(const std::vector<std::weak_ptr<Player>>& v)
{
    std::vector<std::weak_ptr<Player>> false_player;
    for(auto it = v.begin(); it != v.end(); ++it)
    {
        if(it->expired() || !(it->lock()->join_game(this->get_hosted_game())))
        {
            false_player.push_back(*it);
        }
        return false_player;
    }
}

bool Player::close_game()
{
    if(hosted_game != nullptr)
    {
        hosted_game.reset();
        return true;
    }
    return false;
}


std::ostream& Player::print(std::ostream& o) const
{
   o << "[" << this->get_name() << ", " << this->get_mmr() << ", hosts: ";
   if(this->get_hosted_game() == nullptr)
   {
       o << "nothing, ";
   }
   o << this->get_hosted_game() << ", games: {";
   for(auto it = games.begin(); it != games.end(); ++it)
   {
       o << it->first;
        if(it != --games.end())
        {
            o << ", ";
        }
   }
    o << "}]";
    return o;
}
std::ostream& operator<<(std::ostream& o, const Player& player)
{
    player.print(o);
    return o;
}
