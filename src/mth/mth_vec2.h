/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        mth_vec2.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 22.07.2024
 * PURPOSE:     Base math vector 2D header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __mth_vec2_h_
#define __mth_vec2_h_

#include "mth_def.h"

#include <mmintrin.h>

/* Space math namespace */
namespace mth
{
  /* 2D vector type */
  template<typename Type> 
    class vec2
    {
      public:
      Type 
        X, Y; // Vector components

      /* Default constructor */
      vec2() : X(0), Y(0)
      {
      } /* End of 'vec2' function */

      /* Constructor by one component.
       * ARGUMENTS:
       *   - component:
       *       Type N;
       */
      explicit vec2( const Type N ) : X(N), Y(N)
      {
      } /* End of 'vec2' function */

      /* Constructor by 2 components.
       * ARGUMENTS:
       *   - components:
       *       Type A, B;
       */
      vec2( const Type A, const Type B ) : X(A), Y(B)
      {
      } /* End of 'vec2' function */

      /* Random (components 0 - 1) vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) vector random.
       */
      static vec2 Rnd0()
      {
        return vec3(Type(rand()) / RAND_MAX,
                    Type(rand()) / RAND_MAX);
      } /* End of 'Rnd0' function */

      /* Random (components -1 - 1) vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) vector random.
       */
      static vec2 Rnd1()
      {
        return vec2(2 * Type(rand()) / RAND_MAX - 1,
                    2 * Type(rand()) / RAND_MAX - 1);
      } /* End of 'Rnd0' function */


      /* Vec add vec function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2) new vector.
       */
      vec2 operator+( const vec2 &V ) const
      {
        return vec2(X + V.X, Y + V.Y);
      } /* End of 'operator+' function */

      /* Vec sub vec function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2) new vector.
       */
      vec2 operator-( const vec2 &V ) const
      {
        return vec2(X - V.X, Y - V.Y);
      } /* End of 'operator-' function */

      /* Vec mul num function.
       * ARGUMENTS:
       *   - number:
       *       const Type N;
       * RETURNS:
       *   (vec2) new vector.
       */
      vec2 operator*( const Type N ) const
      {
        return vec2(X * N, Y * N);
      } /* End of 'operator+' function */

      /* Vec div num function.
       * ARGUMENTS:
       *   - number:
       *       const Type N;
       * RETURNS:
       *   (vec2) new vector.
       */
      vec2 operator/( const Type N ) const
      {
        return vec2(X / N, Y / N);
      } /* End of 'operator/' function */

      /* Vec length^2 function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (Type) length^2.
       */
      Type Len2() const
      {
        return X * X + Y * Y;
      } /* End of 'Len2' function */

      /* Vec length function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (Type) length.
       */
      Type operator!() const
      {
        return std::sqrt(X * X + Y * Y);
      } /* End of 'operator!' function */

      /* Vec dot vec function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec2 &V;
       * RETURNS:
       *   (Type) vec dot vec.
       */
      Type operator&( const vec2 &V ) const
      {
        return X * V.X + Y * V.Y;
      } /* End of 'operator&' function */

      /* Vec mul num function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2) new vector.
       */
      vec2 operator*( const vec2 &V ) const
      {
        return vec2(X * V.X, Y * V.Y);
      } /* End of 'operator*' function */

      /* +V function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) vector.
       */
      vec2 operator+() const
      {
        return *this;
      } /* End of 'operator+' function */

      /* -V function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) -vector.
       */
      vec2 operator-() const
      {
        return vec2(-X, -Y);
      } /* End of 'operator-' function */

      /* Vec add vec eq function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2 &) this vector.
       */
      vec2 & operator+=( const vec2 &V )
      {
        X += V.X;
        Y += V.Y;

        return *this;
      } /* End of 'operator+=' function */

      /* Vec sub vec eq function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2 &) this vector.
       */
      vec2 & operator-=( const vec2 &V )
      {
        X -= V.X;
        Y -= V.Y;

        return *this;
      } /* End of 'operator-=' function */

      /* Vec mul vec eq function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2 &) this vector.
       */
      vec2 & operator*=( const vec2 &V )
      {
        X *= V.X;
        Y *= V.Y;

        return *this;
      } /* End of 'operator*=' function */

