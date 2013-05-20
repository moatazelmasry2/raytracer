//=============================================================================
//                                                                            
//  ProPraktikum "Photorealistische Bildgenerierung"
//  
//  Einfaches template für quadratische Matrizen fixer Göße
//  
//=============================================================================

//=============================================================================
//
//  Template CMatrixT
//
//=============================================================================

#ifndef MATRIXT_H
#define MATRIXT_H


//== incLUDES =================================================================

#include "VectorT.h"


//== NAMESPACES ===============================================================



//== CLASS DEFINITION =========================================================

#define FOR_LOOP(expr)      for (int i=0; i<DIM*DIM; ++i) expr(i)

/*! 
	A simple and straightforward template for square fixedsize matrices.

	A square matrix is a 2D-array of \e{DIM*DIM} values of type \e Scalar.
*/
template <typename Scalar, int DIM>
class CMatrixT
{
//---------------------------------------------------------------- typedefs
public:

  //! type of this matrix
  typedef CMatrixT<Scalar,DIM>  MatrixType;

  //! returns dimension of the matrix
  static inline int dim() { return DIM; };
 
 //! returns size of matrix
  static inline int size() { return dim()*dim(); };
 
//---------------------------------------------------------------- data
public:
	Scalar m_values[DIM*DIM];

//----------------------------------------------------------------- Row/Column-Wise 
public:
	static inline unsigned int index(unsigned int uiRow, unsigned int uiCol)
	{
		return uiRow*DIM+uiCol;
	};
	static inline void incIndexRow(unsigned int& ruiIndex)
	{
		ruiIndex+=DIM;
	};
	static inline void incIndexCol(unsigned int& ruiIndex)
	{
		ruiIndex+=1;
	};


//-------------------------------------------------------------- constructors
public:

  //! default constructor creates uninitialized values.
  inline CMatrixT() {};

  //! constructor for uniform initialization
  explicit inline CMatrixT(const Scalar& v) 
  {
#define expr(i) (m_values[i]=v);
    FOR_LOOP(expr);
#undef expr
  };

  //! construct from a value array (explicit)
  explicit inline CMatrixT(const Scalar values[DIM*DIM]) 
  {
	memcpy(m_values, values, DIM*DIM*sizeof(Scalar)); 
  };

  //! copy & cast constructor (explicit)
  template<typename otherScalarType>
  explicit inline CMatrixT(const CMatrixT<otherScalarType,DIM>& other) 
  { 
    operator=(other); 
  };


//--------------------------------------------------------------------- casts

  //! cast from matrix with a different scalar type
  template<typename otherScalarType>
  inline MatrixType& operator=(const CMatrixT<otherScalarType,DIM>& other) 
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

   //! get (row,col)-component read-write
   inline Scalar& operator() (unsigned int uiRow, unsigned int uiCol) 
   {
	  assert(uiRow<DIM && uiCol<DIM); return m_values[index(uiRow, uiCol)]; 
   };

  //! get (row,col)-component read-only
  inline const Scalar& operator() (unsigned int uiRow, unsigned int uiCol) const 
  {
	  assert(uiRow<DIM && uiCol<DIM); return m_values[index(uiRow, uiCol)]; 
  };

  //! set i-th row
  void  setRow(unsigned int uiRow, const CVectorT<Scalar,DIM>& crRow)
  {
	  for (unsigned int i=0, j=index(uiRow,0); i<DIM; ++i, incIndexCol(j) ) m_values[j]=crRow[(size_t)i];
  };
   
  //!get i-th row
  CVectorT<Scalar,DIM> getRow(unsigned int uiRow) const
  {
	  CVectorT<Scalar,DIM> row;
	  for (unsigned int i=0, j=index(uiRow,0); i<DIM; ++i, incIndexCol(j) ) row[(size_t)i] = m_values[j];
	  return row;
  };
  //!set i-th col
  void setCol(unsigned int uiCol, const CVectorT<Scalar,DIM>& crCol)
  {
	  for (unsigned int i=0, j=index(0,uiCol); i<DIM; ++i, incIndexRow(j) ) m_values[j]=crCol[(size_t)i];
  };

//!get i-th col
  CVectorT<Scalar,DIM> getCol(unsigned int uiCol) const
  {
	  CVectorT<Scalar,DIM> col;
	  for (unsigned int i=0, j=index(0,uiCol); i<DIM; ++i, incIndexRow(j) ) col[(size_t)i] = m_values[j];
	  return col;
  };

//---------------------------------------------------------------- comparison

