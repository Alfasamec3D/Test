#include <vector>
#include<iostream>
#include<string>
#include <algorithm>
#include<cmath>

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

class Point
{
private:
	std::vector<double> m_coords;
public:
	Point() = default;
	Point(const double x, const double y, const double z) : m_coords{ x,y,z } {}
	double x() const
	{
		return m_coords[0];
	}
	double y() const
	{
		return m_coords[1];
	}
	double z() const
	{
		return m_coords[2];
	}
};

class Vector
{
private:
	std::vector<double> m_coords;
public:
	Vector() = default;
	Vector(const double x, const double y, const double z) : m_coords{ x,y,z } {};
	Vector(const Point point1, const Point point2) : m_coords{ point2.x() - point1.x(),point2.y() - point1.y(),point2.z() - point1.z() } {};
	Vector(const Point point) : m_coords{ point.x(), point.y(), point.z() } {};
	double x() const
	{
		return m_coords[0];
	}
	double y() const
	{
		return m_coords[1];
	}
	double z() const
	{
		return m_coords[2];
	}
};

class Triangle;

class Segment
{
private:
	std::vector<Point> m_edges;
public:
	Segment() = default;
	Segment(const Point edge1, const Point edge2) : m_edges{ std::vector<Point> {edge1, edge2} } {}
	Vector dir() const
	{
		return { m_edges[0], m_edges[1] };
	}
	friend bool compare(const Point point, const Segment segment);
	friend bool compare(const Segment segment1, const Segment segment2);
	friend bool compare(const Segment segment, const Triangle triangle);
	friend Point kintsec(const Segment segment, const Triangle triangle);
};



Vector vectprod(const Vector vector1, const Vector vector2)
{
	return { vector1.y() * vector2.z() - vector2.y() * vector1.z(),
			 vector1.z() * vector2.x() - vector2.z() * vector1.x(),
			 vector1.x() * vector2.y() - vector2.x() * vector1.y()};
}
double scalprod(const Vector vector1, const Vector vector2)
{
	return vector1.x() * vector2.x() + vector1.y() * vector2.y() + vector1.z() * vector2.z();
}
double sqrdist(const Point point1, const Point point2)
{
	return (pow(point2.x() - point1.x(), 2) + pow(point2.y() - point1.y(), 2) + pow(point2.z() - point1.z(), 2));
}
bool operator== (const Point point1, const Point point2)
{
	return (point1.x() == point2.x()) && (point1.y() == point2.y()) && (point1.z() == point2.z());
}
Point operator*(const Point point, const double num)
{
	return { point.x() * num, point.y() * num, point.z() * num };
}
Point operator*(const double num, const Point point)
{
	return { point.x() * num, point.y() * num, point.z() * num };
}
Vector operator*(const double num, const Vector vector)
{
	return { vector.x() * num, vector.y() * num, vector.z() * num };
}
Vector operator*(const Vector vector, const double num)
{
	return { vector.x() * num, vector.y() * num, vector.z() * num };
}
bool operator==(const Vector vector1, const Vector vector2)
{
	return (vector1.x() == vector2.x()) && (vector1.y() == vector2.y()) && (vector1.z() == vector2.z());
}
Vector operator+(const Vector vector1, const Vector vector2)
{
	return { vector1.x() + vector2.x(),vector1.y() + vector2.y() ,vector1.z() + vector2.z() };
}



class Triangle
{
private:
	std::vector <Point> m_apexes;

public:
	Triangle() {}
	Triangle(const Point apex1, const Point apex2, const Point apex3) : m_apexes{ std::vector <Point>{apex1, apex2, apex3} } {}
	Triangle(const double x1, const double y1, const double z1, const double x2, const double y2, const double z2, const double x3, const double y3, const double z3) :
		m_apexes{ std::vector<Point>{Point {x1, y1, z1},Point {x2, y2, z2},Point {x3, y3, z3}} } {}
	
