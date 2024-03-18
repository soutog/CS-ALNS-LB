#ifndef INSTANCE_H
#define INSTANCE_H
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <exception>


#define INST_GRANDE


#ifdef INST_GRANDE
const int I=100,J=200,K=400;
#endif
#ifdef INST_PEQUENA
const int I=50,J=100,K=200;
#endif



struct edge
{
	int i, j, id;
};

using namespace std;

class Instance{	
public:
	
	int q[K];
	int	bf[I][2];			// capacidade e custo das fabricas
	int c[I][J];			// custo enviar produtos fabricas depositos
	int pg[J][2];			// capacidade e custo de depósito
	int d[J][K];			// custo enviar produto depósito cliente
	int tot_demanda;
	int betaFab;
	int piFab;
	int betaDep;
	int piDep;

	void load_file(const char *const file_name);
	void calculaMinMaxFabDep();
	
};
#endif