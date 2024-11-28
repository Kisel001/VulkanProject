/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        pipeline_pattern.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 28.11.2024
 * PURPOSE:     Pipeline pattern file.
 * NOTE:        None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */
#pragma once

#ifndef __pipeline_pattern_h_
#define __pipeline_pattern_h_

#include "res.h"

/* Base project namespace */
namespace pivk
{
  /* Get vertex attribute description function.
   * ARGUMENTS:
   *   - location:
   *       UINT32 Location;
   *   - vulkan format:
   *       VkFormat Format;
   *   - offset to this attribute:
   *       UINT32 Offset;
   *   - binding number of this offset (default = 0):
   *       UINT32 Binding;
   * RETURNS:
   *   (VkVertexInputAttributeDescription) vertex attribute description structure.
   */
  inline VkVertexInputAttributeDescription GetAttributeDescription( UINT32 Location, VkFormat Format, UINT32 Offset, UINT32 Binding = 0 )
  {
    // Attribute description structure
    VkVertexInputAttributeDescription AttributeDescrition
    {
      .location = Location, // Location
      .binding = Binding,   // Binding
      .format = Format,     // Vulkan format
      .offset = Offset,     // Offset
    };

    return AttributeDescrition;
  } /* End of 'VkVertexInputAttributeDescription' function */

  /* Pipeline pattern class */
  class pipeline_pattern : public resource
  {
  public:
    // Vertex format array
    std::map<std::string, VkVertexInputAttributeDescription> VertexFormat;

    /* Pipeline layout constructor */
    pipeline_pattern()
    {
    } /* End of 'pipeline_pattern' class */

    /* Add attribute function.
     * ARGUMENTS:
     *   - name of this attribute:
     *       const std::string &Name;
     *   - vertex input attribute description:
     *       VkVertexInputAttributeDescription &Description;
     * RETURNS: None.
     */
    VOID Add( const std::string &Name, const VkVertexInputAttributeDescription &Description )
    {
      VertexFormat[Name] = Description;
    } /* End of 'Add' function */

  }; /* End of 'pipeline_pattern' class */

  /* Pipeline pattern manager class */
  class pipeline_pattern_manager : public resource_manager<pipeline_pattern, std::string>
  {
  public:
    /* Pipeline pattern manager constructor.
     * ARGUMENTS:
     *   - ref to render:
     *       render &Rnd;
     */
    pipeline_pattern_manager( render &Rnd ) : resource_manager(Rnd)
    {
    } /* End of 'material_manager' function */

  private:
    /* Create pipeline pattern function.
     * ARGUMENTS:
     *   - name of pipeline pattern:
     *       std::string &Name;
     * RETURNS: None.
     */
    pipeline_pattern * PipelinePatternAdd( const std::string &Name )
    {
      return &(Stock[Name] = pipeline_pattern());
    } /* End of 'pipeline_pattern' function */

  public:
    /* Find pipeline pattern */
    pipeline_pattern * PipelinePatternFind( const std::string &Name )
    {
      auto i = Stock.find(Name);

      if (i == Stock.end())
        return PipelinePatternAdd(Name);
      return &Stock[Name];
    } /* End of 'PipelinePatternFind' function */
  }; /* End of 'pipeline_pattern_manager' class */

} /* end of 'pivk' namespace */

#endif /* !__pipeline_pattern_h_ */

/* END OF '__pipeline_pattern_h_' FILE */
