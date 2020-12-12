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

/*The following two functions are used to find the equation of the circle with three given points.
the method is to get a circle defined by 3 points: */
Point get_circle_center(const Point& A, const Point& B,const Point& C) {
    float bx = B.x - A.x, by = B.y - A.y, cx = C.x - A.x, cy = C.y - A.y;
    float AB = bx * bx + by * by;
    float AC = cx * cx + cy * cy;
    float ABC = bx * cy - by * cx;
    return Point((cy * AB - by * AC) / (2 * ABC), (bx * AC - cx * AB) / (2 * ABC));
}
/* Func' that return a unique circle that intersects three points, doint that with
getting circle center to the 3 points: */
Circle circle_from(const Point& A, const Point& B,const Point& C) {
    Point circleCenter = get_circle_center(A,B,C);
    circleCenter.x += A.x;
    circleCenter.y += A.y;
    float radius = float(std::hypot(circleCenter.x - A.x, circleCenter.y - A.y));
    Circle* c = new Circle(circleCenter, radius);
    return *c;
}
/* Function to return the smallest circle that intersects 2 points, in the func, we set 
 the center to be the midpoint of A and B, and the radius to be half the distance AB: */
Circle circle_from(const Point& A, const Point& B) {
    Point* circleCenter = new Point((A.x + B.x) / 2.f, (A.y + B.y) / 2.f );
    float radius = float(std::hypot(A.x - B.x, A.y - B.y)) / 2.f;
    Circle* c = new Circle(*circleCenter, radius);
    return *c;
}
/* Function to check whether a circle encloses *all* the given points,
We are iterating through all the points to check whether the points are inside
the circle bounderies (include the bounderies themselves), if not - return false: */
bool is_valid_circle(const Circle& c, const vector<Point>& points_on_boundary) {
    for (const Point& p : points_on_boundary) {
        if ((std::hypot(c.center.x - p.x, c.center.y - p.y) <= c.radius) == false)
            return false;
    }
    return true;
}
/* Function to return the minimum enclosing circle for N <= 3, and we check as well
if MEC can be determined by 2 points only. */
Circle min_circle_trivial(vector<Point>& points_on_boundary) {
    if(points_on_boundary.size() > 3) {
        throw std::invalid_argument("The Points on the boudary are greater than three");
    }
    if (points_on_boundary.empty()) {
        return Circle(Point(0, 0), 0);
    } else if (points_on_boundary.size() == 1) {
        return Circle(points_on_boundary[0], 0);
    } else if (points_on_boundary.size() == 2) {
        return circle_from(points_on_boundary[0], points_on_boundary[1]);
    }
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            Circle c = circle_from(points_on_boundary[i], points_on_boundary[j]);
            if (is_valid_circle(c, points_on_boundary))
                return c;
        }
    }
    return circle_from(points_on_boundary[0], points_on_boundary[1], points_on_boundary[2]);
}
/* We are Returning the MEC with the Welzl's algorithm.
 It takes a set of input points P and a set R points on the circle boundary.
 n representing the number of points in P that are not yet processed.
 We check the base case -  when all points processed or |R| = 3, if not,
 we take random point than add it to points we finished to work with,
 in a way that check for the added point what is the current minimal circle.
 In the func we check boundries state etc, to make it as efficient as possible:
*/
Circle welzlAlgorithm(Point** points,vector<Point> points_on_boundary, size_t size) {
    if (size == 0 || points_on_boundary.size() == 3) {
        return min_circle_trivial(points_on_boundary);
    }
    int random_index = rand() % size;
    Point p = *points[random_index];
    std::swap(*points[random_index], *points[size - 1]);
    Circle MEC = welzlAlgorithm(points, points_on_boundary, size - 1);
    if ((std::hypot(MEC.center.x - p.x, MEC.center.y - p.y) <= MEC.radius)==true) {
        return MEC;
    }
    points_on_boundary.push_back(p);
    return welzlAlgorithm(points, points_on_boundary, size - 1);
}
//checking minimal circle for given points
Circle findMinCircle(Point** points, size_t size) {
    random_shuffle(&points[0], &points[size - 1]);
    return welzlAlgorithm(points, {}, size);
}
#endif /* MINCIRCLE_H_ */
