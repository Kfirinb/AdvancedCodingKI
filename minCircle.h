//205812290    209013168
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_
#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
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
the method is to get a circle defined by 3 points */
Point get_circle_center(const Point& A, const Point& B,const Point& C) {
    float bx = B.x - A.x, by = B.y - A.y, cx = C.x - A.x, cy = C.y - A.y;
    float AB = bx * bx + by * by;
    float AC = cx * cx + cy * cy;
    float ABC = bx * cy - by * cx;
    return Point((cy * AB - by * AC) / (2 * ABC), (bx * AC - cx * AB) / (2 * ABC));
}

/* Function that return a circle from three points */
Circle circle_from(const Point& A, const Point& B,const Point& C) {
    Point circleCenter = get_circle_center(A,B,C);
    circleCenter.x += A.x;
    circleCenter.y += A.y;
    float radius = float(std::hypot(circleCenter.x - A.x, circleCenter.y - A.y));
    Circle* c = new Circle(circleCenter, radius);
    return *c;
}

/* Function to return the smallest circle from 2 points by setting
 the center of the circle to be the midpoint of A and B, and the radius to be half the distance AB */
Circle circle_from(const Point& A, const Point& B) {
    Point* circleCenter = new Point((A.x + B.x) / 2.f, (A.y + B.y) / 2.f );
    float radius = float(std::hypot(A.x - B.x, A.y - B.y)) / 2.f;
    Circle* c = new Circle(*circleCenter, radius);
    return *c;
}

/* Function to check whether a circle encloses *all* the given points, if not - return false */
bool is_valid_circle(const Circle& c, const vector<Point>& points_on_boundary) {
    for (const Point& p : points_on_boundary) {
        if ((std::hypot(c.center.x - p.x, c.center.y - p.y) <= c.radius) == false)
            return false;
    }
    return true;
}

/* Function to return the minimum enclosing circle for size of points_on_boundary <= 3, and we check as well
if MEC can be determined by 2 points only */
Circle min_circle_enclosing(vector<Point>& points_on_boundary) {
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

/*This function returns the MEC(Minimum Enclosing Circle) with the Welzl's algorithm.
 The function gets a set of points and a set of points on the boundary's circle.
 "size" is the number of points left to cover.
 First the function checks if all points are covered or size of points_on_boundary is 3
 if so then we call the function min_circle_enclosing,if not,
 we take a random point from points and then swap it with the end point of points
 as a way to delete the random point from the points array.
 Then we check if the random point is in the current minimal circle, if not
 then we push it to the points_on_boundary vector.
*/
Circle welzlAlgorithm(Point** points,vector<Point> points_on_boundary, size_t size) {
    if (size == 0 || points_on_boundary.size() == 3) {
        return min_circle_enclosing(points_on_boundary);
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

//This function finds the minimal radius circle for given points using the Welzl algorithm
Circle findMinCircle(Point** points, size_t size) {
    random_shuffle(&points[0], &points[size - 1]);
    return welzlAlgorithm(points, {}, size);
}
#endif /* MINCIRCLE_H_ */
