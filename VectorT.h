//=============================================================================
//                                                                            
//  ProPraktikum "Photorealistische Bildgenerierung"
//  
//  Einfaches template für vektoren fixer Göße
//  
//=============================================================================

//=============================================================================
//
//  Template CVectorT
//
//=============================================================================

#ifndef VECTORT_H
#define VECTORT_H

//== INCLUDES =================================================================

#include <cassert>
#include <iostream>
#include <cmath>

using namespace std;

//== NAMESPACES ===============================================================



//== CLASS DEFINITION =========================================================

#define FOR_LOOP(expr)      for (int i=0; i<DIM; ++i) expr(i)

/*! 
	A typical template for fixedsize vectors.

	A vector is a fixed 1D-array of \e DIM values of type \e Scalar.
*/
template <typename Scalar, int DIM>
class CVectorT {

//---------------------------------------------------------------- typedefs
public:
  //! scalar base type
  typedef Scalar ScalarType;

  //! returns dimension of the vector
  static inline int dim() { return DIM; };
 
 //! returns size of vector
  static inline int size() { return dim(); };
 
//---------------------------------------------------------------- data
public:
	Scalar m_values[DIM];


//-------------------------------------------------------------- constructors
public:

  //! default constructor creates uninitialized values.
  inline CVectorT() {};

  //! special constructor for 2D vectors
  inline CVectorT(const Scalar& v0, const Scalar& v1) 
  {
    assert(DIM==2);
    m_values[0] = v0; m_values[1] = v1; 
  };

  //! special constructor for 3D vectors
  inline CVectorT(const Scalar& v0, const Scalar& v1, const Scalar& v2) 
  {
    assert(DIM==3);
    m_values[0]=v0; m_values[1]=v1; m_values[2]=v2; 
  };

  //! special constructor for 4D vectors
  inline CVectorT(const Scalar& v0, const Scalar& v1, 
		 const Scalar& v2, const Scalar& v3)
  {
    assert(DIM==4); 
    m_values[0]=v0; m_values[1]=v1; m_values[2]=v2; m_values[3]=v3; 
  };

  //! constructor for uniform initialization
  explicit inline CVectorT(const Scalar& v) 
  {
#define expr(i) (m_values[i]=v);
    FOR_LOOP(expr);
#undef expr
  };

  //! construct from a value array (explicit)
  explicit inline CVectorT(const Scalar values[DIM]) 
  {
	memcpy(m_values, values, DIM*sizeof(Scalar)); 
  };

  //! copy & cast constructor (explicit)
  template<typename otherScalarType>
  explicit inline CVectorT(const CVectorT<otherScalarType,DIM>& other) 
  { 
    operator=(other); 
  };

//--------------------------------------------------------------------- casts

  //! cast from matrix with a different scalar type
  template<typename otherScalarType>
  inline CVectorT<Scalar,DIM>& operator=(const CVectorT<otherScalarType,DIM>& other) 
  {
#define expr(i)  m_values[i] = (Scalar)other.m_values[i];
    FOR_LOOP(expr);
#undef expr
    return *this; 
  };

  //! cast to Scalar array
  inline operator Scalar*() { return m_values; };

  //! cast to const Scalar array
  inline operator const Scalar*() const { return m_values; };


//----------------------------------------------------------- element access

   //! get i'th element read-write
  inline Scalar& operator[](int i) 
  {
    assert(i>=0 && i<DIM); return m_values[i]; 
  };

  //! get i'th element read-only
  inline const Scalar& operator[](int i) const 
  {
    assert(i>=0 && i<DIM); return m_values[i]; 
  };

   //! get i'th element read-write
  inline Scalar& operator[](size_t i) 
  {
    assert(i<DIM); return m_values[i]; 
  };

  //! get i'th element read-only
  inline const Scalar& operator[](size_t i) const 
  {
    assert(i<DIM); return m_values[i]; 
  };

//---------------------------------------------------------------- comparison

  //! component-wise comparison
  inline bool operator==(const CVectorT<Scalar,DIM>& other) const 
  {
#define expr(i) if(m_values[i]!=other.m_values[i]) return false;
    FOR_LOOP(expr);
#undef expr
    return true; 
  };

  //! component-wise comparison
  inline bool operator!=(const CVectorT<Scalar,DIM>& other) const 
  {
    return !(*this == other);
  };


//---------------------------------------------------------- scalar operators

