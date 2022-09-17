#include "region.h"
#include <iostream>
#include "pokemon.h"
#include <string> 

using namespace std;


Region::Region(const int min[3], const int max[3]){

	min_x = min[0];
	min_y = min[1];
	min_z = min[2];
	max_x = max[0];
	max_y = max[1];
	max_z = max[2];
	parent = right = left = NULL; 
	pokemon = NULL;
	wrt = 'x';

}
Region::Region(const Region& rhs){
	min_x = rhs.min_x;
	min_y = rhs.min_y;
	min_z = rhs.min_z;
	max_x = rhs.max_x;
	max_y = rhs.max_y;
	max_z = rhs.max_z;
	wrt = rhs.wrt;
	if((rhs.right) != NULL){
		right = new Region(*(rhs.right));
		right -> parent = this;
	}
	else if((rhs.right) == NULL){
		right = NULL;
	}
	if(rhs.left != NULL){
		left = new Region(*(rhs.left));
		left -> parent = this;
	}
	else if((rhs.left) == NULL){
		left = NULL;
	}
	if((rhs.parent) == NULL){
		parent = NULL;
	}
	if(rhs.pokemon != NULL){
		pokemon = new Pokemon(*(rhs.pokemon));
	}
	else if(rhs.pokemon == NULL){
		pokemon = NULL;
	}


}
void Region::deleteRegion(){  //DELETE
	if(pokemon != NULL){
		delete pokemon;
		pokemon = NULL;
	}
	if(right != NULL){
		delete right;
		right = NULL;
	}
	if(left != NULL){
		delete left;
		left = NULL;
	}
	parent = NULL;
	
}

Region& Region::operator=(const Region& rhs){
 	this -> deleteRegion();
	min_x = rhs.min_x;
	min_y = rhs.min_y;
	min_z = rhs.min_z;
	max_x = rhs.max_x;
	max_y = rhs.max_y;
	max_z = rhs.max_z;
	wrt = rhs.wrt;
	if(rhs.pokemon != NULL){
		pokemon = new Pokemon(*(rhs.pokemon));
	}
	else if((rhs.pokemon) == NULL){
		pokemon = NULL;
	}
	if(rhs.right != NULL){
		right =  new Region(*(rhs.right));
		right -> parent = this;
	}
	else if((rhs.right) == NULL){
		right = NULL;
	}
	if(rhs.left != NULL){
		left = new Region(*(rhs.left));
		left -> parent = this;
	}
	else if((rhs.left) == NULL){
		left = NULL;
	}
	if((rhs.parent) == NULL){
		parent = NULL;
	}
	return *this;


}
Region::~Region(){
	this -> deleteRegion();
}

bool Region::isLeaf() const {
	return ((min_x == max_x) && (min_y == max_y) && (min_z == max_z));
}

int Region::getMinBorder(char cord) const{
	if(cord == 'x')
		return min_x;
	else if(cord == 'y')
		return min_y;
	else if(cord == 'z')
		return min_z;
	else 
		return -1; // RETURNS -1 IF THE PARAMETER IS DIFFERENT THAN X,Y,Z !!!
}

int Region::getMaxBorder(char cord) const{
	if(cord == 'x')
		return max_x;
	else if(cord == 'y')
		return max_y;
	else if(cord == 'z')
		return max_z;
	else 
		return -1; // RETURNS -1 IF THE PARAMETER IS DIFFERENT THAN X,Y,Z !!!
}