      /* Vec mul num eq function.
       * ARGUMENTS:
       *   - number:
       *       const Type N;
       * RETURNS:
       *   (vec2 &) this vector.
       */
      vec2 & operator*=( const Type N )
      {
        X *= N;
        Y *= N;

        return *this;
      } /* End of 'operator*=' function */

      /* Vec div num eq function.
       * ARGUMENTS:
       *   - number:
       *       const Type N;
       * RETURNS:
       *   (vec2 &) this vector.
       */
      vec2 & operator/=( const Type N )
      {
        X /= N;
        Y /= N;

        return *this;
      } /* End of 'operator/=' function */

      /* Normalize vector.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2 &) this vector.
       */
      vec2 & Normalize()
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
       *   (vec2) new normalize vector.
       */
      vec2 Normalizing() const
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
      operator Type *() const
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
      Type operator[]( INT i )
      {
        switch (i)
        {
        case 0:
          return X;
        case 1:
          return Y;
        default:
          assert(0 && "Incorrect access to field");
          return Y;
        }
      } /* End of 'operator[]' function */

      /* Get const component function.
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
        default:
          assert(0 && "Incorrect access to field");
          return Type();
        }
      } /* End of 'operator[]' function */

    }; /* End of 'vec2' class */

#if defined(USE_MTH_FLOAT_INTRIN_LIB)
  /* 2D vector type :: float specifiaction */
  template<> 
    class __declspec(align(8)) vec2<FLT>
    {
    public:
      FLT 
        X, Y;

      /* Default constructor */
      vec2()
      {
        __m128 C = _mm_setzero_ps();
        _mm_store_ss(&this->X, C);
        _mm_store_ss(&this->X, C);
      } /* End of 'vec3' function */

      /* Constructor by one component.
       * ARGUMENTS:
       *   - component:
       *       const FLT N;
       */
      explicit vec2( const FLT N )
      {
        __m128 C = _mm_load_ss(&N);
        _mm_store_ss(&this->X, C);
        _mm_store_ss(&this->Y, C);
      } /* End of 'vec3' function */

      /* Constructor by 2 components.
       * ARGUMENTS:
       *   - components:
       *       const FLT A, B;
       */
      vec2( const FLT A, const FLT B )
      {
        _mm_store_ss(&this->X, _mm_load_ss(&A));
        _mm_store_ss(&this->Y, _mm_load_ss(&B));
      } /* End of 'vec3' function */

    private:

      /* Constructor by 2 SSE registers.
       * ARGUMENTS:
       *   - SSE registers:
       *       __m128 A, B;
       */
      vec2( __m128 A, __m128 B )
      {
        _mm_store_ss(&this->X, A);
        _mm_store_ss(&this->Y, B);
      } /* End of 'vec3' function */


    public:
      /* Random (components 0 - 1) vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) vector random.
       */
      static vec2 Rnd0()
      {
        return vec2(FLT(rand()) / RAND_MAX,
                    FLT(rand()) / RAND_MAX);
      } /* End of 'Rnd0' function */

      /* Random (components -1 - 1) vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec3) vector random.
       */
      static vec2 Rnd1()
      {
        return vec2(2 * FLT(rand()) / RAND_MAX - 1,
                    2 * FLT(rand()) / RAND_MAX - 1);
      } /* End of 'Rnd0' function */

      /* Vec add vec function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2) new vector.
       */
      vec2 operator+( const vec2 &V ) const
      {
        __m128 X1 = _mm_load_ss(&X);
        __m128 Y1 = _mm_load_ss(&Y);
        __m128 X2 = _mm_load_ss(&V.X);
        __m128 Y2 = _mm_load_ss(&V.Y);
        
        return vec2(_mm_add_ss(X1, X2), _mm_add_ss(Y1, Y2));
      } /* End of 'operator+' function */

      /* Vec sub vec function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2) new vector.
       */
      vec2 operator-( const vec2 &V ) const
      {
        __m128 X1 = _mm_load_ss(&X);
        __m128 Y1 = _mm_load_ss(&Y);
        __m128 X2 = _mm_load_ss(&V.X);
        __m128 Y2 = _mm_load_ss(&V.Y);
        
        return vec2(_mm_sub_ss(X1, X2), _mm_sub_ss(Y1, Y2));
      } /* End of 'operator-' function */

