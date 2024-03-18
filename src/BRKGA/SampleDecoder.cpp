#include "SampleDecoder.h"
#include <cmath>

#define Infinity 10E6

SampleDecoder::SampleDecoder(Instance &_instance, CheckFeas* _checador,LocalSearch &_ls): instance(_instance),ls(_ls){
	checador = _checador;
	ineficiency = 0;
	BestFitness = 0;
	idindividu.reserve(0.3*80);
}

SampleDecoder::~SampleDecoder() {

}
void SampleDecoder::Recodifica(std::vector<int> &Solution,std::vector<double> &chromosome) {

}
int SampleDecoder::diversity(std::vector< double >& chromosome){
	unsigned n = chromosome.size();
	vector <int> Solution(n);
	vector <int> RepairChromossomeFab,RepairChromossomeDep;
	int objval = 0;
	int capacidadeFab = 0,capacidadeDep = 0;
	for(unsigned i=0; i < n; ++i)
	{
		if (i < I){
			if(chromosome[i]>0.5){
				capacidadeFab += instance.bf[i][0];
				Solution[i] = 1;
			}
			else{
				Solution[i] = 0;
				RepairChromossomeFab.emplace_back(i);
			}
		}else{
			if(chromosome[i]>0.5){
				capacidadeDep += instance.pg[i][0] ;
				Solution[i] = 1;
				
			}
			else{
				Solution[i] = 0;
				RepairChromossomeDep.emplace_back( i - I );
			}


		}
			

	}
	int random = 0;
	if(capacidadeFab < instance.tot_demanda){
		while(capacidadeFab < instance.tot_demanda){
			random = rand()%RepairChromossomeFab.size();
			int index = RepairChromossomeFab[random];
			Solution[index] = 1;
			capacidadeFab += instance.bf[index][0];
			RepairChromossomeFab.erase(RepairChromossomeFab.begin()+random);
		}
	}
	if(capacidadeDep < instance.tot_demanda){
		while(capacidadeDep < instance.tot_demanda){
			random = rand()%RepairChromossomeDep.size();
			int index = RepairChromossomeDep[random];
			Solution[index] = 1;
			capacidadeDep += instance.pg[index][0];
			RepairChromossomeDep.erase(RepairChromossomeDep.begin()+random);
		}
	}
	for(unsigned i=0; i < n; ++i)
	{
			if(Solution[i]>0.5){
				chromosome[i] = 1;
			}
			else{
				chromosome[i] = 0;
			}

	}
	objval = checador->check(Solution);
	//cout << objval << endl;
	return objval;

}
bool sortByBrkgaScore(const pair<int,double> &a, const pair<int,double> &b){ 
    return (a.second > b.second); 
} 
int SampleDecoder::get_RandomIndex(){
	 int random = rand()%idindividu.size();
	 int idretorno = idindividu[random];

	 idindividu.erase(idindividu.begin()+random);

	 return idretorno;
}
int SampleDecoder::decode(std::vector<double> &chromosome,int bestcost,unsigned gen,unsigned MaxGen,unsigned i)
{
	
	unsigned n = chromosome.size();
	int objval;
	vector <int> Solution(I+J);
	if(gen == 0){
		for(unsigned i=0; i < n; ++i)
		{
			if(chromosome[i]>0.5){
				Solution[i] = 1;
			}
			else{
				Solution[i] = 0;
			}

		}
		objval = checador->check(Solution);
	} 
	else{
		if (BestFitness == 0){
			BestFitness = bestcost;
			currentGen = gen;
		}else if (currentGen !=gen){
			currentGen = gen;
			idindividu.clear();
			if(BestFitness <= bestcost){
				ineficiency++;
			}else{
				BestFitness = bestcost;
				ineficiency = 0;

			}
		}
		 for(unsigned i=0; i < n; ++i)
		{
			if(chromosome[i]>0.5){
				Solution[i] = 1;
			}
			else{
				Solution[i] = 0;
			}

		}
		
		objval = checador->check(Solution);
		if (objval >= 500000000){
			//cout << "invalid" << endl;
			idindividu.emplace_back(i);

		}
		
	} 
	

	return objval;
}
