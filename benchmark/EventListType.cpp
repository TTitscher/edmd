#include <algorithm>
#include <benchmark/benchmark.h>
#include <deque>
#include <random>
#include <set>

static void DeleteFirstAndInsertRandomSet(benchmark::State& state)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(0., 42.);

    std::set<double> randomSet;
    size_t num = state.range(0);
    for (size_t i = 0; i < num; ++i)
        randomSet.insert(distribution(generator));

    for (auto _ : state)
    {
        randomSet.erase(randomSet.begin());
        randomSet.insert(distribution(generator));
    }
    state.SetComplexityN(state.range(0));
}


template <typename T>
class SortedVector
{
public:
    void insert(T element)
    {
        auto it = std::lower_bound(mData.begin(), mData.end(), element);
        mData.insert(it, element);
    }
    std::deque<T> mData;
};

static void DeleteFirstAndInsertRandomVector(benchmark::State& state)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(0., 42.);

    SortedVector<double> randomSet;
    size_t num = state.range(0);
    for (size_t i = 0; i < num; ++i)
        randomSet.mData.push_back(distribution(generator));
    std::sort(randomSet.mData.begin(), randomSet.mData.end());

    for (auto _ : state)
    {
        randomSet.mData.pop_front();
        randomSet.insert(distribution(generator));
    }
    state.SetComplexityN(state.range(0));
}


BENCHMARK(DeleteFirstAndInsertRandomSet)->Range(8, 8 << 16)->Complexity();
BENCHMARK(DeleteFirstAndInsertRandomVector)->Range(8, 8 << 16)->Complexity();

BENCHMARK_MAIN();
