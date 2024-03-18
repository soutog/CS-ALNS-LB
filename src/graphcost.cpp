#include "graphcost.h"
#include <iostream>
#define MAX(x,y) (((x) > (y)) ? (x) : (y))

using namespace std;
GraphCost::GraphCost (Instance &_p) : p(_p)
{
    int sts;
    env__ = CPXopenCPLEX(&sts);
    inicializaRede();
    CriaRede();
    
}

void GraphCost::AtualizaRede(vector<int> &solution)
{
  for (int i=0; i< I;++i){
    if(solution[i] == 1){
      red__.totPla_ += p.bf[i][0];
    }
    red__.vetUbAtu_[0] = solution[i] * p.bf[i][0];
    red__.vetIndAtu_[0] = i; 
    // ---------------------------
    // atualizar o arco da rede no CPLEX
    CPXNETchgbds(env__,net__,1,red__.vetIndAtu_,red__.vetTipAtu_,red__.vetUbAtu_);
    // ---------------------------
    // atualizar o total da inviabilidade de fluxo da rede
    red__.totInv_ = MAX(0,p.tot_demanda - red__.totPla_);
  }
  for (int i=0; i< J;++i){
    if(solution[i] == 1){
      red__.totPla_ += p.pg[i][0];
    }
    red__.vetUbAtu_[0] = solution[I+i] * p.pg[i][0];
    red__.vetIndAtu_[0] = I + (I * J) + i; 
    // ---------------------------
    // atualizar o arco da rede no CPLEX
    CPXNETchgbds(env__,net__,1,red__.vetIndAtu_,red__.vetTipAtu_,red__.vetUbAtu_);
    // ---------------------------
    // atualizar o total da inviabilidade de fluxo da rede
    red__.totInv_ = MAX(0,p.tot_demanda - red__.totPla_);
  }
 
} 



void  GraphCost::inicializaRede()
{
    // redes
 net__ = NULL;
 red__.valSol_ = 0.0;
 red__.totInv_ = 0;
	red__.totPla_ = 0;
	red__.totSat_ = 0;
 for(long int j = 0; j < MAX_NOS; j++)
 	 red__.vetSup_[j] = 0.0;
 for(long int j = 0; j < MAX_ARC; j++)
  {
 	 red__.vetIniArc_[j] = 0;
   red__.vetFinArc_[j] = 0;
   red__.vetCstArc_[j] = 0.0;
   red__.vetLbArc_[j] = 0.0;
   red__.vetUbArc_[j] = 0.0;
   red__.vetSol_[j] = 0.0;
  }
 red__.vetUbAtu_[0] = 0.0;
 red__.vetIndAtu_[0] = 0;
 red__.vetTipAtu_[0] = 'U';
 redAux__.valSol_ = 0.0;
 redAux__.totInv_ = 0;
	redAux__.totPla_ = 0;
	redAux__.totSat_ = 0;
 for(long int j = 0; j < MAX_NOS; j++)
 	 redAux__.vetSup_[j] = 0.0;
 for(long int j = 0; j < MAX_ARC; j++)
  {
 	 redAux__.vetIniArc_[j] = 0;
   redAux__.vetFinArc_[j] = 0;
   redAux__.vetCstArc_[j] = 0.0;
   redAux__.vetLbArc_[j] = 0.0;
   redAux__.vetUbArc_[j] = 0.0;
   redAux__.vetSol_[j] = 0.0;
  }
 redAux__.vetUbAtu_[0] = 0.0;
 redAux__.vetIndAtu_[0] = 0;
 redAux__.vetTipAtu_[0] = 'U';
}

