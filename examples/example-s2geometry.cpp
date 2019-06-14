// https://github.com/google/s2geometry/blob/master/doc/examples/point_index.cc
#include "s2/base/commandlineflags.h"
#include "s2/s1chord_angle.h"
#include "s2/s2closest_point_query.h"
#include "s2/s2earth.h"
#include "s2/s2point_index.h"
#include <cinttypes>
#include <cstdint>
#include <cstdio>

// #include "s2/s2testing.h"
struct S2Testing
{
    static double rand01() { return rand() / (double)RAND_MAX; }
    static S2Point RandomPoint()
    {
        double x = (rand01() - 0.5) * 2;
        double y = (rand01() - 0.5) * 2;
        double z = (rand01() - 0.5) * 2;
        return S2Point(x, y, z).Normalize();
    }
};

#define num_index_points 10000 // "Number of points to index"
#define num_queries 10000      // "Number of queries"
#define query_radius_km 100.0  // "Query radius in kilometer

int main(int argc, char **argv)
{
    // Build an index containing random points anywhere on the Earth.
    S2PointIndex<int> index;
    for (int i = 0; i < num_index_points; ++i) {
        index.Add(S2Testing::RandomPoint(), i);
    }

    // Create a query to search within the given radius of a target point.
    S2ClosestPointQuery<int> query(&index);
    query.mutable_options()->set_max_distance(
        S1Angle::Radians(S2Earth::KmToRadians(query_radius_km)));

    // Repeatedly choose a random target point, and count how many index points
    // are within the given radius of that point.
    int64_t num_found = 0;
    for (int i = 0; i < num_queries; ++i) {
        S2ClosestPointQuery<int>::PointTarget target(S2Testing::RandomPoint());
        num_found += query.FindClosestPoints(&target).size();
    }

    std::printf("Found %" PRId64 " points in %d queries\n", num_found,
                num_queries);
    return 0;
}
