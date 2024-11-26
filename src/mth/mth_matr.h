/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        mth_matr.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 22.07.2024
 * PURPOSE:     Base math matrix header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __mth_matr_h_
#define __mth_matr_h_

#include "mth_def.h"

/* Space math namespace */
namespace mth
{
  /* Matrix 4x4 type */
  template<typename Type> 
    class matr
    {
    public:
      Type M[4][4]; // Matr components

      /* Default constructor */
      matr()
      {
        // Do nothing, this is lo-level constructor
      } /* End of 'matr' function */

      /* Constructor by 16 components.
       * ARGUMENTS:
       *   - 16 components:
       *       const Type A00-A33;
       */
      matr( const Type A00, const Type A01, const Type A02, const Type A03,
            const Type A10, const Type A11, const Type A12, const Type A13, 
            const Type A20, const Type A21, const Type A22, const Type A23, 
            const Type A30, const Type A31, const Type A32, const Type A33 )
      {
        M[0][0] = A00;
        M[0][1] = A01;
        M[0][2] = A02;
        M[0][3] = A03;
        M[1][0] = A10;
        M[1][1] = A11;
        M[1][2] = A12;
        M[1][3] = A13;
        M[2][0] = A20;
        M[2][1] = A21;
        M[2][2] = A22;
        M[2][3] = A23;
        M[3][0] = A30;
        M[3][1] = A31;
        M[3][2] = A32;
        M[3][3] = A33;
      } /* End of 'matr' function */

      /* Constructor by 4 vector 4D.
       * ARGUMENTS:
       *   - 4 vectors:
       *       const vec4<Type> &V(0-3);
       */
      matr( const vec4<Type> &V0, const vec4<Type> &V1, const vec4<Type> &V2, const vec4<Type> &V3 )
      {
        std::memcpy(M[0], V0, sizeof(vec4<Type>));
        std::memcpy(M[1], V1, sizeof(vec4<Type>));
        std::memcpy(M[2], V2, sizeof(vec4<Type>));
        std::memcpy(M[3], V3, sizeof(vec4<Type>));
      } /* End of 'matr' function */

      /* Constructor by Type array.
       * ARGUMENTS:
       *   - array:
       *       const Type A[4][4];
       */
      matr( const Type A[4][4] )
      {
        std::memcpy(M[0], A[0], sizeof(matr<Type>));
      } /* End of 'matr' function */

