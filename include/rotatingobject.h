#include <iostream>
#include <vector>

#include "xvec.h"
#include "xmat.h"

#define NUMVERTICESCUBE 8
#define DIM 3
#define EPSILON 1e-5
#define M_PI 3.14159265358979323846  /* pi */

typedef enum {ROTATEX, ROTATEY, ROTATEZ} rotation_t;

class RotatingObject {
public:
	RotatingObject();
	void addVertex(XVec3f vertex);
	void rotateEuler(XVec3f& axis, float theta);
	void rotateRodrigues(XVec3f& axis, float theta);
	void cleanFloatingPointErrors();
	friend std::ostream& operator<<(std::ostream& os, RotatingObject& obj);
private:
	void initAxes();
	XMat3f makeEulerRotationMatrix(XVec3f& axis, float theta);
	XMat3f makeEulerRotationComponentMatrix(float theta,
											rotation_t rotationAxis);
	XMat3f makeRodriguesRotationMatrix(XVec3f& axis, float theta);

	std::vector<XVec3f> vertices;
	XVec3f xAxis;
	XVec3f yAxis;
	XVec3f zAxis;
};

class RotatingCube : public RotatingObject {
public:
	RotatingCube();
private:
	void initVertices();
};

class RotatingPoint : public RotatingObject {
public:
	RotatingPoint();
private:
	void initVertices();
};