#include <benchmark/benchmark.h>

#include "Box.h"
#include "Simulation.h"

#include <iostream>

using namespace EDMD;
constexpr double EPS = 1.e-4;

static void SpheresInBox(benchmark::State& state)
{
    auto walls = Box(-Eigen::Vector3d::Ones(), Eigen::Vector3d::Ones());
    std::vector<Sphere> spheres;
    for (int i = 0; i < state.range(0); ++i)
    {
        Sphere s(Eigen::Vector3d::Random(), Eigen::Vector3d::Random(), 0, 1, i);
        spheres.push_back(s);
    }

    Simulation simulation(spheres, walls);
    for (auto _ : state)
        simulation.DoStep();

    state.SetComplexityN(state.range(0));
}

BENCHMARK(SpheresInBox)->RangeMultiplier(2)->Range(4, 1024)->Complexity();

BENCHMARK_MAIN();
