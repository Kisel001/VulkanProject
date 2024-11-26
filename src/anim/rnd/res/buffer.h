/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

 /* FILE:        buffer.h
  * PROGRAMMER:  IP5
  * LAST UPDATE: 27.10.2024
  * PURPOSE:     Texture class header file.
  * NOTE:        None.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum.
  */

#ifndef __buffer_h_
#define __buffer_h_

//#include "res.h"
#include "shader.h"

/* Base project namespace */
namespace pivk
{
  // System UBO bind points
  auto constexpr BufferBindCamera = 0;
  auto constexpr BufferBindSync = 1;
  auto constexpr BufferBindMaterial = 2;
  auto constexpr BufferBindPrim = 3;
  auto constexpr BufferBindFirstFree = 4;

  /* Buffer class */
  class buffer : public resource
  {
  public:
    // Name of buffer
    std::string Name {};

    // OpenGL buffer Id
    UINT BufId = 0;

    // Binding point
    UINT BindingPoint = 0;

    // Number of buffer quads
    UINT BufSize = 0;

    /* Buffer create function.
     * ARGUMENTS:
     *   - name of buffer:
     *       const std::string &NewName;
     *   - buffer binding point:
     *       UINT BufBindingPoint;
     *   - buffer elements count:
     *       UINT CountOfElems;
     *   - buffer data:
     *       const data_type *Data;
     *   - new render:
     *       render *NewRnd;
     * RETURNS:
     *   (buffer &) self reference.
     */
    template<typename data_type>
      buffer & Create( const std::string &NewName, UINT BufBindingPoint, UINT CountOfElems = 1, const data_type *Data = {}, render *NewRnd = nullptr )
      {
        Free();
        if (NewRnd != nullptr)
          Rnd = NewRnd;

        Name = NewName;
        BufSize = sizeof(data_type);
        if (BufSize == 0)
          return *this;

        BindingPoint = BufBindingPoint;

        Apply();
        return *this;
      } /* End of 'Create' function */

    /* Buffer deleting function.
     * ARUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID )
    {
    } /* End of 'Free' function */

    /* Buffer update function.
     * ARGUMENTS:
     *   - buffer data:
     *       const data_type *Data;
     *   - start of elements update:
     *       INT Start;
     *   - length of elements, which will be updated:
     *       INT Size;
     * RETURNS: None.
     */
    template<typename data_type>
      VOID Update( const data_type *Data, INT Start = 0, INT Size = -1 )
      {
        if (BufId == 0)
          return;
        if (Size < 0)
          Size = BufSize;
        if (Start < 0)
          Start = 0;
        if (Start + Size > BufSize)
          Size = BufSize - Start;
      } /* End of 'Update' finction */

    /* Apply buffer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Apply( VOID )
    {
    } /* End of 'Apply' function */
  }; /* End of 'buffer' class */

  /* Buffer manager class */
  class buffer_manager : public resource_manager<buffer, std::string>
  {
  public:
    /* Buffer manager constructor.
     * ARGUMENTS:
     *   - ref to render:
     *       render &Rnd;
     */
    buffer_manager( render &Rnd ) : resource_manager(Rnd)
    {
    } /* End of 'buffer_manager' function */

    /* Buffer create function.
     * ARGUMENTS:
     *   - name:
     *       const std::string &Name;
     *   - buffer binding point:
     *       UINT BufBindingPoint;
     *   - number of data elements:
     *       INT NumOfElements;
     *   - buffer data:
     *       const data_type *Data;
     * RETURNS:
     *   (buffer *) self reference.
     */
    template<typename data_type>
      buffer * BufCreate( const std::string &Name, UINT BufBindingPoint, INT NumOfElements = 1, const data_type *Data = {} )
      {
        return resource_manager::Add(buffer().Create<data_type>(Name, BufBindingPoint, NumOfElements, Data, &RndRef));
      } /* End of 'BufCreate' function */

    /* Buffer update function.
     * ARGUMENTS:
     *   - name:
     *       const std::string &Name;
     *   - buffer data:
     *       const data_type *Data;
     *   - start of elements update:
     *       INT Start;
     *   - length of elements, which will be updated:
     *       INT Size;
     * RETURNS: None.
     */
    template<typename data_type>
      VOID BufUpdate( const std::string &Name, const data_type* Data, INT Start = 0, INT Size = -1 )
      {
        if (auto *ptr = resource_manager::Find(Name); ptr != nullptr)
          ptr->Update(Data, Start, Size);
      } /* End of 'BufUpdate' function */

    /* Apply buffer function.
     * ARGUMENTS:
     *   - name:
     *       const std::string &Name;
     * RETURNS: None.
     */
    VOID BufApply( const std::string &Name )
    {
      if (auto *ptr = resource_manager::Find(Name); ptr != nullptr)
        ptr->Apply();
    } /* End of 'BufApply' function */
  }; /* End of 'buffer_manager' class */
} /* end of 'pivk' namespace */

#endif // !__buffer_h_

/* END OF 'buffer.h' FILE */
