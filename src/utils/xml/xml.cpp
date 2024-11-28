/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        xml.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 27.11.2024
 * PURPOSE:     XML parser class file.
 * NOTE:        None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Find element in array of subnodes function.
   * ARGUMENTS:
   *   - name of tag:
   *       const std::string &TagName;
   * RETURNS:
   *   (xml_node *) nullptr, if element not finded, or pointer to element.
   */
  xml_node * xml_node::FindElem( const std::string &TagName )
  {
    for (auto& i : SubNodes)
      if (i.Name == TagName)
        return &i;

    return nullptr;
  } /* End of 'xml_node::FindElem' function */

  /* Find element in array of subnodes function.
     * ARGUMENTS:
     *   - array with elements:
     *       std::vector<xml_node *> &Nodes;
     *   - name of tag:
     *       const std::string &TagName;
     * RETURNS: None.
     */
  VOID xml_node::FindElems( std::vector<xml_node*> &NodesPtrs, const std::string &TagName )
  {
    for (auto& i : SubNodes)
      if (i.Name == TagName)
        NodesPtrs.push_back(&i);
  } /* End of 'xml_node::FindElems' function */

  /* XML parser constructor.
   * ARGUMENTS:
   *   - filename:
   *       const std::string &Filename;
   */
  xml_parser::xml_parser( const std::string &Filename )
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
  } /* End of 'xml_parser::xml_parser' function */

  /* Skip first string function.
   * ARGUMENTS:
   *   - reference to pointer to char buffer:
   *       CHAR *&Txt;
   * RETURNS: None.
   */
  VOID xml_parser::SkipFirstString( CHAR *&Txt )
  {
    while (*Txt++ != '\n')
      ;
  } /* End of 'xml_parser::SkipFirstString' function */

  /* Skip spaces function.
   * ARGUMENTS:
   *   - reference to pointer to char buffer:
   *       CHAR *&Txt;
   * RETURNS: None.
   */
  VOID xml_parser::SkipSpaces( CHAR *&Txt )
  {
    while (isspace(*Txt) != 0 && *Txt != 0)
      ++Txt;
  } /* End of 'xml_parser::SkipSpaces' function */

  /* Print spaces function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID xml_parser::PrintSpaces( VOID )
  {
    if (CurDepth > 0)
      printf("%*c", CurDepth * 2, ' ');
  } /* End of 'xml_parser::PrintSpaces' function */

  /* XML parse node function
   * ARGUMENTS:
   *   - text variable:
   *       CHAR *&Txt;
   *   - reference to array with nodes:
   *       std::vector<xml_node> &Nodes;
   * RETURNS: None.
   */
  VOID xml_parser::ParseNode( CHAR *&Txt, std::vector<xml_node> &Nodes )
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
  } /* End of 'xml_parser::ParseNode' function */

} /* end of 'pivk' namespace */

/* END OF 'xml.cpp' FILE */