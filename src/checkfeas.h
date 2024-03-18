#ifndef CHECK_FEAS_H_
#define CHECK_FEAS_H_

#include "instance.h"
#include "graphcost.h"
#include <vector>
#include <queue>



class CheckFeas {
 private:
    Instance &p;
    GraphCost *g;
    
    
    int CalculaCustoAbertura(std::vector<int> &solution);
    bool CalculaCapacidade(std::vector<int> &solution);

 public:
    CheckFeas(Instance &_p,GraphCost *_g);

    ~CheckFeas();

    int check(std::vector<int> &solution);
};

#endif