      /* Get identity matrix function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (matr) Identity matrix;
       */
      static matr Identity()
      {
        return matr(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
      } /* End of 'Identity' function */

      /* Get translated matrix function.
       * ARGUMENTS:
       *   - vector:
       *       const vec3<Type> &T;
       * RETURNS:
       *   (matr) new matrix.
       */
      static matr Translate( const vec3<Type> &T )
      {
        return matr(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    T.X, T.Y, T.Z, 1);
      } /* End of 'Translate' function */

      /* Get scaling matrix function.
       * ARGUMENTS:
       *   - vector:
       *       const vec3<Type> &S;
       * RETURNS:
       *   (matr) scale matr.
       */
      static matr Scale( const vec3<Type> &S )
      {
        return matr(S.X, 0, 0, 0,
                    0, S.Y, 0, 0,
                    0, 0, S.Z, 0,
                    0, 0, 0, 1);
      } /* End of 'Scale' function */

      /* Matrix of rotate around X to angle function.
       * ARGUMENTS:
       *   - angle in degree:
       *       const Type AngleInDegree;
       * RETURNS:
       *   (matr) new matrix.
       */
      static matr RotateX( const Type AngleInDegree )
      {
        Type
          an = D2R(AngleInDegree),
          c = cos(an),
          s = sin(an);

        return matr(1, 0, 0, 0,
                    0, c, s, 0,
                    0, -s, c, 0,
                    0, 0, 0, 1);
      } /* End of 'RotateX' function */

      /* Matrix of rotate around Y to angle function.
       * ARGUMENTS:
       *   - angle in degree:
       *       const Type AngleInDegree;
       * RETURNS:
       *   (matr) new matrix.
       */
      static matr RotateY( const Type AngleInDegree )
      {
        Type
          an = D2R(AngleInDegree),
          c = cos(an),
          s = sin(an);

        return matr(c, 0, -s, 0,
                    0, 1, 0, 0,
                    s, 0, c, 0,
                    0, 0, 0, 1);
      } /* End of 'RotateY' function */

      /* Matrix of rotate around Z to angle function.
       * ARGUMENTS:
       *   - angle in degree:
       *       const Type AngleInDegree;
       * RETURNS:
       *   (matr) new matrix.
       */
      static matr RotateZ( const Type AngleInDegree )
      {
        Type
          an = D2R(AngleInDegree),
          c = cos(an),
          s = sin(an);

        return matr(c, s, 0, 0,
                    -s, c, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
      } /* End of 'RotateZ' function */

      /* Matrix of Rotate around Vector to angle function.
       * ARGUMENTS:
       *   - angle in degree:
       *       const Type AngleInDegree;
       *   - vector of rotate:
       *       const vec3<Type> &R;
       * RETURNS:
       *   (matr) Rotated Matrix.
       */
      static matr Rotate( const Type AngleInDegree, const vec3<Type> &R )
      {
        FLT
          A = D2R(AngleInDegree),
          s = sin(A),
          c = cos(A);
        vec3<Type> V = R.Normalizing();
        
        return matr(c + V.X * V.X * (1 - c),
                    V.X * V.Y * (1 - c) + V.Z * s,
                    V.X * V.Z * (1 - c) - V.Y * s, 
                    0,
                    V.Y * V.X * (1 - c) - V.Z * s,
                    c + V.Y * V.Y * (1 - c),
                    V.Y * V.Z * (1 - c) + V.X * s, 
                    0,
                    V.Z * V.X * (1 - c) + V.Y * s,
                    V.Z * V.Y * (1 - c) - V.X * s, 
                    c + V.Z * V.Z * (1 - c), 0,
                    0, 
                    0, 
                    0, 
                    1);
      } /* End of 'Rotate' function */

      /* Multiplicate two matrixes function.
       * ARGUMENTS: 
       *   - second matrix:
       *       const matr &M;
       * RETURNS:
       *   (MATR) result matrix.
       */
      matr operator*( const matr &M ) const 
      {
        return
          matr(this->M[0][0] * M.M[0][0] + this->M[0][1] * M.M[1][0] + this->M[0][2] * M.M[2][0] + this->M[0][3] * M.M[3][0],
               this->M[0][0] * M.M[0][1] + this->M[0][1] * M.M[1][1] + this->M[0][2] * M.M[2][1] + this->M[0][3] * M.M[3][1],
               this->M[0][0] * M.M[0][2] + this->M[0][1] * M.M[1][2] + this->M[0][2] * M.M[2][2] + this->M[0][3] * M.M[3][2],
               this->M[0][0] * M.M[0][3] + this->M[0][1] * M.M[1][3] + this->M[0][2] * M.M[2][3] + this->M[0][3] * M.M[3][3],
               
               this->M[1][0] * M.M[0][0] + this->M[1][1] * M.M[1][0] + this->M[1][2] * M.M[2][0] + this->M[1][3] * M.M[3][0],
               this->M[1][0] * M.M[0][1] + this->M[1][1] * M.M[1][1] + this->M[1][2] * M.M[2][1] + this->M[1][3] * M.M[3][1],
               this->M[1][0] * M.M[0][2] + this->M[1][1] * M.M[1][2] + this->M[1][2] * M.M[2][2] + this->M[1][3] * M.M[3][2],
               this->M[1][0] * M.M[0][3] + this->M[1][1] * M.M[1][3] + this->M[1][2] * M.M[2][3] + this->M[1][3] * M.M[3][3],
               
               this->M[2][0] * M.M[0][0] + this->M[2][1] * M.M[1][0] + this->M[2][2] * M.M[2][0] + this->M[2][3] * M.M[3][0],
               this->M[2][0] * M.M[0][1] + this->M[2][1] * M.M[1][1] + this->M[2][2] * M.M[2][1] + this->M[2][3] * M.M[3][1],
               this->M[2][0] * M.M[0][2] + this->M[2][1] * M.M[1][2] + this->M[2][2] * M.M[2][2] + this->M[2][3] * M.M[3][2],
               this->M[2][0] * M.M[0][3] + this->M[2][1] * M.M[1][3] + this->M[2][2] * M.M[2][3] + this->M[2][3] * M.M[3][3],
               
               this->M[3][0] * M.M[0][0] + this->M[3][1] * M.M[1][0] + this->M[3][2] * M.M[2][0] + this->M[3][3] * M.M[3][0],
               this->M[3][0] * M.M[0][1] + this->M[3][1] * M.M[1][1] + this->M[3][2] * M.M[2][1] + this->M[3][3] * M.M[3][1],
               this->M[3][0] * M.M[0][2] + this->M[3][1] * M.M[1][2] + this->M[3][2] * M.M[2][2] + this->M[3][3] * M.M[3][2],
               this->M[3][0] * M.M[0][3] + this->M[3][1] * M.M[1][3] + this->M[3][2] * M.M[2][3] + this->M[3][3] * M.M[3][3]);
      } /* End of 'operator*' function */

      /* Matrix transponce function.
       * ARGUMENTS: None
       * RETURNS:
       *   (matr) Result matrix.
       */
      matr Transpose() const
      {
        return matr(M[0][0], M[1][0], M[2][0], M[3][0],
                    M[0][1], M[1][1], M[2][1], M[3][1],
                    M[0][2], M[1][2], M[2][2], M[3][2],
                    M[0][3], M[1][3], M[2][3], M[3][3]);
      } /* End of 'Transponce' function */

      /* Matrix 3*3 determination function.
       * ARGUMENTS:
       *   - arguments of matrix:
       *       FLT A11 - A33;
       * RETURNS:
       *   (FLT) result of determination matrix 3*3.
       */
      static Type Determ3x3( Type A11, Type A12, Type A13,
                             Type A21, Type A22, Type A23,
                             Type A31, Type A32, Type A33 )
      {
        return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
               A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
      } /* End of 'Determ3x3' function */

      /* Matrix 4*4 determination function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (Type) result of determination matrix 4*4.
       */
      Type operator!() const
      {
        return
          +M[0][0] * Determ3x3(M[1][1], M[1][2], M[1][3],
                               M[2][1], M[2][2], M[2][3],
                               M[3][1], M[3][2], M[3][3]) +
          -M[0][1] * Determ3x3(M[1][0], M[1][2], M[1][3],
                               M[2][0], M[2][2], M[2][3],
                               M[3][0], M[3][2], M[3][3]) +
          +M[0][2] * Determ3x3(M[1][0], M[1][1], M[1][3],
                               M[2][0], M[2][1], M[2][3],
                               M[3][0], M[3][1], M[3][3]) +
          -M[0][3] * Determ3x3(M[1][0], M[1][1], M[1][2],
                               M[2][0], M[2][1], M[2][2],
                               M[3][0], M[3][1], M[3][2]);
      } /* End of 'operator!' function */

      /* Inverse of matrix function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (MATR) Inverse Matrix.
       */
      matr Inverse() const
      {
        FLT det = !(*this);
        matr<Type> r;
        static const INT
          s[] = {1, -1},
          P[][3] = {{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}};
        INT
          i, j;

        if (det == 0)
          return Identity();

        for (i = 0; i < 4; i++)
          for (j = 0; j < 4; j++)
            r.M[j][i] =
              s[(i + j) % 2] *
              Determ3x3(M[P[i][0]][P[j][0]], M[P[i][0]][P[j][1]], M[P[i][0]][P[j][2]],
                        M[P[i][1]][P[j][0]], M[P[i][1]][P[j][1]], M[P[i][1]][P[j][2]],
                        M[P[i][2]][P[j][0]], M[P[i][2]][P[j][1]], M[P[i][2]][P[j][2]]) / det;

        return r;
      } /* End of 'Inverse' function */

      /* Matrix look-at viewer setup function.
       * ARGUMENTS:
       *   - viewer position, look-at point, approximate up direction:
       *       const vec3<Type> & Loc, At, Up1;
       * RETURNS:
       *   (matr) result matrix.
       */
      static matr View( const vec3<Type> &Loc, const vec3<Type> &At, const vec3<Type> Up1 )
      {
        vec3<Type>
          Dir = (At - Loc).Normalize(),
          Right = (Dir % Up1).Normalize(),
          Up = (Right % Dir).Normalize();
        
        return
          matr(Right.X, Up.X, -Dir.X, 0, 
               Right.Y, Up.Y, -Dir.Y, 0,
               Right.Z, Up.Z, -Dir.Z, 0,
              -(Loc & Right), -(Loc & Up), (Loc & Dir), 1);
      } /* End of 'View' function */

      /* Frustum projection matrix function.
       * ARGUMENTS:
       *   - parametrs of projection:
       *       const FLT L, R, B, T, N, F;
       * RETURNS:
       *   (matr) new matrix.
       */
      static matr Frustum( const FLT L, const FLT R, const FLT B, const FLT T, const FLT N, const FLT F )
      {
        return matr(2 * N / (R - L), 0, 0, 0,
                    0, 2 * N / (T - B), 0, 0,
                    (R + L) / (R - L), (T + B) / (T - B), -(F + N) / (F - N), -1,
                    0, 0, -2 * N * F / (F - N), 0);
      } /* End of 'Frustum' function */

      /* Ortho projection matrix function.
       * ARGUMENTS:
       *   - Parametrs of projection:
       *       FLT left (L), right (R), bottom (B), top (T), near (N), far (F);
       * RETURNS:
       *   (matr) New matrix.
       */
      static matr Ortho( FLT L, FLT R, FLT B, FLT T, FLT N, FLT F )
      {
        return matr(2 / (R - L), 0, 0, 0,
                    0, 2 / (T - B), 0, 0,
                    0, 0, -2 / (F - N), 0,
                   -(R + L) / (R - L), -(T + B) / (T - B), -(F + N) / (F - N), 1);
      } /* End of 'Ortho' function */

      /* Point radius-vector transformation by matrix function.
       * ARGUMENTS:
       *   - vector:
       *       const vec3<Type> &V;
       * RETURNS:
       *   (vec3<Type>) New vector.
       */
      vec3<Type> TransformPoint( const vec3<Type> &V ) const
      {
        return vec3<Type>((V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0] + M[3][0]),
                          (V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1] + M[3][1]),
                          (V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2] + M[3][2]));
      } /* End of 'TransformPoint' function */

