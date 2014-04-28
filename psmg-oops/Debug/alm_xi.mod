# This file is (c) 2008 Xi Yang, Andreas Grothey, University of Edinburgh
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty.

#
# ALM model with Stochastic dominance constraints written in SML
#

param T;
set TIME ordered = 0..T;
set NODES;
param Parent{NODES} symbolic; # parent of nodes
param Probs{NODES}; # probability distribution of nodes

set ASSETS;
param Price{ASSETS}; # prices of assets
param Return{ASSETS, NODES}; # returns of assets at each node

set BENCHMARK;
param Vbenchmk{BENCHMARK}; # values of benchmarks
param Bench2nd{BENCHMARK}; # 2nd order SD values of benchmarks
param Gamma; # transaction costs

param InitialWealth;

var slack2{BENCHMARK, TIME} >= 0;

block alm stochastic using (nd in NODES, Parent, Probs, st in TIME): {

  var x_hold{ASSETS} >= 0;
  var risk{BENCHMARK} >= 0;
  var cash >= 0;

  stages {0}: {
    subject to StartBudget:
      (1+Gamma) * sum{j0 in ASSETS} (x_hold[j0] * Price[j0]) + cash = InitialWealth;
  }

  stages {1..T}: {

    var x_sold{ASSETS} >= 0;
    var x_bought{ASSETS} >= 0;
    var slack1{BENCHMARK} >= 0;

    subject to CashBalance:
      cash + (1+Gamma) * sum{j1 in ASSETS} (Price[j1] * x_bought[j1]) - ancestor(1).cash -(1-Gamma) * sum{j2 in ASSETS} (Price[j2] * x_sold[j2])=0;

    subject to Inventory{j3 in ASSETS}:
      x_hold[j3] - (1+Return[j3,nd]) * ancestor(1).x_hold[j3] - x_bought[j3] + x_sold[j3] = 0;

    subject to StochasticDominance1{l in BENCHMARK}:
      sum{j in ASSETS}(Price[j] * x_hold[j]) + cash + risk[l] - slack1[l] = Vbenchmk[l];

    subject to StochasticDominance2{l in BENCHMARK}:
      Exp(risk[l]) + slack2[l,st] = Bench2nd[l];
  }

  stages {T}: {

    var wealth >= 0;

    subject to FinalWealth:
      wealth - sum{j in ASSETS} (Price[j] * x_hold[j]) - cash = 0;

    maximize objFunc: wealth;
  }
}
