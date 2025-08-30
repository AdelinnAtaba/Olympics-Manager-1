#include "../include/Olympicsa1.h"
#include <exception>
#include <cassert>

Olympics::Olympics() : countries(), contestants(), teams() {

}

Olympics::~Olympics() {

}

StatusType Olympics::add_country(int countryId, int medals) {
	if (countryId <= 0 || medals < 0) {
		return StatusType::INVALID_INPUT;
	}

	try
	{
		Country toAdd(countryId, medals);
		if (countries.findPtr(toAdd) != nullptr)
		{
			return StatusType::FAILURE;
		}
		countries.insert(toAdd);
	}
	catch (const std::bad_alloc & e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const Failure & j)
	{
		return StatusType::FAILURE;
	}

	return StatusType::SUCCESS;
}

StatusType Olympics::remove_country(int countryId) {
	if (countryId <= 0) {
		return StatusType::INVALID_INPUT;
	}

	try
	{
		Country toFind(countryId);
		Country* toDelete = countries.findPtr(toFind);
		if (toDelete == nullptr || toDelete->country->contestantsNum > 0 || toDelete->country->teamsNum > 0)
		{
			return StatusType::FAILURE;
		}
		countries.deleteVert(*toDelete);
	}
	catch (const std::bad_alloc & e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const Failure & j)
	{
		return StatusType::FAILURE;
	}

	return StatusType::SUCCESS;
}

StatusType Olympics::add_team(int teamId, int countryId, Sport sport) {
	if (teamId <= 0 || countryId <= 0) {
		return StatusType::INVALID_INPUT;
	}

	try
	{
		Country countryToFind(countryId);
		Country* country = countries.findPtr(countryToFind);
		Team teamToAdd(teamId, sport, country);
		Team* teamInData = teams.findPtr(teamToAdd);
		if (teamInData != nullptr || country == nullptr)
		{
			return StatusType::FAILURE;
		}
		teamToAdd.country = country->country.get();
		teams.insert(teamToAdd);
		country->country->teamsNum++;
	}
	catch (const std::bad_alloc & e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const Failure & j)
	{
		return StatusType::FAILURE;
	}

	return StatusType::SUCCESS;
}

StatusType Olympics::remove_team(int teamId) {
	if (teamId <= 0) {
		return StatusType::INVALID_INPUT;
	}

	try
	{
		Team teamToFind(teamId);
		Team* team = teams.findPtr(teamToFind);
		if (team == nullptr || team->playersNum > 0)
		{
			return StatusType::FAILURE;
		}
		team->country->teamsNum--;
		teams.deleteVert(*team);
	}
	catch (const std::bad_alloc & e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const Failure & j)
	{
		return StatusType::FAILURE;
	}

	return StatusType::SUCCESS;
}

StatusType Olympics::add_contestant(int contestantId, int countryId, Sport sport, int strength) {
	if (contestantId <= 0 || countryId <= 0 || strength < 0) {
		return StatusType::INVALID_INPUT;
	}

	try
	{
		Country countryToFind(countryId);
		Country* country = countries.findPtr(countryToFind);
		Contestant contestantToAdd(contestantId, strength, sport, country);
		if (contestants.findPtr(contestantToAdd) != nullptr || country == nullptr)
		{
			return StatusType::FAILURE;
		}
		contestantToAdd.country = country->country.get();
		contestants.insert(contestantToAdd);
		country->country->contestantsNum++;
	}
	catch (const std::bad_alloc & e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const Failure & j)
	{
		return StatusType::FAILURE;
	}

	return StatusType::SUCCESS;
}

