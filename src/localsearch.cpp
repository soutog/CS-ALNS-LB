#include "localsearch.h"
#include <numeric>


LocalSearch::LocalSearch(Instance &p_,GraphCost *_g):p(p_)
{
	S.resize(I+J);
    srand(clock());
    is_feasible = new CheckFeas(p,_g);
	functionvectorRVND.emplace_back(&LocalSearch::NeighborhoodSwap);
	functionvectorRVND.emplace_back(&LocalSearch::NeighborhoodClose);
	functionvectorRVND.emplace_back(&LocalSearch::NeighborhoodOpen);
}

void LocalSearch::sumElements(vector<int> &solution){
	numFabAbe= std::accumulate(solution.begin(),solution.begin()+I,0);
	numDepAbe= std::accumulate(solution.begin()+(I+1),solution.end(),0);
}

void LocalSearch::CalculaCapacidade(std::vector<int> &solution)
{
    int capacidade = 0;
	for (int i=0; i<I; i++){
		if (solution[i] > 0){
			capacidade +=  p.bf[i][0];
		}	
	}
	capFab = capacidade;
	capacidade = 0;
	for (int j=0; j<J; j++){
		if (solution[I+j] > 0){
			capacidade +=  p.pg[j][0];
		}				
	}
    capDep = capacidade;

}

bool LocalSearch::verifyCapacity(vector<int> &solutionCurrent,unsigned i,unsigned j,unsigned neighborhood){
	int capacityCalc = 0;
	if(neighborhood == 1){
		if(i >= I){
			 capacityCalc = (capDep - p.pg[i][0] +p.pg[j][0]);
			 return (capacityCalc >=p.tot_demanda);
		}
		capacityCalc = (capFab - p.bf[i][0] +p.bf[j][0]);
		return (capacityCalc >=p.tot_demanda);
		
		
	}
	if(neighborhood == 2){
		if(i >= I){
			 capacityCalc = (capDep - p.pg[i][0]);
			 return (capacityCalc >=p.tot_demanda);
		}
		capacityCalc = (capFab - p.bf[i][0] );
		return (capacityCalc >=p.tot_demanda);
		
	}
	if(i >= I){
		capacityCalc = (capDep + p.pg[i][0]);
		return (capacityCalc >=p.tot_demanda);
	}
	capacityCalc = (capFab + p.bf[i][0] );
	return (capacityCalc >=p.tot_demanda);	

}

void LocalSearch::solveRVND(vector<int> &solution)
{
	vector <int> Stemp(solution);
	CalculaCapacidade(solution);
	cost = is_feasible->check(solution);
	//cout << cost << endl;
	sumElements(solution);
	int costtemp = cost;
	bool improved = true;
	int neighborhood = 0;
	int neighborhood_max = functionvectorRVND.size();
	random_shuffle( functionvectorRVND.begin(), functionvectorRVND.end() );
	while(neighborhood < neighborhood_max){
		(this->*functionvectorRVND[neighborhood])(Stemp);
		if(cost < costtemp){
			costtemp = cost;
			if(neighborhood > 0 ){
				neighborhood = 0;
			}
		}else{
			neighborhood++;
		}
	}

}
void LocalSearch::NeighborhoodSwap(vector<int> &solution)
{
	vector <int> Stemp(solution);
	CalculaCapacidade(solution);
	cost = is_feasible->check(solution);
	//cout << cost << endl;
	sumElements(solution);
	int costtemp = cost;
	bool improved = true;
	int i,j, bstI=0,bstJ=0,bstVal = cost;

	while(improved)
	{
		improved = false;
		//Swap de fabricas abertas e fechadas
		for(i=0;i<I;++i)
		{
			if(Stemp[i]>0){
				for(j = 0;j<I;++j)
				{
						if(Stemp[j]==0){
							Stemp[i] = 0;
							Stemp[j] = 1;
							if(verifyCapacity(Stemp,i,j,1)){
								costtemp = is_feasible->check(Stemp);
								if (costtemp < bstVal){
									/* cout << "improved swap fabrica" << endl;
									cout << "Cost" << cost << endl;
									cout << "Cost Improved" << costtemp << endl; */
									improved = true;
									bstVal = costtemp;
									bstI = i;
									bstJ = j;
								}
							}
							costtemp=cost;
							Stemp[i] = 1;
							Stemp[j] = 0;

						}
				}
			}
		}
		for(i=0;i<J;++i){
			if(Stemp[I+i]>0){
				for(j = 0;j<J;++j)
				{
						if(Stemp[I+j]==0){
							Stemp[I+i] = 0;
							Stemp[I+j] = 1;
							if(verifyCapacity(Stemp,I+i,I+j,1)){
								costtemp = is_feasible->check(Stemp);
								if (costtemp < bstVal){
									/* cout << "improved swap deposito" << endl;
									cout << "Cost" << cost << endl;
									cout << "Cost Improved" << costtemp << endl; */
									improved = true;
									bstVal = costtemp;
									bstI = I+i;
									bstJ = I+j;
								}
							}
							Stemp[I+i] = 1;
							Stemp[I+j] = 0;
							costtemp = cost;
						}
				}
			}
			 
		}
		if(improved){
			if(bstI > I){
					Stemp[bstI] = 0;
					Stemp[bstJ] = 1;
					solution[bstI] = 0;
					solution[bstJ] = 1;
					capDep = capDep - p.pg[bstI][0] + p.pg[bstJ][0];
			}else{
					Stemp[bstI] = 0;
					Stemp[bstJ] = 1;
					solution[bstI] = 0;
					solution[bstJ] = 1;
					capFab = capFab - p.bf[bstI][0] + p.bf[bstJ][0] ;
			}
			cost = bstVal;
		}

	}
	S = solution;


}