      /* Transform vector by Matrix function.
       * ARGUMENTS:
       *   - vector:
       *       const vec3<Type> &V;
       * RETURNS:
       *   (vec3<Type>) New Vector.
       */
      vec3<Type> TransformVector( const vec3<Type> &V ) const
      {
        return vec3<Type>(V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0],
                          V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1],
                          V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2]);
      } /* End of 'TransformVector' function */

      /* Transform vector by Matrix 4 * 3 function.
       * ARGUMENTS:
       *   - vector:
       *       const vec3<Type> &V;
       * RETURNS:
       *   (vec3<Type>) New Vector.
       */
      vec3<Type> TransformNormal( const vec3<Type> &V ) const
      {
        matr m = this->Transpose().Inverse();

        return vec3<Type>(V.X * m.M[0][0] + V.Y * m.M[1][0] + V.Z * m.M[2][0],
                          V.X * m.M[0][1] + V.Y * m.M[1][1] + V.Z * m.M[2][1],
                          V.X * m.M[0][2] + V.Y * m.M[1][2] + V.Z * m.M[2][2]);
      } /* End of 'TransformVector' function */

      /* Transform 4x4 vector function.
       * ARGUMENTS:
       *   - vector:
       *       const vec3<Type> &V;
       * RETURNS: New Vector.
       */
      vec3<Type> Transform4x4( const vec3<Type> &V ) const
      {
        FLT w = V.X * M[0][3] + V.Y * M[1][3] + V.Z * M[2][3] + M[3][3];

        return vec3<Type>((V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0] + M[3][0]) / w,
                          (V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1] + M[3][1]) / w,
                          (V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2] + M[3][2]) / w);
      } /* End of 'Transform4x4' function */

      /* Transform 4x4 vector function.
       * ARGUMENTS:
       *   - vector:
       *       const vec3<Type> &V;
       * RETURNS: New Vector.
       */
      vec4<Type> Transform4x4( const vec4<Type> &V ) const
      {
        return vec4<Type>((V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0] + V.W * M[3][0]),
                          (V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1] + V.W * M[3][1]),
                          (V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2] + V.W * M[3][2]),
                          (V.X * M[0][3] + V.Y * M[1][3] + V.Z * M[2][3] + V.W * M[3][3]));
      } /* End of 'Transform4x4' function */

      /* Get Type-pointer to matrix function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (Type *) pointer to first element
       */
      operator Type *()
      {
        return &this->M[0][0];
      } /* End of 'operator Type *' function*/

      /* Get matrix line function.
       * ARGUMENTS:
       *   - number of line:
       *       const INT Num;
       * RETURNS:
       *   (Type *) pointer to line.
       */
      Type * operator[]( const INT Num )
      {
        assert(Num >= 0 && Num <= 3);

        return M[Num];
      } /* End of 'operator[]' function */

      /* Get const matrix line function.
       * ARGUMENTS:
       *   - number of line:
       *       const INT Num;
       * RETURNS:
       *   (Type *) pointer to line.
       */
      const Type * operator[]( const INT Num ) const
      {
        assert(Num >= 0 && Num <= 3);

        return M[Num];
      } /* End of 'operator[]' function */

    }; /* End of 'matr' class */

