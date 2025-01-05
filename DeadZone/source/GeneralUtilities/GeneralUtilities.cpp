#include "GeneralUtilities.h"

GeneralUtilities::GeneralUtilities() {

}

GeneralUtilities::~GeneralUtilities() {

}

GeneralUtilities& GeneralUtilities::get()
{
    static GeneralUtilities instance;
    return instance;
}

// Auxiliary functions
// Linear interpolation function
inline double lerp(double a, double b, double t) {
    return a + t * (b - a);
}

// Fade function as defined by Ken Perlin
inline double fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

// Gradient function
inline double grad(int hash, double x, double y) {
    int h = hash & 2;
    double u = (h < 2) ? x : y;
    double v = (h < 2) ? y : x;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

// 2D Perlin noise
double perlin2D(double x, double y, const std::vector<int>& p, const int& hash_size) {
    // Find unit grid cell containing point
    int X = (int)std::floor(x) & (hash_size - 1);
    int Y = (int)std::floor(y) & (hash_size - 1);

    // Get relative x,y coordinates of point within cell
    double xf = x - std::floor(x);
    double yf = y - std::floor(y);

    // Hash coordinates of the 4 corners
    int aa = p[X +     p[Y    ]];
    int ab = p[X +     p[Y + 1]];
    int ba = p[X + 1 + p[Y]];
    int bb = p[X + 1 + p[Y + 1]];

    double u = fade(xf);
    double v = fade(yf);

    double x1 = lerp(grad(aa, xf, yf    ), grad(ba, xf - 1, yf    ), u);
    double x2 = lerp(grad(ab, xf, yf - 1), grad(bb, xf - 1, yf - 1), u);
        
    double val = lerp(x1, x2, v);
    return val;
}   

/*
 * A simple implementation of 2D Perlin Noise
 * ----------------------------------------
 * Reference:
 *  Perlin, Ken. "Improving noise." Proceedings of the 29th annual conference on
 *  Computer graphics and interactive techniques. 2002.
 * 
 * This uses a classic approach with a permuation array.
 */
void GeneralUtilities::generatePerlinMap(const int& width, const int& height, const double& scale, const int& hash_size, std::vector<std::vector<std::string>>& map) {
    // Generate random permuation
    std::vector<int> permutation(hash_size);
    for (int i = 0; i < hash_size; i++)
        permutation[i] = i + 1;
    std::shuffle(permutation.begin(), permutation.end(), Random::randomGen());
    
    std::vector<int> p(hash_size * 2);
    for (int i = 0; i < hash_size; i++)
        p[hash_size + i] = p[i] = permutation[i];
    
    double offsetX = Random::random01() * 1000;
    double offsetY = Random::random01() * 1000;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            double x = (j + offsetX) / scale;
            double y = (i + offsetY) / scale;

            double val = perlin2D(x, y, p, hash_size);

            if (val < -0.4)
                map[i][j] = "M2";
            else if (val < 0.1)
                map[i][j] = ".0";
            else if (val < 0.2)
                map[i][j] = "M0";
            else map[i][j] = ".0";
        }
}

long long GeneralUtilities::getTimeSinceEpochInMs() {
    auto now = std::chrono::system_clock::now();
    auto ms_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ).count();
    return ms_since_epoch;
}