#include "../include/rotatingobject.h"
#include <iostream>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
/*Constructors*/

RotatingObject::
RotatingObject() {
	initAxes();
}

RotatingCube::
RotatingCube() {
	initVertices();
}

RotatingPoint::
RotatingPoint() {
	initVertices();
}

///////////////////////////////////////////////////////////////////////////////
/* Public Functions*/

/* Rotate vertices via composite Euler matrix RyT*RxT*Rz*Rx*Ry */
void RotatingObject::
rotateEuler(XVec3f& axis, float theta) {
	XMat3f eulerRotationMatrix = makeEulerRotationMatrix(axis, theta);
	for (int i = 0; i < vertices.size(); ++i)
		vertices[i] = eulerRotationMatrix * vertices[i];
}

/* Rotate vertices via Rodrigues rotation formula */
void RotatingObject::
rotateRodrigues(XVec3f& axis, float theta) {
	XMat3f rodriguesRotationMatrix = makeRodriguesRotationMatrix(axis, theta);
	for (int i = 0; i < vertices.size(); ++i)
		vertices[i] = rodriguesRotationMatrix * vertices[i];
}

/* Rounds vertices if they fall within EPSILON of
   the integer values 0, 1 */
void RotatingObject::
cleanFloatingPointErrors() {
	for (int i = 0; i < vertices.size(); ++i) {
		for (int j = 0; j < DIM; ++j) {
			if (fabs(vertices[i][j]) <= EPSILON)
				vertices[i][j] = 0.0f;
			else if (fabs(vertices[i][j] - 1.0f) <= EPSILON)
				vertices[i][j] = 1.0f;
		}
 	}
}


///////////////////////////////////////////////////////////////////////////////
/* Private Functions */

void RotatingObject::
addVertex(XVec3f vertex) {
	vertices.push_back(vertex);
} 

/* Define the standard basis coordinate system */
void RotatingObject::
initAxes() {
	xAxis = XVec3f(1.0f, 0.0f, 0.0f);
	yAxis = XVec3f(0.0f, 1.0f, 0.0f);
	zAxis = XVec3f(0.0f, 0.0f, 1.0f);
}

/* Create the rotation matrix for Euler rotation theta radians around axis */
XMat3f RotatingObject::
makeEulerRotationMatrix(XVec3f& axis, float theta) {
	/* Find projection of axis onto xz-plane */
	XVec3f xzComponent = axis - axis.project(yAxis);
	float xzComponentLength = xzComponent.norm();
	float yAngle, xAngle;
	/* Zero length means rotation axis is on y-axis */
	if (xzComponentLength < EPSILON) {
		yAngle = 0.0f;
		xAngle = M_PI / 2.0f;
	}
	/* Otherwise determine angles via projection */
	else {
		xzComponent /= xzComponentLength;
		/* Find rotation angle around y-axis */
		yAngle = 2 * M_PI - acos(zAxis.dot(xzComponent));
		/* Find rotation angle around x-axis */
		xAngle = acos(axis.dot(xzComponent));
	}
	/* Create comonent rotation matrices */
	XMat3f Ry = makeEulerRotationComponentMatrix(yAngle, ROTATEY);
	XMat3f Rx = makeEulerRotationComponentMatrix(xAngle, ROTATEX);
	XMat3f Rz = makeEulerRotationComponentMatrix(theta, ROTATEZ);
	XMat3f RxT = Rx.transpose();
	XMat3f RyT = Ry.transpose();
	/* Return composite rotation matrix */
	return RyT * RxT * Rz * Rx * Ry;
}

/* Create the rotation matrix for Rodrigues' rotation formula for a
   rotation of theta radians around axis */
