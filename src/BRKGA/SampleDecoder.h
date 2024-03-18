#ifndef SAMPLEDECODER_H
#define SAMPLEDECODER_H

#include <list>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <utility>
#include <cmath>

#include "../instance.h"
#include "../checkfeas.h"
#include "../localsearch.h"

using namespace std;

class SampleDecoder {

private:
	CheckFeas* checador;
	Instance &instance;
	LocalSearch &ls;
	vector <int> idindividu;
	unsigned ineficiency;
	unsigned BestFitness;
	unsigned currentGen;

	void Recodifica(std::vector<int> &Solution,std::vector<double> &chromosome);
public:
	SampleDecoder(Instance &_instance, CheckFeas *_checador,LocalSearch &_ls);
	~SampleDecoder();
	int get_Ineficiency(){return ineficiency;}
	int get_idindividuSize(){return idindividu.size();}
	int get_RandomIndex();
	int decode(std::vector< double >& chromosome,int bestcost,unsigned generation,unsigned MaxGen=0,unsigned i =0) ;
	int diversity(std::vector< double >& chromosome);
};

#endif
