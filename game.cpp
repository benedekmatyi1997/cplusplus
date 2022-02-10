//
// Created by bened on 08/05/2021.
//

#include "game.h"
#include "player.h"
#include "gamekey.h"
#include<string>
#include<iostream>
#include<algorithm>
#include<memory>
#include<numeric>
#include<vector>
#include<map>

Game::Game(string name, std::shared_ptr<Player> host): name{name}, host{host} // NOTE: this uses the weak_ptr-ctor overload which TAKES a shared_ptr!
{
    if(name.empty() || host==nullptr)
    {
        throw std::runtime_error("False values");
    }
}

string Game::get_name() const
{
    return name;
}

bool Game::is_allowed(int n) const
{
    std::shared_ptr<Player> host_shared = host.lock();
    return (n > (host_shared->get_mmr())*90/100) && (n < (host_shared->get_mmr())*110/100);
}

bool Game::remove_player(const GameKey& gk, std::shared_ptr<Player> p)
{
    return (players.erase(p->get_name()) == 1);
}

bool Game::add_player(const GameKey& gk, std::shared_ptr<Player> p)
{
    if(players.count(p->get_name()) == 0 || this->is_allowed(p->get_mmr()) )
    {
        // then; insert player
        players[p->get_name()] = std::weak_ptr<Player> (p);
        return true;
    }

    return false;
}

size_t Game::number_of_players() const
{
    /*return std::count_if(players.begin(), players.end(), [](std::pair<string, std::weak_ptr<Player> > item){
        return !item.second.expired();
    });*/
    size_t i=0;
    for(auto it = players.begin(); it!=players.end(); ++it)
    {
        if(!it->second.expired())
        {
            ++i;
        }
    }
    return i;
}

std::weak_ptr<Player> Game::best_player() const
{
    if(this->number_of_players() == 0)
    {
        throw std::runtime_error("Keine aktive Spieler");
    }

   /* return std::max_element(players.begin(), players.end(),
                            [](std::pair<string, std::weak_ptr<Player> > item_1, std::pair<string, std::weak_ptr<Player> > item_2) {
                                return (item_1.second.lock()->get_mmr() < item_2.second.lock()->get_mmr());
                            })->second;*/
   std::weak_ptr<Player> big_mmr=players.begin()->second;
   int biggest_mmr= players.begin()->second.lock()->get_mmr();
   for(auto it=players.begin(); it!=players.end(); ++it)
   {

       if(it->second.lock()->get_mmr() > biggest_mmr)
       {
            biggest_mmr=it->second.lock()->get_mmr();
            big_mmr=it->second;
       }
   }
    return big_mmr;
}

std::weak_ptr<Player> Game::play(size_t i)
{
    for(auto it=players.begin(); it!=players.end(); ++it)
    {
        if(it->second.expired())
        {
            players.erase(it);
        }
    }
    if(i >= players.size() || host.expired())
    {
        throw std::runtime_error("");
    }

    auto winner = players.begin();
    std::advance(winner, i);

    std::weak_ptr<Player> sieger = winner->second;
    {
        std::shared_ptr<Player> shared_sieger = sieger.lock();
        for(auto it = players.begin(); it!=players.end(); ++it)
        {
            if(it->second.lock() != shared_sieger) // if not the winner
            {
                std::shared_ptr<Player> loser = it->second.lock();
                if(loser->get_mmr() > shared_sieger->get_mmr())
                {
                    loser->change_mmr(change(false));
                }
                loser->change_mmr(change(false));
            }
        }

        shared_sieger->change_mmr(change(true));
    }
    return sieger;
}

RGame::RGame(string name, std::shared_ptr<Player> host) : Game(name, host) // delegate to the base-class ctor
{
}

int RGame::change(bool won) const
{
    if(won)
    {
        return 5;
    }
    return -5;
}

std::ostream& UGame::print(std::ostream& o) const
{
    o << "Ranked Game: ";
    return Game::print(o);
}

UGame::UGame(string name, std::shared_ptr<Player> host) : Game(name, host) // delegate to the base-class ctor
{
}
int UGame::change(bool) const
{
    return 0;
}
std::ostream& RGame::print(std::ostream& o) const
{
    o << "Unranked Game: ";
    return Game::print(o);
}

std::ostream& Game::print(std::ostream &o) const
{
    o << "[" << this->name << ", " << host.lock()->get_name() << ", " << host.lock()->get_mmr() << ", player: {";
    for(auto it = players.begin(); it!= players.end(); ++it)
    {
        o << "[" << it->second.lock()->get_name() << ", " << it->second.lock()->get_mmr() << "]";
        if(!it->second.expired())
        {
            o<<", ";
        }
    }
    o << "}]";
    return o;
}

std::ostream& operator<<(std::ostream& o, const Game& game) // params are `lvalue-`s, bcs they are NAMED
{
    game.print(o);
    return o;

}


