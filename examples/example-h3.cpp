// https://github.com/uber/h3/blob/master/examples/index.c
#include <h3/h3api.h>
#include <inttypes.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    // Get the H3 index of some location and print it.
    GeoCoord location;
    location.lat = degsToRads(40.689167);
    location.lon = degsToRads(-74.044444);
    int resolution = 10;
    H3Index indexed = geoToH3(&location, resolution);
    printf("The index is: %" PRIx64 "\n", indexed);

    // Get the vertices of the H3 index.
    GeoBoundary boundary;
    h3ToGeoBoundary(indexed, &boundary);
    // Indexes can have different number of vertices under some cases,
    // which is why boundary.numVerts is needed.
    for (int v = 0; v < boundary.numVerts; v++) {
        printf("Boundary vertex #%d: %lf, %lf\n", v,
               radsToDegs(boundary.verts[v].lat),
               radsToDegs(boundary.verts[v].lon));
    }

    // Get the center coordinates.
    GeoCoord center;
    h3ToGeo(indexed, &center);
    printf("Center coordinates: %lf, %lf\n", radsToDegs(center.lat),
           radsToDegs(center.lon));
}