XMat3f RotatingObject::
makeRodriguesRotationMatrix(XVec3f& axis, float theta) {
	/* Pre-computation steps for efficient matrix composition */
	float axax = axis.x() * axis.x();
	float axay = axis.x() * axis.y();
	float axaz = axis.x() * axis.z();
	float ayay = axis.y() * axis.y();
	float ayaz = axis.y() * axis.z();
	float azaz = axis.z() * axis.z();
	float cosTheta = cos(theta);
	float oneMinusCosTheta = 1 - cosTheta;
	float axayOneMinusCosTheta = axay * oneMinusCosTheta;
	float axazOneMinusCosTheta = axaz * oneMinusCosTheta;
	float ayazOneMinusCosTheta = ayaz * oneMinusCosTheta;
	float sinTheta = sin(theta);
	float sinax = sinTheta * axis.x();
	float sinay = sinTheta * axis.y();
	float sinaz = sinTheta * axis.z();
	/* Create Rodrigues' matrix */
	XMat3f rotationMatrix;
	rotationMatrix(0, 0) = axax * oneMinusCosTheta + cosTheta;
	rotationMatrix(0, 1) = axayOneMinusCosTheta - sinaz;
	rotationMatrix(0, 2) = axazOneMinusCosTheta + sinay;
	rotationMatrix(1, 0) = axayOneMinusCosTheta + sinaz;
	rotationMatrix(1, 1) = ayay * oneMinusCosTheta + cosTheta;
	rotationMatrix(1, 2) = ayazOneMinusCosTheta - sinax;
	rotationMatrix(2, 0) = axazOneMinusCosTheta - sinay;
	rotationMatrix(2, 1) = ayazOneMinusCosTheta + sinax;
	rotationMatrix(2, 2) = azaz * oneMinusCosTheta + cosTheta;

	return rotationMatrix;
}

/* Creates a planar rotation matrix on the xy-, xz-, or yz-plane */
XMat3f RotatingObject::
makeEulerRotationComponentMatrix(float theta, rotation_t rotationAxis) {
	/* Initialize to 3x3 identity matrix */
	XMat3f rotationMatrix;
	/* Pre-compute elements */
	float cosTheta = cos(theta);
	float sinTheta = sin(theta);
	/* Add non-zero elements */
	switch(rotationAxis) {
		case ROTATEX:
			rotationMatrix(0, 0) = 1.0f;
			rotationMatrix(2, 1) = sinTheta;
			rotationMatrix(1, 2) = -sinTheta;
			rotationMatrix(1, 1) = rotationMatrix(2, 2) = cosTheta;
			break;
		case ROTATEY:
			rotationMatrix(1, 1) = 1.0f;
			rotationMatrix(0, 2) = sinTheta;
			rotationMatrix(2, 0) = -sinTheta;
			rotationMatrix(0, 0) = rotationMatrix(2, 2) = cosTheta;
			break;
		case ROTATEZ:
			rotationMatrix(2, 2) = 1.0f;
			rotationMatrix(1, 0) = sinTheta;
			rotationMatrix(0, 1) = -sinTheta;
			rotationMatrix(0, 0) = rotationMatrix(1, 1) = cosTheta;
			break;
		default:
			cout << "Rotation axis " << rotationAxis << " is undefined. "
			     << "Returning identity matrix.\n";
	}
	return rotationMatrix;
}


/* Define the vertices of a unit cube */
void RotatingCube::
initVertices() {
	for (int i = 0; i < NUMVERTICESCUBE; ++i)
		addVertex(XVec3f((float) (i & 0x1),
					     				 (float) ((i & 0x2) >> 1),
					     				 (float) ((i & 0x4) >> 2)));
}

/* Create single vertex */
void RotatingPoint::
initVertices() {
	addVertex(XVec3f(1.0f, 2.0f, 3.0f));
}

///////////////////////////////////////////////////////////////////////////////
/* Friend Functions */

/* Overloaded output operator--lists cube vertices */
ostream&
operator<<(ostream& os, RotatingObject& obj) {
	os << "--Start Cube Vertex List--\n";
	for (int i = 0; i < obj.vertices.size(); ++i)
		os << obj.vertices[i] << '\n';
	os << "--End Cube Vertex List--\n";
	return os;
}

///////////////////////////////////////////////////////////////////////////////
