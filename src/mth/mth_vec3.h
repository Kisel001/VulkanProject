/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        mth_vec3.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 22.07.2024
 * PURPOSE:     Base math vector 3D header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __mth_vec3_h_
#define __mth_vec3_h_

#include "mth_def.h"

/* Space math namespace */
namespace mth
{
  /* 3D vector type */
  template<typename Type> 
    class vec3
    {
    public:
      Type 
        X, Y, Z; // Vector components

      /* Default constructor */
      vec3() : X(0), Y(0), Z(0)
      {
      } /* End of 'vec3' function */

      /* Constructor by one component.
       * ARGUMENTS:
       *   - component:
       *       const Type N;
       */
      explicit vec3( const Type N ) : X(N), Y(N), Z(N)
      {
      } /* End of 'vec3' function */

      /* Constructor by 3 components.
       * ARGUMENTS:
       *   - components:
       *       const Type A, B, C;
       */
      vec3( const Type A, const Type B, const Type C ) : X(A), Y(B), Z(C)
      {
      } /* End of 'vec3' function */

      /* Random (components 0 - 1) vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) vector random.
       */
      static vec3 Rnd0()
      {
        return vec3(Type(rand()) / RAND_MAX,
                    Type(rand()) / RAND_MAX,
                    Type(rand()) / RAND_MAX);
      } /* End of 'Rnd0' function */

      /* Random (components -1 - 1) vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) vector random.
       */
      static vec3 Rnd1()
      {
        return vec3(2 * Type(rand()) / RAND_MAX - 1,
                    2 * Type(rand()) / RAND_MAX - 1,
                    2 * Type(rand()) / RAND_MAX - 1);
      } /* End of 'Rnd0' function */

      /* Vec add vec function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 operator+( const vec3 &V ) const
      {
        return vec3(X + V.X, Y + V.Y, Z + V.Z);
      } /* End of 'operator+' function */

      /* Vec sub vec function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 operator-( const vec3 &V ) const
      {
        return vec3(X - V.X, Y - V.Y, Z - V.Z);
      } /* End of 'operator-' function */

      /* Vec mul num function.
       * ARGUMENTS:
       *   - number:
       *       const Type N;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 operator*( const Type N ) const
      {
        return vec3(X * N, Y * N, Z * N);
      } /* End of 'operator+' function */

      /* Vec div num function.
       * ARGUMENTS:
       *   - number:
       *       const Type N;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 operator/( const Type N ) const
      {
        return vec3(X / N, Y / N, Z / N);
      } /* End of 'operator/' function */

      /* Vec length^2 function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (Type) length^2.
       */
      Type Len2() const
      {
        return X * X + Y * Y + Z * Z;
      } /* End of 'Len2' function */

      /* Vec length function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (Type) length.
       */
      Type operator!() const
      {
        return std::sqrt(X * X + Y * Y + Z * Z);
      } /* End of 'operator!' function */

      /* Vec dot vec function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (Type) vec dot vec.
       */
      Type operator&( const vec3 &V ) const
      {
        return X * V.X + Y * V.Y + Z * V.Z;
      } /* End of 'operator&' function */

      /* Vec cross vec function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 operator%( const vec3 &V ) const
      {
        return vec3(Y * V.Z - Z * V.Y,
                    Z * V.X - X * V.Z,
                    X * V.Y - Y * V.X);
      } /* End of 'operator%' function */

      /* Vec mul num function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 operator*( const vec3 &V ) const
      {
        return vec3(X * V.X, Y * V.Y, Z * V.Z);
      } /* End of 'operator*' function */

      /* +V function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) vector.
       */
      vec3 operator+() const
      {
        return *this;
      } /* End of 'operator+' function */

      /* -V function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) -vector.
       */
      vec3 operator-() const
      {
        return vec3(-X, -Y, -Z);
      } /* End of 'operator-' function */