	Vector norm() const
	{
		return  vectprod({ m_apexes[0], m_apexes[1] }, { m_apexes[1], m_apexes[2] });
	}
	int actual() const
	{
		if ((m_apexes[0] == m_apexes[1]) && (m_apexes[1] == m_apexes[2]))
		{
			return 0;
		}

		if (norm() == Vector{ 0,0,0 })
		{
			return 1;
		}

		return 2;

	}
	Segment segm() const
	{
		if (std::max({ sqrdist(m_apexes[0], m_apexes[1]), sqrdist(m_apexes[1], m_apexes[2]), sqrdist(m_apexes[2], m_apexes[0]) }) == sqrdist(m_apexes[0], m_apexes[1]))
		{
			return Segment(m_apexes[0], m_apexes[1]);
		}

		if (std::max({ sqrdist(m_apexes[0], m_apexes[1]), sqrdist(m_apexes[1], m_apexes[2]), sqrdist(m_apexes[2], m_apexes[0]) }) == sqrdist(m_apexes[1], m_apexes[2]))
		{
			return Segment(m_apexes[1], m_apexes[2]);
		}
		return Segment(m_apexes[2], m_apexes[0]);
	}
	
	friend bool compare(const Triangle triangle1, const Triangle triangle2);
	friend bool compare(const Segment segment, const Triangle triangle);
	friend bool compare(const Point point, const Triangle triangle);
	friend Point kintsec(const Segment segment, const Triangle triangle);
};

Point kintsec(const Segment segment, const Triangle triangle)
{
	Vector temp = segment.m_edges[0] * scalprod(segment.dir(), triangle.norm()) + segment.dir() * scalprod({ segment.m_edges[0],triangle.m_apexes[0] }, triangle.norm());
	return { temp.x(), temp.y(), temp.z() };
}
std::istream& operator>> (std::istream& in, Triangle& triangle)
{
	double x1, y1, z1, x2, y2, z2, x3, y3, z3;
	in >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
	triangle = Triangle{ x1, y1, z1, x2, y2, z2, x3, y3, z3 };
	return in;
}



