#ifndef COMMON_H
#define COMMON_H

#ifndef MAX_PATH
    #define MAX_PATH 256
#endif

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

typedef enum LogLevel {
    INFO_LEVEL,
    DEBUG_LEVEL,
    ERROR_LEVEL,
    FATAL_LEVEL,
} LogLevel;

void logger(LogLevel level, const char* format, ...);

typedef enum Maybe {
    OK,
    FAILED,
} Maybe;

void* siegeMalloc(size_t size);
void* siegeCalloc(size_t count, size_t size);
void* siegeRealloc(void* ptr, size_t newSize);
void  siegeFree(void* mem);

typedef struct TimeStep {
    long    frameTime;          /* time from last frame */
    long    gameClock;          /* current game clock */
    double  frameTimeFraction;  /* time from last frame in fractions of seconds */
} TimeStep;

/*
==============================================
Vector library
==============================================
*/

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

typedef struct Rect {
    float x, y, w, h;
} Rect;

#define rectSetPos(r,pos) ((r).x = pos[0], (r).y = pos[1])
#define rectClear(r) ((r).x = (r).y = (r).w = (r).h = 0)

/*
==============================================
Stretchy Buffer
==============================================
*/


#define stb_sb_free(a)         ((a) ? free(stb__sbraw(a)),0 : 0)
#define stb_sb_push(a,v)       (stb__sbmaybegrow(a,1), (a)[stb__sbn(a)++] = (v))
#define stb_sb_count(a)        ((a) ? stb__sbn(a) : 0)
#define stb_sb_add(a,n)        (stb__sbmaybegrow(a,n), stb__sbn(a)+=(n), &(a)[stb__sbn(a)-(n)])
#define stb_sb_last(a)         ((a)[stb__sbn(a)-1])

#define stb__sbraw(a) ((int *) (a) - 2)
#define stb__sbm(a)   stb__sbraw(a)[0]
#define stb__sbn(a)   stb__sbraw(a)[1]

#define stb__sbneedgrow(a,n)  ((a)==0 || stb__sbn(a)+(n) >= stb__sbm(a))
#define stb__sbmaybegrow(a,n) (stb__sbneedgrow(a,(n)) ? stb__sbgrow(a,n) : 0)
#define stb__sbgrow(a,n)      (*((void **)&(a)) = stb__sbgrowf((a), (n), sizeof(*(a))))

#include <stdlib.h>

static void * stb__sbgrowf(void *arr, int increment, int itemsize)
{
   int dbl_cur = arr ? 2*stb__sbm(arr) : 0;
   int min_needed = stb_sb_count(arr) + increment;
   int m = dbl_cur > min_needed ? dbl_cur : min_needed;
   int *p = (int *) realloc(arr ? stb__sbraw(arr) : 0, itemsize * m + sizeof(int)*2);
   if (p) {
      if (!arr)
         p[1] = 0;
      p[0] = m;
      return p+2;
   } else {
      #ifdef STRETCHY_BUFFER_OUT_OF_MEMORY
      STRETCHY_BUFFER_OUT_OF_MEMORY ;
      #endif
      return (void *) (2*sizeof(int)); // try to force a NULL pointer exception later
   }
}

#endif