  //! component-wise self-multiplication with scalar
  inline CVectorT<Scalar,DIM>& operator*=(const Scalar& s) 
  {
#define expr(i) m_values[i] *= s; 
    FOR_LOOP(expr);
#undef expr
    return *this; 
  };

  //! component-wise self-division by scalar
  inline CVectorT<Scalar,DIM>& operator/=(const Scalar& s) 
  {
    Scalar inv = Scalar(1)/s;
#define expr(i) m_values[i] *= inv; 
    FOR_LOOP(expr);
#undef expr
    return *this; 
  };


  //! component-wise multiplication with scalar
  inline CVectorT<Scalar,DIM> operator*(const Scalar& s) const 
  {
    return CVectorT<Scalar,DIM>(*this) *= s;
  };


  //! component-wise division by scalar
  inline CVectorT<Scalar,DIM> operator/(const Scalar& s) const 
  {
    return CVectorT<Scalar,DIM>(*this) /= s;
  };

//---------------------------------------------------------- vector operators

  //! component-wise self-multiplication 
  inline CVectorT<Scalar,DIM>& operator*=(const CVectorT<Scalar,DIM>& other) 
  {
#define expr(i) m_values[i] *= other[i]; 
    FOR_LOOP(expr);
#undef expr
    return *this; 
  };

  //! component-wise self-division
  inline CVectorT<Scalar,DIM>& operator/=(const CVectorT<Scalar,DIM>& other) 
  {
#define expr(i) m_values[i] /= other[i]; 
    FOR_LOOP(expr);
#undef expr
    return *this; 
  };

  //! vector difference from this
  inline CVectorT<Scalar,DIM>& operator-=(const CVectorT<Scalar,DIM>& other) 
  {
#define expr(i) m_values[i] -= other[i]; 
    FOR_LOOP(expr);
#undef expr
    return *this; 
  };

  //! vector self-addition
  inline CVectorT<Scalar,DIM>& operator+=(const CVectorT<Scalar,DIM>& other) 
  {
#define expr(i) m_values[i] += other[i]; 
    FOR_LOOP(expr);
#undef expr
    return *this; 
  };
  

  //! component-wise vector multiplication
  inline CVectorT<Scalar,DIM> operator*(const CVectorT<Scalar,DIM>& _v) const 
  {
    return CVectorT<Scalar,DIM>(*this) *= _v;
  };

  //! component-wise vector division
  inline CVectorT<Scalar,DIM> operator/(const CVectorT<Scalar,DIM>& _v) const 
  {
    return CVectorT<Scalar,DIM>(*this) /= _v;
  };


  //! component-wise vector addition
  inline CVectorT<Scalar,DIM> operator+(const CVectorT<Scalar,DIM>& _v) const 
  {
    return CVectorT<Scalar,DIM>(*this) += _v;
  };

  //! component-wise vector addition
  inline CVectorT<Scalar,DIM> operator+(const Scalar &_v) const 
  {
	  CVectorT<Scalar,DIM> temp = (*this);
	  for (int i=0; i< DIM; i++) {
		  temp.m_values[i] += _v;
	  }
	  return (*temp);
  };


  //! component-wise vector difference
  inline CVectorT<Scalar,DIM> operator-(const CVectorT<Scalar,DIM>& _v) const 
  {
    return CVectorT<Scalar,DIM>(*this) -= _v;
  };

  //! unary minus
  inline CVectorT<Scalar,DIM> operator-(void) const 
  {
    CVectorT<Scalar,DIM> v;
#define expr(i) v.m_values[i] = -m_values[i];
    FOR_LOOP(expr);
#undef expr
    return v; 
  };

  //! cross product: only defined for Vec3* as specialization
  inline CVectorT<Scalar,3> operator%(const CVectorT<Scalar,3>& other) const 
  {
    return 
      CVectorT<Scalar,3>(m_values[1]*other.m_values[2]-m_values[2]*other.m_values[1],
			m_values[2]*other.m_values[0]-m_values[0]*other.m_values[2],
			m_values[0]*other.m_values[1]-m_values[1]*other.m_values[0]);
  };

