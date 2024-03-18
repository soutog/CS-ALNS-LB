#include <iostream>
#include <stdlib.h>
#include <memory>
#include <math.h>
#include <algorithm>
#include <vector>
#include <ctime>


#include "instance.h"
#include "checkfeas.h"
#include "heuristic.h"
#include "localsearch.h"
#include "graphcost.h"
#include "ExactTSCFL.h"
#include "CSALNS.h"

using namespace std;



void usage(char *argv[]){
	cout << "Usage:" <<endl;
	cout << "\t"<<argv[0]<<" <input_instance_name> <iter> <alpha> <maxiter> <gama> <maxvolume> <ineficiency_max> <delta> <alpha2> <sigma> <timeLB> <seed>" <<endl;
	cout << "\t"<<"<input_instance_name>: instance name"<<endl;
	cout << "\t"<<"<iter>: number of iterations from the algorithm" << endl;
	cout << "\t"<<"<alpha>: value for the constructive between [0,1] "<<endl;
	cout << "\t"<<"<maxiter>: maximum iterations for the metaheuristics CS-ALNS-LB "<<endl;
	cout << "\t"<<"<gama>: Cluster size"<<endl;
	cout << "\t"<<"<maxvolume>: Maximum volume of the cluster before the local search"<<endl;
	cout << "\t"<<"<ineficiency_max>: maximum ineficency for the cluster"<<endl;
	cout << "\t"<<"<delta>: value for the window of the semi-greedy destructive between [0,1]"<< endl;
	cout << "\t"<<"<alpha2>: value for the window of the semi-greedy repairer between [0,1] " << endl;
	cout << "\t"<<"<sigma>: Destroy rate between [0,1]"<< endl;
	cout << "\t"<<"<timeLB>: time for the local branch" << endl;
	cout << "\t"<<"<seed>: Seeds from the algorithm" << endl;	
}

int main(int argc, char *argv[]){
	if(argc!=13){
		usage(argv);
	}else{
		clock_t seedvalue = clock(); 
		//int seedvalue = atoi(argv[12]);
		srand(seedvalue);
		cout << "seed: " <<seedvalue << endl;

		const char* datafile = argv[1];
		int iter = atoi(argv[2]);
		double alpha = atof(argv[3]);
		int max_iter = atoi(argv[4]);
		int gama = atoi(argv[5]);
		int maxvol = atof(argv[6]);
		int ineficiency_max =  atoi(argv[7]);
		double destroy_alpha =  atof(argv[8]);
		double repair_alpha = atof(argv[9]);
		double destroy_rate = atof(argv[10]);
		int timeLB = atof(argv[11]);
		
		double random =  rand();

		Instance instance;
		instance.load_file(datafile);
		instance.calculaMinMaxFabDep();
		GraphCost *g = new GraphCost(instance);
		ExactTSCFL model(instance);
		Heuristic constructive(instance,g);
		model.ConstroiModelo();
		vector<int> Solution;
		
		/* clock_t hI = clock();
		Heuristic constructive(instance,g);
		constructive.run(alpha);
		cout << constructive.get_cost() << endl;
		LocalSearch ls(instance,g);
		ls.solveRVND(constructive.get_Solution()); 
		cout << ls.get_cost() << endl;
		clock_t hF = clock();
		double time = (double)(hF - hI) / CLOCKS_PER_SEC;
		cout << time << endl; */

		int64_t best_cost = 999999999; //p->get_nvertices() + 1;

		std::vector<int64_t> costs(iter);
		std::vector<double> time(iter);

		double avg = 0.0;
		double std_dev = 0.0;
		//avg = 0.0;
		double avg_time = 0.0;
		//std_dev = 0.0;
		double std_dev_time = 0.0;
		for (int64_t i = 0; i < iter; ++i)
		{
			
			CSALNS csalns(instance,g,constructive);
			clock_t hI = clock();
			csalns.solve(alpha,max_iter,gama,ineficiency_max,maxvol,destroy_alpha,repair_alpha,destroy_rate,hI);
			Solution = csalns.get_SolutionBest();
			costs[i]= csalns.get_costBest();
			clock_t hF = clock();
			double timebound = (double)(hF - hI) / CLOCKS_PER_SEC;
			tie(Solution,costs[i]) = model.solveLB(Solution,timeLB,double(costs[i]));
			hF = clock();
			time[i] = (double)(hF - hI) / CLOCKS_PER_SEC;
			cout << costs[i] << endl;
			//cout << time[i] << endl;
			avg += costs[i];
			avg_time += time[i];
			if (costs[i] < best_cost)
			{
				best_cost = costs[i];
			}
		}

		avg = avg / iter;
		avg_time = avg_time / iter;
		for (int64_t i = 0; i < iter; ++i)
		{
			std_dev += (costs[i] - avg) * (costs[i] - avg);
			std_dev_time += (time[i] - avg_time) * (time[i] - avg_time);
		}

		std_dev = std::sqrt(std_dev / iter);
		std_dev_time = std::sqrt(std_dev_time / iter);

		std::cout << "File Name: "<< datafile << " BST: " << best_cost << " AVG: " << avg << " STD: " << std_dev << " AVGTIME: " << avg_time << " STDTIME: " << std_dev_time << std::endl; 
		//std::cout << avg;
		/* CSALNS csalns(instance,g,constructive);
		//AG(double alpha,GraphCost &g, Instance &_instance, unsigned _n, unsigned _p, double _pe, double _pm, double _rhoe, unsigned _K, unsigned _MAXT, long unsigned _rngSeed, unsigned _generation, unsigned _X_INTVL, unsigned _X_NUMBER, unsigned _MAX_GENS)
		clock_t hI = clock();
		csalns.solve(alpha,max_iter,gama,ineficiency_max,maxvol,destroy_alpha,repair_alpha,destroy_rate);
		Solution = csalns.get_SolutionBest();
		int costs= csalns.get_costBest();
		cout << costs; */

	}
 
	return 0;
}
