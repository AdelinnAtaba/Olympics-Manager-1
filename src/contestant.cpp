#include "../include/contestant.h"
#include <cassert>

Country::Country(int country_id, int medals_num) :
	id(country_id), medals(medals_num), teamsNum(0), contestantsNum(0)
{	country = std::make_shared<Country>(*this); }

bool operator==(const Country& elem1, const Country& elem2)
{
	return elem1.id == elem2.id;
}
bool operator!=(const Country& elem1, const Country& elem2)
{
	return elem1.id != elem2.id;
}
bool operator>=(const Country& elem1, const Country& elem2)
{
	return elem1.id >= elem2.id;
}
bool operator<=(const Country& elem1, const Country& elem2)
{
	return elem1.id <= elem2.id;
}
bool operator>(const Country& elem1, const Country& elem2)
{
	return elem1.id > elem2.id;
}
bool operator<(const Country& elem1, const Country& elem2)
{
	return elem1.id < elem2.id;
}



Contestant::Contestant(int contestant_id, int strength, Sport sport, Country* country) :
	id(contestant_id), strength(strength), sport(sport), country(country), teamsId{ 0 }
{	contes = std::make_shared<Contestant>(*this); }
bool Contestant::isActive() const
{
	for (int i = 0; i < 3; i++)
	{
		if (contes->teamsId[i] != 0)
			return true;
	}
	return false;
}
int Contestant::getEmptySlot() const
{
	for (int i = 0; i < 3; i++)
	{
		if (teamsId[i] == 0)
			return i;
	}
	return -1;
}

int Contestant::getIndexOf(int teamId) const
{
	for (int i = 0; i < 3; i++)
	{
		if (teamsId[i] == teamId)
		{
			return i;
		}
	}
	return -1;

}

void Contestant::updateTeam(int oldId, int updatedId)
{
	int indexOfupdated = getTeamIndex(updatedId);
	for (int i = 0; i < 3; i++)
	{
		if (teamsId[i] == oldId)//was in team2
		{
			teamsId[i] = updatedId;//now is in team1
			if (indexOfupdated >= 0)//was in team1 and 2, but there are 2 cells pointing to team1, remove one of them
				teamsId[indexOfupdated] = 0;
			break;
		}
	}
}


int Contestant::getNewStrength(int change) const
{
	return strength + change;
}

int Contestant::getTeamIndex(int team_id) const
{
	for (int i = 0; i < 3; i++)
	{
		if (teamsId[i] == team_id)
			return i;
	}
	return -1;
}

bool Contestant::canAddToTeam(int team_id) const
{
	if (getTeamIndex(team_id) >= 0)
		return false;
	else if (getEmptySlot() < 0)
		return false;
	return true;
}

bool operator==(const Contestant& elem1, const Contestant& elem2)
{
	return elem1.id == elem2.id;
}
bool operator!=(const Contestant& elem1, const Contestant& elem2)
{
	return elem1.id != elem2.id;
}
bool operator>=(const Contestant& elem1, const Contestant& elem2)
{
	return elem1.id >= elem2.id;
}
bool operator<=(const Contestant& elem1, const Contestant& elem2)
{
	return elem1.id <= elem2.id;
}
bool operator>(const Contestant& elem1, const Contestant& elem2)
{
	return elem1.id > elem2.id;
}
bool operator<(const Contestant& elem1, const Contestant& elem2)
{
	return elem1.id < elem2.id;
}







Strength::Strength(int strength, int id) : strength(strength), playerId(id)
{}

bool operator==(const Strength& elem1, const Strength& elem2)
{
	return elem1.strength == elem2.strength && elem1.playerId == elem2.playerId;
}
bool operator!=(const Strength& elem1, const Strength& elem2)
{
	return !(elem1 == elem2);
}
bool operator>=(const Strength& elem1, const Strength& elem2)
{
	return elem1 > elem2 || elem1 == elem2;
}
bool operator<=(const Strength& elem1, const Strength& elem2)
{
	return elem1 < elem2 || elem1 == elem2;
}
bool operator>(const Strength& elem1, const Strength& elem2)
{
	if (elem1.strength != elem2.strength)
		return elem1.strength > elem2.strength;
	else
		return elem1.playerId > elem2.playerId;
}
bool operator<(const Strength& elem1, const Strength& elem2)
{
	if (elem1.strength != elem2.strength)
		return elem1.strength < elem2.strength;
	else
		return elem1.playerId < elem2.playerId;
}




