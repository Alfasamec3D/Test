#include<iostream>

template <typename T> int sgn(T val);

class Point;

class Vector;

class Triangle;

class Segment;



Vector vectprod(const Vector vector1, const Vector vector2);
double scalprod(const Vector vector1, const Vector vector2);
double sqrdist(const Point point1, const Point point2);
bool operator== (const Point point1, const Point point2);
Point operator*(const Point point, const double num);
Point operator*(const double num, const Point point);
Vector operator*(const double num, const Vector vector);
Vector operator*(const Vector vector, const double num);
bool operator==(const Vector vector1, const Vector vector2);
Vector operator+(const Vector vector1, const Vector vector2);

Point kintsec(const Segment segment, const Triangle triangle);
std::istream& operator>> (std::istream& in, Triangle& triangle);


bool compare(const Point point, const Segment segment);
bool compare(const Segment segment1, const Segment segment2);
bool compare(const Point point, const Triangle triangle);
bool compare(const Segment segment, const Triangle triangle);
bool compare(const Triangle triangle1, const Triangle triangle2);
void final();