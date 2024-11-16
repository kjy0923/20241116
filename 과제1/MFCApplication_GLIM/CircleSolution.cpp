
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class CircleSolution {
private:
    double c_radius;
    double xcenter;
    double ycenter;

public:
    CircleSolution(double radius, double x_center, double y_center) {
        c_radius = radius;
        xcenter = x_center;
        ycenter = y_center;
    }

    vector<double> randPoint() {
        double randomradius = sqrt(((double)rand() / RAND_MAX)) * c_radius;
        double randomangle = ((double)rand() / RAND_MAX) * 360.0;
        double yp = sin(randomangle) * randomradius;
        double xp = cos(randomangle) * randomradius;
        return { xp + xcenter, yp + ycenter };
    }
};