/***************************************************************
* Copyright (C) 1992-2024
*    Computer Graphics Support Group of 30 Phys-Math Lyceum
***************************************************************/

/* FILE:        dae.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 21.11.2024
 * PURPOSE:     XML parser class header file.
 * NOTE:        None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */
#pragma once

#ifndef __dae_h_
#define __dae_h_

#include "xml.h"
#include "anim/rnd/res/topology.h"

/* Base project namespace */
namespace pivk
{
  /* DAE class */
  class dae
  {
  public:
    xml_parser Parser; // XML parser

    /*  */

    /* XML Constructor.
     * ARGUMENTS:
     *   - filename:
     *       const std::string &Name;
     */
    dae( const std::string &Name ) : Parser(Name)
    {
    } /* End of 'dae' function */

    /* Load geometry function.
     * ARGUMENTS:
     *   - reference to array with primitives:
     *       std::vector<std::pair<std::vector<vertex::std>, std::vector<INT>>> &PrimsData;
     * RETURNS: None.
     */
    VOID LoadGeometry( std::vector<std::pair<std::vector<vertex::std>, std::vector<INT>>> &PrimsData );

    /* Default destructor */
    ~dae()
    {
    } /* End of '~dae' function */

  }; /* End of 'dae' class */

} /* end of 'pivk' namespace */

#endif /* __dae_h_ */

/* END OF 'dae.h' FILE */
