#ifndef HEURISTIC_H_
#define HEURISTIC_H_

#include <vector>
#include <queue>
#include <algorithm>
#include <random>

#include "instance.h"
#include "checkfeas.h"
#include "stdlib.h"
#include "graphcost.h"
#include "time.h"

class Heuristic{
	private:
		Instance &p;
		CheckFeas *is_feasible;
		Heuristic *heuristic;
		
		vector<int> S;
		vector<pair<int,double>> hcapfab;
		vector<pair<int,double>> hcapdep;
		vector<pair<int,double>> hcapfabMed;
		vector<pair<int,double>> hcapdepMed;
		vector<pair<int,double>> hcapfabCustoAbertura;
		vector<pair<int,double>> hcapdepCustoAbertura;
		vector<pair<int,double>> hcapfabCustoTransporte;
		vector<pair<int,double>> hcapdepCustoTransporte;
		vector<pair<int,double>> hcapfabCustoTransporteMed;
		vector<pair<int,double>> hcapdepCustoTransporteMed;

		vector<int> RCL;

		double cost;
		void construtivo_fab(double alpha,int value_constructive);
		void construtivo_dep(double alpha,int value_constructive);
		int aleatorio(int lower,int upper);
		void CostBenCalculate();
		void CostBenCalculateCustoAbertura();
		void CostBenCalculateCustoTransporte();
		void CostBenCalculateCustoTransporteMed();
		void CostBenCalculateMed();

	public : 
	    Heuristic(Instance &_p,GraphCost *_g);
		void genRCL(vector<pair<int,double>>& candidateList,double hmin,double hmax,double alpha);
		void genRCLMax(vector<pair<int,double>>& candidateList,double hmin,double hmax,double alpha);
		void run(double alpha,int iter);
		vector<int>& get_Solution(){return S;}
		vector<pair<int,double>>& get_hcapfab(){return hcapfab;}
		vector<pair<int,double>>& get_hcapdep(){return hcapdep;}
		vector<pair<int,double>>& get_hcapfabMed(){return hcapfabMed;}
		vector<pair<int,double>>& get_hcapdepMed(){return hcapdepMed;}
		vector<pair<int,double>>& get_hcapfabCustoAbertura(){return hcapfabCustoAbertura;}
		vector<pair<int,double>>& get_hcapdepCustoAbertura(){return hcapdepCustoAbertura;}
		vector<pair<int,double>>& get_hcapfabCustoTransporte(){return hcapfabCustoTransporte;}
		vector<pair<int,double>>& get_hcapdepCustoTransporte(){return hcapdepCustoTransporte;}
		vector<pair<int,double>>& get_hcapfabCustoTransporteMed(){return hcapfabCustoTransporteMed;}
		vector<pair<int,double>>& get_hcapdepCustoTransporteMed(){return hcapdepCustoTransporteMed;}
		vector<int>& get_RCL(){return RCL;}
		double get_cost(){return cost;}
};

#endif
