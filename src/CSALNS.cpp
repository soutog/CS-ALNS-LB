#include "CSALNS.h"

#include "localsearch.h"

using namespace std;
CSALNS::CSALNS(Instance &_p,GraphCost *_g,Heuristic &_constructive):p(_p),constructive(_constructive)
{
	g = _g;
	is_feasible = new CheckFeas(p,_g);

}
bool comparebyfirstGrasp(pair<int,double> a,pair<int,double> b){
	return (a.first < b.first);
}

bool comparebysecondGrasp(pair<int,double> a,pair<int,double> b){
	return (a.second < b.second);
}
bool comparebysecondDesc(pair<int,double> a,pair<int,double> b){
	return (a.second > b.second);
}

void CSALNS::DestroyRandomSolution(vector<int>&Solution,double destroy_rate){
	vector<int> destroySolution;
	for(int i=0;i<Solution.size();++i){
		if(Solution[i]==1){
			destroySolution.emplace_back(i);
		}
	}
	int solutionsDestroyed = 0;
	int maxDestroyed = int(destroy_rate*destroySolution.size());
	int index_destroyed = 0;
	int index_selected = 0;
	while(solutionsDestroyed < maxDestroyed){
		index_destroyed = rand()%destroySolution.size();
		index_selected = destroySolution[index_destroyed];
		Solution[index_selected] = 0;
		destroySolution.erase(destroySolution.begin()+index_destroyed);
		solutionsDestroyed++;
	}
}
void CSALNS::DestroySemiRandomSolution(vector<int>&Solution,double destroy_rate,double alpha,Heuristic constructive, int value_constructive){
	unsigned n = Solution.size();
	vector <pair<int,double>> CandidateListDestroyFab,CandidateListDestroyDep;

	switch (value_constructive)
	{
	case 0:
		CandidateListDestroyFab.insert(CandidateListDestroyFab.end(), constructive.get_hcapfab().begin(), constructive.get_hcapfab().end());
		CandidateListDestroyDep.insert(CandidateListDestroyDep.end(), constructive.get_hcapdep().begin(), constructive.get_hcapdep().end());
		break;
	case 1:
		CandidateListDestroyFab.insert(CandidateListDestroyFab.end(), constructive.get_hcapfabMed().begin(), constructive.get_hcapfabMed().end());
		CandidateListDestroyDep.insert(CandidateListDestroyDep.end(), constructive.get_hcapfabMed().begin(), constructive.get_hcapfabMed().end());
		break;
	case 2:
		CandidateListDestroyFab.insert(CandidateListDestroyFab.end(), constructive.get_hcapfabCustoAbertura().begin(), constructive.get_hcapfabCustoAbertura().end());
		CandidateListDestroyDep.insert(CandidateListDestroyDep.end(), constructive.get_hcapdepCustoAbertura().begin(), constructive.get_hcapdepCustoAbertura().end());
		break;
	case 3:
		CandidateListDestroyFab.insert(CandidateListDestroyFab.end(), constructive.get_hcapfabCustoTransporte().begin(), constructive.get_hcapfabCustoTransporte().end());
		CandidateListDestroyDep.insert(CandidateListDestroyDep.end(), constructive.get_hcapdepCustoTransporte().begin(), constructive.get_hcapdepCustoTransporte().end());
		break;
	case 4:
		CandidateListDestroyFab.insert(CandidateListDestroyFab.end(), constructive.get_hcapfabCustoTransporteMed().begin(), constructive.get_hcapfabCustoTransporteMed().end());
		CandidateListDestroyDep.insert(CandidateListDestroyDep.end(), constructive.get_hcapdepCustoTransporteMed().begin(), constructive.get_hcapdepCustoTransporteMed().end());
		break;
	default:
		exit(0);
		break;
	}
	int objval = 0;
	int capacidadeFab = 0,capacidadeDep = 0;
	sort(CandidateListDestroyFab.begin(),CandidateListDestroyFab.end(),comparebyfirstGrasp);
	sort(CandidateListDestroyDep.begin(),CandidateListDestroyDep.end(),comparebyfirstGrasp);
	int deletedFab = 0,deletedDep=0;
	for(unsigned i=0; i < n; ++i)
	{
		if (i < I){
			if(Solution[i] == 1){
				CandidateListDestroyFab.erase(CandidateListDestroyFab.begin()+(i-deletedFab));
				deletedFab++;
			}
		}else{
			if(Solution[i] == 1){
				CandidateListDestroyDep.erase(CandidateListDestroyDep.begin()+(i-I-deletedDep));
				deletedDep++;
			}
		}
			

	}
	sort(CandidateListDestroyFab.begin(),CandidateListDestroyFab.end(),comparebysecondDesc);
	sort(CandidateListDestroyDep.begin(),CandidateListDestroyDep.end(),comparebysecondDesc);
	double hmin,hmax;
	int random = 0;
	int solutionsDestroyed = 0;
	int maxDestroyed = int(destroy_rate*CandidateListDestroyFab.size());

	while(solutionsDestroyed < maxDestroyed){
			hmin = CandidateListDestroyFab[0].second;
			hmax = CandidateListDestroyFab[CandidateListDestroyFab.size()-1].second;
			constructive.genRCLMax(CandidateListDestroyFab,hmin,hmax,alpha);
			vector<int> RCL(constructive.get_RCL());
			random = rand()%RCL.size();
			int index = CandidateListDestroyFab[RCL[random]].first;
			Solution[index] = 0;
			CandidateListDestroyFab.erase(CandidateListDestroyFab.begin()+random);
			solutionsDestroyed++;
	}
	solutionsDestroyed = 0;
	maxDestroyed = int(destroy_rate*CandidateListDestroyDep.size());
	while(solutionsDestroyed < maxDestroyed){
			hmin = CandidateListDestroyDep[0].second;
			hmax = CandidateListDestroyDep[CandidateListDestroyDep.size()-1].second;
			constructive.genRCLMax(CandidateListDestroyDep,hmin,hmax,alpha);
			vector<int> RCL(constructive.get_RCL());
			random = rand()%RCL.size();
			int index = CandidateListDestroyDep[RCL[random]].first;
			Solution[index] = 0;
			CandidateListDestroyDep.erase(CandidateListDestroyDep.begin()+random);
			solutionsDestroyed++;
	}
}

