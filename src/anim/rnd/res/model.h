/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        model.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 27.10.2024
 * PURPOSE:     Model resource class header file.
 * NOTE:        None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __model_h_
#define __model_h_

#include "prim.h"

/* Base project namespace */
namespace pivk
{
  /* Unstructured data file reader class */
  class rdr
  {
  private:
    // Memory pointer reference
    BYTE *&Ptr;

  public:
    /* Class constructor.
     * ARGUMENTS:
     *   - reference to memory pointer:
     *       BYTE *&PtrRef;
     */
    rdr( BYTE *&PtrRef ) : Ptr(PtrRef)
    {
    } /* End of 'rdr' function */

    /* Read data function.
     * ARGUMENT:
     *   - read data pointer:
     *       type *Data;
     *   - read data count:
     *       INT Count;
     * RETURNS: None.
     */
    template<typename type>
      VOID operator()( type *Data, const INT Count = 1 )
      {
        if (Count == 1)
          *Data = *(type *)Ptr, Ptr += sizeof(type);
        else
          memcpy(Data, Ptr, sizeof(type) * Count), Ptr += sizeof(type) * Count;
      } /* End of 'operator()' function */
  }; /* End of 'rdr' class */

  /* Model class */
  class model : public resource
  {
  public:
    std::vector<prim *> Prims; // Array with primitives.
    matr Transform;            // Trans matrix of model.
    std::string Name;          // String
    vec3 
      Min, Max;                // Bound box
    //rdr rd;                    // Reader

    /* Default constructor */
    model()
    {
    } /* End of 'model' functions */

    /* Default destructor */
    ~model()
    {
    } /* End of '~model' function */

    /* Load g3dm model function
     * ARGUMENTS:
     *   - name of file with model:
     *       const std::string &Filename;
     *   - render:
     *       render *NewRnd;
     * RETURNS:
     *   (model &) reference to this model.
     */
    model & LoadG3DM( const std::string &Filename, render *NewRnd = nullptr );

    /* Draw model function.
     * ARGUMENTS:
     *   - world matrix:
     *       const matr &World;
     * RETURNS: None.
     */
    VOID Draw( const matr &World = matr::Identity() );

    /* Free model function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID )
    {
      for (auto &i : Prims)
        i->Free();
    } /* End of 'Free' function */

  }; /* End of 'model' class */

  /* Model manager class */
  class model_manager : public resource_manager<model, std::string>
  {
  public:
    /* Model manager constructor.
     * ARGUMENTS:
     *   - ref to render:
     *       render &Rnd;
     */
    model_manager( render &Rnd ) : resource_manager(Rnd)
    {
    } /* End of 'model_manager' function */

    /* Load model from g3dm file function.
     * ARGUMENTS:
     *   - filename:
     *       const std::string &Filename;
     * RETURNS:
     *   (model *) new model.
     */
    model * ModelLoadG3DM( const std::string &Filename )
    {
      return resource_manager::Add(model().LoadG3DM(Filename, &RndRef));
    } /* End of 'ModelLoadG3DM' function */

    /* Delete model function.
     * ARGUMENTS:
     *   - pointer to model:
     *       model *Model;
     * RETURNS: None.
     */
    VOID ModelFree( model *Model )
    {
      Model->Free();
      resource_manager::Delete(Model);
    } /* End of 'ModelFree' function */

  }; /* End of 'model_manager' class */
} /* end of 'pivk' namespace */

#endif // !__model_h_

/* END OF 'model.h' FILE */
