/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        def.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 30.07.2024
 * PURPOSE:     Base defines header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __def_h_
#define __def_h_


/***
 * DEBUG support
 ***/
#define T53VLK_DEBUG_MODE_IS_ACTIVE 1

#if (T53VLK_DEBUG_MODE_IS_ACTIVE == 1)
   /* Enable debug mode for application*/
#  define T53VLK_DEBUG_MODE
#endif // T53VLK_DEBUG_MODE_IS_ACTIVE == 1

#ifdef _MSC_VER
/* Debug memory allocation support */
#  if (defined _DEBUG) && (defined T53VLK_DEBUG_MODE)
#    define _CRTDBG_MAP_ALLOC
#    include <crtdbg.h>
static struct __Dummy
{
  /* Structure constructor */
  __Dummy( void )
  {
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF /* | _CRTDBG_CHECK_ALWAYS_DF | \
      _CRTDBG_ALLOC_MEM_DF*/ | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
  } /* End of '__Dummy' constructor */
} __oops;
     /* Reinterpret operator new (set debug new) */
#    define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#  endif /* _DEBUG && T53VLK_DEBUG_MODE */

  /* Disable warnings:
   *   - 6031:
   *       no using return values (useless);
   *   - 26451:
   *       arithmetic overflow (useless);
   *   - 26495:
   *       not inited element of class (useless);
   *   - 28251:
   *       WinMain (?) (useless);
   */
#  pragma warning(disable: 6031 26451 26495 28251 6385 6386 26451 26812 28182)
#endif // _MSC_VER

#include "mth/mth.h"
#include "utils/console.h"

/***
 * C standart library header files
 ***/
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <ctime>
#include <cctype>
#include <clocale>

/***
 * C++ STL library header files
 ***/

/* Algorithms files */
#include <algorithm>
#include <format>
#include <filesystem>
#include <tuple>
#include <optional>

/* Containers header files */
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <array>
#include <initializer_list>

/* Streams header files */
#include <fstream>
#include <iostream>

/* Strings header files */
#include <string>
#include <string_view>

/* Mulitcore header file */
#include <thread>

/* Declare x64 integer data types */
typedef unsigned long long UINT64;
typedef long long INT64;
typedef unsigned long long QWORD;

/* Base project namespace */
namespace pivk
{
  /* Stock class */
  template<typename T>
    class stock : public std::vector<T>
    {
    public:
      /* Put to stock element function.
       * ARGUMENTS:
       *   - element:
       *       const T &X;
       * RETURNS:
       *   (stock &) this stock.
       */
      stock & operator<<( const T &X )
      {
        this->push_back(X);
        return *this;
      } /* End of 'operator<<' function */

      /* Walk throw stock.
       * ARGUMENTS:
       *   - structure with method for walk:
       *       WalkType Walk;
       * RETURNS: None.
       */
      template<class WalkType>
        void Walk( WalkType Walk )
        {
          for (auto &x : *this)
            Walk(x);
        } /* End of 'Walk' function */
    }; /* End of 'stock' class */

  /* Vec2 declare types */
  typedef mth::vec2<FLT>  vec2;
  typedef mth::vec2<FLT>  fvec2;
  typedef mth::vec2<DBL>  dvec2;
  typedef mth::vec2<INT>  ivec2;
  typedef mth::vec2<BOOL> bvec2;

  /* Vec3 declare types */
  typedef mth::vec3<FLT>  vec3;
  typedef mth::vec3<FLT>  fvec3;
  typedef mth::vec3<DBL>  dvec3;
  typedef mth::vec3<INT>  ivec3;
  typedef mth::vec3<BOOL> bvec3;

  /* Vec4 declare types */
  typedef mth::vec4<FLT>  vec4;
  typedef mth::vec4<FLT>  fvec4;
  typedef mth::vec4<DBL>  dvec4;
  typedef mth::vec4<INT>  ivec4;
  typedef mth::vec4<BOOL> bvec4;

  /* Matrix declare types */
  typedef mth::matr<FLT>  matr;
  typedef mth::matr<FLT>  fmatr;
  typedef mth::matr<DBL>  dmatr;
  typedef mth::matr<INT>  imatr;
  typedef mth::matr<BOOL> bmatr;

  typedef mth::camera<FLT> camera;
} /* end of 'pirt' namespace */


#endif // !__def_h_

/* END OF 'def.h' FILE */
