#include <bbt/random/Random.hpp>
#include "util/log/Log.hpp"

namespace game::util
{

class Random
{
public:

static void SetSeed(time_t seed);

static time_t GetSeed();

static int64_t Rand();

private:
    static time_t m_random_seed;
    static bbt::random::mt_random<int64_t, INT64_MIN, INT64_MAX> m_rand;
};

}// namespace end