#if defined(USE_MTH_FLOAT_INTRIN_LIB)
  /* Matrix 4x4 type :: float specifiaction */
  template<> 
    class __declspec(align(64)) matr<FLT>
    {
    public:
      FLT M[4][4];     // Matr components

#define MakeShuffleMask(x,y,z,w)           (x | (y<<2) | (z<<4) | (w<<6))
#define VecSwizzleMask(vec, mask)          _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(vec), mask))
#define VecSwizzle(vec, x, y, z, w)        VecSwizzleMask(vec, MakeShuffleMask(x,y,z,w))
#define VecSwizzle1(vec, x)                VecSwizzleMask(vec, MakeShuffleMask(x,x,x,x))
#define VecSwizzle_0022(vec)               _mm_moveldup_ps(vec)
#define VecSwizzle_1133(vec)               _mm_movehdup_ps(vec)
#define VecShuffle(vec1, _vec2, x,y,z,w)    _mm_shuffle_ps(vec1, _vec2, MakeShuffleMask(x,y,z,w))
#define VecShuffle_0101(vec1, _vec2)        _mm_movelh_ps(vec1, _vec2)
#define VecShuffle_2323(vec1, _vec2)        _mm_movehl_ps(_vec2, vec1)

#define SMALL_NUMBER                       (1.e-8f)

      /* Default constructor */
      matr()
      {
        // Do nothing, this is lo-level constructor
      } /* End of 'matr' function */

      /* Constructor by 16 components.
       * ARGUMENTS:
       *   - 16 components:
       *       const FLT A00-A33;
       */
      matr( const FLT A00, const FLT A01, const FLT A02, const FLT A03,
            const FLT A10, const FLT A11, const FLT A12, const FLT A13, 
            const FLT A20, const FLT A21, const FLT A22, const FLT A23, 
            const FLT A30, const FLT A31, const FLT A32, const FLT A33 )
      {
        M[0][0] = A00;
        M[0][1] = A01;
        M[0][2] = A02;
        M[0][3] = A03;
        M[1][0] = A10;
        M[1][1] = A11;
        M[1][2] = A12;
        M[1][3] = A13;
        M[2][0] = A20;
        M[2][1] = A21;
        M[2][2] = A22;
        M[2][3] = A23;
        M[3][0] = A30;
        M[3][1] = A31;
        M[3][2] = A32;
        M[3][3] = A33;
      } /* End of 'matr' function */

      /* Constructor by 4 vector 4D.
       * ARGUMENTS:
       *   - 4 vectors:
       *       const vec4<FLT> &V(0-3);
       */
      matr( const vec4<FLT> &V0, const vec4<FLT> &V1, const vec4<FLT> &V2, const vec4<FLT> &V3 )
      {
        __m128 _V0 = _mm_load_ps(&V0.X);
        __m128 _V1 = _mm_load_ps(&V1.X);
        __m128 _V2 = _mm_load_ps(&V2.X);
        __m128 _V3 = _mm_load_ps(&V3.X);

        _mm_store_ss(&M[0][0], _V0);
        _mm_store_ss(&M[1][0], _V1);
        _mm_store_ss(&M[2][0], _V2);
        _mm_store_ss(&M[3][0], _V3);
      } /* End of 'matr' function */

      /* Constructor by FLT array.
       * ARGUMENTS:
       *   - array:
       *       const FLT A[4][4];
       */
      matr( const FLT A[4][4] )
      {
        __m256 _S1 = _mm256_load_ps(&A[0][0]);
        __m256 _S2 = _mm256_load_ps(&A[2][0]);
        _mm256_store_ps(&this->M[0][0], _S1);
        _mm256_store_ps(&this->M[2][0], _S2);
      } /* End of 'matr' function */

      /* Get identity matrix function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (matr) Identity matrix;
       */
      static matr Identity()
      {
        matr m;

        __m256 C = _mm256_setzero_ps();
        _mm256_store_ps(&m.M[0][0], C);
        _mm256_store_ps(&m.M[2][0], C);

        __m128 C1 = _mm_set_ss(1.0f);
        _mm_store_ss(&m.M[0][0], C1);
        _mm_store_ss(&m.M[1][1], C1);
        _mm_store_ss(&m.M[2][2], C1);
        _mm_store_ss(&m.M[3][3], C1);

        return m;
      } /* End of 'Identity' function */

      /* Get translated matrix function.
       * ARGUMENTS:
       *   - vector:
       *       const vec3<FLT> &T;
       * RETURNS:
       *   (matr) new matrix.
       */
      static matr Translate( const vec3<FLT> &T )
      {
        matr m;

        __m256 C = _mm256_setzero_ps();
        _mm256_store_ps(&m.M[0][0], C);
        _mm256_store_ps(&m.M[2][0], C);

        __m128 C1 = _mm_set_ss(1.0f);
        _mm_store_ss(&m.M[0][0], C1);
        _mm_store_ss(&m.M[1][1], C1);
        _mm_store_ss(&m.M[2][2], C1);

        _mm_store_ps(&m.M[3][0], _mm_load_ps(&T.X));
        _mm_store_ss(&m.M[3][3], C1);

        return m;
      } /* End of 'Translate' function */

      /* Get scaling matrix function.
       * ARGUMENTS:
       *   - vector:
       *       const vec3<FLT> &S;
       * RETURNS:
       *   (matr) scale matr.
       */
      static matr Scale( const vec3<FLT> &S )
      {
        matr m;

        __m256 C = _mm256_setzero_ps();
        _mm256_store_ps(&m.M[0][0], C);
        _mm256_store_ps(&m.M[2][0], C);

        _mm_store_ss(&m.M[0][0], _mm_load_ss(&S.X));
        _mm_store_ss(&m.M[1][1], _mm_load_ss(&S.X));
        _mm_store_ss(&m.M[2][2], _mm_load_ss(&S.X));
        _mm_store_ss(&m.M[3][3], _mm_set_ss(1.0f));

        return m;
      } /* End of 'Scale' function */

      /* Matrix of rotate around X to angle function.
       * ARGUMENTS:
       *   - angle in degree:
       *       const FLT AngleInDegree;
       * RETURNS:
       *   (matr) new matrix.
       */
      static matr RotateX( const FLT AngleInDegree )
      {
        matr m;
        FLT
          an = D2R(AngleInDegree),
          c = cos(an),
          s = sin(an);
        __m128 _C = _mm_load_ss(&c);
        __m128 _S = _mm_load_ss(&s);
        __m128 _MS = _mm_mul_ss(_S, _mm_set_ss(-1.0f));
        __m128 C1 = _mm_set_ss(1.0f);

        __m256 Z = _mm256_setzero_ps();
        _mm256_store_ps(&m.M[0][0], Z);
        _mm256_store_ps(&m.M[2][0], Z);

        _mm_store_ss(&m.M[0][0], C1);
        _mm_store_ss(&m.M[1][1], _C);
        _mm_store_ss(&m.M[1][2], _S);
        _mm_store_ss(&m.M[2][1], _MS);
        _mm_store_ss(&m.M[2][2], _C);
        _mm_store_ss(&m.M[3][3], C1);

        return m;
      } /* End of 'RotateX' function */

      /* Matrix of rotate around Y to angle function.
       * ARGUMENTS:
       *   - angle in degree:
       *       const FLT AngleInDegree;
       * RETURNS:
       *   (matr) new matrix.
       */
      static matr RotateY( const FLT AngleInDegree )
      {
        matr m;
        FLT
          an = D2R(AngleInDegree),
          c = cos(an),
          s = sin(an);
        __m128 _C = _mm_load_ss(&c);
        __m128 _S = _mm_load_ss(&s);
        __m128 _MS = _mm_mul_ss(_S, _mm_set_ss(-1.0f));
        __m128 C1 = _mm_set_ss(1.0f);

        __m256 Z = _mm256_setzero_ps();
        _mm256_store_ps(&m.M[0][0], Z);
        _mm256_store_ps(&m.M[2][0], Z);

        _mm_store_ss(&m.M[0][0], _C);
        _mm_store_ss(&m.M[1][1], C1);
        _mm_store_ss(&m.M[0][2], _MS);
        _mm_store_ss(&m.M[2][0], _S);
        _mm_store_ss(&m.M[2][2], _C);
        _mm_store_ss(&m.M[3][3], C1);

        return m;
      } /* End of 'RotateY' function */

      /* Matrix of rotate around Z to angle function.
       * ARGUMENTS:
       *   - angle in degree:
       *       const FLT AngleInDegree;
       * RETURNS:
       *   (matr) new matrix.
       */
      static matr RotateZ( const FLT AngleInDegree )
      {
        matr m;
        FLT
          an = D2R(AngleInDegree),
          c = cos(an),
          s = sin(an);
        __m128 _C = _mm_load_ss(&c);
        __m128 _S = _mm_load_ss(&s);
        __m128 _MS = _mm_mul_ss(_S, _mm_set_ss(-1.0f));
        __m128 C1 = _mm_set_ss(1.0f);

        __m256 Z = _mm256_setzero_ps();
        _mm256_store_ps(&m.M[0][0], Z);
        _mm256_store_ps(&m.M[2][0], Z);

        _mm_store_ss(&m.M[0][0], _C);
        _mm_store_ss(&m.M[0][1], _S);
        _mm_store_ss(&m.M[1][0], _MS);
        _mm_store_ss(&m.M[1][1], _C);
        _mm_store_ss(&m.M[2][2], C1);
        _mm_store_ss(&m.M[3][3], C1);

        return m;
      } /* End of 'RotateZ' function */

      /* Matrix of Rotate around Vector to angle function. (NOT OPTIMIZED)
       * ARGUMENTS:
       *   - angle in degree:
       *       const FLT AngleInDegree;
       *   - vector of rotate:
       *       const vec3<FLT> &R;
       * RETURNS:
       *   (matr) Rotated Matrix.
       */
      static matr Rotate( const FLT AngleInDegree, const vec3<FLT> &R )
      {
        FLT
          A = D2R(AngleInDegree),
          s = sin(A),
          c = cos(A);
        vec3<FLT> V = R.Normalizing();
        
        return matr(c + V.X * V.X * (1 - c),
                    V.X * V.Y * (1 - c) + V.Z * s,
                    V.X * V.Z * (1 - c) - V.Y * s, 
                    0,
                    V.Y * V.X * (1 - c) - V.Z * s,
                    c + V.Y * V.Y * (1 - c),
                    V.Y * V.Z * (1 - c) + V.X * s, 
                    0,
                    V.Z * V.X * (1 - c) + V.Y * s,
                    V.Z * V.Y * (1 - c) - V.X * s, 
                    c + V.Z * V.Z * (1 - c), 0,
                    0, 
                    0, 
                    0, 
                    1);
      } /* End of 'Rotate' function */

      /* Multiplicate two matrixes function.
       * ARGUMENTS: 
       *   - second matrix:
       *       const matr &M1;
       * RETURNS:
       *   (MATR) result matrix.
       */
      matr operator*( const matr &M1 ) const 
      {
        matr C;

        __m128 row1 = _mm_load_ps(&M1.M[0][0]);
        __m128 row2 = _mm_load_ps(&M1.M[1][0]);
        __m128 row3 = _mm_load_ps(&M1.M[2][0]);
        __m128 row4 = _mm_load_ps(&M1.M[3][0]);

        for (INT i = 0; i < 4; ++i)
        {
          __m128 brod1 = _mm_broadcast_ss(&M[i][0]);
          __m128 brod2 = _mm_broadcast_ss(&M[i][1]);
          __m128 brod3 = _mm_broadcast_ss(&M[i][2]);
          __m128 brod4 = _mm_broadcast_ss(&M[i][3]);
          __m128 row =   _mm_add_ps(_mm_add_ps(_mm_mul_ps(brod1, row1),
                                               _mm_mul_ps(brod2, row2)),
                                    _mm_add_ps(_mm_mul_ps(brod3, row3),
                                               _mm_mul_ps(brod4, row4)));

          _mm_store_ps(&C.M[i][0], row);
        }

        return C;
      } /* End of 'operator*' function */

      /* Matrix transponce function. (NO OPTIMIZED)
       * ARGUMENTS: None
       * RETURNS:
       *   (matr) Result matrix.
       */
      matr Transpose() const 
      {
        return matr(M[0][0], M[1][0], M[2][0], M[3][0],
                    M[0][1], M[1][1], M[2][1], M[3][1],
                    M[0][2], M[1][2], M[2][2], M[3][2],
                    M[0][3], M[1][3], M[2][3], M[3][3]);
      } /* End of 'Transponce' function */

      /* Matrix 3*3 determination function.
       * ARGUMENTS:
       *   - arguments of matrix:
       *       FLT A11 - A33;
       * RETURNS:
       *   (FLT) result of determination matrix 3*3.
       */
      static FLT Determ3x3( FLT A11, FLT A12, FLT A13,
                            FLT A21, FLT A22, FLT A23,
                            FLT A31, FLT A32, FLT A33 )
      {
        return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
               A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
      } /* End of 'Determ3x3' function */

      /* Matrix 4*4 determination function. (NO OPTIMIZED)
       * ARGUMENTS: None.
       * RETURNS:
       *   (FLT) result of determination matrix 4*4.
       */
      FLT operator!() const
      {
        return
          +M[0][0] * Determ3x3(M[1][1], M[1][2], M[1][3],
                               M[2][1], M[2][2], M[2][3],
                               M[3][1], M[3][2], M[3][3]) +
          -M[0][1] * Determ3x3(M[1][0], M[1][2], M[1][3],
                               M[2][0], M[2][2], M[2][3],
                               M[3][0], M[3][2], M[3][3]) +
          +M[0][2] * Determ3x3(M[1][0], M[1][1], M[1][3],
                               M[2][0], M[2][1], M[2][3],
                               M[3][0], M[3][1], M[3][3]) +
          -M[0][3] * Determ3x3(M[1][0], M[1][1], M[1][2],
                               M[2][0], M[2][1], M[2][2],
                               M[3][0], M[3][1], M[3][2]);
      } /* End of 'operator!' function */

      /* Inverse of matrix function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (MATR) Inverse Matrix.
       */
      matr Inverse() const
      {
#if 0
        FLT det = !(*this);
        matr<FLT> r;
        static const INT
          s[] = {1, -1},
          P[][3] = {{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}};
        INT
          i, j;

        if (det == 0)
          return Identity();

        for (i = 0; i < 4; i++)
          for (j = 0; j < 4; j++)
            r.M[j][i] =
              s[(i + j) % 2] *
              Determ3x3(M[P[i][0]][P[j][0]], M[P[i][0]][P[j][1]], M[P[i][0]][P[j][2]],
                        M[P[i][1]][P[j][0]], M[P[i][1]][P[j][1]], M[P[i][1]][P[j][2]],
                        M[P[i][2]][P[j][0]], M[P[i][2]][P[j][1]], M[P[i][2]][P[j][2]]) / det;

        return r;
#else
        matr r;
        __m128 sizeSqr;

        __m128 _A0 = _mm_load_ps(this->M[0]);
        __m128 _A1 = _mm_load_ps(this->M[1]);
        __m128 _A2 = _mm_load_ps(this->M[0]);
        __m128 _A3 = _mm_load_ps(this->M[3]);

        __m128 t0 =   VecShuffle_0101(_A0, _A1);
        __m128 t1 =   VecShuffle_2323(_A0, _A1);
        __m128 _RA0 = VecShuffle(t0, _A2, 0, 2, 0, 3);
        __m128 _RA1 = VecShuffle(t0, _A2, 1, 3, 1, 3);
        __m128 _RA2 = VecShuffle(t1, _A2, 0, 2, 2, 3);
        __m128 _RA3;
        sizeSqr =                     _mm_mul_ps(_RA0, _RA0);
        sizeSqr = _mm_add_ps(sizeSqr, _mm_mul_ps(_RA1, _RA1));
        sizeSqr = _mm_add_ps(sizeSqr, _mm_mul_ps(_RA2, _RA2));

        __m128 one = _mm_set1_ps(1.f);
        __m128 rSizeSqr = _mm_blendv_ps(_mm_div_ps(one, sizeSqr),
                                        one,
                                        _mm_cmplt_ps(sizeSqr, _mm_set1_ps(SMALL_NUMBER)));

        _RA0 = _mm_mul_ps(_RA0, rSizeSqr);
        _RA1 = _mm_mul_ps(_RA1, rSizeSqr);
        _RA2 = _mm_mul_ps(_RA2, rSizeSqr);

        _RA3 =                  _mm_mul_ps(_RA0, VecSwizzle1(_A3, 0));
        _RA3 = _mm_add_ps(_RA3, _mm_mul_ps(_RA1, VecSwizzle1(_A3, 1)));
        _RA3 = _mm_add_ps(_RA3, _mm_mul_ps(_RA2, VecSwizzle1(_A3, 2)));
        _RA3 = (_mm_setr_ps(0.f, 0.f, 0.f, 1.f), _RA3);

        return r;
#endif
      } /* End of 'Inverse' function */

    public:
      /* Matrix look-at viewer setup function. (not optimized)
       * ARGUMENTS:
       *   - viewer position, look-at point, approximate up direction:
       *       const vec3<FLT> & Loc, At, Up1;
       * RETURNS:
       *   (matr) result matrix.
       */
      static matr View( const vec3<FLT> &Loc, const vec3<FLT> &At, const vec3<FLT> Up1 )
      {
        vec3<FLT>
          Dir = (At - Loc).Normalize(),
          Right = (Dir % Up1).Normalize(),
          Up = (Right % Dir).Normalize(),
          MDir = -Dir;
        
        return
          matr(Right.X, Up.X, MDir.X, 0, 
               Right.Y, Up.Y, MDir.Y, 0,
               Right.Z, Up.Z, MDir.Z, 0,
              -(Loc & Right), -(Loc & Up), (Loc & Dir), 1);
      } /* End of 'View' function */

      /* Frustum projection matrix function.
       * ARGUMENTS:
       *   - parametrs of projection:
       *       const FLT L, R, B, T, N, F;
       * RETURNS:
       *   (matr) new matrix.
       */
      static matr Frustum( const FLT L, const FLT R, const FLT B, const FLT T, const FLT N, const FLT F )
      {
        return matr(2 * N / (R - L), 0, 0, 0,
                    0, 2 * N / (T - B), 0, 0,
                    (R + L) / (R - L), (T + B) / (T - B), -(F + N) / (F - N), -1,
                    0, 0, -2 * N * F / (F - N), 0);
      } /* End of 'Frustum' function */

      /* Ortho projection matrix function.
       * ARGUMENTS:
       *   - Parametrs of projection:
       *       FLT left (L), right (R), bottom (B), top (T), near (N), far (F);
       * RETURNS:
       *   (matr) New matrix.
       */
      static matr Ortho( FLT L, FLT R, FLT B, FLT T, FLT N, FLT F )
      {
        return matr(2 / (R - L), 0, 0, 0,
                    0, 2 / (T - B), 0, 0,
                    0, 0, -2 / (F - N), 0,
                   -(R + L) / (R - L), -(T + B) / (T - B), -(F + N) / (F - N), 1);
      } /* End of 'Ortho' function */

      /* Point radius-vector transformation by matrix function.
       * ARGUMENTS:
       *   - vector:
       *       const vec3<FLT> &V;
       * RETURNS:
       *   (vec3<FLT>) New vector.
       */
      vec3<FLT> TransformPoint( const vec3<FLT> &V ) const
      {
        return vec3<FLT>((V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0] + M[3][0]),
                         (V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1] + M[3][1]),
                         (V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2] + M[3][2]));
      } /* End of 'TransformPoint' function */

      /* Transform vector by Matrix function.
       * ARGUMENTS:
       *   - vector:
       *       const vec3<FLT> &V;
       * RETURNS:
       *   (vec3<FLT>) New Vector.
       */
      vec3<FLT> TransformVector( const vec3<FLT> &V ) const
      {
        return vec3<FLT>(V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0],
                         V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1],
                         V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2]);
      } /* End of 'TransformVector' function */

      /* Transform vector by Matrix 4 * 3 function.
       * ARGUMENTS:
       *   - vector:
       *       const vec3<FLT> &V;
       * RETURNS:
       *   (vec3<FLT>) New Vector.
       */
      vec3<FLT> TransformNormal( const vec3<FLT> &V ) const
      {
        matr m = this->Transpose().Inverse();

        return vec3<FLT>(V.X * m.M[0][0] + V.Y * m.M[1][0] + V.Z * m.M[2][0],
                         V.X * m.M[0][1] + V.Y * m.M[1][1] + V.Z * m.M[2][1],
                         V.X * m.M[0][2] + V.Y * m.M[1][2] + V.Z * m.M[2][2]);
      } /* End of 'TransformVector' function */

      /* Transform 4x4 vector function.
       * ARGUMENTS:
       *   - vector:
       *       const vec3<FLT> &V;
       * RETURNS: New Vector.
       */
      vec3<FLT> Transform4x4( const vec3<FLT> &V ) const
      {
        FLT w = V.X * M[0][3] + V.Y * M[1][3] + V.Z * M[2][3] + M[3][3];

        return vec3<FLT>((V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0] + M[3][0]) / w,
                         (V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1] + M[3][1]) / w,
                         (V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2] + M[3][2]) / w);
      } /* End of 'Transform4x4' function */

      /* Transform 4x4 vector function.
       * ARGUMENTS:
       *   - vector:
       *       const vec3<FLT> &V;
       * RETURNS: New Vector.
       */
      vec4<FLT> Transform4x4( const vec4<FLT> &V ) const
      {
        return vec4<FLT>((V.X * M[0][0] + V.Y * M[1][0] + V.Z * M[2][0] + V.W * M[3][0]),
                         (V.X * M[0][1] + V.Y * M[1][1] + V.Z * M[2][1] + V.W * M[3][1]),
                         (V.X * M[0][2] + V.Y * M[1][2] + V.Z * M[2][2] + V.W * M[3][2]),
                         (V.X * M[0][3] + V.Y * M[1][3] + V.Z * M[2][3] + V.W * M[3][3]));
      } /* End of 'Transform4x4' function */

      /* Get FLT-pointer to matrix function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (FLT *) pointer to first element
       */
      operator FLT *()
      {
        return &(this->M[0][0]);
      } /* End of 'operator FLT *' function*/

      /* Get matrix line function.
       * ARGUMENTS:
       *   - number of line:
       *       const INT Num;
       * RETURNS:
       *   (FLT *) pointer to line.
       */
      FLT * operator[]( const INT Num )
      {
        assert(Num >= 0 && Num <= 3);

        return M[Num];
      } /* End of 'operator[]' function */

      /* Get const matrix line function.
       * ARGUMENTS:
       *   - number of line:
       *       const INT Num;
       * RETURNS:
       *   (FLT *) pointer to line.
       */
      const FLT * operator[]( const INT Num ) const
      {
        assert(Num >= 0 && Num <= 3);

        return M[Num];
      } /* End of 'operator[]' function */

