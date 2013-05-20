//=============================================================================
//                                                                                        
//  ProPraktikum "Photorealistische Bildgenerierung"
//  
//  Einfaches template für Rasterbilder
//  Wichtige Methoden:
//		- create()
//      - setPixel()
//      - load()/save()  //im PPM/PNM-Format und dies auch nur 
//                       //für 8-Bit/channel Bilder 
//=============================================================================

//=============================================================================
//
//  Template CImageT
//
//=============================================================================

#ifndef IMAGE_T_H
#define IMAGE_T_H

//== INCLUDES =================================================================


#include <stdexcept>
#include <cassert>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

#include "ColorT.h"
#include "camera.h"

#pragma warning (disable:4996)

//== NAMESPACES ===============================================================



//== CLASS DEFINITION =========================================================

//these loops occur often...
#define for_all_pixels(pPixel_)   for (PixelType* pPixel_=(m_pPixels+getNumPixels()-1); pPixel_>=m_pPixels; --pPixel_)

//simple (PRELIMINARY!) check of float-type
#define IS_FLOAT_TYPE(T_) (sizeof(T_) > 1)

/*!
	A simple image template.
	
	Note:
	The class always owns the data. Therefore, all copy operations are deep. 
	For efficient assignment use the swap-operation instead.
*/
template <typename T=unsigned char> 
class CImageT
{
//---------------------------------------------------------------- typedefs
public:
	typedef CImageT<T>  ImageType;
	typedef CColor3T<T> PixelType;

//---------------------------------------------------------------- data
private:
	unsigned m_uiWidth;
	unsigned m_uiHeight;

	CColor3T<T>*  m_pPixels;

//---------------------------------------------------------------- helper
private:
	
	bool equalDims(unsigned uiWidth, unsigned uiHeight)  
	{
		return uiWidth==m_uiWidth && uiHeight==m_uiHeight;
	};

	bool hasExtension(const std::string& sFileName, const std::string& sExtension)
	{
		if ( sFileName.size() < sExtension.size() ) return false;
		std::string s_back = sFileName.substr( sFileName.size()-sExtension.size() );
		for(size_t i=0; i < s_back.length(); ++i) std::toupper(s_back[i]);
        std::string tmp_ext(sExtension);
        for(size_t i=0; i < tmp_ext.length(); ++i) std::toupper(tmp_ext[i]);
		return ( s_back == tmp_ext );
	};

//-------------------------------------------------------------- constructors
public:
	//!default constructor - creates empty image
	CImageT(): m_uiWidth(0),
			   m_uiHeight(0),
			   m_pPixels(0)

	{};

	//!standard constructor
	CImageT(unsigned uiWidth, unsigned uiHeight):
																m_uiWidth(uiWidth),
																m_uiHeight(uiHeight),
																m_pPixels(0)
	{
		unsigned ui_num_pixels = getNumPixels();
		if (ui_num_pixels) m_pPixels = new PixelType[ui_num_pixels]; 
	};

	//!constructor which assigns pointer (use with care!!)
	CImageT( PixelType*  pPixels, unsigned uiWidth, unsigned uiHeight ):
																m_uiWidth(uiWidth),
																m_uiHeight(uiHeight),
																m_pPixels(pPixels)

	{
	};


	//!constructor for load from file
	CImageT(const char* sFileName) :  m_uiWidth(0),
								   m_uiHeight(0),
								   m_pPixels(0)
	{ 
		load( sFileName );
	};
	

	//!copy constructor with type conversion
	template<typename otherT> 
	CImageT(const CImageT<otherT>& other): m_uiWidth(other.getWidth()),
										 m_uiHeight(other.getHeight()),
										 m_pPixels(0)
	{
		unsigned ui_num_pixels = getNumPixels();
		if (ui_num_pixels) 
		{
			m_pPixels = new PixelType[ui_num_pixels];
			const typename CImageT<otherT>::PixelType* p_other_pixels = other.getPixelPtr() + ui_num_pixels;
			for_all_pixels(p_pixel) { (*p_pixel)=(*(--p_other_pixels)); };
		} 
    };

	//!copy constructor for same type
    CImageT(const ImageType& other): m_uiWidth(other.getWidth()),
										 m_uiHeight(other.getHeight()),
										 m_pPixels(0)
	{
		unsigned ui_num_pixels = getNumPixels();
		if (ui_num_pixels) 
		{
			m_pPixels = new PixelType[ui_num_pixels];
			memcpy(m_pPixels, other.m_pPixels, ui_num_pixels*sizeof(PixelType));
		} 
    };
	