void Region::placePokemon(const Pokemon& poke, int x , int y, int z){
	if(isLeaf() && min_x == x && min_y == y && min_z == z){
		pokemon = new Pokemon(poke);
		return;
	}
	if( wrt == 'x' && min_x == max_x && min_y != max_y){
		wrt = 'y';
	}
	else if( wrt == 'x' && min_x == max_x && min_y == max_y && min_z != max_z){
		wrt = 'z';
	}
	else if(wrt == 'y' && min_y == max_y && min_z != max_z){
		wrt = 'z';
	}
	else if(wrt == 'y' && min_y == max_y && min_z == max_z && max_x != min_x){
		wrt = 'x';
	}
	else if( wrt == 'z' && min_z == max_z && min_x != max_x){
		wrt = 'x'; 
	}
	else if(wrt == 'z' && min_z == max_z && min_x == max_x && min_y != max_y){
		wrt = 'y';
	}
	if(wrt == 'x' && min_x != max_x){
		if(min_x <= x && min_y <= y &&  min_z <= z && (((min_x + max_x + 1) / 2 )-1) >= x && max_y >= y && max_z >= z ){ // GOES LEFT
			int tmpmax[3] = {(((min_x + max_x + 1)/2)-1),max_y,max_z};
			int tmpmin[3] = {min_x,min_y,min_z};
			if(left == NULL){
				left = new Region(tmpmin,tmpmax);
				left -> parent = this;
			}
			if(tmpmin[1] == tmpmax[1] && tmpmin[2] != tmpmax[2]){
				left -> wrt = 'z';
			}
			else if (tmpmin[1] == tmpmax[1] && tmpmin[2] == tmpmax[2] && tmpmin[0] != tmpmax[0]){
				left -> wrt = 'x';
			}
			else{
				left -> wrt = 'y';
			}
			left -> placePokemon(poke,x,y,z);
		}
		else if(((min_x + max_x+1)/2) <= x && min_y <= y && min_z <= z && max_x >= x && max_y >= y && max_z >= z ){ // GOES RIGHT
			int tmpmax[3] = {max_x,max_y,max_z};
			int tmpmin[3] = {((min_x +max_x+1)/2),min_y,min_z};
			if(right == NULL){
				
				right = new Region(tmpmin,tmpmax);
				right -> parent = this;
			}
			if(tmpmin[1] == tmpmax[1] && tmpmin[2] != tmpmax[2]){
				right -> wrt = 'z';
			}
			else if (tmpmin[1] == tmpmax[1] && tmpmin[2] == tmpmax[2] && tmpmin[0] != tmpmax[0]){
				right -> wrt = 'x';
			}
			else{
				right -> wrt = 'y';
			}
			right -> placePokemon(poke,x,y,z);
		}		
	}
	else if (wrt == 'y' && min_y != max_y){
		
		if(min_x <= x && min_y <= y &&  min_z <= z && max_x >= x && (((min_y + max_y+1)/2)-1) >= y && max_z >= z ) { // GOES LEFT
			int tmpmax[3] = {max_x,(((min_y + max_y+1)/2)-1),max_z};
			int tmpmin[3] = {min_x,min_y,min_z};
			if(left == NULL){
				
				left = new Region(tmpmin, tmpmax);
				left -> parent = this;
			}
			if(tmpmin[2] == tmpmax[2] && tmpmin[0] != tmpmax[0]){
				left -> wrt = 'x';
			}
			else if (tmpmin[2] == tmpmax[2] && tmpmin[0] == tmpmax[0] && tmpmin[1] != tmpmax[1]){
				left -> wrt = 'y';
			}
			else{
				left -> wrt = 'z';
			}
			left -> placePokemon(poke,x,y,z);
		}
		else if(min_x <= x && ((min_y+max_y+1)/2) <= y && min_z <= z && max_x >= x && max_y >= y && max_z >= z){
			int tmpmax[3] = {max_x,max_y,max_z};
			int tmpmin[3] = {min_x,((min_y +max_y+1)/2),min_z};
			if(right == NULL){
				
				right = new Region(tmpmin, tmpmax);
				right -> parent = this;
			}
			if(tmpmin[2] == tmpmax[2] && tmpmin[0] != tmpmax[0]){
				right -> wrt = 'x';
			}
			else if (tmpmin[2] == tmpmax[2] && tmpmin[0] == tmpmax[0] && tmpmin[1] != tmpmax[1]){
				right -> wrt = 'y';
			}
			else{
				right -> wrt = 'z';
			}
			right -> placePokemon(poke,x,y,z);
		}
	}
	else if (wrt == 'z' && min_z != max_z){
		
		if(min_x <= x && min_y <= y &&  min_z <= z && max_x >= x && max_y >= y && (((min_z + max_z+1)/2)-1) >= z ){ // GOES LEFT
			int tmpmax[3] = {max_x,max_y,(((min_z + max_z+1)/2)-1)};
			int tmpmin[3] = {min_x,min_y,min_z};
			if(left == NULL){
				left = new Region(tmpmin,tmpmax);
				left -> parent = this;
			}
			if(tmpmin[0] == tmpmax[0] && tmpmin[1] != tmpmax[1]){
				left -> wrt = 'y';
			}
			else if (tmpmin[0] == tmpmax[0] && tmpmin[1] == tmpmax[1] && tmpmin[2] != tmpmax[2]){
				left -> wrt = 'z';
			}
			else{
				left -> wrt = 'x';
			}
			left -> placePokemon(poke,x,y,z);
		}
		else if(min_x <= x && min_y <= y && ((min_z + max_z+1)/2) <= z && max_x >= x && max_y >= y && max_z >= z){
			int tmpmax[3] = {max_x,max_y,max_z};
			int tmpmin[3] = {min_x,min_y,((min_z +max_z+1)/2)};
			if(right == NULL){
				right = new Region(tmpmin, tmpmax);
				right -> parent = this;	
			}
			if(tmpmin[0] == tmpmax[0] && tmpmin[1] != tmpmax[1]){
				right -> wrt = 'y';
			}
			else if (tmpmin[0] == tmpmax[0] && tmpmin[1] == tmpmax[1] && tmpmin[2] != tmpmax[2]){
				right -> wrt = 'z';
			}
			else{
				right -> wrt = 'x';
			}
			right -> placePokemon(poke,x,y,z);
		}
	}
}



