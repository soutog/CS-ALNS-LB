#include "instance.h"
#include <vector>
#include <algorithm>

#define MAX_E_COST 100
#define MIN_E_COST 10
#define MAX_ALFA 10

void Instance::load_file(const char *const file_name)
{

	ifstream f_inst(file_name);
	if (!f_inst.is_open())
	{
		cout << "ERROR: File " << file_name << " not found!" << endl;
		exit(0);
	}
	int dummy;
	f_inst >> dummy;
	f_inst >> dummy;
	f_inst >> dummy;
	tot_demanda= 0;
	for(int i=0; i<K; ++i){
		f_inst >> q[i];
		tot_demanda += q[i];
		
	}
	for(int i=0; i<I; ++i){
		for(int j=0; j<2;++j){
			f_inst >> bf[i][j];
		}	
	}

	for(int i=0; i<I; ++i){
		for(int j=0; j<J;++j){
			f_inst >> c[i][j];
		}	
	}
	for(int i=0; i<J; ++i){
		for(int j=0; j<2;++j){
			f_inst >> pg[i][j];
		}	
	}
	
	for(int i=0; i<J; ++i){
		for(int j=0; j<K ;++j){
			f_inst >> d[i][j];
		}	
	}

	
}
void Instance::calculaMinMaxFabDep()
{
	std::vector<int> fab(I);
	for(int i =0; i < I; ++i)
	{
		fab[i] = bf[i][0];
	}
	std::sort(fab.begin(),fab.end());
	std::vector<int> dep(J);
	for(int i =0; i < J; ++i)
	{
		dep[i] = pg[i][0];
	}
	std::sort(dep.begin(),dep.end());
	int capacidade = 0;
	betaFab = 0;
	for(int i = 0; i < I; ++i){
		if(capacidade < tot_demanda){
			betaFab++;
			capacidade +=fab[i];
		}else break;
	}
	capacidade = 0;
	piFab = 0;
	for(int i = I-1; i >= 0; --i){
		if(capacidade < tot_demanda){
			piFab++;
			capacidade +=fab[i];
		}else break;
	}
	
	capacidade = 0;
	betaDep = 0;
	for(int i = 0; i < J; ++i){
		if(capacidade < tot_demanda){
			betaDep++;
			capacidade +=dep[i];
		}else break;
	}
	capacidade = 0;
	piDep = 0;
	for(int i = J-1; i >= 0; --i){
		if(capacidade < tot_demanda){
			piDep++;
			capacidade +=dep[i];
		}else break;
	}



}
