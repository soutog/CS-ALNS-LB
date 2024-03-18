#include "heuristic.h"


Heuristic::Heuristic(Instance &p_,GraphCost *_g):p(p_)
{
    S.resize(I+J);
	hcapfab.resize(I);
	hcapdep.resize(J);
	CostBenCalculate();
	hcapfabMed.resize(I);
	hcapdepMed.resize(J);
	CostBenCalculateMed();
	hcapfabCustoAbertura.resize(I);
	hcapdepCustoAbertura.resize(J);
    CostBenCalculateCustoAbertura();
	hcapfabCustoTransporte.resize(I);
	hcapdepCustoTransporte.resize(J);
    CostBenCalculateCustoTransporte();
	hcapfabCustoTransporteMed.resize(I);
	hcapdepCustoTransporteMed.resize(J);
    CostBenCalculateCustoTransporteMed();

    is_feasible = new CheckFeas(p,_g);
}
bool comparebysecond(pair<int,double> a,pair<int,double> b){
	return (a.second < b.second);
}
int Heuristic::aleatorio(int lower,int upper){
   
    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(lower,upper-1);
    
    cout <<distribution(generator) << endl;
    return distribution(generator);
}

void Heuristic::run(double alpha,int iter)
{
	//vector <int> adaptiveVector{0,1,2,3,4};
	vector <int> adaptiveVector{0};
	int costTemp = cost;
	double random;  
    for(int i=0;i<iter;++i){
		random_shuffle(adaptiveVector.begin(),adaptiveVector.end());
		vector <int> Stemp(S);
    	std::fill (S.begin(),S.end(),0);
    	
		construtivo_fab(alpha,adaptiveVector[0]);
    	construtivo_dep(alpha,adaptiveVector[0]);

		costTemp = is_feasible->check(S);
		if(cost == 0){
			cost = costTemp;
		} 
		else if(costTemp < cost){
			cost = costTemp;
			adaptiveVector.emplace_back(adaptiveVector[0]);
		}else{
			for(int i=0; i < I+J; ++i){
				S[i] = Stemp[i];
			}
		}
	}
    
}

void Heuristic::CostBenCalculateCustoAbertura(){
	
	for (int i=0;i<I;i++){
		hcapfabCustoAbertura[i] = make_pair(i,p.bf[i][1]/p.bf[i][0]);
	}
	
	sort(hcapfabCustoAbertura.begin(),hcapfabCustoAbertura.end(),comparebysecond);
	

	for (int j=0; j<J;j++){
			hcapdepCustoAbertura[j] = make_pair(j,p.pg[j][1] /p.pg[j][0]);  
	}

	sort(hcapdepCustoAbertura.begin(),hcapdepCustoAbertura.end(),comparebysecond);
}

void Heuristic::CostBenCalculateCustoTransporte(){

	for (int i=0;i<I;i++){
		int custo = 0;
		for (int j = 0; j < J; j++){
			custo = custo + p.c[i][j];
	    }
		hcapfabCustoTransporte[i] = make_pair(i,custo/p.bf[i][0]); 
	}
	
	sort(hcapfabCustoTransporte.begin(),hcapfabCustoTransporte.end(),comparebysecond);
	

	for (int j=0; j<J;j++){
		int custo = 0;
		for (int k = 0; k < K; k++){
			custo = custo + p.d[j][k];
	    }
		hcapdepCustoTransporte[j] = make_pair(j,custo /p.pg[j][0]); 
	}

	sort(hcapdepCustoTransporte.begin(),hcapdepCustoTransporte.end(),comparebysecond);
}

void Heuristic::CostBenCalculateCustoTransporteMed(){
	for (int i=0;i<I;i++){
		int custo = 0;
		for (int j = 0; j < J; j++){
			custo = custo + p.c[i][j];
	    }
		hcapfabCustoTransporteMed[i] = make_pair(i,(1.0/J*custo)/p.bf[i][0]); 
	}
	
	sort(hcapfabCustoTransporteMed.begin(),hcapfabCustoTransporteMed.end(),comparebysecond);
	
	for (int j=0; j<J;j++){
		int custo = 0;
		for (int k = 0; k < K; ++k){
			custo = custo + p.d[j][k];
	    }
		hcapdepCustoTransporteMed[j] = make_pair(j,(1.0/K*custo) /p.pg[j][0]); 
	}

	sort(hcapdepCustoTransporteMed.begin(),hcapdepCustoTransporteMed.end(),comparebysecond);
}

void Heuristic::CostBenCalculateMed(){
	for (int i=0;i<I;i++){
		int custo = 0;
		for (int j = 0; j < J; j++){
			custo = custo + p.c[i][j];
	    }
		hcapfabMed[i] = make_pair(i,(p.bf[i][1] + (custo*1.0/J) )/p.bf[i][0]);  
	}
	
	sort(hcapfabMed.begin(),hcapfabMed.end(),comparebysecond);
	

	for (int j=0; j<J;j++){
		int custo = 0;
			for (int k = 0; k < K; k++){
				custo = custo + p.d[j][k];
	    		}
			hcapdepMed[j] = make_pair(j,(p.pg[j][1] + (custo*1.0/K))/p.pg[j][0]);  
	}

	sort(hcapdepMed.begin(),hcapdepMed.end(),comparebysecond);
}