	//!Destructor
	~CImageT()
	{
		if ( m_pPixels ) delete[] m_pPixels;	
	};

//----------------------------------------------------------- element access

	unsigned getWidth() const { return m_uiWidth;};
	unsigned getHeight() const {  return m_uiHeight; };
	
	unsigned getNumPixels() const { return m_uiWidth*m_uiHeight; }; 
	
	CColor3T<T>* getPixelPtr() const { return m_pPixels; };

	bool isEmpty() const { return 0 == m_pPixels; };

//----------------------------------------------------------- operators

    //! image copy with type conversion
	template<typename otherT> 
    ImageType& operator=(const CImageT<otherT>& other)
	{ 
		if ( !equalDims(other.getWidth(), other.getHeight()) ) 
		{
			this->create( other.getWidth(), other.getHeight() );
		}
		const typename CImageT<otherT>::PixelType* p_other_pixel = other.getPixelPtr() + this->getNumPixel();
		for_all_pixels(pPixel) { (*pPixel)=(*(--p_other_pixel)); };
		return *this;
    };

	//! image copy with same type
    ImageType& operator=(const ImageType& other)
	{
		if (&other==this) return *this;
		if ( !equalDims(other.getWidth(), other.getHeight()) ) 
		{
			this->create( other.getWidth(), other.getHeight() );
		}
      	memcpy(m_pPixels, other.m_pPixels, sizeof(PixelType)*this->getNumPixel());
		return *this;
    };

	//!access to pixel (non-const)
    PixelType* operator()(unsigned x, unsigned y) 
	{
		unsigned ui_index = m_uiWidth*y+x;
		assert( ui_index < this->getNumPixels() );
		return m_pPixels+ui_index;
    };
	
	//!access to pixel (non-const)
    PixelType* operator()(unsigned x, unsigned y) const
	{
		unsigned ui_index = m_uiWidth*y+x;
		assert( ui_index < this->getNumPixels() );
		return (m_pPixels+ui_index);
    };

	//!multiply
	ImageType& operator*=(float f)
	{
		for_all_pixels(pPixel) { (*pPixel) *= f; };
		return (*this);
	};
	
	//!add images
	ImageType& operator+=( const ImageType& other )
	{
		assert ( equalDims(other.getWidth(), other.getHeight()) ); 

		const PixelType* p_other_pixel = other.m_pPixels + this->getNumPixel();
		for_all_pixels(pPixel) 
		{ 
			(*pPixel)+=(*(--p_other_pixel));
		}
		return (*this);
	};


//----------------------------------------------------------- general access


	//!set pixel at (x,y) in a whole
	void setPixel( unsigned x, unsigned y, const PixelType& pixel )
	{		
		(*this)(x, y)[0] = pixel;
	};
	

//----------------------------------------------------------- helper

	template<typename CooT>
	bool inBound( const CooT& x, const CooT& y ) const
	{
		return (x>=0) && (x<m_uiWidth) && (y>=0) && (y<m_uiHeight);
	};

//----------------------------------------------------------- operations

	//!create image of given dimensions
	void create( unsigned uiWidth, unsigned uiHeight )
	{
		if ( m_pPixels ) delete[] m_pPixels;	
		m_pPixels  = 0;
		m_uiWidth = uiWidth;
		m_uiHeight= uiHeight;
		unsigned ui_num_pixels = getNumPixels();
		if ( ui_num_pixels ) m_pPixels = new PixelType[ui_num_pixels]; 
	};

	//!destroy the image
	void destroy()
	{
		create(0,0,0);
	};

	//! exchange ptrs between images (of same type, of course)
	void swap( ImageType& other )
	{
		std::swap( this->m_uiWidth, other.m_uiWidth );
		std::swap( this->m_uiHeight, other.m_uiHeight );
		std::swap( this->m_pPixels, other.m_pPixels );
	};

	//! zero out
	void setZero()
	{
		memset( getPixelPtr(), 0, getNumPixels() * sizeof(PixelType) );
	};

	void setValue( const PixelType& val )
	{
		for_all_pixels(pPixel) 
		{
			*pPixel = val;
		}
	};

	void clamp( const T low, const T high )
	{
		for_all_pixels( pPixel ) pPixel->clamp( low, high );
	};

//----------------------------------------------------------- I/O

	//!load from file (determines format from extension)
	void load(const char* sFileName, int iStartLine=-1, int iEndLine=-1)
	{
		if ( hasExtension( sFileName, "ppm" ) )
		{
			ImageType( loadPPM( sFileName ) ).swap( *this );
			if ( IS_FLOAT_TYPE( T ) ) this->operator *= ( 1/255.f );
			return;
		}
		throw std::invalid_argument( "CImageT: format not recognized" );
	};

