#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include "Trainer.h"

#include <vector>

class Tournament
{
public:
	// Public Methods

	Tournament(const std::string& tournamentName);
	~Tournament();

	// Trainer count will always be 2^n.
    void registerTrainers(const std::vector<Trainer*>& trainers);
    
    // Commence the tournament, and return the champion.
	Trainer* commence();

	// Return 1 if Trainer1 wins.
	// Return -1 if Trainer2 wins.
	// It may be called from main.cpp, aside from a Tournament.
	int duelBetween(Trainer* trainer1, Trainer* trainer2, Arena currentArena);

    // Getters

    const std::string& getTournamentName() const;

	// Own Methods
	Trainer* Fight(Trainer* T1,Trainer* T2);
	std::vector<Trainer*> commencehelp(std::vector<Trainer*> trainers1);
	
private:
	std::string tournamentName;
	std::vector<Trainer*> trainers;

	// Return 1 if Pokemon1 wins.
	// Return -1 if Pokemon2 wins.
	// It won't (can't) be called from main.cpp, this method is for your own convenience.
	int duelBetween(Pokemon *pokemon1, Pokemon *pokemon2, Arena currentArena);

	// Own Attributes

};

#endif