#undef MakeShuffleMask
#undef VecSwizzleMask
#undef VecSwizzle
#undef VecSwizzle1
#undef VecSwizzle_0022
#undef VecSwizzle_1133
#undef VecShuffle
#undef VecShuffle_0101
#undef VecShuffle_2323

#undef SMALL_NUMBER   

    }; /* End of 'matr<FLT>' class */
#endif
} /* end of 'mth' namespace */

/* Print to stdout vector data function.
 * ARGUMENTS:
 *   - out object:
 *       std::ostream & coutvar;
 *   - vector:
 *       mth::vec3<Type> &V;
 * RETURNS:
 *   (std::ostream &) out object.
 */
template<typename Type>
  inline std::ostream & operator<<( std::ostream & coutvar, mth::matr<Type> &M )
  {
    std::cout << '(';

    for (INT i = 0; i < 4; ++i)
    {
      for (INT j = 0; j < 4; ++j)
      {
        if (j == 0 && i != 0)
          std::cout << ' ';
        std::cout << M[i][j];
        if (i == 3 && j == 3)
          std::cout << ')';
        else
          std::cout << ',';
      }
      if (i != 3)
        std::cout << std::endl;
    }

    return coutvar;
  } /* End of 'operator<<' function */

#endif // !__mth_matr_h_

/* END OF 'mth_matr.h' FILE */
