#pragma once
#include "instance.h"
#include "ilcplex/ilocplex.h"
#include <vector>


typedef IloArray<IloNumVarArray> IloNumVarMatrix;

class ExactTSCFL {
    public:
        Instance instance;
        ExactTSCFL(Instance &instance);
        ~ExactTSCFL();
        void ConstroiModelo();
        std::tuple <std::vector<int>,int> solveLB(std::vector<int> &solution,int timebound,double upperbound);
        void setMIPstart(vector<int> &Solution, IloCplex &cplex);
    private:
        IloEnv env;
        IloModel model;
        IloIntVarArray y;
        IloIntVarArray z;
        void addConstraintFluxoDepositoCliente(IloEnv &env,IloModel &model,IloNumVarMatrix &s,Instance &instance);
        void addObjective(IloEnv &env,IloModel &model,IloNumVarMatrix &x,IloNumVarMatrix &s,IloIntVarArray &z,IloIntVarArray &y);
        void addConstraintLimiteFabricaAbertaSuperior(IloEnv &env,IloModel &model,IloIntVarArray &y,Instance &instance);
        void addConstraintLimiteDepositoAbertoSuperior(IloEnv &env,IloModel &model,IloIntVarArray &z,Instance &instance);
        void addConstraintLimiteFabricaAbertaInferior(IloEnv &env,IloModel &model,IloIntVarArray &y,Instance &instance);
        void addConstraintLimiteDepositoAbertoInferior(IloEnv &env,IloModel &model,IloIntVarArray &z,Instance &instance);
        void addConstraintFluxoFabricaDeposito(IloEnv &env,IloModel &model,IloNumVarMatrix &x,IloNumVarMatrix &s);
        void addConstraintLimiteCapacidadeFabrica(IloEnv &env,IloModel &model,IloIntVarArray &y,IloNumVarMatrix &x,Instance &instance);
        void addConstraintLimiteCapacidadeDeposito(IloEnv &env,IloModel &model,IloIntVarArray &z,IloNumVarMatrix &s,Instance &instance);
        void addConstraintLocalBranching(IloEnv &env,IloModel &model,IloIntVarArray &y,IloIntVarArray &z,std::vector<int> &solution);
        
};