  //! component-wise comparison
  inline bool operator==(const MatrixType& other) const 
  {
#define expr(i) if(m_values[i]!=other.m_values[i]) return false;
    FOR_LOOP(expr);
#undef expr
    return true; 
  };

  //! component-wise comparison
  inline bool operator!=(const MatrixType& other) const 
  {
    return !(*this == other);
  };


//---------------------------------------------------------- scalar operators

  //! component-wise self-multiplication with scalar
  inline MatrixType& operator*=(const Scalar& s) 
  {
#define expr(i) m_values[i] *= s; 
    FOR_LOOP(expr);
#undef expr
    return *this; 
  };

  //! component-wise self-division by scalar
  inline MatrixType& operator/=(const Scalar& s) 
  {
#define expr(i) m_values[i] /= s; 
    FOR_LOOP(expr);
#undef expr
    return *this; 
  };


  //! component-wise multiplication with scalar
  inline MatrixType operator*(const Scalar& s) const 
  {
    return MatrixType(*this) *= s;
  };

  //! component-wise division by with scalar
  inline MatrixType operator/(const Scalar& s) const 
  {
    return MatrixType(*this) /= s;
  };

//---------------------------------------------------------- matrix operators

  //! matrix self-product 
  inline MatrixType& operator*=(const MatrixType& other) 
  {
    (*this) = (*this) * other;
	return (*this);
  };

  //! matrix product 
  inline MatrixType operator*(const MatrixType& other) 
  {
	MatrixType result;
    for (unsigned int ui_row = 0; ui_row < DIM; ++ui_row)
    {
        for (unsigned int ui_col = 0; ui_col < DIM; ++ui_col)
        {
			unsigned int ui_index = index(ui_row, ui_col);
            result.m_values[ui_index] = (Scalar)0.0;
			for (unsigned int ui_mid = 0; ui_mid < DIM; ++ui_mid)
            {
                result.m_values[ui_index] +=
                    this->m_values[index(ui_row,ui_mid)]*other.m_values[index(ui_mid,ui_col)];
            }
        }
    }
    return result;   
  };

  //! vector difference from this
  inline MatrixType& operator-=(const MatrixType& other) 
  {
#define expr(i) m_values[i] -= other.m_values[i]; 
    FOR_LOOP(expr);
#undef expr
    return *this; 
  };

  //! matrix self-addition
  inline MatrixType& operator+=(const MatrixType& other) 
  {
#define expr(i) m_values[i] += other.m_values[i]; 
    FOR_LOOP(expr);
#undef expr
    return *this; 
  };
  

  //! matrix product
  inline MatrixType operator*(const MatrixType& other) const 
  {
    return MatrixType(*this) *= other;
  };

  //! matrix addition
  inline MatrixType operator+(const MatrixType& other) const 
  {
    return MatrixType(*this) += other;
  };

  //! matrix difference
  inline MatrixType operator-(const MatrixType& other) const 
  {
    return MatrixType(*this) -= other;
  };

  //! unary minus
  inline MatrixType operator-(void) const 
  {
    MatrixType result;
#define expr(i) result.m_values[i] = -m_values[i];
    FOR_LOOP(expr);
#undef expr
    return result; 
  };

//------------------------------------------------------------ matrix-vector operators

  //matrix-vector multiplication
  inline CVectorT<Scalar,DIM> operator*(const CVectorT<Scalar,DIM>& vec) const
  {
	  CVectorT<Scalar,DIM> result;
	  for (unsigned int ui_row = 0; ui_row < DIM; ++ui_row)
      {
		 result[(size_t)ui_row] = 0;
		 unsigned int ui_index = index(ui_row, 0);
		 for (unsigned int ui_col = 0; ui_col < DIM; ++ui_col, incIndexCol(ui_index) )
         {
		     result[(size_t)ui_row]+=this->m_values[ui_index] * vec[(size_t)ui_col];
		 }
	  }
	  return result;
  };

//------------------------------------------------------------ norm

  //! compute frobenius norm 
  inline Scalar norm_fr() const { return (Scalar)sqrt(sum_sqr()); };

