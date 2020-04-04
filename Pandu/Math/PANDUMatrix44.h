/********************************************************************
	filename: 	PANDUMatrix44
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUMatrix44_h__
#define __PANDUMatrix44_h__

#include "PANDUVector3.h"
#include <assert.h>

namespace Pandu
{
	class Quaternion;
    class Matrix44
    {
	public:

        union 
		{
            float m[4][4];
            float arr[16];
        };
      
        inline Matrix44()
        {
        }

		inline Matrix44(const Matrix44& _mat)
		{
			Set(_mat[0][0], _mat[0][1], _mat[0][2], _mat[0][3],
				_mat[1][0], _mat[1][1], _mat[1][2], _mat[1][3],
				_mat[2][0], _mat[2][1], _mat[2][2], _mat[2][3],
				_mat[3][0], _mat[3][1], _mat[3][2], _mat[3][3]);
		}

        inline Matrix44(float _m00, float _m01, float _m02, float _m03,
						float _m10, float _m11, float _m12, float _m13,
						float _m20, float _m21, float _m22, float _m23,
						float _m30, float _m31, float _m32, float _m33 )
        {
            Set(_m00, _m01, _m02, _m03,_m10, _m11, _m12, _m13,_m20, _m21, _m22, _m23,_m30, _m31, _m32, _m33);
        } 

		inline void Set(float _m00, float _m01, float _m02, float _m03,
						float _m10, float _m11, float _m12, float _m13,
						float _m20, float _m21, float _m22, float _m23,
						float _m30, float _m31, float _m32, float _m33 )
		{
			m[0][0] = _m00;			m[0][1] = _m01;			m[0][2] = _m02;			m[0][3] = _m03;
            m[1][0] = _m10;			m[1][1] = _m11;			m[1][2] = _m12;			m[1][3] = _m13;
            m[2][0] = _m20;			m[2][1] = _m21;			m[2][2] = _m22;			m[2][3] = _m23;
            m[3][0] = _m30;			m[3][1] = _m31;			m[3][2] = _m32;			m[3][3] = _m33;
		}

		inline Matrix44& operator = ( const Matrix44& _mat ) 
		{
			Set(_mat[0][0], _mat[0][1], _mat[0][2], _mat[0][3],
				_mat[1][0], _mat[1][1], _mat[1][2], _mat[1][3],
				_mat[2][0], _mat[2][1], _mat[2][2], _mat[2][3],
				_mat[3][0], _mat[3][1], _mat[3][2], _mat[3][3]);
			return *this;
		}

		inline float* operator [] ( unsigned short _row )
        {
            assert( _row < 4 );
            return m[_row];
        }

        inline const float *operator [] ( unsigned short _row ) const
        {
            assert( _row < 4 );
            return m[_row];
        }


        inline Matrix44 operator * ( const Matrix44& _right ) const
        {
            Matrix44 result;

            result.m[0][0] = m[0][0] * _right.m[0][0] + m[0][1] * _right.m[1][0] + m[0][2] * _right.m[2][0] + m[0][3] * _right.m[3][0];
            result.m[0][1] = m[0][0] * _right.m[0][1] + m[0][1] * _right.m[1][1] + m[0][2] * _right.m[2][1] + m[0][3] * _right.m[3][1];
            result.m[0][2] = m[0][0] * _right.m[0][2] + m[0][1] * _right.m[1][2] + m[0][2] * _right.m[2][2] + m[0][3] * _right.m[3][2];
            result.m[0][3] = m[0][0] * _right.m[0][3] + m[0][1] * _right.m[1][3] + m[0][2] * _right.m[2][3] + m[0][3] * _right.m[3][3];

            result.m[1][0] = m[1][0] * _right.m[0][0] + m[1][1] * _right.m[1][0] + m[1][2] * _right.m[2][0] + m[1][3] * _right.m[3][0];
            result.m[1][1] = m[1][0] * _right.m[0][1] + m[1][1] * _right.m[1][1] + m[1][2] * _right.m[2][1] + m[1][3] * _right.m[3][1];
            result.m[1][2] = m[1][0] * _right.m[0][2] + m[1][1] * _right.m[1][2] + m[1][2] * _right.m[2][2] + m[1][3] * _right.m[3][2];
            result.m[1][3] = m[1][0] * _right.m[0][3] + m[1][1] * _right.m[1][3] + m[1][2] * _right.m[2][3] + m[1][3] * _right.m[3][3];

            result.m[2][0] = m[2][0] * _right.m[0][0] + m[2][1] * _right.m[1][0] + m[2][2] * _right.m[2][0] + m[2][3] * _right.m[3][0];
            result.m[2][1] = m[2][0] * _right.m[0][1] + m[2][1] * _right.m[1][1] + m[2][2] * _right.m[2][1] + m[2][3] * _right.m[3][1];
            result.m[2][2] = m[2][0] * _right.m[0][2] + m[2][1] * _right.m[1][2] + m[2][2] * _right.m[2][2] + m[2][3] * _right.m[3][2];
            result.m[2][3] = m[2][0] * _right.m[0][3] + m[2][1] * _right.m[1][3] + m[2][2] * _right.m[2][3] + m[2][3] * _right.m[3][3];

            result.m[3][0] = m[3][0] * _right.m[0][0] + m[3][1] * _right.m[1][0] + m[3][2] * _right.m[2][0] + m[3][3] * _right.m[3][0];
            result.m[3][1] = m[3][0] * _right.m[0][1] + m[3][1] * _right.m[1][1] + m[3][2] * _right.m[2][1] + m[3][3] * _right.m[3][1];
            result.m[3][2] = m[3][0] * _right.m[0][2] + m[3][1] * _right.m[1][2] + m[3][2] * _right.m[2][2] + m[3][3] * _right.m[3][2];
            result.m[3][3] = m[3][0] * _right.m[0][3] + m[3][1] * _right.m[1][3] + m[3][2] * _right.m[2][3] + m[3][3] * _right.m[3][3];

            return result;
  
        }

        inline Vector3 operator * ( const Vector3& _right ) const
        {
			float fW = m[3][0] * _right.x + m[3][1] * _right.y + m[3][2] * _right.z + m[3][3];

			if(fW > (std::numeric_limits<float>::epsilon() ) )
			{
				Vector3 result;

				const float fInvW = 1.0f / fW;

				result.x = ( m[0][0] * _right.x + m[0][1] * _right.y + m[0][2] * _right.z + m[0][3] ) * fInvW;
				result.y = ( m[1][0] * _right.x + m[1][1] * _right.y + m[1][2] * _right.z + m[1][3] ) * fInvW;
				result.z = ( m[2][0] * _right.x + m[2][1] * _right.y + m[2][2] * _right.z + m[2][3] ) * fInvW;

				return result;
			}

            return _right;
        }
   
        inline Matrix44 operator + ( const Matrix44& _right ) const
        {
            Matrix44 result;

            result.m[0][0] = m[0][0] + _right.m[0][0];
            result.m[0][1] = m[0][1] + _right.m[0][1];
            result.m[0][2] = m[0][2] + _right.m[0][2];
            result.m[0][3] = m[0][3] + _right.m[0][3];

            result.m[1][0] = m[1][0] + _right.m[1][0];
            result.m[1][1] = m[1][1] + _right.m[1][1];
            result.m[1][2] = m[1][2] + _right.m[1][2];
            result.m[1][3] = m[1][3] + _right.m[1][3];

            result.m[2][0] = m[2][0] + _right.m[2][0];
            result.m[2][1] = m[2][1] + _right.m[2][1];
            result.m[2][2] = m[2][2] + _right.m[2][2];
            result.m[2][3] = m[2][3] + _right.m[2][3];

            result.m[3][0] = m[3][0] + _right.m[3][0];
            result.m[3][1] = m[3][1] + _right.m[3][1];
            result.m[3][2] = m[3][2] + _right.m[3][2];
            result.m[3][3] = m[3][3] + _right.m[3][3];

            return result;
        }

        inline Matrix44 operator - ( const Matrix44& _right ) const
        {
            Matrix44 result;
            result.m[0][0] = m[0][0] - _right.m[0][0];
            result.m[0][1] = m[0][1] - _right.m[0][1];
            result.m[0][2] = m[0][2] - _right.m[0][2];
            result.m[0][3] = m[0][3] - _right.m[0][3];

            result.m[1][0] = m[1][0] - _right.m[1][0];
            result.m[1][1] = m[1][1] - _right.m[1][1];
            result.m[1][2] = m[1][2] - _right.m[1][2];
            result.m[1][3] = m[1][3] - _right.m[1][3];

            result.m[2][0] = m[2][0] - _right.m[2][0];
            result.m[2][1] = m[2][1] - _right.m[2][1];
            result.m[2][2] = m[2][2] - _right.m[2][2];
            result.m[2][3] = m[2][3] - _right.m[2][3];

            result.m[3][0] = m[3][0] - _right.m[3][0];
            result.m[3][1] = m[3][1] - _right.m[3][1];
            result.m[3][2] = m[3][2] - _right.m[3][2];
            result.m[3][3] = m[3][3] - _right.m[3][3];

            return result;
        }

		inline Vector3 RotateVec( const Vector3& _v)
		{
			Vector3 prod;
	 
			prod.x = m[0][0] * _v.x + m[0][1] * _v.y + m[0][2] * _v.z;
			prod.y = m[1][0] * _v.x + m[1][1] * _v.y + m[1][2] * _v.z;
			prod.z = m[2][0] * _v.x + m[2][1] * _v.y + m[2][2] * _v.z;
	        
			return prod;
		}

        inline Matrix44 GetTranspose() const
        {
            return Matrix44(m[0][0], m[1][0], m[2][0], m[3][0],
                           m[0][1], m[1][1], m[2][1], m[3][1],
                           m[0][2], m[1][2], m[2][2], m[3][2],
                           m[0][3], m[1][3], m[2][3], m[3][3]);
        }

		inline void Transpose()
        {
            Set(	m[0][0], m[1][0], m[2][0], m[3][0],
					m[0][1], m[1][1], m[2][1], m[3][1],
                    m[0][2], m[1][2], m[2][2], m[3][2],
                    m[0][3], m[1][3], m[2][3], m[3][3]);
        }

        inline void SetTranslate( const Vector3& _trans )
        {
            m[0][3] = _trans.x;
            m[1][3] = _trans.y;
            m[2][3] = _trans.z;
        }

        inline Vector3 GetTranslate() const
        {
          return Vector3(m[0][3], m[1][3], m[2][3]);
        }
        
		inline void SetScale( const Vector3& _scale )
		{
			m[0][0] = _scale.x;
			m[1][1] = _scale.y;
			m[2][2] = _scale.z;
		}

        inline void MakeTranslationMatrix( const Vector3& _trans )
        {
            m[0][0] = 1.0;		m[0][1] = 0.0;		m[0][2] = 0.0;		m[0][3] = _trans.x;
            m[1][0] = 0.0;		m[1][1] = 1.0;		m[1][2] = 0.0;		m[1][3] = _trans.y;
            m[2][0] = 0.0;		m[2][1] = 0.0;		m[2][2] = 1.0;		m[2][3] = _trans.z;
            m[3][0] = 0.0;		m[3][1] = 0.0;		m[3][2] = 0.0;		m[3][3] = 1.0;
        }

		inline void SetColumnVec3(unsigned short _colInd, const Vector3& vec)
		{
			assert( _colInd < 3 );
			m[0][_colInd] = vec.x;
			m[1][_colInd] = vec.y;
			m[2][_colInd] = vec.z;

		}
		
		inline void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
		{
			SetColumnVec3(0,xAxis);
			SetColumnVec3(1,yAxis);
			SetColumnVec3(2,zAxis);
		}

		inline Vector3 GetColumnVec3(unsigned short _colInd) const
		{
			assert( _colInd < 4 );
			Vector3 col(m[0][_colInd],m[1][_colInd],m[2][_colInd]);
			return col;
		}
		
		Matrix44 Adjoint() const;
		float Determinant() const;
		Matrix44 GetInverse() const;
		void Inverse();
			
		void MakeTransform(const Vector3& _position, const Quaternion& _orientation);
        void MakeInverseTransform(const Vector3& _position, const Quaternion& _orientation);

		static const Matrix44 ZERO;
		static const Matrix44 IDENTITY;
	};
}
#endif
