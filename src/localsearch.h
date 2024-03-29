#ifndef LOCAL_SEARCH_H_
#define LOCAL_SEARCH_H_

#include <vector>
#include <queue>
#include <algorithm>
#include <random>

#include "instance.h"
#include "checkfeas.h"
#include "stdlib.h"
#include "graphcost.h"
#include "time.h"

class LocalSearch{
	typedef void (LocalSearch::*LocalSearchFunc)(std::vector<int> &solution);
	private:
		Instance p;
		CheckFeas *is_feasible;

		int numFabAbe;
        int numDepAbe;
		double capFab;
		double capDep;
		vector<int> S;
		vector<LocalSearchFunc> functionvectorRVND;
		
		double cost;
        void sumElements(vector<int> &solution);
		void CalculaCapacidade(std::vector<int> &solution);
		bool verifyCapacity(vector<int> &solutionCurrent,unsigned i,unsigned j,unsigned neighborhood);
		void NeighborhoodOpen(vector<int> &solution);
		void NeighborhoodClose(vector<int> &solution);
		void NeighborhoodSwap(vector<int> &solution);
	public : 
	    LocalSearch(Instance &_p,GraphCost *_g);
		void solveRVND(vector<int> &solution);
		void solve(vector<int> &solution);
		void solveFI(vector<int> &solution);
		vector<int>& get_Solution(){return S;}
		double get_cost(){return cost;}
};

#endif