void LocalSearch::NeighborhoodClose(vector<int> &solution)
{
	vector <int> Stemp(solution);
	CalculaCapacidade(solution);
	cost = is_feasible->check(solution);
	//cout << cost << endl;
	sumElements(solution);
	int costtemp = cost;
	bool improved = true;
	int i,j, bstI=0,bstJ=0,bstVal = cost;
	
	while(improved)
	{
		improved = false;
		//Fechamento de fabricas
		for(i = 0;i<I;++i)
		{
			if(numFabAbe > p.piFab){
				if(Stemp[i]>0){
					Stemp[i]=0;
					if(verifyCapacity(Stemp,i,j,2)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < bstVal){
							/* cout << "improved fechamento fabrica" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost Improved" << costtemp << endl; */
							improved = true;
							bstI = i;
							bstJ = -1;	
						}
					}
					Stemp[i]=1;
					costtemp = cost;
				}
			}else{
			
				break;
			}
		}
		//Fechamento de depositos
		for(i = 0;i< J;++i)
		{
			if(numDepAbe > p.piDep){
				if(Stemp[i+I]>0){
					Stemp[i+I]=0;
					if(verifyCapacity(Stemp,I+i,j,2)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < bstVal){
						/* 	cout << "improved fechamento deposito" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost Improved" << costtemp << endl; */
							improved = true;
							bstVal = costtemp;
							bstI = i+I;
							bstJ = -1;
						}
					}
					Stemp[i+I]=1;
					costtemp = cost;
				}
			}else{
				break;
			}
		}
		if(improved){
			//cout << "veio aki";
			if(bstI > I){
					Stemp[bstI] = 0;
					solution[bstI] = 0;
					numDepAbe--;
					capDep = capDep - p.pg[bstI][0];
			}
			else{
					Stemp[bstI] = 0;
					solution[bstI] = 0;
					numFabAbe--;
					capFab = capFab - p.bf[bstI][0];
			}
			cost = bstVal;
		}
	}
	S = solution;
}

