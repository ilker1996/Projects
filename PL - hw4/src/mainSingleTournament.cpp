#include "TrainerCenter.h"
#include "Tournament.h"
#include "Trainer.h"

#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
	if(argc < 3)
	{
		return -1;
	}

	TrainerCenter newCenter(argv[1]);

	std::vector<Trainer *> allTrainers = newCenter.getAllTrainers();

	Tournament newTournament(argv[2]);
	newTournament.registerTrainers(allTrainers);
	
	Trainer *champion = newTournament.commence();
    
	std::cout << argv[2] << " has finished. The champion is " << champion->getName() << "!" << std::endl;

	return 0;
}
