#pragma once

namespace EDMD
{
class Id
{
public:
    Id(int firstId, bool isIncreasing = true)
        : mIsIncreasing(isIncreasing)
        , currentId(firstId)
    {
    }

    int operator()()
    {
        if (mIsIncreasing)
            return currentId++;
        else
            return currentId--;
    }

    static bool Reverse()
    {
        return false;
    }

private:
    bool mIsIncreasing;
    int currentId;
};
} /* EDMD */