      /* Vec add vec eq function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3 &) this vector.
       */
      vec3 & operator+=( const vec3 &V )
      {
        X += V.X;
        Y += V.Y;
        Z += V.Z;

        return *this;
      } /* End of 'operator+=' function */

      /* Vec sub vec eq function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3 &) this vector.
       */
      vec3 & operator-=( const vec3 &V )
      {
        X -= V.X;
        Y -= V.Y;
        Z -= V.Z;

        return *this;
      } /* End of 'operator-=' function */

      /* Vec mul vec eq function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3 &) this vector.
       */
      vec3 & operator*=( const vec3 &V )
      {
        X *= V.X;
        Y *= V.Y;
        Z *= V.Z;

        return *this;
      } /* End of 'operator*=' function */

      /* Vec mul num eq function.
       * ARGUMENTS:
       *   - number:
       *       const Type N;
       * RETURNS:
       *   (vec3 &) this vector.
       */
      vec3 & operator*=( const Type N )
      {
        X *= N;
        Y *= N;
        Z *= N;

        return *this;
      } /* End of 'operator*=' function */

      /* Vec div num eq function.
       * ARGUMENTS:
       *   - number:
       *       const Type N;
       * RETURNS:
       *   (vec3 &) this vector.
       */
      vec3 & operator/=( const Type N )
      {
        X /= N;
        Y /= N;
        Z /= N;

        return *this;
      } /* End of 'operator/=' function */

      /* Normalize vector.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3 &) this vector.
       */
      vec3 & Normalize()
      {
        Type len2 = this->Len2();

#ifdef _DEBUG
        if (len2 == 0 || len2 == 1)
          return *this;
#endif // _DEBUG

        return *this /= std::sqrt(len2);
      } /* End of 'Normalize' function */

      /* Get normalize vector.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) new normalize vector.
       */
      vec3 Normalizing() const
      {
        Type len2 = this->Len2();

#ifdef _DEBUG
        if (len2 == 0 || len2 == 1)
          return *this;
#endif // _DEBUG

        return *this / std::sqrt(len2);
      } /* End of 'Normalize' function */

      /* Get Type-pointer to vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (Type *) pointer to first element
       */
      operator Type *()
      {
        return &this->X;
      } /* End of 'operator Type *' function*/

      /* Get component function.
       * AGRUMENTS:
       *   - component number:
       *       INT i;
       * RETURNS:
       *   (Type &) V[i] component.
       */
      Type & operator[]( INT i )
      {
        switch (i)
        {
        case 0:
          return X;
        case 1:
          return Y;
        case 2:
          return Z;
        default:
          assert(0 && "Incorrect access to field");
          return Z;
        }
      } /* End of 'operator[]' function */

      /* Get component function.
       * AGRUMENTS:
       *   - component number:
       *       INT i;
       * RETURNS:
       *   (Type &) V[i] component.
       */
      Type operator[]( INT i ) const
      {
        switch (i)
        {
        case 0:
          return X;
        case 1:
          return Y;
        case 2:
          return Z;
        default:
          assert(0 && "Incorrect access to field");
          return Type();
        }
      } /* End of 'operator[]' function */
      
      /* Reflect function.
       * ARGUMENTS:
       *   - vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 Reflect( const vec3 &n ) const
      {
        return *this - (n * (*this & n) * 2);
      } /* End of 'Reflect' function */

      /* Get max component function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (Type) max component.
       */
      Type MaxComponent( VOID ) const
      {
        Type MaxC = X;
        if (MaxC < Y)
          MaxC = Y;
        if (MaxC < Z)
          MaxC = Z;

        return MaxC;
      } /* End of 'MaxComponent' function */

      /* Get vector from minimal components function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 Min( const vec3 &V )
      {
        return vec3(min(X, V.X), min(Y, V.Y), min(Z, V.Z));
      } /* End of 'Min' function */ 