	//! load file from ppm format
	static CImageT<unsigned char> loadPPM(const char* sFileName) 
	{ 
		std::ifstream fin( sFileName, std::ios_base::binary );		
		if ( !fin.is_open() ) 
		{
			throw std::invalid_argument( "CImageT: file open error" );
		}

		char ch_line[255];
		char ch_data[255];
		char* pch_next;
		int i_read;
		unsigned ui_width;
		unsigned ui_height;
		//read the header part by part
		int i_part = 0;
		while ( 1 )
		{
			fin.getline ( ch_line, sizeof ( ch_line ) );
	
			if ( fin.eof() ) return false;

			pch_next = ch_line;
			//comment
			if ( pch_next[0] == '#' ) continue;
		
			//magic number
			if ( i_part == 0 )
			{
				if ( EOF == sscanf ( pch_next, "%255s%n", ch_data, &i_read ) ) continue;
				pch_next += i_read;
				if ( (ch_data[0]!='P') || (ch_data[1]!='6') ) return false;
				i_part = 1;
			}
			//width
			if ( i_part == 1 )
			{
				if ( EOF == sscanf ( pch_next, "%d%n", &ui_width, &i_read) ) continue;
				pch_next += i_read;
				i_part = 2;
			}
			//height
			if ( i_part == 2 )
			{
				if ( EOF == sscanf ( pch_next, "%d%n", &ui_height, &i_read) ) continue;
				pch_next += i_read;
				i_part = 3;
			}
			//bitdepth
			if ( i_part == 3 )
			{
				int i_max;
				if ( EOF == sscanf ( pch_next, "%d%n", &i_max, &i_read) ) continue;
				if ( i_max != 255 ) 
				{
					throw std::runtime_error( "CImageT::loadPPM: bitdepth not supported" );
				}
				pch_next += i_read;
				//ready!
				break;
			}
		}
		CColor3T<char>* p_data = new CColor3T<char>[ui_width*ui_height];
	
		//read data
		unsigned x, y;
		unsigned index = 0;
		for ( y = 0; y<ui_height; ++y )
		{	
			for ( x = 0; x <ui_width; ++x )
			{
				char r, g, b;
				fin.get(r);
				fin.get(g);
				fin.get(b);
				p_data[index++].setRgb( r, g, b );
			}
		}
		//CImageT<unsigned char> image(p_data, ui_width, ui_height );
		CImageT<unsigned char> image;
		return 	image;
	};

	void writeHeaderPNM( std::ofstream& rostrFile )
	{
		rostrFile << "P6" << std::endl;
		char ac_output[256];
		sprintf( ac_output, "%d", m_uiWidth );
		rostrFile << ac_output << " ";
		sprintf( ac_output, "%d", m_uiHeight );
		rostrFile << ac_output << std::endl;
		rostrFile << 255 << std::endl;

		if ( !rostrFile.good() )
			throw std::runtime_error( "writeHeaderPNM: error writing data" );
	}

	void writeDataPNM( std::ofstream& rostrFile )
	{
		unsigned int ui_size = m_uiWidth * m_uiHeight * 3;

		if ( ui_size != 0 )
			rostrFile.write( (const char*)getPixelPtr(), sizeof(unsigned char)*ui_size );

		if ( !rostrFile.good() )
			throw std::runtime_error( "writeDataPNM: error writing data" );
	}

	void savePNM( const char* cpcFileName )
	{
		if ( sizeof( T ) != sizeof( unsigned char ) )
		{
			throw std::runtime_error( "savePNM: only images with 8 bit per-channel supported" );
		}
		
		std::ofstream ostr_file;
		ostr_file.open( cpcFileName, std::ofstream::out | std::ofstream::binary );
		if ( !ostr_file.good() )
			throw std::runtime_error( "SaveFile: error opening file" );

		writeHeaderPNM( ostr_file );
		writeDataPNM( ostr_file );

		ostr_file.close();
	}

	//!save to file (determines format from extension)
	void save( const char* sFileName )
	{
		if ( hasExtension( sFileName, "ppm" ) || hasExtension( sFileName, "pgm" ) )
		{
			savePNM( sFileName );
			return;
		}
		throw std::runtime_error( "CImageT: format not supported" );
	};

};

#undef for_all_pixels
#undef for_all_pixels

//== TYPEDEFS =================================================================


typedef CImageT<unsigned char> ImageTypeByte;
typedef CImageT<float> ImageTypeFloat;

//=============================================================================
#endif //IMAGET_H
//=============================================================================
