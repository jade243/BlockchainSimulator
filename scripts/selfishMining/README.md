# Selfish Mining

## Introduction

This part simulates the selfish mining strategy, described in the report at the root of this repository.

The idea is to estimate the rewards that a pool can obtain by adopting a specific strategy.

![FiniteState](../../../master/figures/finiteState.png)

Here, the state *i* means the pool has mined *i* blocks ahead of the honest miners.

&alpha; : the hash rate of the pool

&gamma; : the propagation rate for selfish miners' blocks

## Results

Then, we can simulate this strategy with different size of pools and different values of &gamma;.

![SelfishExample1](../../../master/figures/selfishGraph.png)

![SelfishExample2](../../../master/figures/largestPools0.png)

![SelfishExample3](../../../master/figures/largestPools1.png)
