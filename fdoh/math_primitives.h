#include <cmath>
#include <chsvlib/chsvexception.h>

#ifndef MATH_PRIMITIVES_H
#define MATH_PRIMITIVES_H

struct point_t
{
	double x = double(), y = double(), z = double();
	point_t() = default;
	constexpr explicit point_t(double X) noexcept: x(X) {}
	constexpr point_t(double X, double Y, double Z = double()) noexcept: x(X), y(Y), z(Z) {}
	constexpr point_t operator-(const point_t& right) const noexcept
	{
		return point_t(this->x - right.x, this->y - right.y, this->z - right.z);
	}
	constexpr point_t operator+(const point_t& right) const noexcept
	{
		return point_t(this->x + right.x, this->y + right.y, this->z + right.z);
	}
	constexpr point_t operator-() const noexcept
	{
		return point_t{-x, -y, -z};
	}
	constexpr point_t operator*(double val) const noexcept
	{
		return point_t{val * x, val * y, val * z};
	}
	friend constexpr point_t operator*(double val, const point_t& vec) noexcept
	{
		return point_t{val * vec.x, val * vec.y, val * vec.z};
	}
	inline point_t& operator*=(double val) noexcept
	{
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}
	constexpr bool operator==(const point_t& right) const
	{
		//using Chusov::Math::abs;
		//double err = FreeIntersectConfig::CalculationError();
		//return abs(x - right.x) <= err && abs(y - right.y) <= err && abs(z - right.z) <= err;
		return x == right.x && y == right.y && z == right.z;
	}
	inline bool operator!=(const point_t& right) const
	{
		return !(*this == right);
	}
	constexpr point_t cross_product(const point_t& right) const noexcept
	{
		return point_t(this->y*right.z - this->z*right.y, this->z*right.x - this->x*right.z, this->x*right.y - this->y*right.x);
	}
	constexpr double dot_product(const point_t& right) const noexcept
	{
		return this->x*right.x + this->y*right.y + this->z*right.z;
	}
	inline double module() const noexcept
	{
		return std::sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
	}
	inline point_t normalize() const noexcept
	{
		double x_sq = x * x, y_sq = y * y, z_sq = z * z;
		auto len_sq = x_sq + y_sq + z_sq;
		return point_t(std::copysign(x_sq, x) / len_sq, std::copysign(y_sq, y) / len_sq, std::copysign(z_sq, z) / len_sq); //forall t \in {x, y, z}: t / sqrt(x^2 + y^2 + z^2) = sign(t) * t^2 / (x^2 + y^2 + z^2)
	}
};

using vector_t = point_t;

class line_t
{
	point_t m_pt;
	vector_t m_dir; //replace with another point
public:
	line_t() = default;
	constexpr line_t(const point_t& pt, const vector_t& dir) noexcept: m_pt(pt), m_dir(dir) {}
	constexpr const point_t& point() const noexcept
	{
		return m_pt;
	}
	constexpr const vector_t& direction() const noexcept
	{
		return m_dir;
	}
	//enum class intersection {exists_at_origin = -1, exists, does_not_exist, infinite, exists_beyond_spacial_limits};
	//intersection intersects(const plain_t& pn, _Out_ point_t* intersection_pt) const noexcept;
	//intersection intersects(const poly_t& pn, _Out_ point_t* intersection_pt) const noexcept;
};

//constexpr bool intersection_point_defined(line_t::intersection intesection_result) noexcept
//{
//	return static_cast<int>(intesection_result) <= 0;
//}

class ray_t
{
	point_t m_origin;
	vector_t m_dir;
public:
	ray_t() = default;
	constexpr ray_t(const point_t& origin, const vector_t& vector) noexcept: m_origin(origin), m_dir(vector) {}
	constexpr const point_t& origin() const noexcept
	{
		return m_origin;
	}
	constexpr const vector_t& direction() const noexcept
	{
		return m_dir;
	}
	explicit constexpr operator line_t() const noexcept
	{
		return line_t{m_origin, m_dir};
	}
	/*using intersection = line_t::intersection;
	intersection intersects(const plain_t& pn, _Out_ point_t* intersection_pt) const noexcept;
	intersection intersects(const poly_t& pn, _Out_ point_t* intersection_pt) const noexcept;*/
};

class segment_t
{
	point_t p1, p2;
public:
	segment_t() = default;
	constexpr segment_t(const point_t& pt1, const point_t& pt2) noexcept: p1(pt1), p2(pt2) {}
	constexpr const point_t& start_point() const noexcept
	{
		return p1;
	}
	constexpr const point_t& end_point() const noexcept
	{
		return p2;
	}
	explicit constexpr operator line_t() const noexcept
	{
		return line_t(p1, p2 - p1);
	}
	/*using intersection = line_t::intersection;
	intersection intersects(const plain_t& pn, _Out_ point_t* intersection_pt) const noexcept;
	intersection intersects(const poly_t& poly, _Out_ point_t* intersection_pt) const noexcept;*/
};

class directed_segment_t:public segment_t
{
public:
	directed_segment_t() = default;
	constexpr directed_segment_t(const point_t& pt1, const point_t& pt2) noexcept: segment_t(pt1, pt2) {}
	explicit constexpr directed_segment_t(const segment_t& segm) noexcept: segment_t(segm) {}
	constexpr vector_t vector() const noexcept
	{
		return this->end_point() - this->start_point();
	}
	explicit constexpr operator ray_t() const noexcept
	{
		return ray_t(this->start_point(), this->vector());
	}
	/*using intersection = segment_t::intersection;
	intersection intersects(const plain_t& pn, _Out_ point_t* intersection_pt) const noexcept;
	intersection intersects(const poly_t& poly, _Out_ point_t* intersection_pt) const noexcept;*/
};

#endif //MATH_PRIMITIVES_H