StatusType Olympics::remove_contestant(int contestantId)
{
	if (contestantId <= 0)
		return StatusType::INVALID_INPUT;
	try {
		Contestant tmp(contestantId);
		Contestant* toDelete = contestants.findPtr(tmp);
		if (toDelete == nullptr || toDelete->isActive())
			return StatusType::FAILURE;
		toDelete->country->contestantsNum--;
		contestants.deleteVert(*toDelete);
	}
	catch (std::bad_alloc & e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (Failure & j)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}
void Olympics::addPlayerToTeamHelper(Player& player, Team* team)
{
	//team is valid and player is valid and may be added to team
	std::shared_ptr<avlTree<Player>> tree1 = team->players[0];
	std::shared_ptr<avlTree<Player>> tree3 = team->players[2];
	int size1 = tree1->getSize();
	int size3 = tree3->getSize();
	Player* max1 = size1 > 0 ? tree1->GetMaxPtr() : nullptr;
	Player* min3 = size3 > 0 ? tree3->GetMinPtr() : nullptr;
	team->playersNum++;
	Strength pow(player.strength, player.id);
	if (size1 > 0 && player.id < max1->id)
		team->addPlayerToTeam(player, pow, 0);
	else if (size3 > 0 && player.id > min3->id)
		team->addPlayerToTeam(player, pow, 2);
	else
		team->addPlayerToTeam(player, pow, 1);
	team->fix();
}

void Olympics::deletePlayerfromTeamHelper(Player& player, Team* team)
{
	//team is valid and player is valid and may be removed from team
	int index = 0;
	for (int i = 0; i < 3; i++)
	{
		if (team->players[i]->findPtr(player) != nullptr)
			index = i;
	}
	Strength pow(player.strength, player.id);
	team->players[index]->deleteVert(player);
	team->strengths[index]->deleteVert(pow);
	team->playersNum--;
	team->fix();
}


StatusType Olympics::add_contestant_to_team(int teamId, int contestantId)
{
	if (contestantId <= 0 || teamId <= 0)
		return StatusType::INVALID_INPUT;
	try {
		Contestant tmp(contestantId);
		Contestant* toAdd = contestants.findPtr(tmp);
		Team tmp1(teamId);
		Team* team = teams.findPtr(tmp1);
		if (toAdd == nullptr || team == nullptr)
			return StatusType::FAILURE;
		if (team->country->id != toAdd->country->id || team->sport != toAdd->sport)
			return StatusType::FAILURE;
		int strength = toAdd->strength;
		toAdd = toAdd->contes.get();
		if (toAdd->canAddToTeam(teamId) == false)
			return StatusType::FAILURE;
		int emptyCell = toAdd->getEmptySlot();
		toAdd->teamsId[emptyCell] = teamId;
		Player player(toAdd->id, strength, toAdd);
		addPlayerToTeamHelper(player, team);
	}
	catch (std::bad_alloc & e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (Failure & j)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType Olympics::remove_contestant_from_team(int teamId, int contestantId)
{
	if (contestantId <= 0 || teamId <= 0)
		return StatusType::INVALID_INPUT;
	try {
		Contestant tmp(contestantId);
		Contestant* toRemove = contestants.findPtr(tmp);
		Team tmp1(teamId);
		Team* team = teams.findPtr(tmp1);
		if (toRemove == nullptr || team == nullptr)
			return StatusType::FAILURE;
		int strength = toRemove->strength;
		toRemove = toRemove->contes.get();
		if (toRemove->getTeamIndex(teamId) < 0)
			return StatusType::FAILURE;
		int index = toRemove->getTeamIndex(teamId);
		toRemove->teamsId[index] = 0;
		Player player(toRemove->id, strength, toRemove);
		deletePlayerfromTeamHelper(player, team);
	}
	catch (std::bad_alloc & e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (Failure & j)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType Olympics::update_contestant_strength(int contestantId, int change)
{
	if (contestantId <= 0)
		return StatusType::INVALID_INPUT;
	try {
		Contestant tmp(contestantId);
		Contestant* toUpdate = contestants.findPtr(tmp);
		if (toUpdate == nullptr)
			return StatusType::FAILURE;
		int newStrength = toUpdate->getNewStrength(change);
		int oldStrength = toUpdate->strength;
		if (newStrength < 0)
			return StatusType::FAILURE;
		toUpdate->strength = newStrength;//strength will be updated in tree 'contestants'
		toUpdate = toUpdate->contes.get();
		for (int i = 0; i < 3; i++)//update strength in each team
		{
			if (toUpdate->teamsId[i] != 0)
			{
				Team tmp1(toUpdate->teamsId[i]);
				Team* ptr = teams.findPtr(tmp1);
				Player player(toUpdate->id, oldStrength, toUpdate);
				deletePlayerfromTeamHelper(player, ptr);
				player.strength = newStrength;
				addPlayerToTeamHelper(player, ptr);
			}
		}
	}
	catch (std::bad_alloc & e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (Failure & j)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

output_t<int> Olympics::get_strength(int contestantId)
{
	if (contestantId <= 0)
		return StatusType::INVALID_INPUT;
	try {
		Contestant tmp(contestantId);
		Contestant* contestant = contestants.findPtr(tmp);
		if (contestant == nullptr)
			return StatusType::FAILURE;
		return contestant->strength;
	}
	catch (std::bad_alloc & e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (Failure & j)
	{
		return StatusType::FAILURE;
	}
	return 0;
}

output_t<int> Olympics::get_medals(int countryId)
{
	if (countryId <= 0)
		return StatusType::INVALID_INPUT;
	try {
		Country tmp(countryId);
		Country* country = countries.findPtr(tmp);
		if (country == nullptr)
			return StatusType::FAILURE;
		return country->country->medals;
	}
	catch (std::bad_alloc & e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (Failure & j)
	{
		return StatusType::FAILURE;
	}
	return 0;
}

output_t<int> Olympics::get_team_strength(int teamId)
{
	if (teamId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		Team tmp(teamId);
		Team* team = teams.findPtr(tmp);
		if (team == nullptr)
			return StatusType::FAILURE;
		return team->getStrength();
	}
	catch (std::bad_alloc & e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (Failure & j)
	{
		return StatusType::FAILURE;
	}
	return 0;
}


StatusType Olympics::unite_teams(int teamId1, int teamId2) {
	if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
		return StatusType::INVALID_INPUT;
	}

	try
	{
		Team team1ToFind(teamId1);
		Team team2ToFind(teamId2);

		Team* team1 = teams.findPtr(team1ToFind);
		Team* team2 = teams.findPtr(team2ToFind);
		if (team1 == nullptr || team2 == nullptr)
			return StatusType::FAILURE;
		bool sameCountry = team1->country == team2->country;
		bool sameSport = team1->sport == team2->sport;
		if (!sameCountry || !sameSport)
		{
			return StatusType::FAILURE;
		}
		// Check if one of the teams is not empty
		if (!team1->isEmpty() || !team2->isEmpty())
		{
			// Create 2 arrays of players for each team
			int len1 = 0, len2 = 0;
			Player* team1Players = convertTeamPlayersIntoArr(team1, &len1);
			Player* team2Players = convertTeamPlayersIntoArr(team2, &len2);

			// Merge
			int united_len = len1 + len2;
			Player* unitedPlayers = avlTree<Player>::mergeSortedArr(team1Players, team2Players, len1, len2, &united_len);

			// Free memory teams
			if (team1Players != nullptr)
			{
				delete[] team1Players;
			}
			if (team2Players != nullptr)
			{
				delete[] team2Players;
			}

			// Create 2 arrays of strengths for each team
			int len3 = 0, len4 = 0;
			Strength* team1Strengths = convertTeamStrengthsIntoArr(team1, &len3);
			Strength* team2Strengths = convertTeamStrengthsIntoArr(team2, &len4);

			// Merge
			int united_len2 = 0;
			Strength* unitedStrengths = avlTree<Strength>::mergeSortedArr(team1Strengths, team2Strengths, len3, len4, &united_len2);

			// Free memory teams
			if (team1Strengths != nullptr)
			{
				delete[] team1Strengths;
			}
			if (team2Strengths != nullptr)
			{
				delete[] team2Strengths;
			}



			// Update team Id for every contestant in player
			for (int i = 0; i < united_len; i++)
			{
				unitedPlayers[i].contestant->updateTeam(team2->team_id, team1->team_id);
			}

			// Partition strengths over 3 groups matching ids.
			// group length can be 0 if there are less than 3 players in both teams(1-2 players in total)
			int group_len = united_len / 3;
			int flooredLen = 3 * group_len;
			// Deal with 1-2 players total seperately
			Strength* groups = new Strength[united_len];
			if (group_len != 0)
			{
				// united_len = 3*group_len + r
				int minGroup2Id = unitedPlayers[group_len].id;
				int maxGroup2Id = unitedPlayers[2 * group_len - 1].id;
				int currIndex[3] = { 0 };

				int idExtra1 = united_len > flooredLen ? unitedPlayers[flooredLen].id : -1;
				int idExtra2 = united_len > (flooredLen + 1) ? unitedPlayers[flooredLen + 1].id : -1;
				for (int i = 0; i < united_len; i++)
				{
					int currId = unitedStrengths[i].playerId;
					int currGroup = 0;
					//don't add strengths that match the extraIds
					if (currId == idExtra1)
					{
						groups[flooredLen] = unitedStrengths[i];
						continue;
					}
					if (currId == idExtra2)
					{
						groups[flooredLen + 1] = unitedStrengths[i];
						continue;
					}
					if (minGroup2Id <= currId && currId <= maxGroup2Id)
					{
						currGroup = 1;
					}
					else if (maxGroup2Id < currId)
					{
						currGroup = 2;
					}
					int tmp_index = currGroup * group_len + currIndex[currGroup];
					groups[tmp_index] = unitedStrengths[i];
					currIndex[currGroup]++;
				}

			}
			
			// Convert the arrays to matching trees for each type of data
			for (int i = 0; i < 3; i++)
			{
				team1->players[i] = std::shared_ptr<avlTree<Player>>
					(avlTree<Player>::sortedArrayToTree(unitedPlayers + group_len * i, group_len, nullptr));
				team1->strengths[i] = std::shared_ptr<avlTree<Strength>>
					(avlTree<Strength>::sortedArrayToTree(groups + group_len * i, group_len, nullptr));

			}

			// Update number of players
			team1->playersNum = flooredLen;
			team2->playersNum = 0;

			//  Insert superflous players
			for (int i = flooredLen; i < united_len; i++)
			{
				Player* player = unitedPlayers + i;
				addPlayerToTeamHelper(*player, team1);
			}
			// Free strengths ***********************************************************************************
			// Free Merges ***********************************************************************************
			// Must free unitedPlayers
			if (unitedPlayers != nullptr)
			{
				delete[] unitedPlayers;
			}
			if (unitedStrengths != nullptr)
			{
				delete[] unitedStrengths;
			}
			if (groups != nullptr)
			{
				delete[] groups;
			}
		}
		remove_team(team2->team_id);
		return StatusType::SUCCESS;
	}
	catch (const std::bad_alloc & e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (const Failure & j)
	{
		return StatusType::FAILURE;
	}

	return StatusType::SUCCESS;
}

StatusType Olympics::play_match(int teamId1, int teamId2)
{
	if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
		return StatusType::INVALID_INPUT;
	try
	{
		Team tmp1(teamId1);
		Team* team1 = teams.findPtr(tmp1);
		Team tmp2(teamId2);
		Team* team2 = teams.findPtr(tmp2);
		if (team1 == nullptr || team2 == nullptr || team1->sport != team2->sport)
			return StatusType::FAILURE;
		int score1 = get_team_strength(teamId1).ans() + (team1->country->medals);
		int score2 = get_team_strength(teamId2).ans() + (team2->country->medals);
		if (score1 > score2)
			team1->country->medals++;
		else if (score1 < score2)
			team2->country->medals++;
		//tiko so nothing changes or the changes were already made
		return StatusType::SUCCESS;
	}
	catch (std::bad_alloc & e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (Failure & j)
	{
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

output_t<int> Olympics::austerity_measures(int teamId)
{
	if (teamId <= 0)
		return StatusType::INVALID_INPUT;
	try
	{
		Team tmp(teamId);
		Team* team = teams.findPtr(tmp);
		if (team == nullptr || team->playersNum < 3)
			return StatusType::FAILURE;
		if (team->playersNum % 3 != 0 || team->playersNum == 3)
			return 0;
		//there is at least 6 players-> in each tree at least 2 players->there is a max and min for each tree
		Player* edge4 = team->players[0]->GetMaxPtr();
		Player* edge1 = team->players[1]->GetMinPtr();
		Player* edge2 = team->players[1]->GetSecondMin();
		Player* edge5 = team->players[1]->GetSecondMax();
		Player* edge6 = team->players[1]->GetMaxPtr();
		Player* edge3 = team->players[2]->GetMinPtr();
		Strength* max1 = team->strengths[0]->GetMaxPtr();
		Strength* secondMax1 = team->strengths[0]->GetSecondMax();
		Strength* max3 = team->strengths[2]->GetMaxPtr();
		Strength* secondMax3 = team->strengths[2]->GetSecondMax();
		Strength* max2 = team->strengths[1]->GetMaxPtr();
		Strength* secondMax2 = team->strengths[1]->GetSecondMax();
		Strength* tempp = team->strengths[1]->GetThirdMax();
		Strength temp(0, 0);
		Strength* thirdMax2 = tempp == nullptr ? &temp : tempp;
		Strength* minIn2 = team->strengths[1]->GetMinPtr();
		int max = team->getStrength();
		//if edge4 was the max in 1
		int max1No4 = max1->playerId == edge4->id ? secondMax1->strength : max1->strength;
		//if edge3 was the max in 3
		int max3No3 = max3->playerId == edge3->id ? secondMax3->strength : max3->strength;
		int max2No6 = max2->playerId == edge6->id ? secondMax2->strength : max2->strength;
		int max2No1 = max2->playerId == edge1->id ? secondMax2->strength : max2->strength;
		int tmpSum = 0;
		if (team->playersNum != 6)//if 6 then each tree has 2 elem,cannot remove 3
		{
			// 3 0 0

			tmpSum = getMaxIn2(max2, secondMax2, thirdMax2, edge1->id, edge2->id, edge3->strength);
			if (team->playersNum == 9)//each tree has 3 elem, after removing 3 from tree1->empty
				tmpSum += calcMax(-1, edge1->strength, edge2->strength) + max3No3;
			else
				tmpSum += calcMax(max1->strength, edge1->strength, edge2->strength) + max3No3;
			max = calcMax(tmpSum, max, -1);
			//0 3 0
			if (team->playersNum == 9)
				tmpSum = calcMax(-1, edge4->strength, edge3->strength);
			else
				tmpSum = calcMax(max2->strength, edge4->strength, edge3->strength);
			tmpSum += max1No4 + max3No3;
			max = calcMax(tmpSum, max, -1);
			//0 0 3
			tmpSum = getMaxIn2(max2, secondMax2, thirdMax2, edge5->id, edge6->id, edge4->strength);
			if (team->playersNum == 9)
				tmpSum += calcMax(-1, edge5->strength, edge6->strength) + max1No4;
			else
				tmpSum += calcMax(max3->strength, edge5->strength, edge6->strength) + max1No4;
			max = calcMax(tmpSum, max, -1);
		}

		//2 1 0
		if (team->playersNum == 6)//after removing 2->tree is empty
			tmpSum = max2->strength + edge3->strength + max3No3;
		else
		{
			Player* moved = getWhoMoves(max2, secondMax2, minIn2, edge1, edge2, &tmpSum);
			//now tmpSum has the max in 2 after removing elem from 2
			tmpSum = calcMax(tmpSum, edge3->strength, -1);
			tmpSum += calcMax(max1->strength, moved->strength, -1) + max3No3;
		}
		max = calcMax(tmpSum, max, -1);

		//1 2 0
		if (team->playersNum == 6)
			tmpSum = max1->strength + edge3->strength + max3No3;
		else
			tmpSum = max1->strength + calcMax(max2->strength, edge3->strength, -1) + max3No3;
		max = calcMax(tmpSum, max, -1);

		//2 0 1
		if (team->playersNum == 6)
			tmpSum = max3->strength + edge1->strength + edge2->strength;
		else
			tmpSum = max3->strength + calcMax(max1->strength, edge1->strength, -1) + max2No1;
		max = calcMax(tmpSum, max, -1);

		//0 2 1
		if (team->playersNum == 6)
			tmpSum = max3->strength + edge4->strength + max1No4;
		else
			tmpSum = max3->strength + calcMax(max2->strength, edge4->strength, -1) + max1No4;
		max = calcMax(tmpSum, max, -1);

		//0 1 2
		if (team->playersNum == 6)
			tmpSum = max2->strength + edge4->strength + max1No4;
		else
		{
			Player* moved = getWhoMoves(max2, secondMax2, minIn2, edge6, edge5, &tmpSum);
			//now tmpSum has the max in 2 after removing elem from 2
			tmpSum = calcMax(tmpSum, edge4->strength, -1);
			tmpSum += calcMax(max3->strength, moved->strength, -1) + max1No4;
		}

		max = calcMax(tmpSum, max, -1);

		//1 0 2
		if (team->playersNum == 6)
			tmpSum = max1->strength + edge1->strength + edge2->strength;
		else
			tmpSum = max1->strength + calcMax(max3->strength, edge6->strength, -1) + max2No6;
		max = calcMax(tmpSum, max, -1);


		return max;
	}
	catch (std::bad_alloc & e)
	{
		return StatusType::ALLOCATION_ERROR;
	}
	catch (Failure & j)
	{
		return StatusType::FAILURE;
	}
	return 0;
}



int Olympics::calcMax(int a, int b, int c)
{
	int max = a > b ? a : b;
	max = max > c ? max : c;
	return max;
}

int Olympics::getMaxIn2(Strength* max, Strength* secondMax, Strength* thirdMax, int edge1, int edge2, int added)
{
	int tmpSum = max->strength;
	bool changed = false;
	if (edge1 == max->playerId || edge2 == max->playerId)//one of the edges is the max
	{
		tmpSum = secondMax->strength;
		changed = true;
	}
	if (changed && (secondMax->playerId == edge1 || secondMax->playerId == edge2))
		tmpSum = thirdMax->strength;
	tmpSum = calcMax(tmpSum, added, -1);
	return tmpSum;
}

Player* Olympics::getWhoMoves(Strength* max, Strength* secondMax, Strength* min, Player* edge1, Player* edge2, int* upMax)
{
	int curr = max->strength;
	if (min->playerId == edge1->id)//1 is removed from tree
	{
		//edge 2 moves to another tree
		*upMax = max->playerId == edge2->id ? secondMax->strength : curr;
		return edge2;
	}
	//1 moves to another tree
	*upMax = max->playerId == edge1->id ? secondMax->strength : curr;
	return edge1;
}


Player* Olympics::convertTeamPlayersIntoArr(const Team* team, int* len)
{
	if (team->isEmpty())
	{
		return nullptr;
	}
	*len = team->playersNum;
	Player* players = new Player[*len];
	int index = 0;
	for (int group = 0; group < 3; group++)
	{
		team->players[group]->PrintInOrder(*len, &index, players);
	}
	return players;
}

Strength* Olympics::convertTeamStrengthsIntoArr(const Team* team, int* len)
{
	if (team->isEmpty())
	{
		return nullptr;
	}
	//there is at least one node in all trees, there could be 2 empty strength trees.
	
	// Create 3 strength arrays
	Strength* strengths[3] = { nullptr };
	int lens[3] = { 0 };
	for (int i = 0; i < 3; i++)
	{
		strengths[i] = convertTeamStrengthIntoArr(team, i, lens + i);
	}

	// Merge strengths arrays
	int numm = 0;
	Strength* merges_1 = avlTree<Strength>::mergeSortedArr(strengths[0], strengths[1], lens[0], lens[1], &numm);
	Strength* finalMerge = avlTree<Strength>::mergeSortedArr(merges_1, strengths[2], lens[0] + lens[1], lens[2], &numm);

	*len = team->playersNum;
	for (int i = 0; i < 3; i++)
	{
		if (strengths[i] != nullptr)
			delete[] strengths[i];
	}
	if (merges_1 != nullptr)
		delete[] merges_1;
	return finalMerge;
}

Strength* Olympics::convertTeamStrengthIntoArr(const Team* team, int i, int* len)
{
	if (team->isEmpty())
	{
		return nullptr;
	}
	assert(len != nullptr);
	assert(i < 3);

	*len = team->strengths[i]->getSize();
	if (*len == 0)
	{
		return nullptr;
	}

	Strength* arr = new Strength[*len];
	int index = 0;
	team->strengths[i]->PrintInOrder(*len, &index, arr);
	return arr;
}

void Olympics::print_data_structure() const
{
	std::cout << "=== OLYMPICS DATA STRUCTURE ===" << std::endl;
	
	// Print Countries
	std::cout << "\n--- COUNTRIES ---" << std::endl;
	int countriesSize = countries.getSize();
	std::cout << "Total Countries: " << countriesSize << std::endl;
	if (countriesSize > 0) {
		std::cout << "Countries Data:" << std::endl;
		// We'll use a simple traversal approach since we can't create arrays easily
		printCountriesData();
	}
	
	// Print Contestants
	std::cout << "\n--- CONTESTANTS ---" << std::endl;
	int contestantsSize = contestants.getSize();
	std::cout << "Total Contestants: " << contestantsSize << std::endl;
	if (contestantsSize > 0) {
		std::cout << "Contestants Data:" << std::endl;
		printContestantsData();
	}
	
	// Print Teams
	std::cout << "\n--- TEAMS ---" << std::endl;
	int teamsSize = teams.getSize();
	std::cout << "Total Teams: " << teamsSize << std::endl;
	if (teamsSize > 0) {
		std::cout << "Teams Data:" << std::endl;
		printTeamsData();
	}
	
	std::cout << "\n===============================" << std::endl;
}

void Olympics::printCountriesData() const
{
	if (countries.getSize() == 0) return;
	
	// Show sample data using min/max methods
	Country* minCountry = countries.GetMinPtr();
	Country* maxCountry = countries.GetMaxPtr();
	
	if (minCountry) {
		std::cout << "  Min Country ID: " << minCountry->id 
				  << ", Medals: " << minCountry->medals
				  << ", Teams: " << minCountry->teamsNum
				  << ", Contestants: " << minCountry->contestantsNum << std::endl;
	}
	
	if (maxCountry && maxCountry != minCountry) {
		std::cout << "  Max Country ID: " << maxCountry->id 
				  << ", Medals: " << maxCountry->medals
				  << ", Teams: " << maxCountry->teamsNum
				  << ", Contestants: " << maxCountry->contestantsNum << std::endl;
	}
	
	if (countries.getSize() > 2) {
		std::cout << "  ... and " << (countries.getSize() - (maxCountry != minCountry ? 2 : 1)) << " more countries" << std::endl;
	}
}

void Olympics::printContestantsData() const
{
	if (contestants.getSize() == 0) return;
	
	// Show sample data using min/max methods
	Contestant* minContestant = contestants.GetMinPtr();
	Contestant* maxContestant = contestants.GetMaxPtr();
	
	if (minContestant) {
		std::cout << "  Min Contestant ID: " << minContestant->id 
				  << ", Strength: " << minContestant->strength
				  << ", Sport: " << (int)minContestant->sport
				  << ", Country: " << (minContestant->country ? minContestant->country->id : -1) << std::endl;
	}
	
	if (maxContestant && maxContestant != minContestant) {
		std::cout << "  Max Contestant ID: " << maxContestant->id 
				  << ", Strength: " << maxContestant->strength
				  << ", Sport: " << (int)maxContestant->sport
				  << ", Country: " << (maxContestant->country ? maxContestant->country->id : -1) << std::endl;
	}
	
	if (contestants.getSize() > 2) {
		std::cout << "  ... and " << (contestants.getSize() - (maxContestant != minContestant ? 2 : 1)) << " more contestants" << std::endl;
	}
}

void Olympics::printTeamsData() const
{
	if (teams.getSize() == 0) return;
	
	// Show sample data using min/max methods
	Team* minTeam = teams.GetMinPtr();
	Team* maxTeam = teams.GetMaxPtr();
	
	if (minTeam) {
		std::cout << "  Min Team ID: " << minTeam->team_id 
				  << ", Sport: " << (int)minTeam->sport
				  << ", Country: " << (minTeam->country ? minTeam->country->id : -1)
				  << ", Players: " << minTeam->playersNum
				  << ", Strength: " << minTeam->getStrength() << std::endl;
	}
	
	if (maxTeam && maxTeam != minTeam) {
		std::cout << "  Max Team ID: " << maxTeam->team_id 
				  << ", Sport: " << (int)maxTeam->sport
				  << ", Country: " << (maxTeam->country ? maxTeam->country->id : -1)
				  << ", Players: " << maxTeam->playersNum
				  << ", Strength: " << maxTeam->getStrength() << std::endl;
	}
	
	if (teams.getSize() > 2) {
		std::cout << "  ... and " << (teams.getSize() - (maxTeam != minTeam ? 2 : 1)) << " more teams" << std::endl;
	}
}