void LocalSearch::NeighborhoodOpen(vector<int> &solution)
{
	vector <int> Stemp(solution);
	CalculaCapacidade(solution);
	cost = is_feasible->check(solution);
	//cout << cost << endl;
	sumElements(solution);
	int costtemp = cost;
	bool improved = true;
	int i,j, bstI=0,bstJ=0,bstVal = cost;
	
	while(improved)
	{
		improved = false;
		//Abertura de fabricas
		for(i = 0;i<I;++i)
		{
			if(numFabAbe < p.betaFab){
				if(Stemp[i]==0){
					Stemp[i]=1;
					if(verifyCapacity(Stemp,i,j,3)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < bstVal){
							/* cout << "improved abertura fabrica" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost Improved" << costtemp << endl; */
							improved = true;
							bstVal = costtemp;
							bstI = i;
							bstJ = -1;	
						}
					}
					Stemp[i]=0;
					costtemp = cost;
				}
			}
			else{
				break;
			}
		}
		for(i = 0;i< J;++i)
		{
			if(numDepAbe < p.betaDep){
				if(Stemp[i+I]==0){
					Stemp[i+I]=1;
					if(verifyCapacity(Stemp,I+i,j,3)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < bstVal){
							/* cout << "improved abertura deposito" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost Improved" << costtemp << endl; */
							improved = true;
							bstVal = costtemp;
							bstI = i+I;
							bstJ = -1;
							
						}
					}
					Stemp[i+I]=0;
					costtemp = cost;
				}
			}else{
				break;
			}
		}
		if(improved){
			//cout << "veio aki";
			if(bstI > I){
					Stemp[bstI] = 1;
					solution[bstI] = 1;
					numDepAbe++;
					capDep = capDep + p.pg[bstI][0];
			}
			else{
					Stemp[bstI] = 1;
					solution[bstI] = 1;
					numFabAbe++;
					capFab = capFab + p.bf[bstI][0];
			}
			cost = bstVal;
		}
	}
	S = solution;

}

void LocalSearch::solveFI(vector<int> &solution)
{
	vector <int> Stemp(solution);
	sumElements(solution);
	CalculaCapacidade(solution);
	cost = is_feasible->check(solution);
	int costtemp = cost;
	bool improved = true;
	int i,j;
	
	
	while(improved)
	{
		improved = false;
		//Swap de fabricas abertas e fechadas
		for(i=0;i<I;++i)
		{
			if(Stemp[i]>0){
				for(j = 0;j<I;++j)
				{
					if(i != j){
						if(Stemp[j]==0){
							Stemp[i] = 0;
							Stemp[j] = 1;
							if(verifyCapacity(Stemp,i,j,1)){
								costtemp = is_feasible->check(Stemp);
								if (costtemp < cost){
									cout << "improved swap fabrica" << endl;
									cout << "Cost" << cost << endl;
									cout << "Cost Improved" << costtemp << endl;
									improved = true;
									cost = costtemp;
									solution[i] = 0;
									solution[j] = 1;
									//goto SolutionEnd;
								}else{
									costtemp=cost;
									Stemp[i] = 1;
									Stemp[j] = 0;
								}
							}else{
								costtemp=cost;
								Stemp[i] = 1;
								Stemp[j] = 0;
							}

						}
					}
				}
			}
		}
		//Fechamento de fabricas
		for(i = 0;i<I;++i)
		{
			if(numFabAbe > p.piFab){
				if(Stemp[i]>0){
					Stemp[i]=0;
					if(verifyCapacity(Stemp,i,j,2)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < cost){
							cout << "improved fechamento fabrica" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost Improved" << costtemp << endl;
							improved = true;
							cost = costtemp;
							solution[i] = 0;
							numFabAbe--;
						}
						else{
							Stemp[i]=1;
							costtemp = cost;
						}
					}else{
						Stemp[i]=1;
						costtemp = cost;
					}
				}
			}else{
			
				break;
			}
		}
		//Abertura de fabricas
		for(i = 0;i<I;++i)
		{
			if(numFabAbe < p.betaFab){
				if(Stemp[i]==0){
					Stemp[i]=1;
					if(verifyCapacity(Stemp,i,j,3)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < cost){
							cout << "improved abertura fabrica" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost Improved" << costtemp << endl;
							improved = true;
							cost = costtemp;
							solution[i] = 1;
							numFabAbe++;
							//goto SolutionEnd;	
						}
						else{
							Stemp[i]=0;
							costtemp = cost;
						}
					}else{
							Stemp[i]=0;
							costtemp = cost;
					}
				}
			}
			else{
				break;
			}
		}
		//Swap de depositos abertos e fechados
		for(i=0;i<J;++i){
			if(Stemp[I+i]>0){
				for(j = 0;j<J;++j)
				{
					if(Stemp[I+j]==0){
						Stemp[I+i] = 0;
						Stemp[I+j] = 1;
						if(verifyCapacity(Stemp,I+i,I+j,1)){
							costtemp = is_feasible->check(Stemp);
							if (costtemp < cost){
								cout << "improved swap deposito" << endl;
								cout << "Cost" << cost << endl;
								cout << "Cost Improved" << costtemp << endl;
								improved = true;
								cost=costtemp;
								solution[I+i] = 0;
								solution[I+j] = 1;
								//goto SolutionEnd;
							}else{
								costtemp=cost;
								Stemp[I+i] = 1;
								Stemp[I+j] = 0;
							}
						}else{
								costtemp=cost;
								Stemp[I+i] = 1;
								Stemp[I+j] = 0;
						}
					
					}
				}
			}
			
		} 
		//Fechamento de depositos
		for(i = 0;i< J;++i)
		{
			if(numDepAbe > p.piDep){
				if(Stemp[i+I]>0){
					Stemp[i+I]=0;
					if(verifyCapacity(Stemp,I+i,j,2)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < cost){
							cout << "improved fechamento deposito" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost Improved" << costtemp << endl;
							improved = true;
							cost = costtemp;
							solution[i+I] = 0;
							numDepAbe--;
							//goto SolutionEnd;	
						}
						else{
							Stemp[i+I]=1;
							costtemp = cost;
						}
					}else{
							Stemp[i+I]=1;
							costtemp = cost;
					}
				}
			}else{
				break;
			}
		}
		
		for(i = 0;i< J;++i)
		{
			if(numDepAbe < p.betaDep){
				if(Stemp[i+I]==0){
					Stemp[i+I]=1;
					costtemp = is_feasible->check(Stemp);
					if(verifyCapacity(Stemp,I+i,j,3)){
						if (costtemp < cost){
							cout << "improved abertura deposito" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost Improved" << costtemp << endl;
							improved = true;
							cost = costtemp;
							solution[i+I] = 1;
							numDepAbe++;
							//goto SolutionEnd;	
						}
						else{
							Stemp[i+I]=0;
							costtemp = cost;
						}
					}else{
							Stemp[i+I]=0;
							costtemp = cost;
					}
				}
			}else{
				break;
			}
		}

	
	
	}
   S = solution;

    
}