      /* Get vector from maximal components function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 Max( const vec3 &V )
      {
        return vec3(max(X, V.X), max(Y, V.Y), max(Z, V.Z));
      } /* End of 'Max' function */ 

    }; /* End of 'vec3' class */

#if defined(USE_MTH_FLOAT_INTRIN_LIB)
  /* 3D vector type :: float specifiaction */
  template<> 
    class __declspec(align(16)) vec3<FLT>
    {
    public:
      FLT 
        X, Y, Z; // Vector components

    private:
      // We used XMM (Inter(R) SSE) 16-bytes registers
      // for optization of vector arithmetic.
      // We must storage 4-th component (also 0)
      // for 16-bytes alignment.
      FLT align_reserved_w;

    public:

      /* Default constructor */
      vec3()
      {
        _mm_store_ps(&this->X, _mm_setzero_ps());
      } /* End of 'vec3' function */

      /* Constructor by one component.
       * ARGUMENTS:
       *   - component:
       *       const FLT N;
       */
      explicit vec3( const FLT N )
      {
        __m128 C = _mm_load_ss(&N);
        _mm_store_ss(&this->X, C);
        _mm_store_ss(&this->Y, C);
        _mm_store_ss(&this->Z, C);
        _mm_store_ss(&this->align_reserved_w, _mm_set_ss(0.0));
      } /* End of 'vec3' function */

      /* Constructor by 3 components.
       * ARGUMENTS:
       *   - components:
       *       const FLT A, B, C;
       */
      vec3( const FLT A, const FLT B, const FLT C )
      {
        _mm_store_ss(&this->X, _mm_load_ss(&A));
        _mm_store_ss(&this->Y, _mm_load_ss(&B));
        _mm_store_ss(&this->Z, _mm_load_ss(&C));
        _mm_store_ss(&this->align_reserved_w, _mm_set_ss(0.0));
      } /* End of 'vec3' function */

    private:

      /* Constructor by SSE register.
       * ARGUMENTS:
       *   - SSE registers:
       *       __m128 C;
       */
      vec3( __m128 C )
      {
        _mm_store_ps(&this->X, C);
      } /* End of 'vec3' function */

      /* Constructor by 3 SSE registers.
       * ARGUMENTS:
       *   - SSE registers:
       *       __m128 A, B, C;
       */
      vec3( __m128 A, __m128 B, __m128 C )
      {
        _mm_store_ss(&this->X, A);
        _mm_store_ss(&this->Y, B);
        _mm_store_ss(&this->Z, C);
        _mm_store_ss(&this->align_reserved_w, _mm_set_ss(0.0));
      } /* End of 'vec3' function */


    public:
      /* Random (components 0 - 1) vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) vector random.
       */
      static vec3 Rnd0()
      {
        return vec3(FLT(rand()) / RAND_MAX,
                    FLT(rand()) / RAND_MAX,
                    FLT(rand()) / RAND_MAX);
      } /* End of 'Rnd0' function */

      /* Random (components -1 - 1) vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) vector random.
       */
      static vec3 Rnd1()
      {
        return vec3(2 * FLT(rand()) / RAND_MAX - 1,
                    2 * FLT(rand()) / RAND_MAX - 1,
                    2 * FLT(rand()) / RAND_MAX - 1);
      } /* End of 'Rnd0' function */

      /* Vec add vec function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 operator+( const vec3 &V ) const
      {
        return vec3(_mm_add_ps(_mm_load_ps(&this->X), _mm_load_ps(&V.X)));
      } /* End of 'operator+' function */

      /* Vec sub vec function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 operator-( const vec3 &V ) const
      {
        return vec3(_mm_sub_ps(_mm_load_ps(&this->X), _mm_load_ps(&V.X)));
      } /* End of 'operator-' function */

      /* Vec mul num function.
       * ARGUMENTS:
       *   - number:
       *       const FLT N;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 operator*( const FLT N ) const
      {
        return vec3(_mm_mul_ps(_mm_load_ps(&this->X), _mm_load1_ps(&N)));
      } /* End of 'operator+' function */

