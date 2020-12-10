// 205812290	ID2

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_
#include <iostream>
//----Testing---
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <vector>
//
using namespace std;
// ------------ DO NOT CHANGE -----------
class Point{
public:
	float x,y;
	Point(float x,float y):x(x),y(y){}
};

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------
/*
// implement
Circle findMinCircle(Point** points,size_t size){
	return Circle(Point(0,0),0);
}
*/
// you may add helper functions here
//-------------------------TESTING--------------------------------

// Defining infinity
const float INF = 1e18;

// **The following two functions are used to find the equation of the circle with three given points**
// Method to get a circle defined by 3 points, as it a critical part of the solution:
Point get_circle_center(float bx, float by,
                        float cx, float cy)
{
    float B = bx * bx + by * by;
    float C = cx * cx + cy * cy;
    float D = bx * cy - by * cx;
    return { (cy * B - by * C) / (2 * D),
             (bx * C - cx * B) / (2 * D) };
}
// Function to return a unique circle that
// intersects three points
Circle circle_from(const Point& A, const Point& B,const Point& C)
{
    Point I = get_circle_center(B.x - A.x, B.y - A.y,
                                C.x - A.x, C.y - A.y);
    I.x += A.x;
    I.y += A.y;
    //using euclidean distance in the right side of the return function line below
    return { I, float(sqrt(pow(I.x - A.x, 2) + pow(I.y - A.y, 2))) };
}
// Function to return the smallest circle
// that intersects 2 points
Circle circle_from(const Point& A, const Point& B)
{
    // Set the center to be the midpoint of A and B:
    Point C = { (A.x + B.x) / 2.f, (A.y + B.y) / 2.f };

    // Set the radius to be half the distance AB, doing that with
    // using euclidean distance in the right side of the return function line below:
    return { C, float(sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2))) / 2.f };
}

// Function to check whether a circle encloses the given points:
bool is_valid_circle(const Circle& c, const vector<Point>& points_on_boundary)
{

    // Iterating through all the points to check whether the
    // points inside the circle bounderies (include the bounderies themselves) or not:
    for (const Point& p : points_on_boundary)
        if ((sqrt(pow(c.center.x - p.x, 2)
         + pow(c.center.y - p.y, 2)) <= c.radius)==false) //checking if point inside bounderies
                                                        //(include the bounderies themselves), if not, than - 
            return false;
    return true;
}

// Function to return the minimum enclosing
// circle for N <= 3
Circle min_circle_trivial(vector<Point>& points_on_boundary)
{
    assert(points_on_boundary.size() <= 3);
    if (points_on_boundary.empty()) {
        return { { 0, 0 }, 0 };
    }
    else if (points_on_boundary.size() == 1) {
        return { points_on_boundary[0], 0 };
    }
    else if (points_on_boundary.size() == 2) {
        return circle_from(points_on_boundary[0], points_on_boundary[1]);
    }

    // To check if MEC can be determined
    // by 2 points only
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            Circle c = circle_from(points_on_boundary[i], points_on_boundary[j]);
            if (is_valid_circle(c, points_on_boundary))
                return c;
        }
    }
    return circle_from(points_on_boundary[0], points_on_boundary[1], points_on_boundary[2]);
}
/*
 We are Returning the MEC with the Welzl's algorithm.
 It takes a set of input points P and a set R points on the circle boundary. 
 n representing the number of points in P that are not yet processed.

Circle welzl_helper(vector<Point>& P,vector<Point> R, int n) */
Circle welzlAlgorithm(Point** points,vector<Point> points_on_boundary, size_t size) {
    // Base case when all points processed or |R| = 3
    if (size == 0 || points_on_boundary.size() == 3) {
        return min_circle_trivial(points_on_boundary);
    }
    // Pick a random point randomly
    int random_index = rand() % size;
    Point p = *points[random_index];

    // Put the picked point at the end of P as it's more efficient than deleting from the middle of the vector
    swap(*points[random_index], *points[size - 1]);

    // Get the MEC circle d from the set of points P - {p}
    Circle MEC = welzlAlgorithm(points, points_on_boundary, size - 1);

    //If d contains p, return d, meaning if inside (boundry *), line below calc oclid dis between thet point and the radius, if lesser that, its mean its within bounderies
    if ((sqrt(pow(MEC.center.x - p.x, 2) + pow(MEC.center.y - p.y, 2)) <= MEC.radius)==true) {
        return MEC;
    }

    // Otherwise, must be on the boundary of the MEC
    points_on_boundary.push_back(p);

    // Return the MEC for P - {p} and R U {p}
   return welzlAlgorithm(points, points_on_boundary, size - 1);
}

//Circle welzl(const vector<Point>& P)
Circle findMinCircle(Point** points, size_t size)
{
   // random_shuffle(*points[0], *points[size - 1]);
    return welzlAlgorithm(points, {}, size);
}
#endif /* MINCIRCLE_H_ */