void CSALNS::VerifyCapacity(vector<int> &Solution){
	int capacidadeFab = 0, capacidadeDep=0;
	for(int i =0;i<I;++i){
		if(Solution[i] == 1){
			capacidadeFab += p.bf[i][0];
		}
		
	}
	for(int i =0;i<J;++i){
		if(Solution[i+I] == 1){
			capacidadeDep += p.pg[i][0];
		}
	}
	cout << capacidadeFab << endl;
	cout << capacidadeDep << endl;
}

void CSALNS::RepairRandom(vector<int> &Solution){
	unsigned n = Solution.size();
	vector <int> RepairSolutionFab,RepairSolutionDep;
	int objval = 0;
	int capacidadeFab = 0,capacidadeDep = 0;
	for(unsigned i=0; i < n; ++i)
	{
		if (i < I){
			if(Solution[i] == 1){
				capacidadeFab += p.bf[i][0];
			}
			else{
				RepairSolutionFab.emplace_back(i);
			}
		}else{
			if(Solution[i] == 1){
				capacidadeDep += p.pg[i-I][0] ;
			}
			else{
				RepairSolutionDep.emplace_back(i-I);
			}


		}
			

	}
	int random = 0;
	if(capacidadeFab < p.tot_demanda){
		while(capacidadeFab < p.tot_demanda){
			random = rand()%RepairSolutionFab.size();
			int index = RepairSolutionFab[random];
			Solution[index] = 1;
			capacidadeFab += p.bf[index][0];
			RepairSolutionFab.erase(RepairSolutionFab.begin()+random);
		}
	}
	if(capacidadeDep < p.tot_demanda){
		while(capacidadeDep < p.tot_demanda){
			random = rand()%RepairSolutionDep.size();
			int index = RepairSolutionDep[random];
			Solution[index+I] = 1;
			capacidadeDep += p.pg[index][0];
			RepairSolutionDep.erase(RepairSolutionDep.begin()+random);
		}
	}
	
}

