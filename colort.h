//=============================================================================
//                                                                            
//  ProPraktikum "Photorealistische Bildgenerierung"
//  
//  Einfaches template für rgb-Farbe
//  
//=============================================================================

//=============================================================================
//
//  Template CColor3T
//
//=============================================================================

#ifndef COLOR3T_H
#define COLOR3T_H

#include <ostream>
using namespace std;
//== INCLUDES =================================================================


//== NAMESPACES ===============================================================


//== CLASS DEFINITION =========================================================

/*!
	A simple template for storing RGB-Colors.
*/
template <typename Scalar>
class CColor3T
{
//-------------------------------------------------------------- typedefs
public:
	//!Pack 3-component-color into integer
	typedef unsigned int Packed;
	//!abbreviation	
	typedef CColor3T<Scalar>  ColorType;

//-------------------------------------------------------------- private data
public:	
	Scalar m_rgb[3];

//-------------------------------------------------------------- private methods
private:

public:
//-------------------------------------------------------------- constructors
	//! standard constructor
	inline CColor3T() {};
	//! copy constructor
	template<typename otherScalarType>
	explicit inline CColor3T(const CColor3T<otherScalarType>& other) 
	{ 
		operator=(other); 
	};    
	//!initializing constructor version 0
    inline CColor3T( const Scalar red, 
					 const Scalar green, 
			         const Scalar blue )
	{
		m_rgb[0] = red; m_rgb[1] = green; m_rgb[2] = blue;
	};

	//!initializing constructor version 1
    inline CColor3T( const Scalar gray )
	{
		m_rgb[0] = m_rgb[1] = m_rgb[2] = gray;
	};


	//! destructor	
	~CColor3T() {};

//--------------------------------------------------------------------- casts

    //! cast from color with a different scalar type
	template<typename otherScalarType>
	inline ColorType& operator=(const CColor3T<otherScalarType>& other) 
	{
		m_rgb[0] = (Scalar)other.red();
		m_rgb[1] = (Scalar)other.green();
		m_rgb[2] = (Scalar)other.blue();
	    return *this; 
	};

	//! cast to Scalar array
	inline operator Scalar*() { return m_rgb; };

	//! cast to const Scalar array
	inline operator const Scalar*() const { return m_rgb; };

//----------------------------------------------------------- element access
	//!Set RGB-components
	inline void setRgb( const Scalar red, 
		                const Scalar green, 
				        const Scalar blue )
	{
		m_rgb[0] = red; m_rgb[1] = green; m_rgb[2] = blue;
	};

	inline void setRgb( const Scalar* p )
	{
		m_rgb[0] = p[0];
		m_rgb[1] = p[1];
		m_rgb[2] = p[2];
	};

	//!Get red component
	inline Scalar red()   const { return m_rgb[0]; };
	//!Get green component
	inline Scalar green() const { return m_rgb[1]; };
	//!Get blue component
	inline Scalar blue()  const { return m_rgb[2]; };

	//!Get graylevel value
	inline Scalar gray() const { return Scalar( (red()*11+green()*16+blue()*5)/32. ); };
	inline Scalar lum() const { return gray(); };

//----------------------------------------------------------- operators	
	//!color equals other
    inline bool  operator==(const ColorType& other ) const
	{
		return m_rgb[0]==other.m_rgb[0] && m_rgb[1]==other.m_rgb[1] && m_rgb[2]==other.m_rgb[2];
	};
	//!color not equals other
    inline bool    operator!=(const ColorType& other ) const
	{
		return !( (*this) == other );
	};

	//!add color to self
	inline ColorType& operator+=(const ColorType& other) 
	{
		m_rgb[0] += other.m_rgb[0];
		m_rgb[1] += other.m_rgb[1];
		m_rgb[2] += other.m_rgb[2];
		return (*this);
	};

	//!sub color from self
	inline ColorType& operator-=(const ColorType& other) 
	{
		m_rgb[0] -= other.m_rgb[0];
		m_rgb[1] -= other.m_rgb[1];
		m_rgb[2] -= other.m_rgb[2];
		return (*this);
	};

	//!self-multiply with color
	inline ColorType& operator*=(const ColorType& other) 
	{
		m_rgb[0] *= other.m_rgb[0];
		m_rgb[1] *= other.m_rgb[1];
		m_rgb[2] *= other.m_rgb[2];
		return (*this);
	};

	//!self-divide with color
	inline ColorType& operator/=(const ColorType& other) 
	{
		m_rgb[0] /= other.m_rgb[0];
		m_rgb[1] /= other.m_rgb[1];
		m_rgb[2] /= other.m_rgb[2];
		return (*this);
	};

