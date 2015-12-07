/******************************************************************************
*******************************************************************************
*****Performance comparison of Rodrigues' vs Euler rotation on a unit cube*****
******************Completed as part of a linear algebra class******************
*********************project at the University of Michigan*********************
*********************************Max Morrison**********************************
*******************************28 November 2015********************************
*******************************************************************************
******************************************************************************/
#include <chrono>

#include "../include/rotatingobject.h"

#define NUMROTATIONS 10000000

using namespace std;
using namespace chrono;

void cubeTest();
void pointTest();
void tenMillionEulerCubeRotations();
void tenMillionRodriguesCubeRotations();
void tenMillionEulerPointRotations();
void tenMillionRodriguesPointRotations();

int
main() {
    // cubeTest();
    // pointTest();

    /* Euler Rotation Timed Test: Ten Million Cube Rotations */
    steady_clock::time_point cubeEulerStart = steady_clock::now();
    tenMillionEulerCubeRotations();
    steady_clock::time_point cubeEulerEnd = steady_clock::now();
    steady_clock::duration cubeEulerTime = cubeEulerEnd - cubeEulerStart;   
    cout << "Ten million Euler cube rotations in "
         << duration_cast<microseconds>(cubeEulerTime).count()
         << " microseconds\n";

    /* Rodrigues Rotation Timed Test: Ten Million Cube Rotations */
    steady_clock::time_point cubeRodriguesStart = steady_clock::now();
    tenMillionRodriguesCubeRotations();
    steady_clock::time_point cubeRodriguesEnd = steady_clock::now();
    steady_clock::duration cubeRodriguesTime = cubeRodriguesEnd - cubeRodriguesStart;
    cout << "Ten million Rodrigues cube rotations in "
         << duration_cast<microseconds>(cubeRodriguesTime).count()
         << " microseconds\n";

    /* Euler Rotation Timed Test: Ten Million Point Rotations */
    steady_clock::time_point pointEulerStart = steady_clock::now();
    tenMillionEulerPointRotations();
    steady_clock::time_point pointEulerEnd = steady_clock::now();
    steady_clock::duration pointEulerTime = pointEulerEnd - pointEulerStart;
    cout << "Ten million Euler point rotations in "
         << duration_cast<microseconds>(pointEulerTime).count()
         << " microseconds\n";

    /* Rodrigues Rotation Timed Test: Ten Million Point Rotations */
    steady_clock::time_point pointRodriguesStart = steady_clock::now();
    tenMillionRodriguesPointRotations();
    steady_clock::time_point pointRodriguesEnd = steady_clock::now();
    steady_clock::duration pointRodriguesTime = pointRodriguesEnd - pointRodriguesStart;
    cout << "Ten million Rodrigues point rotations in "
         << duration_cast<microseconds>(pointRodriguesTime).count()
         << " microseconds\n";

    return 0;
}

void cubeTest() {
    RotatingCube cube;
    XVec3f rotationAxis(1.0f, 0.0f, 0.0f);
    float rotationAngle = M_PI;
    rotationAxis.normalize();
    cout << cube;
    cube.rotateEuler(rotationAxis, rotationAngle);
    cout << cube;
    cube.cleanFloatingPointErrors();
    cout << cube;
    cube.rotateRodrigues(rotationAxis, rotationAngle);
    cout << cube;
    cube.cleanFloatingPointErrors();
    cout << cube;
}

void pointTest() {
    RotatingPoint point;
    XVec3f rotationAxis = XVec3f(1.0f, 1.0f, 1.0f);
    float rotationAngle = M_PI / 6.0f;
    rotationAxis.normalize();
    cout << point;
    point.rotateEuler(rotationAxis, rotationAngle);
    cout << point;
    point.cleanFloatingPointErrors();
    cout << point;
    point.rotateRodrigues(rotationAxis, rotationAngle);
    cout << point;
    point.cleanFloatingPointErrors();
    cout << point;
}

void tenMillionEulerCubeRotations() {
    RotatingCube cube;
    XVec3f rotationAxis(1.0f, 1.0f, 1.0f);
    float rotationAngle = 2 * M_PI / NUMROTATIONS;
    for (int i = 0; i < NUMROTATIONS; ++i)
        cube.rotateEuler(rotationAxis, rotationAngle);
}

void tenMillionRodriguesCubeRotations() {
    RotatingCube cube;
    XVec3f rotationAxis(1.0f, 1.0f, 1.0f);
    float rotationAngle = 2 * M_PI / NUMROTATIONS;
    for (int i = 0; i < NUMROTATIONS; ++i)
        cube.rotateRodrigues(rotationAxis, rotationAngle);
}

void tenMillionEulerPointRotations() {
    RotatingPoint point;
    XVec3f rotationAxis(1.0f, 1.0f, 1.0f);
    float rotationAngle = 2 * M_PI / NUMROTATIONS;
    for (int i = 0; i < NUMROTATIONS; ++i)
        point.rotateEuler(rotationAxis, rotationAngle);
}

void tenMillionRodriguesPointRotations() {
    RotatingPoint point;
    XVec3f rotationAxis(1.0f, 1.0f, 1.0f);
    float rotationAngle = 2 * M_PI / NUMROTATIONS;
    for (int i = 0; i < NUMROTATIONS; ++i)
        point.rotateRodrigues(rotationAxis, rotationAngle);
}