void CSALNS::RepairSemiRandom(vector<int> &Solution,double alpha, Heuristic &constructive,int value_constructive){
	unsigned n = Solution.size();
	vector <pair<int,double>> CandidateListRepairFab,CandidateListRepairDep; 
	switch (value_constructive)
	{
	case 0:
		CandidateListRepairFab.insert(CandidateListRepairFab.end(), constructive.get_hcapfab().begin(), constructive.get_hcapfab().end());
		CandidateListRepairDep.insert(CandidateListRepairDep.end(), constructive.get_hcapdep().begin(), constructive.get_hcapdep().end());
		break;
	case 1:
		CandidateListRepairFab.insert(CandidateListRepairFab.end(), constructive.get_hcapfabMed().begin(), constructive.get_hcapfabMed().end());
		CandidateListRepairDep.insert(CandidateListRepairDep.end(), constructive.get_hcapdepMed().begin(), constructive.get_hcapdepMed().end());
		break;
	case 2:
		CandidateListRepairFab.insert(CandidateListRepairFab.end(), constructive.get_hcapfabCustoAbertura().begin(), constructive.get_hcapfabCustoAbertura().end());
		CandidateListRepairDep.insert(CandidateListRepairDep.end(), constructive.get_hcapdepCustoAbertura().begin(), constructive.get_hcapdepCustoAbertura().end());
		break;
	case 3:
		CandidateListRepairFab.insert(CandidateListRepairFab.end(), constructive.get_hcapfabCustoTransporte().begin(), constructive.get_hcapfabCustoTransporte().end());
		CandidateListRepairDep.insert(CandidateListRepairDep.end(), constructive.get_hcapdepCustoTransporte().begin(), constructive.get_hcapdepCustoTransporte().end());
		break;
	case 4:
		CandidateListRepairFab.insert(CandidateListRepairFab.end(), constructive.get_hcapfabCustoTransporteMed().begin(), constructive.get_hcapfabCustoTransporteMed().end());
		CandidateListRepairDep.insert(CandidateListRepairDep.end(), constructive.get_hcapdepCustoTransporteMed().begin(), constructive.get_hcapdepCustoTransporteMed().end());
		break;
	default:
		exit(0);
		break;
	}
	
	int objval = 0;
	int capacidadeFab = 0,capacidadeDep = 0;
	sort(CandidateListRepairFab.begin(),CandidateListRepairFab.end(),comparebyfirstGrasp);
	sort(CandidateListRepairDep.begin(),CandidateListRepairDep.end(),comparebyfirstGrasp);
	int deletedFab = 0,deletedDep=0; 
	for(unsigned i=0; i < n; ++i)
	{
		if (i < I){
			if(Solution[i] == 1){
				capacidadeFab += p.bf[i][0];
				CandidateListRepairFab.erase(CandidateListRepairFab.begin()+(i-deletedFab));
				deletedFab++;
			}
		}else{
			if(Solution[i] == 1){
				capacidadeDep += p.pg[i-I][0];
				CandidateListRepairDep.erase(CandidateListRepairDep.begin()+(i-I-deletedDep));
				deletedDep++;
			}
		}
			

	}
	sort(CandidateListRepairFab.begin(),CandidateListRepairFab.end(),comparebysecondGrasp);
	sort(CandidateListRepairDep.begin(),CandidateListRepairDep.end(),comparebysecondGrasp);
	double hmin,hmax;
	int random = 0;
	if(capacidadeFab < p.tot_demanda){
		while(capacidadeFab < p.tot_demanda){
			hmin = CandidateListRepairFab[0].second;
			hmax = CandidateListRepairFab[CandidateListRepairFab.size()-1].second;
			constructive.genRCL(CandidateListRepairFab,hmin,hmax,alpha);
			vector<int> RCL(constructive.get_RCL());
			random = rand()%RCL.size();
			int index = RCL[random];
			Solution[index] = 1;
			capacidadeFab += p.bf[index][0];
			CandidateListRepairFab.erase(CandidateListRepairFab.begin()+random);
		}
	}
	if(capacidadeDep < p.tot_demanda){
		while(capacidadeDep < p.tot_demanda){
			hmin = CandidateListRepairDep[0].second;
			hmax = CandidateListRepairDep[CandidateListRepairDep.size()-1].second;
			constructive.genRCL(CandidateListRepairDep,hmin,hmax,alpha);
			vector<int> RCL(constructive.get_RCL());
			random = rand()%RCL.size();
			int index = RCL[random];
			Solution[index+I] = 1;
			capacidadeDep += p.pg[index][0];
			CandidateListRepairDep.erase(CandidateListRepairDep.begin()+random);
		}
	}
}


int CSALNS::HammingDistance(vector<int>&Solution1,vector<int>&Solution2){
	int distance = 0;
	for(int i=0; i < Solution1.size();++i){
		if (Solution1[i] != Solution2[i] ){
			distance ++;
		}
	}
	return distance;
}