Pokemon& Region::operator()(int x, int y, int z){  
	int tmparr[3] = {x,y,z};
	if(getPokemonCount(tmparr,tmparr) == 0)
		throw pokemonException();
	if(!isLeaf() && right == NULL && left == NULL )
		throw pokemonException();
	else if(isLeaf() && min_x == x && min_y == y && min_z == z){
		if(pokemon == NULL){
			throw pokemonException();
		}
		else{
			Pokemon *tmp = new Pokemon(*pokemon);
			delete pokemon;
			pokemon = NULL;
			Region * i = this;
			while(i->parent){
				i = i -> parent;
			}
			i -> deleteOperator();
			return *tmp;
		}
	}
	else if(right != NULL && right -> getMaxBorder('x') >= x && right -> getMaxBorder('y') >= y && right -> getMaxBorder('z') >= z && right -> getMinBorder('x') <= x && right -> getMinBorder('y') <= y && right -> getMinBorder('z') <= z &&  right != NULL ){
		return right -> operator()(x,y,z);
	}
	else if(left != NULL && left -> getMaxBorder('x') >= x && left  -> getMaxBorder('y') >= y && left  -> getMaxBorder('z') >= z && left  -> getMinBorder('x') <= x && left  -> getMinBorder('y') <= y && left  -> getMinBorder('z') <= z &&  left != NULL ){
		return left -> operator()(x,y,z);
	}
}

void Region::deleteOperator(){
	if(right != NULL){
		int tmpmin1[3] = {right -> min_x,right -> min_y , right -> min_z};
		int tmpmax1[3] = {right -> max_x,right -> max_y , right -> max_z};
		if(right -> getPokemonCount(tmpmin1,tmpmax1) == 0 ){
			delete right;
			right = NULL;
		}
		else if(right -> getPokemonCount(tmpmin1,tmpmax1) != 0){
			right -> deleteOperator();
			
		}	
	}
	
	if(left  != NULL){
		int tmpmin1[3] = {left-> min_x,left -> min_y,left -> min_z};
		int tmpmax1[3] = {left-> max_x,left -> max_y,left -> max_z};
		if( (left -> getPokemonCount(tmpmin1,tmpmax1) ) == 0){
			delete left;
			left = NULL;
		}
		else if( (left -> getPokemonCount(tmpmin1,tmpmax1)) != 0){
			left -> deleteOperator();
		}
	}
}

int Region::getPokemonCount(const int min[3], const int max[3]) const { 
	if(isLeaf() && pokemon != NULL && min_x >= min[0] && max_x <= max[0] && min_y >= min[1] && max_y <= max[1] && min_z >= min[2] && max_z <= max[2] ){
		return 1;
	}
	else if(isLeaf() && pokemon == NULL ){
		return 0;
	}
	else if(right == NULL && left == NULL && !isLeaf()){
		return 0; 
	}
	else if(!isLeaf() && right != NULL && left == NULL){
		return right -> getPokemonCount(min,max);
	}
	else if(!isLeaf() && left != NULL && right == NULL){
		return  left -> getPokemonCount(min,max);
	}
	else if(!isLeaf() && right != NULL && left != NULL){
		return (right -> getPokemonCount(min,max) +  left -> getPokemonCount(min,max));
	}
	else 
		return 0;
	
}