      /* Vec mul num function.
       * ARGUMENTS:
       *   - number:
       *       const FLT N;
       * RETURNS:
       *   (vec2) new vector.
       */
      vec2 operator*( const FLT N ) const
      {
        __m128 X1 = _mm_load_ss(&X);
        __m128 Y1 = _mm_load_ss(&Y);
        __m128 _N = _mm_load_ss(&N);

        return vec2(_mm_mul_ss(X1, _N), _mm_sub_ss(Y1, _N));
      } /* End of 'operator+' function */

      /* Vec div num function.
       * ARGUMENTS:
       *   - number:
       *       const FLT N;
       * RETURNS:
       *   (vec2) new vector.
       */
      vec2 operator/( const FLT N ) const
      {
        __m128 X1 = _mm_load_ss(&X);
        __m128 Y1 = _mm_load_ss(&Y);
        __m128 _N = _mm_load_ss(&N);

        return vec2(_mm_div_ss(X1, _N), _mm_div_ss(Y1, _N));
      } /* End of 'operator/' function */

      /* Vec length^2 function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (FLT) length^2.
       */
      FLT Len2() const
      {
        __m128 X1 = _mm_load_ss(&X);
        __m128 Y1 = _mm_load_ss(&Y);
        X1 = _mm_mul_ss(X1, X1);
        Y1 = _mm_mul_ss(Y1, Y1);

        return _mm_cvtss_f32(_mm_add_ss(X1, Y1));
      } /* End of 'Len2' function */

      /* Vec length function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (FLT) length.
       */
      FLT operator!() const
      {
        __m128 X1 = _mm_load_ss(&X);
        __m128 Y1 = _mm_load_ss(&Y);
        X1 = _mm_mul_ss(X1, X1);
        Y1 = _mm_mul_ss(Y1, Y1);

        return _mm_cvtss_f32(_mm_sqrt_ss(_mm_add_ss(X1, Y1)));
      } /* End of 'operator!' function */

      /* Vec dot vec function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec2 &V;
       * RETURNS:
       *   (FLT) vec dot vec.
       */
      FLT operator&( const vec2 &V ) const
      {
        __m128 X1 = _mm_load_ss(&X);
        __m128 Y1 = _mm_load_ss(&Y);
        __m128 X2 = _mm_load_ss(&V.X);
        __m128 Y2 = _mm_load_ss(&V.Y);
        X1 = _mm_mul_ss(X1, X2);
        Y1 = _mm_mul_ss(Y1, Y2);

        return _mm_cvtss_f32(_mm_add_ss(X1, Y1));
      } /* End of 'operator&' function */

      /* Vec mul vec function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec3 &V;
       * RETURNS:
       *   (vec3) new vector.
       */
      vec2 operator*( const vec2 &V ) const
      {
        __m128 X1 = _mm_load_ss(&X);
        __m128 Y1 = _mm_load_ss(&Y);
        __m128 X2 = _mm_load_ss(&V.X);
        __m128 Y2 = _mm_load_ss(&V.Y);
        
        return vec2(_mm_mul_ss(X1, X2), _mm_mul_ss(Y1, Y2));
      } /* End of 'operator*' function */

      /* +V function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) vector.
       */
      vec2 operator+() const
      {
        return *this;
      } /* End of 'operator+' function */

      /* -V function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) -vector.
       */
      vec2 operator-() const
      {
        return (*this) * -1;
      } /* End of 'operator-' function */

      /* Vec add vec eq function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2 &) this vector.
       */
      vec2 & operator+=( const vec2 &V )
      {
        __m128 X1 = _mm_load_ss(&X);
        __m128 Y1 = _mm_load_ss(&Y);
        __m128 X2 = _mm_load_ss(&V.X);
        __m128 Y2 = _mm_load_ss(&V.Y);
        
        return *this = vec2(_mm_add_ss(X1, X2), _mm_add_ss(Y1, Y2));
      } /* End of 'operator+=' function */