  //!compute scalar product
  inline Scalar operator|(const CVectorT<Scalar,DIM>& other) const 
  {
  Scalar p(0);
#define expr(i) p += m_values[i] * other.m_values[i]; 
  FOR_LOOP(expr);
#undef expr
    return p; 
  };

//------------------------------------------------------------ euclidean norm

  //! \name Euclidean norm calculations
  //@{
  //! compute euclidean norm 
  inline Scalar norm() const { return (Scalar)sqrt(sum_sqr()); }

  //! compute squared euclidean norm
  inline Scalar sum_sqr() const 
  {
    Scalar s(0);
#define expr(i) s += m_values[i] * m_values[i]; 
    FOR_LOOP(expr);
#undef expr
    return s;
  };
  //@}

  //! normalize vector, return normalized vector
  inline CVectorT<Scalar,DIM>& normalize() 
  {
#ifdef NDEBUG
    operator*=(((Scalar)1.0)/norm());
#else
    Scalar n = norm();
    if (n != (Scalar)0.0)
      *this *= Scalar(1.0/n);
#endif
    return *this;
  };

//------------------------------------------------------------ max, min, mean

  //! return the maximal component
  inline Scalar max() const 
  {
    Scalar m(m_values[0]);
    for(int i=1; i<DIM; ++i) if(m_values[i]>m) m=m_values[i];
    return m; 
  };
    
  /// return the minimal component
  inline Scalar min() const 
  {
    Scalar m(m_values[0]); 
    for(int i=1; i<DIM; ++i) if(m_values[i]<m) m=m_values[i];
    return m; 
  };
  
  //! return index of maximal component
  inline int argMax() const 
  {
    Scalar m(m_values[0]);
	int result = 0;
	for(int i=1; i<DIM; ++i) if(m_values[i]>m) { m=m_values[i]; result=i; };
    return result; 
  };

  //! return index of minimal component
  inline int argMin() const 
  {
    Scalar m(m_values[0]);
	int result = 0;
	for(int i=1; i<DIM; ++i) if(m_values[i]<m) { m=m_values[i]; result=i; };
    return result; 
  };

  //! return arithmetic mean
  inline Scalar mean() const 
  {
    Scalar m(m_values[0]); 
    for(int i=1; i<DIM; ++i) m+=m_values[i];
    return m/Scalar(DIM); 
  };

  //------------------------------------------------------------ misc functions

  CVectorT<Scalar,DIM>& abs()
  {
#define expr(i) m_values[i] = fabs(m_values[i]);
	  FOR_LOOP(expr);
#undef expr
	  return *this;	
  };

};


//== GLOBAL FUNCTIONS =========================================================

  //! component-wise multiplication with scalar
template<typename Scalar, int DIM>
inline CVectorT<Scalar,DIM> operator*(Scalar s, const CVectorT<Scalar,DIM>& v) 
{
	return CVectorT<Scalar,DIM>(v) *= s;
};

//!Normalization
template<typename Scalar, int DIM>
inline CVectorT<Scalar,DIM> Normalize(const CVectorT<Scalar,DIM>& v) 
{
	CVectorT<Scalar, DIM> vec( v );
	return vec.normalize();
};

//!Absolute values
template<typename Scalar, int DIM>
inline CVectorT<Scalar,DIM> Abs(const CVectorT<Scalar,DIM>& v) 
{
	CVectorT<Scalar,DIM> vec(v);
	return vec.abs();
};


//! read the space-separated components of a vector from a stream
template <typename Scalar,int DIM>
inline std::istream& 
operator>>(std::istream& is, CVectorT<Scalar,DIM>& vec)
{
#define expr(i) is >> std::ws >> vec[i];
  FOR_LOOP(expr);
#undef expr
  return is;
};


//! output a vector by printing its space-separated compontens
template <typename Scalar,int DIM>
inline std::ostream& 
operator<<(std::ostream& os, const CVectorT<Scalar,DIM>& vec) 
{
  for(int i=0; i<DIM-1; ++i) os << vec[i] << " ";
  os << vec[DIM-1];
  return os;
};



#undef FOR_LOOP

//== TYPEDEFS =================================================================

typedef CVectorT<float,2> Vec2f;
typedef CVectorT<double,2> Vec2d;

typedef CVectorT<float,3> Vec3f;
typedef CVectorT<double,3> Vec3d;


//=============================================================================
#endif // VECTORT_H
//=============================================================================




