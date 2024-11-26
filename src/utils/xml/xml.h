/***************************************************************
* Copyright (C) 1992-2024
*    Computer Graphics Support Group of 30 Phys-Math Lyceum
***************************************************************/

/* FILE:        xml.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 21.11.2024
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
    xml_node * FindElem( const std::string &TagName )
    {
      for (auto &i : SubNodes)
        if (i.Name == TagName)
          return &i;

      return nullptr;
    } /* End of 'FindElem' function */

    /* Find element in array of subnodes function.
     * ARGUMENTS:
     *   - array with elements:
     *       std::vector<xml_node *> &Nodes;
     *   - name of tag:
     *       const std::string &TagName;
     * RETURNS: None.
     */
    VOID FindElems( std::vector<xml_node *> &NodesPtrs, const std::string &TagName )
    {
      for (auto &i : SubNodes)
        if (i.Name == TagName)
          NodesPtrs.push_back(&i);
    } /* End of 'FindElems' function */

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
    xml_parser( const std::string &Filename )
    {
      FILE *F;
    
      /* Open file */
      if ((F = std::fopen(Filename.c_str(), "rb")) == nullptr)
        return;
    
      /* Read file */
      std::vector<CHAR> Txt;
      std::fseek(F, 0, SEEK_END);
      INT size = std::ftell(F);
      Txt.resize(UINT_PTR(size + 1));
    
      std::fseek(F, 0, SEEK_SET);
      std::fread(Txt.data(), 1, size, F);
    
      Txt[size] = 0;
      fclose(F);
    
      CHAR *ptr = Txt.data();
    
      /* Skip first string */
      SkipFirstString(ptr);
    
      std::vector<xml_node> Nodes;
      Nodes.reserve(20);
    
      /* Parse tree */
      ParseNode(ptr, Nodes);
    
      Tree.Nodes = std::move(Nodes);
    } /* End of 'xml_parser' function */
    
    /* Skip first string function.
     * ARGUMENTS:
     *   - reference to pointer to char buffer:
     *       CHAR *&Txt;
     * RETURNS: None.
     */
    static VOID SkipFirstString( CHAR *&Txt )
    {
      while (*Txt++ != '\n')
        ;
    } /* End of 'SkipFirstString' function */
    
    /* Skip spaces function.
     * ARGUMENTS:
     *   - reference to pointer to char buffer:
     *       CHAR *&Txt;
     * RETURNS: None.
     */
    static VOID SkipSpaces( CHAR *&Txt )
    {
      while (isspace(*Txt) != 0 && *Txt != 0)
        ++Txt;
    } /* End of 'SkipSpaces' function */
    
    /* Print spaces function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID PrintSpaces( VOID )
    {
      if (CurDepth > 0)
        printf("%*c", CurDepth * 2, ' ');
    } /* End of 'PrintSpaces' function */

    /* XML parse node function
     * ARGUMENTS:
     *   - text variable:
     *       CHAR *&Txt;
     *   - reference to array with nodes:
     *       std::vector<xml_node> &Nodes;
     * RETURNS: None.
     */
    VOID ParseNode( CHAR *&Txt, std::vector<xml_node> &Nodes )
    {
      CurDepth++;
      SkipSpaces(Txt);
      
      // Nodes of this level
      while (*Txt == '<')
      {
        Txt++;
      
        // Leave flag
        if (*Txt == '/')
        {
          while (*Txt++ != '>')
            ;
          break;
        }
      
        std::string TagName;
        std::map<std::string, std::string> Args;
      
        TagName.reserve(1000);
      
        // Read name
        while (isspace(*Txt) == 0 && *Txt != '>' && *Txt != '/')
          TagName.push_back(*Txt++);
      
        if (*Txt != '>' && *Txt != '/')
        {
          // Read tag arguments
          while (*Txt != '>' && *Txt != '/')
          {
            // Skip space
            Txt++;
      
            /// Read tag argument
            std::string ArgName;
            std::string ArgParam;
      
            ArgName.reserve(1000);
            ArgParam.reserve(1000);
      
            // Read argument name
            while (*Txt != '=')
              ArgName.push_back(*Txt++);
      
            // Skip '=' and '"'
            Txt += 2;

            // Read argument parameter
            while (*Txt != '"')
              ArgParam.push_back(*Txt++);
      
            // Skip '"'
            Txt++;
      
            Args[ArgName] = ArgParam;
          }
        }
      
#ifdef XML_DEBUG
        PrintSpaces();
        printf("%s\n", TagName.c_str());
#endif

        // One tag
        if (*Txt == '/')
        {
          Txt += 2;
          SkipSpaces(Txt);
          Nodes.push_back(std::move(xml_node(std::move(TagName), std::move(Args), std::vector<xml_node>(), std::string())));
      
          continue;
        }
      
        Txt++;
        SkipSpaces(Txt);
      
        std::string Data;
        Data.reserve(1000);
      
        // Tag data
        while (*Txt != '<')
          Data.push_back(*Txt++);
      
#ifdef XML_DEBUG
        PrintSpaces();
        printf("{\n");
#endif

        // Subnodes of this node
        std::vector<xml_node> SubNodes;

        SubNodes.reserve(20);
      
        // New element
        if (*Txt == '<')
          ParseNode(Txt, SubNodes);
      
        Nodes.push_back(std::move(xml_node(std::move(TagName), std::move(Args), std::move(SubNodes), std::move(Data))));

#ifdef XML_DEBUG
        PrintSpaces();
        printf("}\n");
#endif

        SkipSpaces(Txt);
      }

      CurDepth--;
    } /* End of 'ParseNode' function */

    /* Default destructor */
    ~xml_parser()
    {
    } /* End of '~xml_parser' function */

  }; /* End of 'xml_parser' class */
} /* end of 'pivk' namespace */

#endif /* !__xml_h_ */

/* END OF 'xml.h' FILE */
