
#include <iostream>
#include <cmath>
#include "ExactTSCFL.h"




using namespace std;



ExactTSCFL::ExactTSCFL(Instance &instance):instance(instance){}

ExactTSCFL::~ExactTSCFL(){ env.end();}

void ExactTSCFL::addConstraintFluxoDepositoCliente(IloEnv &env,IloModel &model,IloNumVarMatrix &s,Instance &instance){
    for(int k= 0; k<K;++k){
        IloExpr summation(env);
        for(int j=0;j <J;++j){ 
            summation +=s[j][k];
        }
        summation -=instance.q[k];
        IloRange constr_fluxo_deposito_cliente(env, 0, summation, IloInfinity);
        stringstream name;
        name << "FluxoDepositoCliente[" << k << "]: ";
        constr_fluxo_deposito_cliente.setName(name.str().c_str());
        model.add(constr_fluxo_deposito_cliente);
    }

}
void ExactTSCFL::addConstraintFluxoFabricaDeposito(IloEnv &env,IloModel &model,IloNumVarMatrix &x,IloNumVarMatrix &s){
    for(int j= 0; j<J;++j){
        IloExpr summationFab(env);
        IloExpr summationDep(env);
        IloExpr constraint(env);
        for(int i=0;i <I;++i){ 
            summationFab +=x[i][j];
        }
        for(int k=0;k <K;++k){ 
            summationDep +=s[j][k];
        }
        constraint = summationFab - summationDep;
        IloRange constr_fluxo_fabrica_deposito(env, 0, constraint, IloInfinity);
        stringstream name;
        name << "FluxoFabricaDeposito[" << j << "]: ";
        constr_fluxo_fabrica_deposito.setName(name.str().c_str());
        model.add(constr_fluxo_fabrica_deposito);
    }
}
void ExactTSCFL::addConstraintLimiteCapacidadeFabrica(IloEnv &env,IloModel &model,IloIntVarArray &y,IloNumVarMatrix &x,Instance &instance){
    for(int i= 0; i<I;++i){
        IloExpr summation(env);
        for(int j=0;j <J;++j){ 
            summation +=x[i][j];
        }
        summation -= instance.bf[i][0] * y[i];
        IloRange constr_limite_capacidade_fabrica(env, -IloInfinity, summation, 0);
        stringstream name;
        name << "LimiteCapacidadeFabrica[" << i << "]: ";
        constr_limite_capacidade_fabrica.setName(name.str().c_str());
        model.add(constr_limite_capacidade_fabrica);
    }
}
void ExactTSCFL::addConstraintLimiteCapacidadeDeposito(IloEnv &env,IloModel &model,IloIntVarArray &z,IloNumVarMatrix &s,Instance &instance){
     for(int j= 0; j<J;++j){
        IloExpr summation(env);
        for(int k=0;k <K;++k){ 
            summation +=s[j][k];
        }
        summation -=instance.pg[j][0] * z[j];
        IloRange constr_limite_capacidade_deposito(env, -IloInfinity, summation, 0);
        stringstream name;
        name << "LimiteCapacidadeDeposito[" << j << "]: ";
        constr_limite_capacidade_deposito.setName(name.str().c_str());
        model.add(constr_limite_capacidade_deposito);
    }
}


void ExactTSCFL::addObjective(IloEnv &env,IloModel &model,IloNumVarMatrix &x,IloNumVarMatrix &s,IloIntVarArray &z,IloIntVarArray &y){

    IloExpr summation(env);
    for(int i=0;i<I;++i){
        summation +=y[i]*instance.bf[i][1];
        for(int j=0;j<J;++j){
            summation +=x[i][j]*instance.c[i][j];
        }
    }
    for (int j=0;j<J;++j){
        summation+=z[j]*instance.pg[j][1];
        for(int k=0;k<K;++k){
            summation +=s[j][k]*instance.d[j][k];
        }
    }    

    model.add(IloMinimize(env,summation));




}

