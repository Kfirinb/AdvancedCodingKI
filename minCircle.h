//205812290    209013168

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_
#include <iostream>
#include <algorithm>
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

// **The following two functions are used to find the equation of the circle with three given points**
// Method to get a circle defined by 3 points, as it a critical part of the solution:
Point get_circle_center(const Point& A, const Point& B,const Point& C) {
    float bx = B.x - A.x, by = B.y - A.y, cx = C.x - A.x, cy = C.y - A.y;
    float AB = bx * bx + by * by;
    float AC = cx * cx + cy * cy;
    float ABC = bx * cy - by * cx;
    return Point((cy * AB - by * AC) / (2 * ABC), (bx * AC - cx * AB) / (2 * ABC));

}

// Function to return a unique circle that
// intersects three points
Circle circle_from(const Point& A, const Point& B,const Point& C)
{
    //Point circleCenter = get_circle_center(B.x - A.x, B.y - A.y,C.x - A.x, C.y - A.y);
    Point circleCenter = get_circle_center(A,B,C);
    circleCenter.x += A.x;
    circleCenter.y += A.y;
    float radius = float(std::hypot(circleCenter.x - A.x, circleCenter.y - A.y));
    Circle* c = new Circle(circleCenter, radius);
    return *c;
}
// Function to return the smallest circle
// that intersects 2 points
Circle circle_from(const Point& A, const Point& B) {
    // Set the center to be the midpoint of A and B:
    Point* circleCenter = new Point((A.x + B.x) / 2.f, (A.y + B.y) / 2.f );

    // Set the radius to be half the distance AB, doing that with
    float radius = float(std::hypot(A.x - B.x, A.y - B.y)) / 2.f;

    Circle* c = new Circle(*circleCenter, radius);
    return *c;
}

// Function to check whether a circle encloses the given points:
bool is_valid_circle(const Circle& c, const vector<Point>& points_on_boundary)
{

    // Iterating through all the points to check whether the
    // points inside the circle bounderies (include the bounderies themselves) or not:
    for (const Point& p : points_on_boundary) {
        if ((std::hypot(c.center.x - p.x, c.center.y - p.y) <= c.radius) == false) //checking if point inside bounderies
            //(include the bounderies themselves), if not, than -
            return false;
    }
    return true;
}

// Function to return the minimum enclosing
// circle for N <= 3
Circle min_circle_trivial(vector<Point>& points_on_boundary)
{
    if(points_on_boundary.size() > 3) {
        throw std::invalid_argument( "Points on boudary greater than 3!");
    }
    if (points_on_boundary.empty()) {
        return Circle(Point(0, 0), 0);
    } else if (points_on_boundary.size() == 1) {
        return Circle(points_on_boundary[0], 0);
    } else if (points_on_boundary.size() == 2) {
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
 */
Circle welzlAlgorithm(Point** points,vector<Point> points_on_boundary, size_t size) {
    // Base case when all points processed or |R| = 3
    if (size == 0 || points_on_boundary.size() == 3) {
        return min_circle_trivial(points_on_boundary);
    }
    // Pick a random point randomly
    int random_index = rand() % size;
    Point p = *points[random_index];

    // Put the picked point at the end of P as it's more efficient than deleting from the middle of the vector
    std::swap(*points[random_index], *points[size - 1]);

    // Get the MEC circle d from the set of points P - {p}
    Circle MEC = welzlAlgorithm(points, points_on_boundary, size - 1);

    //If d contains p, return d, meaning if inside (boundry *), line below calc oclid dis between thet point and the radius, if lesser that, its mean its within bounderies
    if ((std::hypot(MEC.center.x - p.x, MEC.center.y - p.y) <= MEC.radius)==true) {
        return MEC;
    }

    // Otherwise, must be on the boundary of the MEC
    points_on_boundary.push_back(p);

    // Return the MEC for P - {p} and R U {p}
    return welzlAlgorithm(points, points_on_boundary, size - 1);
}

Circle findMinCircle(Point** points, size_t size)
{
    random_shuffle(&points[0], &points[size - 1]);
    return welzlAlgorithm(points, {}, size);
}
#endif /* MINCIRCLE_H_ */
