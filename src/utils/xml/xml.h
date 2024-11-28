/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        xml.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 27.11.2024
 * PURPOSE:     XML parser class header file.
 * NOTE:        None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */
#pragma once

#ifndef __xml_h_
#define __xml_h_

#include "def.h"

/* State of debug */
#define XML_IS_DEBUG 0
/* Checking that XML_IS_DEBUG == 1 */
#if defined(_DEBUG) && defined(XML_IS_DEBUG) && (XML_IS_DEBUG == 1)
#  define XML_DEBUG
#endif /* XML_IS_DEBUG == 1 */

/* Base project namespace */
namespace pivk
{
  /* XML node class */
  class xml_node
  {
  public:
    std::string Name;                         // Real name of tag
    std::map<std::string, std::string> Args;  // Arguments of node
    std::vector<xml_node> SubNodes;           // Subnodes of tag
    std::string Data;                         // Data of tag

    /* Default constructor */
    xml_node() = default;

    /* XML node constructor
     * ARGUMENTS:
     *   - real name of tag:
     *       std::string &Name;
     *   - arguments of tag:
     *       std::map<std::string, std::string> &NewArgs;
     *   - subnodes of xml node:
     *       std::vector<xml_node> &NewSubNodes;
     *   - data of xml node:
     *       std::string &NewData;
     */
    xml_node( std::string &NewName,
              std::map<std::string, std::string> &NewArgs,
              std::vector<xml_node> &NewSubNodes,
              std::string &NewData ) :
      Name(NewName),
      Args(NewArgs),
      SubNodes(NewSubNodes),
      Data(NewData)
    {
    } /* End of 'xml_node' function */

    /* XML node constructor
     * ARGUMENTS:
     *   - real name of tag:
     *       std::string &NewName;
     *   - arguments of tag:
     *       std::map<std::string, std::string> &&NewArgs;
     *   - subnodes of xml node:
     *       vector<xml_node> &&NewSubNodes;
     *   - data of xml node:
     *       std::string &&NewData;
     */
    xml_node( std::string &&NewName,
              std::map<std::string, std::string> &&NewArgs,
              std::vector<xml_node> &&NewSubNodes,
              std::string &&NewData ) :
      Name(NewName),
      Args(NewArgs),
      SubNodes(NewSubNodes),
      Data(NewData)
    {
    } /* End of 'xml_node' function */

    /* Find element in array of subnodes function.
     * ARGUMENTS:
     *   - name of tag:
     *       const std::string &TagName;
     * RETURNS:
     *   (xml_node *) nullptr, if element not finded, or pointer to element.
     */
    xml_node * FindElem( const std::string &TagName );

    /* Find element in array of subnodes function.
     * ARGUMENTS:
     *   - array with elements:
     *       std::vector<xml_node *> &Nodes;
     *   - name of tag:
     *       const std::string &TagName;
     * RETURNS: None.
     */
    VOID FindElems( std::vector<xml_node *> &NodesPtrs, const std::string &TagName );

    /* XML node destructor */
    ~xml_node()
    {
    } /* End of '~xml_node' function */

  }; /* End of 'xml_node' class */

  /* XML tree class */
  class xml_tree
  {
  public:
    std::vector<xml_node> Nodes; // Nodes of XML

    /* Default constructor */
    xml_tree()
    {
    } /* End of 'xml_tree' function */

    /* XML tree constructor.
     * ARGUEMENTS:
     *   - nodes of XML tree:
     *       std::map<std::string, xml_node> &NewNodes;
     * RETURNS: None.
     */
    xml_tree( std::vector<xml_node> &&NewNodes ) :
      Nodes(NewNodes)
    {
    }
  }; /* End of 'xml_tree' class */

  /* XML class */
  class xml_parser
  {
  private:
    INT CurDepth = -1; // Current depth of parse

  public:
    xml_tree Tree;     // XML parsed tree
  
    /* XML parser constructor.
     * ARGUMENTS:
     *   - filename:
     *       const std::string &Filename;
     */
    xml_parser( const std::string &Filename );
    
    /* Skip first string function.
     * ARGUMENTS:
     *   - reference to pointer to char buffer:
     *       CHAR *&Txt;
     * RETURNS: None.
     */
    static VOID SkipFirstString( CHAR *&Txt );
    
    /* Skip spaces function.
     * ARGUMENTS:
     *   - reference to pointer to char buffer:
     *       CHAR *&Txt;
     * RETURNS: None.
     */
    static VOID SkipSpaces( CHAR *&Txt );
    
    /* Print spaces function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID PrintSpaces( VOID );

    /* XML parse node function
     * ARGUMENTS:
     *   - text variable:
     *       CHAR *&Txt;
     *   - reference to array with nodes:
     *       std::vector<xml_node> &Nodes;
     * RETURNS: None.
     */
    VOID ParseNode( CHAR *&Txt, std::vector<xml_node> &Nodes );

    /* Default destructor */
    ~xml_parser()
    {
    } /* End of '~xml_parser' function */

  }; /* End of 'xml_parser' class */
} /* end of 'pivk' namespace */

#endif /* !__xml_h_ */

/* END OF 'xml.h' FILE */
