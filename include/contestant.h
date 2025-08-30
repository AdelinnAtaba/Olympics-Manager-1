#ifndef CONTESTANT_H
#define CONTESTANT_H
#include<iostream>
#include <memory>
#include "vertice.h"
#include "AvlTree.h"
#include "wet1util.h"


struct Country {
	int id;
	int medals;
	int teamsNum;
	int contestantsNum;
	std::shared_ptr<Country> country;
	explicit Country(int country_id, int medals_num = 0);
	~Country() = default;
	Country(const Country& cp) = default;
	Country& operator=(const Country& elem) = default;
};

bool operator==(const Country& elem1, const Country& elem2);
bool operator!=(const Country& elem1, const Country& elem2);
bool operator>=(const Country& elem1, const Country& elem2);
bool operator<=(const Country& elem1, const Country& elem2);
bool operator>(const Country& elem1, const Country& elem2);
bool operator<(const Country& elem1, const Country& elem2);



struct Contestant {
	int id;
	int strength;
	Sport sport;
	Country* country;
	int teamsId[3];
	std::shared_ptr<Contestant> contes;
	explicit Contestant(int contestant_id, int strength = 0, Sport sport = Sport::SWIMMING, Country* country = nullptr);
	~Contestant() = default;
	Contestant(const Contestant& cp) = default;
	Contestant& operator=(const Contestant& elem) = default;
	bool isActive() const; //check if contestant is active in any team
	int getEmptySlot() const;//returns the empty slot in teamsId, if there is none returns -1
	// Removes old team Id and adds the new team id.
	void updateTeam(int oldId, int updatedId);
	// Returns index of team Id.
	int getIndexOf(int teamId) const;
	int getNewStrength(int change) const;
	bool canAddToTeam(int team_id) const;
	int getTeamIndex(int team_id) const;
};
bool operator==(const Contestant& elem1, const Contestant& elem2);
bool operator!=(const Contestant& elem1, const Contestant& elem2);
bool operator>=(const Contestant& elem1, const Contestant& elem2);
bool operator<=(const Contestant& elem1, const Contestant& elem2);
bool operator>(const Contestant& elem1, const Contestant& elem2);
bool operator<(const Contestant& elem1, const Contestant& elem2);


struct Strength {
	int strength;
	int playerId;
	explicit Strength(int strength = 0, int id = 0);
	~Strength() = default;
	Strength(const Strength& cp) = default;
	Strength& operator=(const Strength& elem) = default;
};
bool operator==(const Strength& elem1, const Strength& elem2);
bool operator!=(const Strength& elem1, const Strength& elem2);
bool operator>=(const Strength& elem1, const Strength& elem2);
bool operator<=(const Strength& elem1, const Strength& elem2);
bool operator>(const Strength& elem1, const Strength& elem2);
bool operator<(const Strength& elem1, const Strength& elem2);

struct Player {
	int id;
	int strength;
	Contestant* contestant;
	explicit Player(int id = 0, int strength = 0, Contestant* contestant = nullptr);
	~Player() = default;
	Player(const Player& cp) = default;
	Player& operator=(const Player& elem) = default;

};
bool operator==(const Player& elem1, const Player& elem2);
bool operator!=(const Player& elem1, const Player& elem2);
bool operator>=(const Player& elem1, const Player& elem2);
bool operator<=(const Player& elem1, const Player& elem2);
bool operator>(const Player& elem1, const Player& elem2);
bool operator<(const Player& elem1, const Player& elem2);

struct Team {
	int team_id;
	Sport sport;
	Country* country;
	int playersNum;
	std::shared_ptr<avlTree<Player>> players[3];
	std::shared_ptr<avlTree<Strength>> strengths[3];
	static const int GROUPS = 3;

	Team(int id, Sport sport = Sport::SWIMMING, Country* country = nullptr);
	~Team() = default;
	Team(const Team&) = default;
	Team& operator=(const Team&) = default;
	bool isEmpty() const;
	void addPlayerToTeam(Player player, Strength strength, int i);
	void removeFromAddTo(int removeFrom, int addIn, Player player);
	void fix();
	int getStrength();
};
bool operator==(const Team& elem1, const Team& elem2);
bool operator!=(const Team& elem1, const Team& elem2);
bool operator>=(const Team& elem1, const Team& elem2);
bool operator<=(const Team& elem1, const Team& elem2);
bool operator>(const Team& elem1, const Team& elem2);
bool operator<(const Team& elem1, const Team& elem2);

#endif