Player::Player(int id, int strength, Contestant* contestant) : id(id), strength(strength), contestant(contestant)
{}
bool operator==(const Player& elem1, const Player& elem2)
{
	return elem1.id == elem2.id;
}
bool operator!=(const Player& elem1, const Player& elem2)
{
	return elem1.id != elem2.id;
}
bool operator>=(const Player& elem1, const Player& elem2)
{
	return elem1.id >= elem2.id;
}
bool operator<=(const Player& elem1, const Player& elem2)
{
	return elem1.id <= elem2.id;
}
bool operator>(const Player& elem1, const Player& elem2)
{
	return elem1.id > elem2.id;
}
bool operator<(const Player& elem1, const Player& elem2)
{
	return elem1.id < elem2.id;
}






Team::Team(int id, Sport sport, Country* country) :
	team_id(id), sport(sport), country(country), playersNum(0)
{
	for (int i = 0; i < GROUPS; i++)
	{
		players[i] = std::make_shared<avlTree<Player>>(); //empty  trees
		strengths[i] = std::make_shared<avlTree<Strength>>();
	}
}

bool Team::isEmpty() const
{
	return playersNum == 0;
}

void Team::addPlayerToTeam(Player player, Strength strength, int i)
{
	players[i]->insert(player);
	strengths[i]->insert(strength);
}

void Team::removeFromAddTo(int removeFrom, int addIn, Player player)
{
	
	Strength pow(player.strength, player.id);
	players[removeFrom]->deleteVert(player);
	strengths[removeFrom]->deleteVert(pow);
	players[addIn]->insert(player);
	strengths[addIn]->insert(pow);
}
void Team::fix()
{
	//i=index of tree in array players that needs fixing
	int k = playersNum / 3;
	
	for (int i = 0; i < 2; i++)
	{
		int size1 = players[0]->getSize();
		int size2 = players[1]->getSize();
		int size3 = players[2]->getSize();
		//fix tree1
		if (size1 < k)//size get size, if null send def const
		{
			if (size2 > 0)
				removeFromAddTo(1, 0, *(players[1]->GetMinPtr()));
		}
		else if (size1 > (k + 1))
			removeFromAddTo(0, 1, *(players[0]->GetMaxPtr()));
		//fix tree2
		if (size2 < k)
		{
			if (size1 == (k + 1))
				removeFromAddTo(0, 1, *(players[0]->GetMaxPtr()));
			else if (size3 > 0)//size1==k
				removeFromAddTo(2, 1, *(players[2]->GetMinPtr()));
		}
		else if (size2 > (k + 1))
		{
			if (size1 == k)
				removeFromAddTo(1, 0, *(players[1]->GetMinPtr()));
			else//size1==k
				removeFromAddTo(1, 2, *(players[1]->GetMaxPtr()));
		}
		//fix tree3
		if (size3 < k)
		{
			if (size2 > 0)
				removeFromAddTo(1, 2, *(players[1]->GetMaxPtr()));
		}

		else if (size3 > (k + 1))
			removeFromAddTo(2, 1, *(players[2]->GetMinPtr()));

	}

}
int Team::getStrength()
{
	if (playersNum % 3 != 0 || playersNum == 0)
		return 0;
	int strength = 0;
	for (int i = 0; i < Team::GROUPS; i++)//there is at least one player in each tree
	{
		strength += strengths[i]->GetMaxPtr()->strength;
	}
	return strength;
}

bool operator==(const Team& elem1, const Team& elem2)
{
	return elem1.team_id == elem2.team_id;
}
bool operator!=(const Team& elem1, const Team& elem2)
{
	return elem1.team_id != elem2.team_id;
}
bool operator>=(const Team& elem1, const Team& elem2)
{
	return elem1.team_id >= elem2.team_id;
}
bool operator<=(const Team& elem1, const Team& elem2)
{
	return elem1.team_id <= elem2.team_id;
}
bool operator>(const Team& elem1, const Team& elem2)
{
	return elem1.team_id > elem2.team_id;
}
bool operator<(const Team& elem1, const Team& elem2)
{
	return elem1.team_id < elem2.team_id;
}