      /* Vec div num function.
       * ARGUMENTS:
       *   - number:
       *       const FLT N;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 operator/( const FLT N ) const
      {
        return vec3(_mm_div_ps(_mm_load_ps(&this->X), _mm_load1_ps(&N)));
      } /* End of 'operator/' function */

      /* Vec length^2 function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (FLT) length^2.
       */
      FLT Len2() const
      {
        __m128 C = _mm_load_ps(&this->X);
        __m128 C1 = _mm_mul_ps(C, C);
        C1 = _mm_hadd_ps(C1, C1);
        C1 = _mm_hadd_ps(C1, C1);

        return _mm_cvtss_f32(C1);
      } /* End of 'Len2' function */

      /* Vec length function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (FLT) length.
       */
      FLT operator!() const
      {
        __m128 C = _mm_load_ps(&this->X);
        __m128 C1 = _mm_mul_ps(C, C);
        C1 = _mm_hadd_ps(C1, C1);
        C1 = _mm_hadd_ps(C1, C1);
        C1 = _mm_sqrt_ps(C1);

        return _mm_cvtss_f32(C1);
      } /* End of 'operator!' function */

      /* Vec dot vec function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (FLT) vec dot vec.
       */
      FLT operator&( const vec3 &V ) const
      {
        __m128 C1 = _mm_mul_ps(_mm_load_ps(&this->X), _mm_load_ps(&V.X));
        C1 = _mm_hadd_ps(C1, C1);
        C1 = _mm_hadd_ps(C1, C1);

        return _mm_cvtss_f32(C1);
      } /* End of 'operator&' function */

      /* Vec cross vec function.
       * ARGUMENTS:
       *   - second vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 operator%( const vec3 &V ) const
      {
        __m128 X1 = _mm_load_ss(&X);
        __m128 Y1 = _mm_load_ss(&Y);
        __m128 Z1 = _mm_load_ss(&Z);
        __m128 X2 = _mm_load_ss(&V.X);
        __m128 Y2 = _mm_load_ss(&V.Y);
        __m128 Z2 = _mm_load_ss(&V.Z);

        return vec3(_mm_sub_ss(_mm_mul_ss(Y1, Z2), _mm_mul_ss(Z1, Y2)),
                    _mm_sub_ss(_mm_mul_ss(Z1, X2), _mm_mul_ss(X1, Z2)),
                    _mm_sub_ss(_mm_mul_ss(X1, Y2), _mm_mul_ss(Y1, X2)));
      } /* End of 'operator%' function */

      /* Vec mul num function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 operator*( const vec3 &V ) const
      {
        return vec3(_mm_mul_ps(_mm_load_ps(&this->X), _mm_load_ps(&V.X)));
      } /* End of 'operator*' function */

      /* +V function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) vector.
       */
      vec3 operator+() const
      {
        return *this;
      } /* End of 'operator+' function */

      /* -V function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) -vector.
       */
      vec3 operator-() const
      {
        return (*this) * -1;
      } /* End of 'operator-' function */

      /* Vec add vec eq function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3 &) this vector.
       */
      vec3 & operator+=( const vec3 &V )
      {
        return *this = vec3(_mm_add_ps(_mm_load_ps(&this->X), _mm_load_ps(&V.X)));
      } /* End of 'operator+=' function */

      /* Vec sub vec eq function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3 &) this vector.
       */
      vec3 & operator-=( const vec3 &V )
      {
        return *this = vec3(_mm_sub_ps(_mm_load_ps(&this->X), _mm_load_ps(&V.X)));;
      } /* End of 'operator-=' function */

      /* Vec mul vec eq function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3 &) this vector.
       */
      vec3 & operator*=( const vec3 &V )
      {
        return *this = vec3(_mm_mul_ps(_mm_load_ps(&this->X), _mm_load_ps(&V.X)));
      } /* End of 'operator*=' function */