bool compare(const Point point, const Segment segment)
{
	return ((std::pow(scalprod(Vector{ segment.m_edges[0], point }, Vector{ segment.m_edges[1], point }), 2) == sqrdist(segment.m_edges[0], point) * sqrdist(segment.m_edges[1], point)) && (scalprod(Vector{ segment.m_edges[0], point }, Vector{ segment.m_edges[1], point }) <= 0));
}
bool compare(const Segment segment1, const Segment segment2)
{
	if (scalprod({ segment1.m_edges[0],segment2.m_edges[0] }, vectprod(segment1.dir(), segment2.dir())) != 0)
	{
		return false;
	}
	Vector n1 = vectprod(segment1.dir(), vectprod(segment1.dir(), segment2.dir()));
	Vector n2 = vectprod(segment2.dir(), vectprod(segment1.dir(), segment2.dir()));
	int d11 = sgn(scalprod(n2, { segment2.m_edges[0],segment1.m_edges[0] }));
	int d12 = sgn(scalprod(n2, { segment2.m_edges[0],segment1.m_edges[1] }));
	int d21 = sgn(scalprod(n1, { segment1.m_edges[0],segment2.m_edges[0] }));
	int d22 = sgn(scalprod(n1, { segment1.m_edges[0],segment2.m_edges[1] }));
	return ((d11 != d12) && (d21 != d22)) ||
		((d11 == 0) && (d12 == 0) && ((compare(segment1.m_edges[0], segment2)) || (compare(segment1.m_edges[1], segment2)) || (compare(segment2.m_edges[0], segment1))));
}
bool compare(const Point point, const Triangle triangle)
{
	if (scalprod({ triangle.m_apexes[0],point }, triangle.norm()) != 0)
	{
		return false;
	}
	return triangle.norm() ==
		Triangle{ point, triangle.m_apexes[0], triangle.m_apexes[1] }.norm() * sgn(scalprod(triangle.norm(), Triangle{ point, triangle.m_apexes[0], triangle.m_apexes[1] }.norm())) +
		Triangle{ point, triangle.m_apexes[1], triangle.m_apexes[2] }.norm() * sgn(scalprod(triangle.norm(), Triangle{ point, triangle.m_apexes[1], triangle.m_apexes[2] }.norm())) +
		Triangle{ point, triangle.m_apexes[2], triangle.m_apexes[0] }.norm() * sgn(scalprod(triangle.norm(), Triangle{ point, triangle.m_apexes[2], triangle.m_apexes[0] }.norm()));
}
bool compare(const Segment segment, const Triangle triangle)
{
	int d1 = sgn(scalprod({ triangle.m_apexes[0],segment.m_edges[0] }, triangle.norm()));
	int d2 = sgn(scalprod({ triangle.m_apexes[0], segment.m_edges[1] }, triangle.norm()));
	if ((d1 + d2 == 2) || (d1 + d2 == -2))
	{
		return false;
	}
	else if ((d1 == 0) && (d2 == 0))
	{
		return compare(segment.m_edges[0], triangle) ||
			compare(segment.m_edges[1], triangle) ||
			compare(segment, { triangle.m_apexes[0],triangle.m_apexes[1] }) ||
			compare(segment, { triangle.m_apexes[1],triangle.m_apexes[2] }) ||
			compare(segment, { triangle.m_apexes[2],triangle.m_apexes[0] });
	}
	else if ((d1 == 0) && (d2 != 0))
	{
		if (compare(segment.m_edges[0], triangle))
		{
			return true;
		}
	}
	else if ((d1 != 0) && (d2 == 0))
	{
		if (compare(segment.m_edges[1], triangle))
		{
			return true;
		}
	}
	double k = scalprod(segment.dir(), triangle.norm());
	return compare(kintsec(segment, triangle), Triangle{ triangle.m_apexes[0] * k,triangle.m_apexes[1] * k, triangle.m_apexes[2] * k });
}
bool compare(const Triangle triangle1, const Triangle triangle2)
{
	if ((triangle1.actual() == 0) && (triangle2.actual() == 0))
	{
		return triangle1.m_apexes[0] == triangle2.m_apexes[0];
	}
	if ((triangle1.actual() == 0) && (triangle2.actual() == 1))
	{
		return (compare(triangle1.m_apexes[0], triangle2.segm()));
	}
	if ((triangle1.actual() == 1) && (triangle2.actual() == 0))
	{
		return (compare(triangle2.m_apexes[0], triangle1.segm()));
	}
	if ((triangle1.actual() == 1) && (triangle2.actual() == 1))
	{
		return (compare(triangle1.segm(), triangle2.segm()));
	}
	if ((triangle1.actual() == 1) && (triangle2.actual() == 2))
	{
		return (compare(triangle1.segm(), triangle2));
	}
	if ((triangle1.actual() == 2) && (triangle2.actual() == 1))
	{
		return (compare(triangle2.segm(), triangle1));
	}
	if ((triangle1.actual() == 2) && (triangle2.actual() == 2))
	{
		std::vector<int>d1(3);
		std::vector<int>d2(3);
		for (int i = 0; i < 3; i++)
		{
			d1[i] = sgn(scalprod({ triangle2.m_apexes[0],triangle1.m_apexes[i] }, triangle2.norm()));
		}
		for (int i = 0; i < 3; i++)
		{
			d2[i] = sgn(scalprod({ triangle1.m_apexes[0] , triangle2.m_apexes[i] }, triangle1.norm()));
		}
		if (((d1[0] == d1[1]) && (d1[1] == d1[2]) && (d1[0] != 0)) || ((d2[0] == d2[1]) && (d2[1] == d2[2]) && (d2[0] != 0)))
		{
			return false;
		}
		if ((d1[0] == 0) && (d1[1] == 0) && (d1[2] == 0))
		{
			return compare(triangle1.m_apexes[0], triangle2) || compare(triangle1.m_apexes[1], triangle2) || compare(triangle1.m_apexes[2], triangle2) ||
				compare(triangle2.m_apexes[0], triangle1) || compare(triangle2.m_apexes[1], triangle1) || compare(triangle2.m_apexes[2], triangle1);
		}
		//���������� 2 ������������� ������ ��������, ��� ��� ������������ ����� ���������� ���� ����� ���������, �� �� �����������.
		Segment segment11, segment12, segment21, segment22;
		Point t11, t12, t21, t22;
		double l11, l12, l21, l22;
		int k1{ 0 }, k2{ 0 };
		for (int i = 0; i < 3; i++)
		{
			//������� �� ����� ������������ 1 �� ����� � ��������� ������������ 2:
			if ((d1[i] == -d1[(i + 1) % 3]) && (d1[(i + 1) % 3] == d1[(i + 2) % 3]))
			{

				k1 = 2;
				segment11 = { triangle1.m_apexes[i], triangle1.m_apexes[(i + 1) % 3] };
				segment12 = { triangle1.m_apexes[(i + 2) % 3], triangle1.m_apexes[i] };
				t11 = kintsec(segment11, triangle2);
				t12 = kintsec(segment12, triangle2);
				l11 = scalprod(segment11.dir(), triangle2.norm());
				l12 = scalprod(segment12.dir(), triangle2.norm());
				break;
			}
			//������ 2 ����� ������������ 1 ����� � ��������� ������������ 2:
			if ((d1[i] == 0) && (d1[(i + 1) % 3] == 0))
			{
				k1 = 2;
				t11 = triangle1.m_apexes[i];
				l11 = 1;
				t12 = triangle1.m_apexes[(i + 1) % 3];
				l12 = 1;
				break;
			}
			//������ 1 ����� ������������ 1 ����� � ��������� ������������ 2:
			if ((d1[i] == 0) && (d1[(i + 1) % 3] == -d1[(i + 2) % 3]))
			{
				k1 = 2;
				t11 = triangle1.m_apexes[i];
				l11 = 1;
				segment11 = { triangle1.m_apexes[(i + 1) % 3] ,triangle1.m_apexes[(i + 2) % 3] };
				t12 = kintsec(segment11, triangle2);
				l12 = scalprod(segment11.dir(), triangle2.norm());
				break;
			}

			if ((d1[i] == 0) && (d1[(i + 1) % 3] == d1[(i + 2) % 3]))
			{
				k1 = 1;
				t11 = triangle1.m_apexes[i];
				l11 = 1;
				break;
			}

		}
		for (int i = 0; i < 3; i++)
		{
			if ((d2[i] == -d2[(i + 1) % 3]) && (d2[(i + 1) % 3] == d2[(i + 2) % 3]))
			{
				k2 = 2;
				segment21 = { triangle2.m_apexes[i], triangle2.m_apexes[(i + 1) % 3] };
				segment22 = { triangle2.m_apexes[(i + 2) % 3], triangle2.m_apexes[i] };
				t21 = kintsec(segment21, triangle1);
				t22 = kintsec(segment22, triangle1);
				l21 = scalprod(segment21.dir(), triangle1.norm());
				l22 = scalprod(segment22.dir(), triangle1.norm());
				break;
			}

			if ((d2[i] == 0) && (d2[(i + 1) % 3] == 0))
			{
				k2 = 2;
				t21 = triangle2.m_apexes[i];
				l21 = 1;
				t22 = triangle2.m_apexes[(i + 1) % 3];
				l22 = 1;
				break;
			}

			if ((d2[i] == 0) && (d2[(i + 1) % 3] == -d2[(i + 2) % 3]))
			{
				k2 = 2;
				t21 = triangle2.m_apexes[i];
				l21 = 1;
				segment21 = { triangle2.m_apexes[(i + 1) % 3] ,triangle2.m_apexes[(i + 2) % 3] };
				t22 = kintsec(segment21, triangle1);
				l22 = scalprod(segment21.dir(), triangle1.norm());
				break;
			}

			if ((d2[i] == 0) && (d2[(i + 1) % 3] == d2[(i + 2) % 3]))
			{
				k2 = 1;
				t21 = triangle2.m_apexes[i];
				l21 = 1;
				break;
			}
		}
		if ((k1 == 1) && (k2 == 1))
		{
			return ((t21 * l11) == (t11 * l21));
		}
		else if ((k1 == 1) && (k2 == 2))
		{
			return compare(t11 * l21 * l22, Segment{ t21 * l11 * l22, t22 * l11 * l21 });
			}
		else if ((k1 == 2) && (k2 == 1))
		{
			return compare(t21 * l11 * l12, Segment{ t11 * l12 * l21, t12 * l11 * l21 });
			}
		else if ((k1 == 2) && (k2 == 2))
		{
			return compare(Segment{ t11 * l12 * l21 * l22, t12 * l11 * l21 * l22 }, Segment{ t21 * l11 * l12 * l22, t22 * l11 * l12 * l21 });
		}
	}
}
void final()
{
	int N;
	std::cin >> N;
	std::vector<Triangle> mass(N);
	for (int i = 0; i < N; i++)
	{
		std::cin >> mass[i];
	}
	std::vector<int> res;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (j != i)
			{
				if (compare(mass[i], mass[j]))
				{
					res.push_back(i);
					break;
				}
			}
		}

	}
	for (std::vector<int>::iterator it = res.begin(); it != res.end(); ++it)
		std::cout << ' ' << *(it)+1<<std::endl;
}