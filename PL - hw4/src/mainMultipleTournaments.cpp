#include "TrainerCenter.h"
#include "Tournament.h"
#include "Trainer.h"

#include <iostream>
#include <vector>

#include <stdio.h>

int main(int argc, char** argv)
{
	std::vector<Trainer *> champions;

	std::vector<TrainerCenter *> trainerCenters;

	for(int i = 1; i < 9; i++)
	{
		char inputFileName[100];
		char tournamentName[100];

		sprintf(inputFileName, "./InputOutput/M%d_Input.txt", i);
		sprintf(tournamentName, "Tournament_%d", i);
		
		TrainerCenter *newCenter = new TrainerCenter(inputFileName);

		trainerCenters.push_back(newCenter);

		std::vector<Trainer *> allTrainers = newCenter->getAllTrainers();

		Tournament newTournament(tournamentName);
		newTournament.registerTrainers(allTrainers);
		
		Trainer *champion = newTournament.commence();

		champions.push_back(champion);
	}

	Tournament championsTournament("ChampionsTournament");
	championsTournament.registerTrainers(champions);

	Trainer *championOfChampions = championsTournament.commence();

	std::cout << "ChampionsTournament has finished. The champion is " << championOfChampions->getName() << "!" << std::endl;

	for(int i = 0; i < trainerCenters.size(); i++)
	{
		delete trainerCenters[i];
	}

	return 0;
}
