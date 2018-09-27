#include "PokeGrass.h"

using namespace std;


PokeGrass::PokeGrass(int pokemonID,const std::string& name):Pokemon(pokemonID,name){
	this -> HP = 800;
	this -> ATK = 40;
	this -> MAG_DEF = 0;
	this -> PHY_DEF = 30;
}
void PokeGrass::levelUp(){
	lvl++;
	HP +=80;
	ATK +=4;
	MAG_DEF += 0;
	PHY_DEF += 3;

}
void PokeGrass::Restart(){
	HP = 800 + lvl *80;
	ATK = 40 + lvl *4;
	MAG_DEF = 0 + lvl *0;
	PHY_DEF = 30 + lvl * 3;	
	rooted = electrified = burning = drowning = 0;
}

void PokeGrass::BuffMachine(Arena currentArena){
	if(currentArena == FOREST){
		HP += 100;
		ATK += 10;
	}
	else if (currentArena == MAGMA || currentArena == SKY){
		HP -= 100;
		ATK -= 10;
	}
}
void PokeGrass::special_effect(){
	if(isDrowning() && HP > 0){
		cout << "\t\t\t" << name << "(" << HP << ")" << " is drowning" << "!!" << endl;
		HP -= std::max(0,DROWN_DAMAGE - MAG_DEF);
	}
	if (isElectrified() && HP > 0){
		cout << "\t\t\t" << name << "(" << HP << ")" << " is electrified" << "!!" << endl;
		HP -= std::max(0, ELECTRIFY_DAMAGE - MAG_DEF);
	}
	if (isBurning() && HP > 0){
		cout << "\t\t\t" << name << "(" << HP << ")" << " is burning" << "!!!!" << endl;
		HP -= std::max(0,2 * BURN_DAMAGE - MAG_DEF);
	}
}
void PokeGrass::attackTo(Pokemon* target,Arena currentArena){
	special_effect();
	if(currentArena == STADIUM || currentArena == ELECTRICITY || currentArena == OCEAN ){
		if(HP > 0){
			cout << "\t\t\t" << name << "(" << HP << ")" << " hit " << target -> getName() << "(" << target -> getHP() << ") " << std::max(1,(ATK - (target -> getPHY_DEF()))) << "(/)" << endl;
			target -> setHP(target -> getHP() - std::max(1,(ATK - (target -> getPHY_DEF()))));
			target -> setRooted(true);
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
	else if(currentArena == FOREST){
		if(HP > 0){
			cout << "\t\t\t" << name << "(" << HP << ")" << " hit " << target -> getName() << "(" << target -> getHP() << ") " << std::max(1,(ATK - (target -> getPHY_DEF()))) << "(+)" << endl;
			target -> setHP(target -> getHP() - std::max(1,(ATK - (target -> getPHY_DEF()))));
			target -> setRooted(true);
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
	else if(currentArena == MAGMA || currentArena == SKY){
		if(HP > 0){
			cout << "\t\t\t" << name << "(" << HP << ")" << " hit " << target -> getName() << "(" << target -> getHP() << ") " << std::max(1,(ATK - (target -> getPHY_DEF()))) << "(-)" << endl;
			target -> setHP(target -> getHP() - std::max(1,(ATK - (target -> getPHY_DEF()))));
			target -> setRooted(true);
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

void PokeGrass::setRooted(bool rooted){
	rooted = 0;
}