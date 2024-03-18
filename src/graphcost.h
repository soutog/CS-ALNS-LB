#ifndef GRAPH_COST_H_
#define GRAPH_COST_H_
#include "instance.h"
#include <cplex.h>
#include <vector>

#define MAX_PLA 150
#define MAX_SAT 250
#define MAX_CLI 450

#define MAX_SOL 3
#define MAX_CLT 10

#define MAX_NOS (MAX_PLA + 2 * MAX_SAT + MAX_CLI + 1)
#define MAX_ARC (MAX_PLA + MAX_PLA * MAX_SAT + MAX_SAT + MAX_SAT * MAX_CLI)


static long int numNos__;                    // n�mero de n�s das redes
static long int numArc__;                    // n�mero de arcos das redes

typedef struct tCplex
{
 long int numVar_; // n�mero de vari�veis do modelo no CPLEX
 long int numRes_; // n�mero de restri��es do modelo no CPLEX
 double valSol_;   // valor da solu��o obtida pelo CPLEX 
 double vMelNo_;   // valor do melhor n� (limitante inferior) obtido pelo CPLEX
 double gap_;      // gap obtido pelo CPLEX
}Cplex;

typedef struct tRede
{
        double valSol_;             // valor da solu��o da rede (fluxo)
        long int totInv_;           // valor com a "inviabilidade" da solu��o (demanda maior que oferta)
        long int totPla_;           // valor de oferta das plantas abertas
        long int totSat_;           // valor de oferta dos sat�lites abertos
        double vetSup_[MAX_NOS];    // vetor com a oferta (+) ou demanda (-) de cada n�
        int vetIniArc_[MAX_ARC];    // vetor com o n� inicial de cada arco
        int vetFinArc_[MAX_ARC];    // vetor com o n� final de cada arco
        double vetCstArc_[MAX_ARC]; // vetor com o custo de cada arco
        double vetLbArc_[MAX_ARC];  // vetor com o limitante inferior de fluxo em cada arco
        double vetUbArc_[MAX_ARC];  // vetor com o limitante superior de fluxo em cada arco
        double vetSol_[MAX_ARC];    // vetor com a solu��o da rede (fluxo nos arcos)
        double vetUbAtu_[1];        // limitante superior do fluxo do arco a ser atualizado (usado na atualiza��o da rede)
        char vetTipAtu_[1];         // tipo de limitante do fluxo do arco a ser atualizado - sempre limitante superior 'U' (usado na atualiza��o da rede)
        int vetIndAtu_[1];          // �ndice do arco a ser atualizado (usado na atualiza��o da rede)
}Rede;
        static CPXENVptr env__;                             // ambiente do CPLEX
        static CPXNETptr net__;                             // rede no CPLEX                          // informa��es do CPLEX   
   
class GraphCost{
    private:
        Instance &p;
        void CriaRede();
        void  inicializaRede();
   
         Rede red__;                           // informa��es da rede
         Rede redAux__;
         Cplex cpx__;
         
         
       
    public:
        GraphCost(Instance &_p);
        void AtualizaRede(std::vector<int> &solution);
        int CalculaFluxo();
        ~GraphCost();
};
#endif