	//!self-multiply with scalar
	inline ColorType& operator*=(const Scalar val)
	{
	    m_rgb[0] *= val;
	    m_rgb[1] *= val;
	    m_rgb[2] *= val;
		return *this;
	};

	//!self-divide with scalar
	inline ColorType& operator/=(const Scalar val)
	{
		return operator*=(Scalar(1.)/val);
	};

	//!add colors
	inline ColorType operator+(const ColorType& other) const
	{
		return ColorType(*this)+=other;
	};

	//!sub colors
	inline ColorType operator-(const ColorType& other) const
	{
		return ColorType(*this)-=other;
	};

	//!multiply colors
	inline ColorType operator*(const ColorType& other) const
	{
		return ColorType(*this)*=other;
	};

	//!Multiply with scalar
    inline ColorType operator*( const Scalar val ) const
	{
		return ColorType(*this) *= val ;
	};

	//!Divide by scalar
    inline ColorType operator/( const Scalar val ) const
	{
		Scalar val_inv = Scalar(1.)/val;
		return ColorType(*this) *= val_inv;
	};

//----------------------------------------------------------- functionality
	//!Clamp to range
	inline ColorType& clamp( Scalar lb, Scalar hb )
	{
		if (m_rgb[0]<lb) m_rgb[0] = lb;
		if (m_rgb[1]<lb) m_rgb[1] = lb;
		if (m_rgb[2]<lb) m_rgb[2] = lb;
		if (m_rgb[0]>hb) m_rgb[0] = hb;
		if (m_rgb[1]>hb) m_rgb[1] = hb;
		if (m_rgb[2]>hb) m_rgb[2] = hb;
		return (*this);
	};

	//!Get energy
	inline Scalar energy() const
	{
		return m_rgb[0]*m_rgb[0]+m_rgb[1]*m_rgb[1]+m_rgb[2]*m_rgb[2];
	};

	//!Apply gamma correction
	inline const ColorType& gamma( Scalar val_gamma )
	{
		m_rgb[0] = pow(m_rgb[0], val_gamma );
		m_rgb[1] = pow(m_rgb[1], val_gamma );
		m_rgb[2] = pow(m_rgb[2], val_gamma );
		return (*this);
	};

	//!Apply gamma correction
	inline const ColorType& gammaInv( Scalar val_gamma )
	{
		m_rgb[0] = log(m_rgb[0])/log(val_gamma);
		m_rgb[1] = log(m_rgb[1])/log(val_gamma);
		m_rgb[2] = log(m_rgb[2])/log(val_gamma);
		return (*this);
	};

	//!Get maximum component value
	inline Scalar max() const
	{
		return std::max( m_rgb[0], std::max( m_rgb[1], m_rgb[2] ));
	};

	//!Get absolute maximum component value
	inline Scalar absMax() const 
	{
		return std::max( fabs(m_rgb[0]), std::max( fabs(m_rgb[1]), fabs(m_rgb[2]) ));
	};

	//!Get minimum component value
	inline Scalar min() const
	{
		return std::min( m_rgb[0], std::min( m_rgb[1], m_rgb[2] ));
	};

	inline bool isZero() const
	{
		return m_rgb[0] == 0 && m_rgb[1] == 0 && m_rgb[2] == 0;
	};

	ColorType RGB2YIQ() const
	{
		return ColorType( .299f * m_rgb[0] + .587f * m_rgb[1] + .114f * m_rgb[2],
			              .596f * m_rgb[0] - .274f * m_rgb[1] - .322f * m_rgb[2],
						  .211f * m_rgb[0] - .522f * m_rgb[1] + .311f * m_rgb[2] );
	};

	ColorType YIQ2RGB() const
	{
		return ColorType( 1 * m_rgb[0] + .956f * m_rgb[1] + .623f * m_rgb[2],
			              1 * m_rgb[0] - .272f * m_rgb[1] - .648f * m_rgb[2],
						  1 * m_rgb[0] -1.105f * m_rgb[1] +1.705f * m_rgb[2] );
	};
	
};


//! output a color by printing its space-separated compontens
template <typename Scalar>
inline std::ostream& 
operator<<(std::ostream& os, const CColor3T<Scalar>& col) 
{
	os << col.red() << " " << col.green() << " " << col.blue();
    return os;
};

//== TYPEDEFS =================================================================

/*! float color */
typedef CColor3T<float> Color3f;

/*! double color */
typedef CColor3T<double> Color3d;


//=============================================================================
#endif // COLOR3T_H
//=============================================================================
