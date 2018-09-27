#ifndef TRAINER_CENTER_H
#define TRAINER_CENTER_H

#include "Trainer.h"
#include "Pokemon.h"

#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>

class TrainerCenter
{
public:
	// Public Methods

	TrainerCenter(const std::string& inputFileName);
    ~TrainerCenter();
    
	std::vector<Trainer*>& getAllTrainers();

	Trainer* getTrainerWithID(int trainerID);

	// Own Methods

private:
	// Own Attributes
	std::vector<Trainer*> trainers;
	std::string filename;

};

#endif