  //! compute sum of squares
  inline Scalar sum_sqr() const 
  {
    Scalar s(0);
#define expr(i) s += m_values[i] * m_values[i]; 
    FOR_LOOP(expr);
#undef expr
    return s;
  };
  
//------------------------------------------------------------ max, min, mean

  //! return the maximal component
  inline Scalar max() const 
  {
    Scalar m(m_values[0]);
    for(int i=1; i<DIM*DIM; ++i) if(m_values[i]>m) m=m_values[i];
    return m; 
  };
    
  //! return the minimal component
  inline Scalar min() const 
  {
    Scalar m(m_values[0]); 
    for(int i=1; i<DIM*DIM; ++i) if(m_values[i]<m) m=m_values[i];
    return m; 
  };
    
  //! return arithmetic mean
  inline Scalar mean() const 
  {
    Scalar m(m_values[0]); 
    for(int i=1; i<DIM*DIM; ++i) m+=m_values[i];
    return m/Scalar(DIM); 
  };
  
//------------------------------------------------------------ misc functions
  
  //!return transpose
  inline MatrixType transpose() const
  {
	  MatrixType result;
	  for (unsigned int ui_row = 0; ui_row < DIM; ++ui_row)
      {
		 for (unsigned int ui_col = 0; ui_col < DIM; ++ui_col)
         {
			 result.m_values[index(ui_row, ui_col)] = this->m_values[index(ui_col, ui_row)];
		 }
	  }	  
	  return result;
  };

  //!zero all elements
  inline MatrixType& makeZero()
  {
#define expr(i) m_values[i]=0;
    FOR_LOOP(expr);
#undef expr
	return *this;
  };

  //!Make identity
  inline MatrixType& makeIdentity()
  {
    makeZero();
    for(unsigned int ui=0; ui<DIM*DIM; incIndexRow(ui)) m_values[ui++]=1;
	return *this;
  };

  //! component-wise apply function object with Scalar operator()(Scalar).
  template<typename Functor>
  inline MatrixType apply(const Functor& func) const 
  {
    MatrixType result;
#define expr(i) result[i] = func(m_values[i]);
    FOR_LOOP(expr);
#undef expr
    return result; 
  };

};


//== GLOBAL FUNCTIONS =========================================================

//! read the space-separated components of a matrix from a stream
template <typename Scalar, int DIM>
inline std::istream& 
operator>>(std::istream& is, CMatrixT<Scalar,DIM>& m)
{
	CVectorT<Scalar,DIM> row;
	for (int i=0; i<DIM; ++i)
	{
		is >> row;
		m.setRow(i, row );
	}
    return is;
};


//! output a matrix by printing its space-separated compontens
template <typename Scalar, int DIM>
inline std::ostream& 
operator<<(std::ostream& os, const CMatrixT<Scalar,DIM>& m) 
{
	for (unsigned int i=0; i<DIM; ++i)
	{
		os << m.getRow(i) << std::endl;
	}
    return os;
};

//! \relates BCGL::CMatrixT
//! scalar * matrix
template<typename Scalar,int DIM>
inline CMatrixT<Scalar,DIM> operator*(Scalar s, const CMatrixT<Scalar,DIM>& m) 
{
  return CMatrixT<Scalar,DIM>(m) *= s;
};

//! \relates BCGL::CMatrixT
//! vector * matrix
template<typename Scalar,int DIM>
CVectorT<Scalar,DIM> operator*(const CVectorT<Scalar,DIM>& v, 
							   const CMatrixT<Scalar,DIM>& m)
{
	CVectorT<Scalar,DIM> result;
	for (unsigned int ui_col = 0; ui_col < DIM; ++ui_col)
    {
		result[(size_t)ui_col] = 0;
		unsigned int ui_index = CMatrixT<Scalar,DIM>::index(0, ui_col);
		for (unsigned int ui_row = 0; ui_row < DIM; ++ui_row, CMatrixT<Scalar,DIM>::incIndexRow(ui_index) )
        {
		    result[(size_t)ui_col]+=m.m_values[ui_index] * v[(size_t)ui_row];
		}
	}
	return result;
};


#undef FOR_LOOP

//=============================================================================
#endif // MATRIXT_H
//=============================================================================
