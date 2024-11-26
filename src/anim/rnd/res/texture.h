/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        texture.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 13.09.2024
 * PURPOSE:     Texture class header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __texture_h_
#define __texture_h_

#include "res.h"
#include "image.h"

/* Space aigl namespace*/
namespace pivk
{ 
  /* Texture class */
  class texture : public resource
  {
  public:
    std::string Name;                         // Texture name
    INT TexW, TexH;                           // Texture size
    //UINT TexId;                             // Texture id
    //BOOL IsCubeMap = FALSE;                 // Is cube map flag
    VkImage Image;                            // Vulkan image
    VkImageView ImageView;                    // Vulkan image view
    VkSampler Sampler;                        // Vulkan sampler
    VmaAllocation Allocation;                 // AMD VMA allocate for image
    VkDescriptorSet TextureDescriptorSets[9]; // Vulkan texture descriptor set
    BOOL IsUpdated = FALSE;                   // Information about updat
    
    /* Default constructor */
    texture() : Name()
    {
    } /* End of 'texture' function */

    /* Constructor by name of texture.
     * ARGUMENTS: 
     *   - name texture:
     *       const std::string &Name;
     * RETURNS: None.
     */
    texture( const std::string &Name ) : Name(Name)
    {
    } /* End of 'texture' function */

    /* Texture add from image function.
     * ARGUMENTS: 
     *   - texture size in pixels:
     *       INT W, INT H;
     *   - number of components per point:
     *       INT C;
     *   - pointer to an array with components:
     *       VOID *Bits;
     *   - render:
     *       render *NewRnd;
     * RETURNS:
     *   (texture &) result texture.
     */
    texture & TexAddImg( INT W, INT H, INT C, const VOID *Bits, render *NewRnd = nullptr );

    // TODO:
    // ???::
    /* Add texture by OpenGL low-level format to stock function.
     * ARGUMENTS: 
     *   - texture size in pixels:
     *       INT W, H;
     *   - OpenGL texture element data type:
     *       INT GLType;
     *   - render:
     *       render *NewRnd;
     * RETURNS:
     *   (texture &) result texture.
     */
    texture & TexAddFmt( INT W, INT H, INT GLType, render *NewRnd = nullptr )
    {
      throw;

      if (NewRnd != nullptr)
        Rnd = NewRnd;
      assert(Rnd != nullptr);

      TexW = W;
      TexH = H;
      return *this;
    } /* End of 'TexAddFmt' function */

    /* Texture cube map add function.
     * ARGUMENTS:
     *   - render:
     *       render *NewRnd;
     * RETURNS:
     *   (texture &) result texture.
     */
    texture & TexAddCubeMap( render *NewRnd = nullptr );

    /* Texture add from file function.
     * ARGUMENTS:
     *   - render:
     *       render *NewRnd;
     * RETURNS:
     *   (texture &) result texture.
     */
    texture & TexAddFile( render *NewRnd = nullptr );

    /* Texture free function.
     * ARGUMENTS: None.
     * RETURNS:
     *   - (texture &) this texture.
     */
    texture & TexFree( VOID );

    /* Texture apply function.
     * ARGUMENTS:
     *   - bind point:
     *       INT BindPoint;
     * RETURNS: None.
     */
    VOID TexApply( INT BindPoint );

  }; /* End of 'texture' class */

  /* Texture manager */
  class texture_manager : public resource_manager<texture, std::string> 
  {
  public:
    BOOL UpdateTexturesArray = TRUE; // Update material SSBO flag.

    /* Texture manager constructor.
     * ARGUMENTS:
     *   - ref to render:
     *       render &Rnd;
     */
    texture_manager( render &Rnd ) : resource_manager(Rnd)
    {
    } /* End of 'texture_manager' function */
    
    /* Init texture manager function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID TextureInit( VOID )
    {
      static BYTE Txt[2][2] = {{0xFF, 0x0}, {0x0, 0xFF}};

      image Img {"bin/textures/skytex.g24", img_type::G24};

      //resource_manager::Add(texture("Skytex").TexAddImg(Img.W, Img.H, 4, Img.Pixels, &RndRef));
      resource_manager::Add(texture("Default").TexAddImg(2, 2, 1, Txt, &RndRef));
    } /* End of 'material_patern_manager' function */

    /* Create material pattern function.
     * ARGUMENTS: 
     *   - file name:
     *       const std::string &FileName;
     * RETURNS:
     *   - (texture *) pointer to new texture.
     */
    texture * TextureCreateFromFile( const std::string &FileName )
    {
      if (resource_manager::Find(FileName) == nullptr)
      {
        UpdateTexturesArray = TRUE;
        return resource_manager::Add(texture(FileName).TexAddFile(&RndRef));
      }
      return resource_manager::Find(FileName); 
    } /* End of 'TextureCreateFromFile' function */

    /* Create material pattern function.
     * ARGUMENTS: 
     *   - file name:
     *       const std::string &FileName;
     *   - Texture size in pixels:
     *       INT W, INT H;
     *   - Number of components per point:
     *       INT C;
     *   - Pointer to an array with components:
     *       VOID *Bits;
     * RETURNS:
     *   - (texture *) pointer to new texture.
     */
    texture * TextureCreateFromImg( const std::string &FileName, INT W, INT H, INT C, VOID *Bits )
    {
      if (resource_manager::Find(FileName) == nullptr)
      {
        UpdateTexturesArray = TRUE;
        return resource_manager::Add(texture(FileName).TexAddImg(W, H, C, Bits, &RndRef));
      }
      return resource_manager::Find(FileName); 
    } /* End of 'TextureCreateFromImg' function */

    /* Add texture by OpenGL low-level format to stock function.
     * ARGUMENTS: 
     *    - texture name:
     *        const std::string &FileName;
     *   - texture size in pixels:
     *       INT W, H;
     *   - OpenGL texture element data type:
     *       INT GLType;
     * RETURNS: (texture *) result texture.
     */
    texture * TextureCreateFromFmt( const std::string &FileName, INT W, INT H, INT GLType )
    {
      if (resource_manager::Find(FileName) == nullptr)
      {
        UpdateTexturesArray = TRUE;
        return resource_manager::Add(texture(FileName).TexAddFmt(W, H, GLType));
      }
      return resource_manager::Find(FileName); 
    } /* End of 'TextureCreateFromFmt' function */

    /* Create material pattern function.
     * ARGUMENTS: 
     *   - name texture:
     *       const std::string &FileName ;
     * RETURNS:
     *   - (texture *) pointer to new texture.
     */
    texture * TextureCreateFromCubeMap( const std::string &FileName )
    {
      if (resource_manager::Find(FileName) == nullptr)
      {
        UpdateTexturesArray = TRUE;
        return resource_manager::Add(texture(FileName).TexAddCubeMap());
      }
      return resource_manager::Find(FileName); 
    } /* End of 'TextureCreateFromCubeMap' function */

    /* Find by name texture function.
     * ARGUMENTS: 
     *   - name texture:
     *       std::string Name;
     * RETURNS:
     *   - (texture *) pointer to texture.
     */
    texture * TextureFind( std::string Name )
    {
      return resource_manager::Find(Name);
    } /* End of 'TextureFind' function */

    /* Update texture array function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID TexUpdateArray( VOID );

  }; /* End of 'texture_manager' class */
} /* end of 'aigl' namespace */

#endif // !__texture_h_

/* END OF 'texture.h' FILE */