      /* Vec mul num eq function.
       * ARGUMENTS:
       *   - number:
       *       const FLT N;
       * RETURNS:
       *   (vec3 &) this vector.
       */
      vec3 & operator*=( const FLT N )
      {
        return *this = vec3(_mm_mul_ps(_mm_load_ps(&this->X), _mm_load1_ps(&N)));
      } /* End of 'operator*=' function */

      /* Vec div num eq function.
       * ARGUMENTS:
       *   - number:
       *       const FLT N;
       * RETURNS:
       *   (vec3 &) this vector.
       */
      vec3 & operator/=( const FLT N )
      {
        return *this = vec3(_mm_mul_ps(_mm_load_ps(&this->X), _mm_load1_ps(&N)));
      } /* End of 'operator/=' function */

      /* Normalize vector.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3 &) this vector.
       */
      vec3 & Normalize()
      {
        __m128 C = _mm_load_ps(&this->X);
        __m128 C1 = _mm_mul_ps(C, C);
        C1 = _mm_hadd_ps(C1, C1);
        C1 = _mm_hadd_ps(C1, C1);
        C1 = _mm_sqrt_ps(C1);

        return *this = vec3(_mm_div_ps(C, C1));
      } /* End of 'Normalize' function */

      /* Get normalize vector.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) new normalize vector.
       */
      vec3 Normalizing() const
      {
        __m128 C = _mm_load_ps(&this->X);
        __m128 C1 = _mm_mul_ps(C, C);
        C1 = _mm_hadd_ps(C1, C1);
        C1 = _mm_hadd_ps(C1, C1);
        C1 = _mm_sqrt_ps(C1);

        return vec3(_mm_div_ps(C, C1));
      } /* End of 'Normalize' function */

      /* Get FLT-pointer to vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (FLT *) pointer to first element
       */
      operator FLT *()
      {
        return &this->X;
      } /* End of 'operator FLT *' function*/

      /* Get component function.
       * AGRUMENTS:
       *   - component number:
       *       INT i;
       * RETURNS:
       *   (FLT &) V[i] component.
       */
      FLT & operator[]( INT i )
      {
        switch (i)
        {
        case 0:
          return X;
        case 1:
          return Y;
        case 2:
          return Z;
        default:
          assert(0 && "Incorrect access to field");
          return Z;
        }
      } /* End of 'operator[]' function */

      /* Get component function.
       * AGRUMENTS:
       *   - component number:
       *       INT i;
       * RETURNS:
       *   (FLT &) V[i] component.
       */
      FLT operator[]( INT i ) const
      {
        switch (i)
        {
        case 0:
          return X;
        case 1:
          return Y;
        case 2:
          return Z;
        default:
          assert(0 && "Incorrect access to field");
          return FLT();
        }
      } /* End of 'operator[]' function */

      /* Get vector from minimal components function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 Min(const vec3 &V)
      {
        return vec3(min(X, V.X), min(Y, V.Y), min(Z, V.Z));
      } /* End of 'Min' function */

      /* Get vector from maximal components function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec3 Max(const vec3 &V)
      {
        return vec3(max(X, V.X), max(Y, V.Y), max(Z, V.Z));
      } /* End of 'Max' function */

    }; /* End of 'vec3<FLT>' class */
#endif

} /* end of 'mth' namespace */

/* Print to stdout vector data function.
 * ARGUMENTS:
 *   - out object:
 *       std::ostream & coutvar;
 *   - vector:
 *       const mth::vec3<Type> &V;
 * RETURNS:
 *   (std::ostream &) out object.
 */
template<typename Type>
  inline std::ostream & operator<<( std::ostream & coutvar, const mth::vec3<Type> &V )
  {
    coutvar << '(' << V.X << ',' << V.Y << ',' << V.Z << ')';
    return coutvar;
  } /* End of 'operator<<' function */

#endif // !__mth_vec3_h_

/* END OF 'mth_vec3.h' FILE */
