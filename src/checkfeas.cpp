#include "checkfeas.h"

CheckFeas::CheckFeas(Instance &_p,GraphCost *_g):p(_p)
{
	g = _g;
}


int CheckFeas::CalculaCustoAbertura(std::vector<int> &solution)
{
    int custo_abertura = 0;
	//Calculo do custo de abertura de fabricas
    for (int i=0; i<I; i++){
		if (solution[i] > 0){
			custo_abertura = custo_abertura + p.bf[i][1];
		}	
	}
	
	//Calculo do custo de abertura de depositos
	for (int j=0; j<J; j++){
		if (solution[I+j] > 0){
			custo_abertura = custo_abertura +  p.pg[j][1];
		}				
	}
	return custo_abertura;

}

bool CheckFeas::CalculaCapacidade(std::vector<int> &solution)
{
    int capacidade = 0;
	for (int i=0; i<I; i++){
		if (solution[i] > 0){
			capacidade +=  p.bf[i][0];
		}	
	}
	if (capacidade < p.tot_demanda) return false;
	capacidade = 0;
	for (int j=0; j<J; j++){
		if (solution[I+j] > 0){
			capacidade +=  p.pg[j][0];
		}				
	}
    if (capacidade < p.tot_demanda) return false;
	return true;

}

int CheckFeas::check(std::vector<int> & solution)
{
    int obj;
    if(CalculaCapacidade(solution)){
        obj = CalculaCustoAbertura(solution);
        //cout << obj << endl;
		g->AtualizaRede(solution);
        obj += g->CalculaFluxo();
        //cout << obj << endl;
        return obj;
    }
    obj = 500000000;
    return obj;
}

CheckFeas::~CheckFeas(){}