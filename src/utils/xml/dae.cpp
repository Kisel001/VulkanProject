/***************************************************************
* Copyright (C) 1992-2024
*    Computer Graphics Support Group of 30 Phys-Math Lyceum
***************************************************************/

/* FILE:        dae.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 27.11.2024
 * PURPOSE:     XML parser class header file.
 * NOTE:        None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Load geometry function.
   * ARGUMENTS:
   *   - reference to array with primitives:
   *       std::vector<std::pair<std::vector<vertex::std>, std::vector<INT>>> &PrimsData;
   * RETURNS: None.
   */
  VOID dae::LoadGeometry( std::vector<std::pair<std::vector<vertex::std>, std::vector<INT>>> &PrimsData )
  {
    //std::setlocale(LC_ALL, "en_US.UTF-8");

    enum DaeGeomType
    {
      Float = 1,   // 1 float
      Vector2 = 2, // 2D vector (2 floats)
      Vector3 = 3, // 3D vector (3 floats)
      Vector4 = 4, // 4D vector (4 floats)
      Matrix3x3 = 9,  // Matrix 3x3 (9 floats)
      Matrix3x4 = 12, // Matrix 3x4 (12 floats)
      Matrix4x4 = 16, // Matrix 4x4 (16 floats)
    };

    if (Parser.Tree.Nodes.empty())
      return;

    xml_node *Node = &Parser.Tree.Nodes[0];
    xml_node *Find = Node;

    if ((Find = Find->FindElem("library_geometries")) == nullptr)
      return;

    std::vector<xml_node *> Geoms;
    Geoms.reserve(20);
    Find->FindElems(Geoms, "geometry");

    for (auto Geom : Geoms)
    {
      std::map<std::string, std::pair<std::vector<FLOAT>, DaeGeomType>> SourcesArray;
      std::map<std::string, std::string *> VerticesArray;

      xml_node *Find = Geom;

      if ((Find = Find->FindElem("mesh")) == nullptr)
        continue;

      std::vector<xml_node *> Sources;
      Sources.reserve(20);
      Find->FindElems(Sources, "source");

      // Loading sourcses
      for (auto Source : Sources)
      {
        std::pair<std::vector<FLOAT>, DaeGeomType> SourceData;
        std::string AccessName;
        xml_node *Find = Source;

        if ((Find = Source->FindElem("float_array")) == nullptr)
          continue;

        std::string StrSize = Find->Args["count"];
        //CHAR *tmp;
        UINT32 Size = strtoul(StrSize.c_str(), nullptr, 10);

        SourceData.first.resize(UINT_PTR(Size));
        const CHAR *src = Find->Data.c_str();
        CHAR *tmp;

        for (auto &i : SourceData.first)
        {
          i = strtof(src, &tmp);
          if (tmp == nullptr || tmp == src)
            break;
          src = tmp;
          //src = tmp + 1;
        }

        if ((Find = Source->FindElem("technique_common")) == nullptr)
          continue;

        if ((Find = Find->FindElem("accessor")) == nullptr)
          continue;

        AccessName = '#' + Source->Args["id"];
        //AccessName = Find->Args["source"];
        src = Find->Args["stride"].c_str();

        UINT32 Stride = strtol(src, nullptr, 10);
        SourceData.second = Stride == 2 ? Vector2 : Stride == 3 ? Vector3 : Stride == 4 ? Vector4 : Float;
        SourcesArray[AccessName] = std::move(SourceData);
      }

      std::vector<xml_node *> Vertices;
      Vertices.reserve(20);
      Find->FindElems(Vertices, "vertices");

      // Get vertices
      for (auto i : Vertices)
      {
        xml_node *Input;
        std::string Access = "#" + i->Args["id"];

        if ((Input = i->FindElem("input")) == nullptr)
          continue;

        VerticesArray[Access] = &Input->Args["source"];
      }

      std::vector<xml_node *> TrianglesArray;
      TrianglesArray.reserve(5);

      Find->FindElems(TrianglesArray, "triangles");
      //if ((Triangles = Find->FindElem("triangles")) == nullptr)
      //  continue;

      for (auto &Triangles : TrianglesArray)
      {
        std::pair<std::vector<vertex::std>, std::vector<INT>> VerticesFinal;

        std::vector<xml_node *> InputFormat;
        InputFormat.reserve(20);
        Triangles->FindElems(InputFormat, "input");

        enum format
        {
          Vertex = 0, // "VERTEX"
          Normal = 1, // "NORMAL"
          Tc = 2,     // "TEXCOORD"
          Color = 3,  // "COLOR"
          Unknown = 4, // "Unknown"
        };

        std::vector<std::pair<format, std::string *>> Format;
        Format.resize(10);
        UINT FormatSize = 0;

        // Get input format
        for (auto i : InputFormat)
        {
          std::string &Semantic = i->Args["semantic"];

          Format[FormatSize].second = &i->Args["source"];

          if (Semantic == "VERTEX")
            Format[FormatSize++].first = format::Vertex;
          else if (Semantic == "NORMAL")
            Format[FormatSize++].first = format::Normal;
          else if (Semantic == "TEXCOORD")
            Format[FormatSize++].first = format::Tc;
          else
            Format[FormatSize++].first = format::Unknown;
        }

        std::vector<INT> P;

        P.reserve(1000);

        xml_node *PSrc;

        if ((PSrc = Triangles->FindElem("p")) == nullptr)
          continue;

        // Get count of triangles
        UINT Count = strtol((Triangles->Args["count"]).c_str(), nullptr, 10);

        //const CHAR *

        // 
        VerticesFinal.first.resize(Count);
        VerticesFinal.second.resize(Count);

        UINT CountOfI = 0;

        // Fill index array
        for (auto &i : VerticesFinal.second)
        {
          i = CountOfI++;
        }

        const CHAR *src = PSrc->Data.c_str();
        CHAR *tmp;

        //std::vector<ivec3> Indexes;
        //Indexes.resize(Count * 3);
        //
        //for (INT j = 0; j < Count * 3; j++)
        //{
        //  Indexes[j].X = strtol(src, &tmp, 10);
        //  src = tmp;
        //  Indexes[j].Y = strtol(src, &tmp, 10);
        //  src = tmp;
        //  Indexes[j].Z = strtol(src, &tmp, 10);
        //  src = tmp;
        //}
        //
        //src = PSrc->Data.c_str();

        // Parse array with ids
        for (INT i = 0; i < Count; i++)
        {
          for (INT j = 0; j < FormatSize; j++)
          {
            LONG Index = strtol(src, &tmp, 10);

            if (tmp == nullptr || tmp == src)
            {
              __debugbreak();
            }
            else
            {
              src = tmp;
            }

            switch (Format[j].first)
            {
            case format::Vertex:
              VerticesFinal.first[i].P.X = SourcesArray[*VerticesArray[*Format[j].second]].first[3 * Index];
              VerticesFinal.first[i].P.Y = SourcesArray[*VerticesArray[*Format[j].second]].first[3 * Index + 1];
              VerticesFinal.first[i].P.Z = SourcesArray[*VerticesArray[*Format[j].second]].first[3 * Index + 2];
              break;

            case format::Normal:
              VerticesFinal.first[i].N.X = SourcesArray[*Format[j].second].first[3 * Index];
              VerticesFinal.first[i].N.Y = SourcesArray[*Format[j].second].first[3 * Index + 1];
              VerticesFinal.first[i].N.Z = SourcesArray[*Format[j].second].first[3 * Index + 2];
              break;

            case format::Tc:
              //VerticesFinal.first[i].T.X = SourcesArray[*Format[j].second].first[2 * Index];
              //VerticesFinal.first[i].T.Y = SourcesArray[*Format[j].second].first[2 * Index + 1];
              break;

            case format::Color:
              break;

            case format::Unknown:
              break;
            }
          }
        }

        PrimsData.push_back(std::move(VerticesFinal));
      }
    }
    //if ((Find = Find->FindElem("Asset")) == nullptr)
    //  return;

    //std::setlocale(LC_ALL, "ru-RU");
  } /* End of 'LoadGeometry' function */
} /* end of 'pivk' namespace */

/* END OF 'dae.cpp' FILE */