void LocalSearch::solve(vector<int> &solution)
{
	vector <int> Stemp(solution);
	S.resize(I+J);
	CalculaCapacidade(solution);
	cost = is_feasible->check(solution);
	//cout << cost << endl;
	sumElements(solution);
	int costtemp = cost;
	bool improved = true;
	int i,j, bstI=0,bstJ=0,bstVal = cost;
	
	while(improved)
	{
		improved = false;
		//Swap de fabricas abertas e fechadas
		for(i=0;i<I;++i)
		{
			if(Stemp[i]>0){
				for(j = 0;j<I;++j)
				{
						if(Stemp[j]==0){
							Stemp[i] = 0;
							Stemp[j] = 1;
							if(verifyCapacity(Stemp,i,j,1)){
								costtemp = is_feasible->check(Stemp);
								if (costtemp < bstVal){
									/* cout << "improved swap fabrica" << endl;
									cout << "Cost" << cost << endl;
									cout << "Cost Improved" << costtemp << endl; */
									improved = true;
									bstVal = costtemp;
									bstI = i;
									bstJ = j;
								}
							}
							costtemp=cost;
							Stemp[i] = 1;
							Stemp[j] = 0;

						}
				}
			}
		}
		//Fechamento de fabricas
		for(i = 0;i<I;++i)
		{
			if(numFabAbe > p.piFab){
				if(Stemp[i]>0){
					Stemp[i]=0;
					if(verifyCapacity(Stemp,i,j,2)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < bstVal){
							/* cout << "improved fechamento fabrica" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost Improved" << costtemp << endl; */
							improved = true;
							bstI = i;
							bstJ = -1;	
						}
					}
					Stemp[i]=1;
					costtemp = cost;
				}
			}else{
			
				break;
			}
		}
		//Abertura de fabricas
		for(i = 0;i<I;++i)
		{
			if(numFabAbe < p.betaFab){
				if(Stemp[i]==0){
					Stemp[i]=1;
					if(verifyCapacity(Stemp,i,j,3)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < bstVal){
							/* cout << "improved abertura fabrica" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost Improved" << costtemp << endl; */
							improved = true;
							bstVal = costtemp;
							bstI = i;
							bstJ = -1;	
						}
					}
					Stemp[i]=0;
					costtemp = cost;
				}
			}
			else{
				break;
			}
		}
		//Swap de depositos abertos e fechados
		for(i=0;i<J;++i){
			if(Stemp[I+i]>0){
				for(j = 0;j<J;++j)
				{
						if(Stemp[I+j]==0){
							Stemp[I+i] = 0;
							Stemp[I+j] = 1;
							if(verifyCapacity(Stemp,I+i,I+j,1)){
								costtemp = is_feasible->check(Stemp);
								if (costtemp < bstVal){
									/* cout << "improved swap deposito" << endl;
									cout << "Cost" << cost << endl;
									cout << "Cost Improved" << costtemp << endl; */
									improved = true;
									bstVal = costtemp;
									bstI = I+i;
									bstJ = I+j;
								}
							}
							Stemp[I+i] = 1;
							Stemp[I+j] = 0;
							costtemp = cost;
						}
				}
			}
			 
		}
		//Fechamento de depositos
		for(i = 0;i< J;++i)
		{
			if(numDepAbe > p.piDep){
				if(Stemp[i+I]>0){
					Stemp[i+I]=0;
					if(verifyCapacity(Stemp,I+i,j,2)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < bstVal){
						/* 	cout << "improved fechamento deposito" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost Improved" << costtemp << endl; */
							improved = true;
							bstVal = costtemp;
							bstI = i+I;
							bstJ = -1;
						}
					}
					Stemp[i+I]=1;
					costtemp = cost;
				}
			}else{
				break;
			}
		}
		
		for(i = 0;i< J;++i)
		{
			if(numDepAbe < p.betaDep){
				if(Stemp[i+I]==0){
					Stemp[i+I]=1;
					if(verifyCapacity(Stemp,I+i,j,3)){
						costtemp = is_feasible->check(Stemp);
						if (costtemp < bstVal){
							/* cout << "improved abertura deposito" << endl;
							cout << "Cost" << cost << endl;
							cout << "Cost Improved" << costtemp << endl; */
							improved = true;
							bstVal = costtemp;
							bstI = i+I;
							bstJ = -1;
							
						}
					}
					Stemp[i+I]=0;
					costtemp = cost;
				}
			}else{
				break;
			}
		}

		if(improved){
			//cout << "veio aki";
			if(bstI > I){
				if(bstJ== -1 && solution[bstI] == 0){
					Stemp[bstI] = 1;
					solution[bstI] = 1;
					numDepAbe++;
					capDep = capDep + p.pg[bstI][0];
				}else if(bstJ== -1){
					Stemp[bstI] = 0;
					solution[bstI] = 0;
					numDepAbe--;
					capDep = capDep - p.pg[bstI][0];
				}else{
					Stemp[bstI] = 0;
					Stemp[bstJ] = 1;
					solution[bstI] = 0;
					solution[bstJ] = 1;
					capDep = capDep - p.pg[bstI][0] + p.pg[bstJ][0] ;
				}
			}else{
				if(bstJ== -1 && solution[bstI] == 0){
					Stemp[bstI] = 1;
					solution[bstI] = 1;
					numFabAbe++;
					capFab = capFab + p.bf[bstI][0];
				}else if(bstJ== -1){
					Stemp[bstI] = 0;
					solution[bstI] = 0;
					numFabAbe--;
					capFab = capFab - p.bf[bstI][0];
				}else{
					Stemp[bstI] = 0;
					Stemp[bstJ] = 1;
					solution[bstI] = 0;
					solution[bstJ] = 1;
					capFab = capFab - p.bf[bstI][0] + p.bf[bstJ][0] ;
				}
			}
			cost = bstVal;
		}
	}
    S = solution;

    
}