Region Region::crop(const int min[3] ,const int max[3] ) const{
	if((min_x == min[0]) && (max_x == max[0]) && (min_y == min[1]) && (max_y == max[1]) && (min_z == min[2]) && (max_z == max[2]) ){
		return *this;
	}
	else if((right != NULL) && (right -> getMaxBorder('x') >= max[0] ) && (right -> getMaxBorder('y') >= max[1]) && (right -> getMaxBorder('z') >= max[2]) && (right -> getMinBorder('x') <= min[0]) && (right -> getMinBorder('y') <= min[1]) && (right -> getMinBorder('z') <= min[2]) &&  (right != NULL)){
		return right -> crop(min,max);
	}
	else if((left != NULL) && (left -> getMaxBorder('x') >= max[0]) && (left  -> getMaxBorder('y') >= max[1]) && (left  -> getMaxBorder('z') >= max[2]) && (left  -> getMinBorder('x') <= min[0]) && (left  -> getMinBorder('y') <= min[1]) && (left  -> getMinBorder('z') <= min[2]) &&  (left != NULL)){
		return left -> crop(min,max);
	}
}
Pokemon* Region::getPokemon(int x, int y, int z){  // DAHA BİTMEDIIIIIIII !!!!  SIKINTI VARR BI DAHA BAKK
	int tmparr[3] = {x,y,z};
	if(getPokemonCount(tmparr,tmparr) == 0)
		return NULL;
	if(!isLeaf() && right == NULL && left == NULL )
		return NULL;
	else if(isLeaf() && min_x == x && min_y == y && min_z == z){
		if(pokemon == NULL){
			return NULL;
		}
		else{
			//Pokemon *tmp = new Pokemon(*pokemon);
			return pokemon;
		}
	}
	else if(right != NULL && right -> getMaxBorder('x') >= x && right -> getMaxBorder('y') >= y && right -> getMaxBorder('z') >= z && right -> getMinBorder('x') <= x && right -> getMinBorder('y') <= y && right -> getMinBorder('z') <= z &&  right != NULL ){
		return right -> getPokemon(x,y,z);
	}
	else if(left != NULL && left -> getMaxBorder('x') >= x && left  -> getMaxBorder('y') >= y && left  -> getMaxBorder('z') >= z && left  -> getMinBorder('x') <= x && left  -> getMinBorder('y') <= y && left  -> getMinBorder('z') <= z &&  left != NULL ){
		return left -> getPokemon(x,y,z);
	}
}

void Region::deletePokemon(int x,int y,int z){
	int tmparr[3] = {x,y,z};
	if(getPokemonCount(tmparr,tmparr) == 0)
		return ;
	if(!isLeaf() && right == NULL && left == NULL )
		return;
	else if(isLeaf() && min_x == x && min_y == y && min_z == z){
		if(pokemon == NULL){
			return;
		}
		else{
			delete pokemon;
			pokemon = NULL;
		}
	}
	else if(right != NULL && right -> getMaxBorder('x') >= x && right -> getMaxBorder('y') >= y && right -> getMaxBorder('z') >= z && right -> getMinBorder('x') <= x && right -> getMinBorder('y') <= y && right -> getMinBorder('z') <= z &&  right != NULL ){
		return right -> deletePokemon(x,y,z);
	}
	else if(left != NULL && left -> getMaxBorder('x') >= x && left  -> getMaxBorder('y') >= y && left  -> getMaxBorder('z') >= z && left  -> getMinBorder('x') <= x && left  -> getMinBorder('y') <= y && left  -> getMinBorder('z') <= z &&  left != NULL ){
		return left -> deletePokemon(x,y,z);
	}
}


void Region::patch(Region rhs){
	for (int i = rhs.min_x; i <= rhs.max_x; i++) {
        for (int j=rhs.min_y; j <= rhs.max_y; j++) {
            for (int k= rhs.min_z; k <= rhs.max_z; k++) {
            	Pokemon *tmp = rhs.getPokemon(i,j,k);
      			if (tmp !=  NULL){
                    this->placePokemon(*tmp,i,j,k);
      			}
      			else{
      				this -> deletePokemon(i,j,k);

      			}
            }
        }
    }
    this -> deleteOperator();
}