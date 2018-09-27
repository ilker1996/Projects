#include "PokeElectric.h"

using namespace std;

PokeElectric::PokeElectric(int pokemonID,const std::string& name):Pokemon(pokemonID,name){
	this -> HP = 500;
	this -> ATK = 70;
	this -> MAG_DEF = 30;
	this -> PHY_DEF = 0;
}
void PokeElectric::levelUp(){
	lvl++;
	HP +=50;
	ATK +=7;
	MAG_DEF += 3;
	PHY_DEF += 0;

}
void PokeElectric::Restart(){
	HP = 500 + lvl * 50;
	ATK = 70 +lvl *7;
	MAG_DEF = 30 + lvl *3;
	PHY_DEF = 0 + lvl * 0;
	rooted = electrified = burning = drowning = 0;
}

void PokeElectric::BuffMachine(Arena currentArena){
	if(currentArena == ELECTRICITY){
		HP += 100;
		ATK += 10;
	}
	else if (currentArena == MAGMA || currentArena == FOREST){
		HP -= 100;
		ATK -= 10;
	}
}
void PokeElectric::special_effect(){
	if(isDrowning() && HP > 0){
		cout << "\t\t\t" << name << "(" << HP << ")" << " is drowning" << "!!" << endl;
		HP -= std::max(0, DROWN_DAMAGE - MAG_DEF);
	}
	if (isBurning()&& HP > 0){
		cout << "\t\t\t" << name << "(" << HP << ")" << " is burning" << "!!" << endl;
		HP -= std::max(0, BURN_DAMAGE - MAG_DEF);
	}
	if (isRooted()&& HP > 0){
		cout << "\t\t\t" << name << "(" << HP << ")" << " is rooted" << "!!!!" << endl;
		HP -= std::max(0,2 * ROOT_DAMAGE - MAG_DEF);
	}
}
void PokeElectric::attackTo(Pokemon* target,Arena currentArena){
	special_effect();
	if(currentArena == STADIUM || currentArena == SKY || currentArena == OCEAN ){
		if(HP > 0){
			cout << "\t\t\t" << name << "(" << HP << ")" << " hit " << target -> getName() << "(" << target -> getHP() << ") " << std::max(1,(ATK - (target -> getPHY_DEF()))) << "(/)" << endl;
			target -> setHP(target -> getHP() - (std::max(1, ATK - (target -> getPHY_DEF()))));
			target -> setElectrified(true);
			if(target -> getHP() <= 0){
				cout << "\t\t" << "WinnerP:" << name << endl;
				levelUp();	
			}

		}
		else if ( HP <= 0){
			cout << "\t\t" << "WinnerP:" << target -> getName() << endl;
			target -> levelUp();
		}
	}
	else if(currentArena == ELECTRICITY){
		if(HP > 0){
			cout << "\t\t\t" << name << "(" << HP << ")" << " hit " << target -> getName() << "(" << target -> getHP() << ") " << std::max(1,(ATK - (target -> getPHY_DEF()))) << "(+)" << endl;
			target -> setHP(target -> getHP() - std::max(1,(ATK - (target -> getPHY_DEF()))));
			target -> setElectrified(true);
			if(target -> getHP() <= 0){
				cout << "\t\t" << "WinnerP:" << name << endl;
				levelUp();	
			}
		}
		else if ( HP <= 0){
			cout << "\t\t" << "WinnerP:" << target -> getName() << endl;
			target -> levelUp();
		}
	}
	else if(currentArena == MAGMA || currentArena == FOREST){
		if(HP > 0){
			cout << "\t\t\t" << name << "(" << HP << ")" << " hit " << target -> getName() << "(" << target -> getHP() << ") " << std::max(1,(ATK - (target -> getPHY_DEF()))) << "(-)" << endl;
			target -> setHP(target -> getHP() - std::max(1,(ATK - (target -> getPHY_DEF()))));
			target -> setElectrified(true);
			if(target -> getHP() <= 0){
				cout << "\t\t" << "WinnerP:" << name << endl;
				levelUp();	
			}
		}
		else if ( HP <= 0){
			cout << "\t\t" << "WinnerP:" << target -> getName() << endl;
			target -> levelUp();
		}
	}

}

void PokeElectric::setElectrified(bool electrified){
	electrified = 0;
}