void Heuristic::CostBenCalculate(){
	
	for (int i=0;i<I;i++){
		int custo = 0;
		for (int j = 0; j < J; j++){
			custo = custo + p.c[i][j];
	    }
		hcapfab[i] = make_pair(i,(p.bf[i][1] + custo )/p.bf[i][0]);  
	}
	
	sort(hcapfab.begin(),hcapfab.end(),comparebysecond);
	
	
	for (int j=0; j<J;j++){
		int custo = 0;
			for (int k = 0; k < K; k++){
				custo = custo + p.d[j][k];
	    		}
			hcapdep[j] = make_pair(j,(p.pg[j][1] + custo)/p.pg[j][0]);  
	}

	sort(hcapdep.begin(),hcapdep.end(),comparebysecond);
}

void Heuristic::genRCL(vector<pair<int,double>>& candidateList,double hmin,double hmax,double alpha){
	vector<int> newRCL;
	double epsilon = 1.0E-6;
	for (int i=0;i<I;i++){
		if((candidateList[i].second - (hmax - alpha*(hmax - hmin))) < epsilon){
			newRCL.emplace_back(candidateList[i].first);				
		}else{
			break;
		}
	}
	RCL=newRCL;
}

void Heuristic::genRCLMax(vector<pair<int,double>>& candidateList,double hmin,double hmax,double alpha){
	vector<int> newRCL;
	double epsilon = 1.0E-6;
	for (int i=0;i<I;i++){
		if((candidateList[i].second - (hmin + alpha*(hmax - hmin))) > epsilon){
			newRCL.emplace_back(candidateList[i].first);				
		}else{
			break;
		}
	}
	RCL=newRCL;
}

void Heuristic::construtivo_fab(double alpha,int value_constructive){

	int capacidade_total_fab = 0;  
	int random;
	double hmin, hmax;
    vector<pair<int,double>> candidateListFab;
    
    
	switch (value_constructive)
	{
	case 0:
		candidateListFab.insert(candidateListFab.end(), hcapfab.begin(), hcapfab.end());
		break;
	case 1:
		candidateListFab.insert(candidateListFab.end(), hcapfabMed.begin(), hcapfabMed.end());
		break;
	case 2:
		candidateListFab.insert(candidateListFab.end(), hcapfabCustoAbertura.begin(), hcapfabCustoAbertura.end());
		break;
	case 3:
		candidateListFab.insert(candidateListFab.end(), hcapfabCustoTransporte.begin(), hcapfabCustoTransporte.end());
		break;
	case 4:
		candidateListFab.insert(candidateListFab.end(), hcapfabCustoTransporteMed.begin(), hcapfabCustoTransporteMed.end());
		break;
	default:
		exit(0);
		break;
	}

	// abre fabrica com base no melhor custo beneficio
	while (capacidade_total_fab <= p.tot_demanda){ // enquanto a capacidade das fabricas for menor do que a demanda dos clientes
		//Emite lista de candidatos restrita
		hmin = candidateListFab[0].second;
		hmax = candidateListFab[candidateListFab.size()-1].second;
		genRCL(candidateListFab,hmin,hmax,alpha);
		
		//random = aleatorio(0,l); // escolhe uma valor aleatorio na lista de candidatos restrita
        random = rand()%RCL.size(); 
		S[RCL[random]] = 1; 
		capacidade_total_fab += p.bf[RCL[random]][0]; 
		candidateListFab.erase(candidateListFab.begin()+random);
	}
    
}

void Heuristic::construtivo_dep(double alpha,int value_constructive){

	int capacidade_total_dep = 0;
	int random;
	double hmin, hmax;
	//Abre os depositos com base no melhor custo beneficio
	vector<pair<int,double>> candidateListDep;
    
    switch (value_constructive)
	{
	case 0:
		candidateListDep.insert(candidateListDep.end(), hcapdep.begin(), hcapdep.end());
		break;
	case 1:
		candidateListDep.insert(candidateListDep.end(), hcapdepMed.begin(), hcapdepMed.end());
		break;
	case 2:
		candidateListDep.insert(candidateListDep.end(), hcapdepCustoAbertura.begin(), hcapdepCustoAbertura.end());
		break;
	case 3:
		candidateListDep.insert(candidateListDep.end(), hcapdepCustoTransporte.begin(), hcapdepCustoTransporte.end());
		break;
	case 4:
		candidateListDep.insert(candidateListDep.end(), hcapdepCustoTransporteMed.begin(), hcapdepCustoTransporteMed.end());
		break;
	default:
		exit(0);
		break;
	}
	

	// abre fabrica com base no melhor custo beneficio
	while (capacidade_total_dep <= p.tot_demanda){ // enquanto a capacidade das fabricas for menor do que a demanda dos clientes
		//Emite lista de candidatos restrita
		hmin = candidateListDep[0].second;
		hmax = candidateListDep[candidateListDep.size()-1].second;
		genRCL(candidateListDep,hmin,hmax,alpha);
		
		//random = aleatorio(0,l); // escolhe uma valor aleatorio na lista de candidatos restrita
        random = rand()%RCL.size(); 
		S[RCL[random]+I] = 1; 
		capacidade_total_dep += p.pg[RCL[random]][0]; 
		candidateListDep.erase(candidateListDep.begin()+random);
	}
        
}