void ExactTSCFL::addConstraintLimiteFabricaAbertaSuperior(IloEnv &env,IloModel &model,IloIntVarArray &y,Instance &instance){
        IloExpr summation(env);
        for(int i= 0; i<I;++i){
            summation +=y[i];       
        }
        summation -= instance.betaFab;
        IloRange constr_limite_fabrica_aberta_superior(env, -IloInfinity, summation, 0);
        stringstream name;
        name << "LimiteFabricaAbertaSuperior: ";
        constr_limite_fabrica_aberta_superior.setName(name.str().c_str());
        model.add(constr_limite_fabrica_aberta_superior);
}
void ExactTSCFL::addConstraintLimiteDepositoAbertoSuperior(IloEnv &env,IloModel &model,IloIntVarArray &z,Instance &instance){
        IloExpr summation(env);
        for(int j= 0; j<J;++j){
            summation +=z[j];       
        }
        summation -= instance.betaDep;
        IloRange constr_limite_deposito_aberto_superior(env, -IloInfinity, summation, 0);
        stringstream name;
        name << "LimiteDepositoAbertoSuperior: ";
        constr_limite_deposito_aberto_superior.setName(name.str().c_str());
        model.add(constr_limite_deposito_aberto_superior);
}
void ExactTSCFL::addConstraintLimiteFabricaAbertaInferior(IloEnv &env,IloModel &model,IloIntVarArray &y,Instance &instance){
        IloExpr summation(env);
        for(int i= 0; i<I;++i){
            summation +=y[i];       
        }
        summation -= instance.piFab;
        IloRange constr_limite_fabrica_aberta_inferior(env, 0, summation, IloInfinity);
        stringstream name;
        name << "LimiteFabricaAbertaInferior: ";
        constr_limite_fabrica_aberta_inferior.setName(name.str().c_str());
        model.add(constr_limite_fabrica_aberta_inferior);
}
void ExactTSCFL::addConstraintLimiteDepositoAbertoInferior(IloEnv &env,IloModel &model,IloIntVarArray &z,Instance &instance){
        IloExpr summation(env);
        for(int j= 0; j<J;++j){
            summation +=z[j];       
        }
        summation -= instance.piDep;
        IloRange constr_limite_deposito_aberto_inferior(env, 0, summation, IloInfinity);
        stringstream name;
        name << "LimiteDepositoAbertoInferior: ";
        constr_limite_deposito_aberto_inferior.setName(name.str().c_str());
        model.add(constr_limite_deposito_aberto_inferior);
}

void ExactTSCFL::addConstraintLocalBranching(IloEnv &env,IloModel &model,IloIntVarArray &y,IloIntVarArray &z,std::vector<int> &solution){
    double deltaFab = 0.3 * I;
    double deltaDep = 0.3 * J;
    IloExpr summationFab(env);
    IloExpr summationDep(env);

    for(int i=0;i<I;++i){
        if (solution[i] == 1){
            summationFab += (1 - y[i]);
        }else{
            summationFab += y[i];
        }
    }
    
     for(int j=0;j<J;++j){
        if (solution[I+j] == 1){
            summationDep += (1 -z[j]);
        }else{
            summationDep += z[j];
        }
    }
    //LB FAB
    summationFab -= deltaFab; 
    IloRange constr_localbranch_fab(env, -IloInfinity, summationFab, 0);
    stringstream lbfab;
    lbfab << "constr_localbranch_fab";
    constr_localbranch_fab.setName(lbfab.str().c_str());
    model.add(constr_localbranch_fab);    
    //LB DEP
    summationDep -= deltaDep; 
    IloRange constr_localbranch_dep(env, -IloInfinity, summationDep, 0);
    stringstream lbdep;
    lbdep << "constr_localbranch_dep";
    constr_localbranch_dep.setName(lbdep.str().c_str());
    model.add(constr_localbranch_dep);  
}
void ExactTSCFL::setMIPstart(vector<int> &Solution, IloCplex &cplex) {  	
    int solvaly[I]; 	
    int solvalz[J];  	  	
    for (int i=0;i<I;++i){ 		 		
        solvaly[i] = Solution[i]; 	
    }  
    for(int j=0;j<J;++j) 	{ 		
        solvalz[j] = Solution[I+j]; 	
    }

    IloNumVarArray startVar(env); 	
    IloNumArray startVal(env);  	
    for (int i=0;i<I;++i) 	{ 		
        startVar.add(y[i]); 		
        startVal.add(solvaly[i]); 	
    }  	
    for (int j=0;j<J;++j){ 		
        startVar.add(z[j]); 		
        startVal.add(solvalz[j]); 	
    }  	
    cplex.addMIPStart(startVar, startVal);
} 
    