void CSALNS::solve(double alpha,int maxiter,int _gama,int _ineficiency_max,int _maxvolume,double _destroy_alpha,double _repair_alpha,double _destroy_rate,clock_t hI){
	gama = _gama;
	int max_volume = _maxvolume;
	int qtd_clusters=0;
	cluster_solution.resize(gama);
	cluster_volume.resize(gama);
	cluster_cost.resize(gama);
	cluster_ineficiency.resize(gama);
	ineficiency_max = _ineficiency_max;
	clock_t hF;
	double probabilityDestroy = 0.5;
	double probabilityRepair = 0.5; 
	double random = 0;
	double destroyalpha = _destroy_alpha;
	double destroyrate = _destroy_rate;
	double repairalpha = _repair_alpha;
	double rate_destroy_penality = 0.05;

	int destroychosen;

	LocalSearch ls(p,g);
	costBest =0;
	
		constructive.run(alpha,1);
		vector<int> Solution(constructive.get_Solution());
		int cost=constructive.get_cost();

		SolutionBest = vector<int> (Solution);
		//cout << cost << endl;
		costBest = cost;
		
		
		int SemiRandomValue = 0;
		for(int i=0; i < maxiter;++i){
			
			random = (float) rand()/RAND_MAX;
			if(random > probabilityDestroy){
				DestroyRandomSolution(Solution,destroyrate);
				destroychosen = 0;
			}else{
				SemiRandomValue = rand()%3;
				DestroySemiRandomSolution(Solution,destroyrate,destroyalpha,constructive,SemiRandomValue);
				destroychosen = 1;
			} 
			SemiRandomValue = rand()%3;
			RepairSemiRandom(Solution,repairalpha,constructive,SemiRandomValue);

			cost=is_feasible->check(Solution);

			if (cost < costBest) {
				SolutionBest = vector<int>(Solution);
				costBest = cost;
				hF = clock();
				besttime = (double)(hF - hI) / CLOCKS_PER_SEC;
				if(destroychosen == 0){
					probabilityDestroy -=rate_destroy_penality;
				}else{
					probabilityDestroy +=rate_destroy_penality;
				}
				//cout << costBest << endl;
			}
			if(qtd_clusters < gama){
				cluster_solution[qtd_clusters] = Solution;
				cluster_cost[qtd_clusters] = cost;
				cluster_volume[qtd_clusters] = 0;
				cluster_ineficiency[qtd_clusters] = 0;
				qtd_clusters++;
			}else{
				//cout << "veio aki" << endl;
				int distancemin = 400,ci = 0;
				
				for(int c = 0; c < gama ; ++c ){
					if(HammingDistance(Solution,cluster_solution[c]) < distancemin){
						ci = c;
					}
				}
				cluster_volume[ci]++;
				if(cost <  cluster_cost[ci] ){
					cluster_solution[ci] = Solution;
					cluster_cost[ci] = cost;
				}
				if(cluster_volume[ci] == max_volume){
					cluster_volume[ci] = 0;
					
					ls.solveRVND(cluster_solution[ci]);
					cost = ls.get_cost();

					if( cost < cluster_cost[ci] ){
						cluster_cost[ci] = cost;
						cluster_solution[ci] = ls.get_Solution();
						if(cluster_cost[ci] < costBest){
							SolutionBest = vector<int>(cluster_solution[ci]);
							costBest = cluster_cost[ci];
							hF = clock();
							besttime = (double)(hF - hI) / CLOCKS_PER_SEC;
							//cout << costBest << endl;
						}
						cluster_ineficiency[ci] = 0;
					}else{
						cluster_ineficiency[ci] ++;
						if(cluster_ineficiency[ci] == ineficiency_max){
							SemiRandomValue = rand()%3;
							DestroySemiRandomSolution(cluster_solution[ci],destroyrate,destroyalpha,constructive,SemiRandomValue);	
							SemiRandomValue = rand()%3;
							RepairSemiRandom(cluster_solution[ci],repairalpha,constructive,SemiRandomValue);

							cluster_cost[ci] = is_feasible->check(cluster_solution[ci]);
						}
					}

				}

				
			}

		}

	
	//cout << endl << is_feasible->check(SolutionBest) << endl;
	/* cout << "end" << endl; */

}
