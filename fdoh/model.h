#include "sbo_vector.h"
#include "math_primitives.h"
#include <memory>

#ifndef MODEL_H_
#define MODEL_H_

class TableRadiationPattern
{
	
};

class Source
{
	//double 
public:
	double field_at(double freq, double azimuth, double zenith) const;
};

class Hgt
{
	std::unique_ptr<unsigned short> m_hgt_data;
	std::size_t m_hgt_rows;
public:
	bool is_visible(const point_t& observer, const point_t& target) const;
};

class InputModel
{
	double m_cx, m_cy, m_cz;
	std::unique_ptr<double> m_field;

};

#endif //MODEL_H_
