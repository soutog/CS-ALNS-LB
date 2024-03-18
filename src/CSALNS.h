#ifndef CSALNS_H
#define CSALNS_H
#include "instance.h"
#include "graphcost.h"
#include "heuristic.h"
#include "ExactTSCFL.h"

#include <vector>



class CSALNS{
 private:
    GraphCost *g;
    Instance &p;
    CheckFeas *is_feasible;
    Heuristic &constructive;

    std::vector<vector<int>> cluster_solution;
    std::vector<int> cluster_ineficiency;
    std::vector<int> cluster_volume;
    std::vector<int> cluster_cost;
    std::vector<std::pair<int,double>> costbenfab;
    std::vector<std::pair<int,double>> costbendep;
    double besttime;
    int costBest;
    std::vector<int> SolutionBest;
    int costBestTarget;
    std::vector<int> SolutionBestTarget;
    int gama;
    int ineficiency_max;
    void CreateZipVectorCost(vector<double> costben);
    void DestroyRandomSolution(vector<int>&Solution,double destroy_rate);
    void DestroySemiRandomSolution(vector<int>&Solution,double destroy_rate,double alpha,Heuristic constructive, int value_construtive);
    void RepairSemiRandom(vector<int> &Solution,double alpha,Heuristic &constructive, int value_construtive);
    void RepairRandom(vector<int> &Solution);
    int HammingDistance(std::vector<int>&Solution1,std::vector<int>&Solution2);
    void VerifyCapacity(vector<int> &Solution);

 public:
    CSALNS(Instance &_p,GraphCost *_g,Heuristic &_constructive);
    void solve(double alpha,int maxiter,int _gama,int _ineficiency_max,int _maxvolume,double _destroy_alpha,double _repair_alpha,double _destroy_rate,clock_t hI);
    std::vector<int> get_SolutionBest(){return SolutionBest;}
    double get_besttime(){return besttime;}
    int get_costBest(){return costBest;}
};
#endif