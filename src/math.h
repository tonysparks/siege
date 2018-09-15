#ifndef MATH_H
#define MATH_H

#include <math.h>

typedef float Vec2[2];

#define VectorClear(v) ((v)[0]=(v)[1]=0)
#define VectorSet(v, x, y) ((v)[0]=(x),(v)[1]=(y))
#define VectorCopy(a, b) ((b)[0]=(a)[0],(b)[1]=(a)[1])
#define VectorCrossProduct(a, b) (((a)[0] * (b)[1]) - ((a)[1] * (b)[0]))
#define VectorDotProduct(a, b) (((a)[1] * (b)[0]) + ((a)[0] * (b)[1]))

#define VectorAdd(a, b, d) ((d)[0]=(a)[0] + (b)[0], (d)[1]=(a)[1] + (b)[1])
#define VectorAddScalar(a, s, d) ((d)[0]=(a)[0] + (s), (d)[1]=(a)[1] + (s))

#define VectorSub(a, b, d) ((d)[0]=(a)[0] - (b)[0], (d)[1]=(a)[1] - (b)[1])
#define VectorSubScalar(a, s, d) ((d)[0]=(a)[0] - (s), (d)[1]=(a)[1] - (s))

#define VectorRotate(a, r, d) ((d)[0] = ((a)[0] * cos(r)) - ((a)[1] * sin(r)), (d)[1] = ((a)[0] * sin(r)) + ((a)[1] * cos(r)))
#define VectorAngle(a, b) (atan2((b)[1], (b)[0]) - atan2((a)[1], (a)[0]))

#define VectorMA(a, b, s, d) ((d)[0] = (a)[0] + (b)[0] * s, (d)[1] = (a)[1] + (b)[1] * s)
#define VectorMS(a, b, s, d) ((d)[0] = (a)[0] - (b)[0] * s, (d)[1] = (a)[1] - (b)[1] * s)

#define VectorEq(a, b) ((a)[0] == (b)[0] && (a)[1] == (b)[1])
#define floatEq(a, b, e) (abs((a) - (b)) < e)
#define VectorEqe(a, b, e) (floatEq((a)[0], (b)[0], (e)) && floatEq((a)[1], (b)[1], (e)))

#define VectorLength(a) (sqrt(((a)[0] * (a)[0]) + ((a)[1] * (a)[1])))
#define VectorLengthSq(a) (((a)[0] * (a)[0]) + ((a)[1] * (a)[1]))

//#define VectorDistance(a, b)

#endif