void  GraphCost::CriaRede()
{
int sts;
 // ---------------------------
 // calcular o n�mero de n�s e arcos
 numNos__ = I + (2 * J) + K + 1;
 numArc__ = I + (I * J) + J + (J * K);
 // --------------------------- 
 red__.valSol_  = 0;
 red__.totInv_  = 0;
 red__.totPla_  = 0;
 red__.totSat_  = 0;
 // ---------------------------
 // calcular a inviabilidade de fluxo na rede
 /* for(int i = 0; i < I; i++)
   red__.totPla_ += solution[i][i] * p.bf[i][0];
 for(int i = 0; i < J; i++)
   red__.totSat_ += solution[i][I+i] * p.pg[i][0];
 red__.totInv_ = MAX(0,MAX(p.tot_demanda - red__.totPla_,p.tot_demanda - red__.totSat_)); */
 // ---------------------------
 // preencher os n�s com suas demandas (ou capacidades)
 // plantas
 for(int i = 0; i < I; i++)
   red__.vetSup_[i] = 0;
 // ---------------------------
 // sat�lites
 for(int i = 0; i < J; i++)
   red__.vetSup_[I+i] = 0;
 // ---------------------------
 // sat�lites artificiais
 for(int i = 0; i < J; i++)
   red__.vetSup_[I+J+i] = 0;
 // ---------------------------
 // clientes
 for(int i = 0; i < K; i++)
   red__.vetSup_[I+J+J+i] = -p.q[i];
 // ---------------------------
 // planta artificial
 red__.vetSup_[I+J+J+K] = p.tot_demanda;
 // ---------------------------
 // preencher os arcos
 int a = I;
 // planta artificial --> plantas
 for(int i = 0; i < I; i++)
  {
   red__.vetIniArc_[i] = I + J + J + K;
   red__.vetFinArc_[i] = i;
   red__.vetCstArc_[i] = 0;
   red__.vetLbArc_[i] = 0;
   red__.vetUbArc_[i] = 0;
  }
 // plantas -> sat�lites artificiais
 for(int i = 0; i < I; i++)
   for(int j = 0; j < J; j++)
    {
     red__.vetIniArc_[a] = i;
     red__.vetFinArc_[a] = I + j;
     red__.vetCstArc_[a] = p.c[i][j];
     red__.vetLbArc_[a] = 0;
     red__.vetUbArc_[a] = p.pg[j][0];
     a++;
    }
 // sat�lites artificiais --> sat�lites
 for(int i = 0; i < J; i++)
  {
   red__.vetIniArc_[a] = I + i;
   red__.vetFinArc_[a] = I + J + i;
   red__.vetCstArc_[a] = 0;
   red__.vetLbArc_[a] = 0;
   red__.vetUbArc_[a] = 0;
   a++;
  }
 // sat�lites -> clientes
 for(int i = 0; i < J; i++)
   for(int j = 0; j < K; j++)
    {
     red__.vetIniArc_[a] = I + J + i;
     red__.vetFinArc_[a] = I + J + J + j;
     red__.vetCstArc_[a] = p.d[i][j];
     red__.vetLbArc_[a] = 0;
     red__.vetUbArc_[a] = p.q[j];
     a++;
    }
 // ---------------------------
 // montar o problema no CPLEX
 net__ = CPXNETcreateprob(env__,&sts,"");
 if(sts != 0)
  {
   printf("\n\n>>> Erro %d ao criar a rede no CPLEX!",sts);
   exit(1);
   //_getch();
  }
 sts = CPXNETchgobjsen(env__,net__,CPX_MIN);
 if(sts != 0)
  {
   printf("\n\n>>> Erro %d ao definir o objetivo da rede no CPLEX!",sts);
   exit(1);
   //_getch();
  }
 sts = CPXNETaddnodes(env__,net__,numNos__,red__.vetSup_,NULL);
 if(sts != 0)
  {
   printf("\n\n>>> Erro %d ao criar os nos da rede no CPLEX!",sts);
   exit(1);
   //_getch();
  }
 sts = CPXNETaddarcs(env__,net__,numArc__,red__.vetIniArc_,red__.vetFinArc_,red__.vetLbArc_,red__.vetUbArc_,red__.vetCstArc_,NULL);
 if(sts != 0)
  {
   printf("\n\n>>> Erro %d ao criar os arcos da rede no CPLEX!",sts);
   exit(1);
   //_getch();
  }
}
int GraphCost::CalculaFluxo()
{
   int sts;
   sts = CPXNETprimopt(env__,net__);
   if(sts != 0)
   {
    printf("\n\n>>> Erro %d ao resolver a rede no CPLEX!",sts);
    exit(1);
    //_getch();
   }
   sts = CPXNETsolution(env__,net__,&sts,&red__.valSol_,NULL,NULL,NULL,NULL);
   if(sts != 0)
   {
     printf("\n\n>>> Erro %d ao obter a solucao da rede no CPLEX!",sts);
     exit(1);
     //_getch();
   }
   //cout << red__.valSol_ << endl;
   return int(red__.valSol_);
    
}

GraphCost::~GraphCost(){
}