void ExactTSCFL::ConstroiModelo(){
    
    env = IloEnv();

    try
    {
    
    model = IloModel(env);
    
    IloNumVarMatrix x(env,I);
    IloNumVarMatrix s(env,J);
    y = IloIntVarArray(env,I,0,1);
    z = IloIntVarArray(env,J,0,1);
    
    for (int i = 0; i < I; ++i){
        x[i] = IloNumVarArray (env,J,0,IloInfinity,ILOFLOAT);
        stringstream vary;
        vary << "y[" << i << "]";
        y[i].setName(vary.str().c_str());
        model.add(y[i]);
        for(int j = 0; j < J; ++j){
            stringstream varx;
            varx << "x[" << i << "][" << j << "]";
            x[i][j].setName(varx.str().c_str());
            model.add(x[i][j]);
        }
    
    }
    
    for (int j = 0; j < J; ++j){
        s[j] = IloNumVarArray(env,K,0,IloInfinity,ILOFLOAT);
        stringstream varz;
        varz << "z[" << j << "]";
        z[j].setName(varz.str().c_str());
        model.add(z[j]);
        for (int k=0; k < K;++k){
            stringstream vars;
            vars << "s[" << j << "][" << k << "]";
            s[j][k].setName(vars.str().c_str());
            model.add(s[j][k]);  
            }
    }
    
    
    
    addConstraintFluxoDepositoCliente(env,model,s,instance);
    addConstraintFluxoFabricaDeposito(env,model,x,s);
    addConstraintLimiteCapacidadeFabrica(env,model,y,x,instance);
    addConstraintLimiteCapacidadeDeposito(env,model,z,s,instance);
    addObjective(env,model,x,s,z,y);

    //IloCplex cplex(model);
    //cplex.exportModel("FlowModel.lp");
    //cplex.setParam();
    /* cplex.setParam(IloCplex::Param::Preprocessing::Linear, 0);
	cplex.setParam(IloCplex::Param::Preprocessing::Aggregator, 0);
	cplex.setParam(IloCplex::Param::Preprocessing::Presolve, 0); */
    //cplex.setParam(IloCplex::Param::Preprocessing::Symmetry, 0);

	/* cplex.setParam(	IloCplex::Param::Advance, 0); //AdvInd = 1 ou 2

	cplex.setParam(IloCplex::Param::Threads, 1);

	//cplex.setParam(IloCplex::Param::Preprocessing::Pre, IloFalse);
    

	cplex.setParam(IloCplex::Param::MIP::Strategy::HeuristicFreq, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::Cliques, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::Covers, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::Disjunctive, -1);
    cplex.setParam(IloCplex::Param::MIP::Cuts::Implied, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::FlowCovers, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::PathCut, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::Gomory, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::GUBCovers, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::MIRCut, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::ZeroHalfCut, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::MCFCut, -1);
	cplex.setParam(IloCplex::Param::MIP::Cuts::LiftProj, -1); 
 */
    //cplex.solve();
 

   }
   catch (IloException& e) {
      cerr << "Concert exception caught: " << e << endl;
   }
   catch (...) {
      cerr << "Unknown exception caught" << endl;
   }
    
    
    //cout << " Max =" << cplex.getObjValue() << endl ; 


    


}
tuple <vector<int>,int> ExactTSCFL::solveLB(vector<int> &solution,int timebound,double upperbound){
    try{
        ofstream logFileAddress("outputcplex.log", ios::out | ios::app);
        addConstraintLocalBranching(env,model,y,z,solution);
        IloCplex cplex(model);
        //cplex.setOut(env.getNullStream());
        cplex.setOut(logFileAddress);
        cplex.setParam(IloCplex::Param::TimeLimit,timebound);
        cplex.setParam(IloCplex::Param::Emphasis::MIP,1);
        //cplex.setParam(IloCplex::Param::Simplex::Limits::UpperObj,upperbound+1);
        cplex.setParam(IloCplex::Param::Threads, 1);
        setMIPstart(solution,cplex);
        cplex.solve();
        vector <int> Solution(I+J);
        for(int i=0;i<I;++i){
            if(cplex.getValue(y[i]) > 0.5){
                Solution[i]=1;
            }else{
                Solution[i]=0;
            }
        }
        for(int j=0;j<J;++j){
            if(cplex.getValue(z[j]) > 0.5){
                Solution[I+j]=1;
            }else{
                Solution[I+j]=0;
            }
        }
        return make_pair(Solution,int(cplex.getObjValue()));
        logFileAddress.close();
    }catch (IloException& e) {
      cerr << "Concert exception caught: " << e << endl;
    }
    catch (...) {
      cerr << "Unknown exception caught" << endl;
    }
}