      /* Vec sub vec eq function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2 &) this vector.
       */
      vec2 & operator-=( const vec2 &V )
      {
        __m128 X1 = _mm_load_ss(&X);
        __m128 Y1 = _mm_load_ss(&Y);
        __m128 X2 = _mm_load_ss(&V.X);
        __m128 Y2 = _mm_load_ss(&V.Y);
        
        return *this = vec2(_mm_sub_ss(X1, X2), _mm_sub_ss(Y1, Y2));
      } /* End of 'operator-=' function */

      /* Vec mul vec eq function.
       * ARGUMENTS:
       *   - other vector:
       *       const vec2 &V;
       * RETURNS:
       *   (vec2 &) this vector.
       */
      vec2 & operator*=( const vec2 &V )
      {
        __m128 X1 = _mm_load_ss(&X);
        __m128 Y1 = _mm_load_ss(&Y);
        __m128 X2 = _mm_load_ss(&V.X);
        __m128 Y2 = _mm_load_ss(&V.Y);
        
        return *this = vec2(_mm_mul_ss(X1, X2), _mm_mul_ss(Y1, Y2));
      } /* End of 'operator*=' function */

      /* Vec mul num eq function.
       * ARGUMENTS:
       *   - number:
       *       const FLT N;
       * RETURNS:
       *   (vec2 &) this vector.
       */
      vec2 & operator*=( const FLT N )
      {
        __m128 X1 = _mm_load_ss(&X);
        __m128 Y1 = _mm_load_ss(&Y);
        __m128 _N = _mm_load_ss(&N);
        
        return *this = vec2(_mm_mul_ss(X1, _N), _mm_mul_ss(Y1, _N));
      } /* End of 'operator*=' function */

      /* Vec div num eq function.
       * ARGUMENTS:
       *   - number:
       *       const FLT N;
       * RETURNS:
       *   (vec2 &) this vector.
       */
      vec2 & operator/=( const FLT N )
      {
        __m128 X1 = _mm_load_ss(&X);
        __m128 Y1 = _mm_load_ss(&Y);
        __m128 _N = _mm_load_ss(&N);
        
        return *this = vec2(_mm_div_ss(X1, _N), _mm_div_ss(Y1, _N));
      } /* End of 'operator/=' function */

      /* Normalize vector.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2 &) this vector.
       */
      vec2 & Normalize()
      {
        __m128 X1 = _mm_load_ss(&X);
        __m128 Y1 = _mm_load_ss(&Y);
        __m128 _X1 = _mm_mul_ss(X1, X1);
        __m128 _Y1 = _mm_mul_ss(Y1, Y1);
        _Y1 = _mm_sqrt_ss(_mm_add_ss(_X1, _Y1));

        return *this = vec2(_mm_div_ss(X1, _Y1), _mm_div_ss(Y1, _Y1));
      } /* End of 'Normalize' function */

      /* Get normalize vector.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec2) new normalize vector.
       */
      vec2 Normalizing() const
      {
        __m128 X1 = _mm_load_ss(&X);
        __m128 Y1 = _mm_load_ss(&Y);
        __m128 _X1 = _mm_mul_ss(X1, X1);
        __m128 _Y1 = _mm_mul_ss(Y1, Y1);
        _Y1 = _mm_sqrt_ss(_mm_add_ss(_X1, _Y1));

        return vec2(_mm_div_ss(X1, _Y1), _mm_div_ss(Y1, _Y1));
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
        default:
          assert(0 && "Incorrect access to field");
          return Y;
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
        default:
          assert(0 && "Incorrect access to field");
          return FLT();
        }
      } /* End of 'operator[]' function */

    }; /* End of 'vec2<FLT>' class */
#endif
} /* end of 'mth' namespace */

/* Print to stdout vector data function.
 * ARGUMENTS:
 *   - out object:
 *       std::ostream & coutvar;
 *   - vector:
 *       const mth::vec2<Type> &V;
 * RETURNS:
 *   (std::ostream &) out object.
 */
template<typename Type>
  inline std::ostream & operator<<( std::ostream & coutvar, const mth::vec2<Type> &V )
  {
    coutvar << '(' << V.X << ',' << V.Y  << ')';
    return coutvar;
  } /* End of 'operator<<' function */


#endif // !__mth_vec2_h_

/* END OF 'mth_vec2.h' FILE */
