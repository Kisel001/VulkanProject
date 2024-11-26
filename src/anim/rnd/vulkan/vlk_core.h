/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        vlk_core.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 28.10.2024
 * PURPOSE:     Vulkan core header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __vlk_core_h_
#define __vlk_core_h_

#include "def.h"
#include "utils/memtools.h"
#include "utils/console.h"

#if defined(_WIN32) || defined(_WIN64)
   /* Using win32 platform */
#  define T53_WINDOWS_PLATFROM
#  define VK_USE_PLATFORM_WIN32_KHR
#endif // _WIN32 || _WIN64

#include <vulkan.h>

#include "vk_mem_alloc.h"

#ifdef _MSC_VER
#  pragma comment(lib, "vulkan-1.lib")
#endif // _MSC_VER

/* Base project namespace */
namespace pivk
{
  /* Declare render class */
  class render;

#if _DEBUG
  /* Filenames of logs */
  inline constexpr auto LogInfoFileName = "ip5_vk_info_log{30}.log";          // Info log filename
  inline constexpr auto LogWarningFileName = "ip5_vk_warn_log{30}.log";       // Warning log filename
  inline constexpr auto LogErrorFileName = "ip5_vk_error_log{30}.log";        // Error log filename
  inline constexpr auto LogPerfomanceFileName = "ip5_vk_perf_log{30}.log";    // Perfomance log filename
  inline constexpr auto LogUserInfoFileName = "ip5_vk_user_info_log{30}.log"; // User info log filename

  /* Vulkan information logger class */
  inline class logger
  {
  public:
    FILE
      *InfoF,    // File info variable
      *WarningF, // File warning variable
      *ErrorF,   // File error variable
      *PerfF,    // File perfomance variable
      *UInfoF;   // File user info variable

    /* Vulkan logger constructor
     * ARGUMENTS: None.
     */
    logger( VOID ) :
      InfoF(fopen(LogInfoFileName, "w")),
      WarningF(fopen(LogWarningFileName, "w")),
      ErrorF(fopen(LogErrorFileName, "w")),
      PerfF(fopen(LogPerfomanceFileName, "w")),
      UInfoF(fopen(LogUserInfoFileName, "w"))
    {
      if (InfoF == nullptr || WarningF == nullptr || ErrorF == nullptr || PerfF == nullptr || UInfoF == nullptr)
        throw std::exception("Log files not opened!");
    } /* End of 'logger' function */

    /* Message log function.
     * ARGUMENTS:
     *   - string with message:
     *       const std::string &Message;
     *   - type of message:
     *       INT Type;
     * RETURNS: None.
     */
    VOID MessageLog( const std::string &Message, INT Type )
    {
      switch (Type)
      {
      // Warning
      case 1:
        fprintf(WarningF, Message.c_str());
        Console.SetConsoleColor(console::GetConsoleColor(console::color::YELLOW));
        printf(Message.c_str());
        break;
      // Error
      case 2:
        fprintf(ErrorF, Message.c_str());
        Console.SetConsoleColor(console::GetConsoleColor(console::color::RED));
        printf(Message.c_str());
        break;
      // Perfomance
      case 3:
        fprintf(PerfF, Message.c_str());
        Console.SetConsoleColor(console::GetConsoleColor(console::color::GREEN));
        printf(Message.c_str());
        break;
      // User info
      case 4:
        fprintf(UInfoF, Message.c_str());
        Console.SetConsoleColor(console::GetConsoleColor(console::color::CYAN));
        printf(Message.c_str());
        break;
      // Info & other
      default:
        fprintf(InfoF, Message.c_str());
        Console.SetConsoleColor(console::GetConsoleColor());
        printf(Message.c_str());
        break;
      }
      Console.SetConsoleColor(console::GetConsoleColor());
    } /* End of 'MessageLog' function */

    /* Default destructor */
    ~logger()
    {
      fclose(InfoF);
      fclose(WarningF);
      fclose(ErrorF);
      fclose(PerfF);
      fclose(UInfoF);
    } /* End of '~logger' function */
  } Log;

  /* Message log function.
   * ARGUMENTS:
   *   - string with message:
   *       const std::string &Message;
   *   - type of message (0 - info, 1 - warning, 2 - error, 3 - perfomance, 4 - user info, ...):
   *       INT Type;
   * RETURNS: None.
   */
  inline VOID MessageLog( const std::string &Message, INT Type = 4 )
  {
    Log.MessageLog(Message, Type);
  } /* End of 'MessageLog' function */
#endif

  /* Declare vulkan core class */
  class vlk;

  /* Vulkan buffer class */
  class vlk_buf
  {
  public:
    VkBuffer Buffer = nullptr;           // Vulkan buffer
    //VkDeviceMemory DeviceMemory;       // Vulkan device memory
    UINT_PTR Size;                       // Size of vulkan buffer
    UINT_PTR CountOfElements;            // Count of elements
    VmaAllocation Allocation {};         // AMD VMA allocation memory variable
    VkBufferUsageFlags BufferUsageFlags; // Variable with usage flags

    vlk *VlkCore;                        // Vulkan core

    /* Default constuctor */
    vlk_buf( vlk *NewVlkCore ) : VlkCore(NewVlkCore)
    {
    } /* End of 'vlk_buf' class */

    /* Default constructor */
    vlk_buf() : VlkCore(nullptr)
    {
    } /* End of 'vlk_buf' function */

    /* Create buffer function
     * ARGUMENTS:
     *   - size:
     *       UINT_PTR Size;
     *   - type of using:
     *       VkBufferUsageFlags UsageFlags;
     * RETURNS:
     *    (vlk_buf &) reference to this buf.
     */
    vlk_buf & Create( UINT_PTR Size, UINT_PTR NewCount, VkBufferUsageFlags UsageFlags );

    /* Write data to vulkan buffer function.
     * ARGUMENTS:
     *   - memory data buffer:
     *       const VOID *Data;
     * RETURNS: None.
     */
    VOID Write( const VOID *Data );

    /* Resize buffer function.
     * ARGUMENTS:
     *   - new size:
     *       UINT_PTR NewSize;
     * RETURNS: None.
     */
    VOID Resize( UINT_PTR NewSize, UINT_PTR NewCount );

    /* Delete buffer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

  }; /* End of 'vlk_buf' class */

  /* Vulkan buffers manager class */
  class vlk_buf_manager
  {
  private:
    // Stock with buffers
    std::map<INT, vlk_buf> Stock;
    // Size of stock
    INT StockSize = 0;
    // Reference to vulkan core:
    vlk &VlkCore;

  public:
    /* Default constructor */
    vlk_buf_manager( vlk &NewVlkCore ) : VlkCore(NewVlkCore)
    {
    } /* End of 'vlk_buf_manager' function */

    /* Default destructor */
    ~vlk_buf_manager()
    {
    } /* End of '~vlk_buf_manager' function */

    /* Create buffer function */
    vlk_buf * BufCreate( UINT_PTR Size, UINT_PTR Count, VkBufferUsageFlags UsageFlags )
    {
      return &(Stock[StockSize++] = vlk_buf(&VlkCore).Create(Size, Count, UsageFlags));
    } /* End of 'BufCreate' function */

    /* Write data to vulkan buffer function.
     * ARGUMENTS:
     *   - vulkan buffer:
     *       vlk_buf *VlkBuf;
     *   - memory data buffer:
     *       const VOID *&Data;
     * RETURNS: None.
     */
    VOID BufWrite( vlk_buf *VlkBuf, VOID *Data )
    {
      VlkBuf->Write(Data);
    } /* End of 'BufWrite' function */

    /* Free all buffers function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID BufsFree( VOID )
    {
      for (auto &i : Stock)
      {
        i.second.Free();
        i.second = nullptr;
      }
      
      Stock.clear();
      StockSize = 0;
    } /* End of 'BufsFree' function */

  }; /* End of 'vlk_buf_manager' function */

  /* Vulkan descriptor class */
  class vlk_descriptor
  {
  public:
    vlk *VlkCore;
    VkDescriptorSet DescriptorSet;

    /* Default constructor */
    vlk_descriptor() : VlkCore(nullptr)
    {
    } /* End of 'vlk_descriptor' function */

    /* Vulkan descriptor constructor.
     * ARGUMENTS:
     *   - pointer to vulkan core:
     *       vlk *NewVlkCore;
     */
    vlk_descriptor( vlk *NewVlkCore ) : VlkCore(NewVlkCore)
    {
    } /* End of 'vlk_descriptor' function */

    /* Create vulkan descriptor function.
     *   - descriptor set handle:
     *       VkDescriptorSet NDS;
     * RETURNS:
     *   (vlk_descriptor &) reference to this descriptor.
     */
    vlk_descriptor & Create( VkDescriptorSet NDS );

    /* Delete vulkan descriptor function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

    /* Update vulkan descriptor function.
     * ARGUMENTS:
     *   - buffer:
     *       vlk_buf *Buffer;
     *   - size of buffer (in bytes):
     *       UINT_PTR BufSize;
     *   - offset of buffer:
     *       UINT_PTR BufOffset;
     *   - type of buffer: (0 - UBO, 1 - SSBO)
     *       INT Type;
     * RETURNS: None.
     */
    VOID Update( vlk_buf *Buffer, UINT BindPoint, UINT_PTR BufSize = 0, UINT_PTR BufOffset = 0, INT Type = 0 );

    /* Update vulkan texture descriptor function.
     * ARGUMENTS:
     *   - vulkan image view:
     *       VkImageView ImageView;
     *   - vulkan image sampler:
     *       VkSampler Sampler;
     *   - bind point:
     *       UINT BindPoint;
     * RETURNS: None.
     */
    VOID TexUpdate( VkImageView ImageView, VkSampler Sampler, UINT BindPoint );
    
    /* Bind descriptor function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Bind( VOID );

  }; /* End of 'vlk_descriptor' function */

  /* Vulkan descriptor manager */
  class vlk_descriptor_manager
  {
  public:
    std::map<INT, vlk_descriptor> Stock;
    UINT_PTR StockSize;
    vlk &VlkCore;

    /* Descriptor manager constructor
     * ARGUMENTS:
     *   - reference to vulkan core:
     *       vlk &NewVlkCore;
     */
    vlk_descriptor_manager( vlk &NewVlkCore ) : VlkCore(NewVlkCore)
    {
    } /* End of 'vlk_buf_manager' function */

    /* Default destructor */
    ~vlk_descriptor_manager()
    {
    } /* End of '~vlk_buf_manager' function */

    /* Init descriptors function.
     * ARGUMENTS:
     *   - count of descriptors:
     *       UINT Count.
     * RETURNS: None.
     */
    VOID CreateDescriptors( VOID );

    /* Update vulkan descriptor function.
     * ARGUMENTS:
     *   - buffer:
     *       vlk_buf *Buffer;
     *   - bind point:
     *       INT BindPoint;
     *   - type of buffer:
     *       INT Type;
     *   - size of buffer (in bytes):
     *       UINT_PTR BufSize;
     *   - offset of buffer:
     *       UINT_PTR BufOffset;
     * RETURNS: None.
     */
    VOID UpdateDescriptor( vlk_buf *Buffer, INT BindPoint, INT Type = 0, UINT_PTR BufSize = 0, UINT_PTR BufOffset = 0 );

    /* Update vulkan texture descriptor function.
     * ARGUMENTS:
     *   - vulkan descriptor set:
     *       VkDescriptorSet DescriptorSet;
     *   - vulkan image view:
     *       VkImageView ImageView;
     *   - vulkan image sampler:
     *       VkSampler Sampler;
     *   - bind point:
     *       UINT BindPoint;
     * RETURNS: None.
     */
    VOID TexUpdateDescriptor( VkDescriptorSet DescriptorSet, VkImageView ImageView, VkSampler Sampler, UINT BindPoint );

    /* Delete vulkan descriptors function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID FreeDescriptors( VOID );

    /* Bind descriptor function.
     * ARGUMENTS:
     *    - bind point:
     *        INT BindPoint;
     * RETURNS: None.
     */
    VOID BindDescriptor( INT BindPoint );

  }; /* End of 'vlk_descriptor_manager' function */

  /* Vulkan core class */
  class vlk : 
    public vlk_buf_manager,
    public vlk_descriptor_manager
  {
  public:
    HWND &hWndRef;                                  // Reference to hwnd
    UINT FrameW{}, FrameH{};                        // Size of frame

    VkInstance Instance{};                          // Vulkan instance
    VkPhysicalDevice PhysDevice{};                  // Vulkan physical device
    VkPhysicalDeviceProperties DeviceProperties{};  // Main settings
    VkDevice Device{};                              // Vulkan device
    VmaAllocator Allocator {};                      // AMD VMA allocator
    VmaAllocatorCreateInfo AllocatorCreateInfo {};  // AMD VMA allocator creation information data
    VkSurfaceKHR Surface{};                         // Vulkan surface
    VkSwapchainKHR Swapchain{};                     // Vulkan swapchain
    std::vector<VkImage>
      SwapchainImages;                              // Vulkan swapchain images
    std::vector<VkImageView>
      SwapchainImageViews;                          // Vulkan swapchain image views
    VkImage DepthImage{};                           // Vulkan depth image
    VkImageView DepthImageView{};                   // Vulkan depth image view
    VmaAllocation DepthAllocation{};                // AMD VMA depth image allocation
    VkRenderPass RenderPass{};                      // Vulkan render pass
    std::vector<VkFramebuffer>
      FrameBuffers;                                 // Array with vulkan frame buffers.
    VkQueue CommandQueue{};                         // Vulkan command queue
    VkCommandPool CommandPool{};                    // Vulkan command pool
    VkCommandBuffer CommandBuffer{};                // Vulkan command buffer
    VkFence Fence{};                                // Vulkan fence
    UINT32 CurImgIdx{};                             // Vulkan current drawing image
    VkViewport Viewport{};                          // Vulkan viewport
    VkRect2D Scissor{};                             // Vulkan scissor
    VkPipelineLayout PipelineLayout{};              // Vulkan pipeline layout
    VkDescriptorPool DescriptorPool{};              // Vulkan descriptor pool
    VkDescriptorSetLayout DescriptorSetLayout{};    // Vulkan descriptor set layout
    VkDescriptorSetLayout TexDescriptorSetLayout{}; // Vulkan texture descriptor set layout
    UINT CountOfDescriptorSets = 0;                 // Count of descriptors

    /* Queue family indices */
    struct queue_family_indices
    {
      std::optional<UINT32> GraphicsFamily; // Index of queue family of rendering
      std::optional<UINT32> PresentFamily;  // Index of queue family of screen output
    };

    /* Vulkan core constructor
     * ARGUMENTS:
     *   - window handle:
     *       HWND hWnd;
     */
    vlk( HWND &hWnd ) :
      hWndRef(hWnd), vlk_buf_manager(*this), vlk_descriptor_manager(*this)
    {
    } /* End of 'vlk' function */

  private:
    /* Create instance of vulkan function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID CreateInstance( VOID );

    /* Create physical device of vulkan function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID CreatePhysDevice( VOID );

    /* Create device of vulkan function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID CreateDevice( VOID );

    /* Create surface of vulkan core function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID CreateSurface( VOID );

    /* Create swapchain of vulkan core function
     * ARGUMENTS:
     *   - new size:
     *       UINT NewFrameW, NewFrameH;
     *   - old swapchain (nullptr if this is first creating of swapchain):
     *       VkSwapchainKHR OldSwapchain;
     * RETURNS: None.
     */
    VOID CreateSwapchain( UINT NewFrameW, UINT NewFrameH, VkSwapchainKHR OldSwapchain = VK_NULL_HANDLE );

    /* Resize vulkan swapchain function.
     * ARGUMENTS: 
     *   - new size:
     *       INT NewW, NewH;
     * RETURNS: None.
     */
    VOID ResizeSwapchain( INT NewW, INT NewH );

    /* Create vulkan render pass function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID CreateRenderPass( VOID );

    /* Create frame buffers function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID CreateFrameBuffers( VOID );

    /* Create command pool function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID CreateCommandPool( VOID );

    /* Get queue family indices function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (queue_family_indices) suitable queue family indices.
     */
    queue_family_indices GetQueueFamilyIndices( VOID );

    /* Create command buffer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID CreateCommandBuffer( VOID );

    /* Create command queue function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID CreateCommandQueue( VOID );

    /* Create descriptors function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID CreateDescriptors( VOID );

    /* Create vulkan pipeline layout function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID CreatePipelineLayout( VOID );

    /* Begin render pass function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID RenderPassBegin( VOID );

    /* Begin render pass function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID RenderPassEnd( VOID );

  public:
    /* Init vulkan core function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Init( VOID );

    /* Vulkan core close function
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Close( VOID );

    /* Vulkan core resize function.
     * ARGUMENTS:
     *   - frame size:
     *       UINT NewW, NewH;
     * RETURNS: None.
     */
    VOID Resize( UINT NewW, UINT NewH );

    /* Vulkan render start function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID RenderStart( VOID );

    /* Vulkan render end function.
     * ARGUMETNS: None.
     * RETURNS: None.
     */
    VOID RenderEnd( VOID );

    //================================
    //
    //  Images block
    // 
    //================================

    /* Vulkan image creation function.
     * ARGUMENTS:
     *   - vulkan image handle output reference:
     *       VkImage &Image;
     *   - vulkan image view handle output reference:
     *       VkImageView &ImageView;
     *   - VMA AMD image memory handle output reference:
     *       VmaAllocation &Memory;
     *   - image size:
     *       INT W, H;
     *   - image color flag (otherwise Image is depth buffer):
     *       BOOL IsColor,
     *   - cube map flag (create 6 layers and 'PixelData' contain 6 joined same size images):
     *       BOOL IsCube,
     *   - vulkan image Format (see VK_FORMAT_***):
     *       VkFormat Format;
     *   - vulkan image usage flags bitwise combination (VK_IMAGE_USAGE_***):
     *       VkImageUsageFlags UsageFlags;
     *   - vulkan image layout (see VK_IMAGE_LAYOUT_***):
     *       VkImageLayout Layout;
     *   - image pixel data:
     *       const VOID *PixelsData;
     *   - image pixel data size (in bytes):
     *       UINT PixelsDataSize;
     *   - number of mipmaps to be create:
     *       UINT MipCount;
     * RETURNS:
     *   (BOOL) success creation result.
     */
    BOOL ImageCreate( VkImage &Image, VkImageView &ImageView, VmaAllocation &Allocation,
                      INT W, INT H, BOOL IsColor, BOOL IsCube,
                      VkFormat Format, VkImageUsageFlags UsageFlags, VkImageLayout Layout,
                      const VOID *PixelsData, UINT PixelsDataSize, UINT MipCount );

    /* Vulkan image free function.
     * ARGUMENTS:
     *   - vulkan image handle output reference:
     *       VkImage &Image;
     *   - vulkan image view handle output reference:
     *       VkImageView &ImageView;
     *   - AMD VMA image memory handle output reference:
     *       VmaAllocation &Memory;
     * RETURNS: None.
     */
    VOID ImageFree( VkImage &Image, VkImageView &ImageView, VmaAllocation &Allocation );

  private:
    /* Vulkan image set layout function.
     * Create an image memory 'ImageMemoryBarrier' for changing the layout of
     * an Image and put it into an active command buffer
     * ARGUMENTS:
     *   - vulkan image handle:
     *       VkImage Image;
     *   - vulkan image old and new layouts (see VK_IMAGE_LAYOUT_***):
     *       VkImageLayout OldImageLayout, NewImageLayout;
     *   - vulkan image subrsourcse description:
     *       VkImageSubresourceRange SubresourceRange;
     *   - vulkan pipeline source and destination stage flags (see VK_PIPELINE_STAGE_***):
     *       VkPipelineStageFlags SrcStageMask, DstStageMask;
     * RETURNS: None.
     */
    VOID ImageSetLayout( VkImage Image,
                         VkImageLayout OldImageLayout, VkImageLayout NewImageLayout,
                         VkImageSubresourceRange SubresourceRange,
                         VkPipelineStageFlags SrcStageMask, VkPipelineStageFlags DstStageMask );

    /* Vulkan image set layout (on first mip level and layer) function.
     * ARGUMENTS:
     *   - vulkan image handle:
     *       VkImage Image;
     *   - vulkan image old and new layouts (see VK_IMAGE_LAYOUT_***):
     *       VkImageLayout OldImageLayout, NewImageLayout;
     *   - vulkan image subrsourcse description:
     *       VkImageSubresourceRange SubresourceRange;
     *   - vulkan pipeline source and destination stage flags (see VK_PIPELINE_STAGE_***):
     *       VkPipelineStageFlags SrcStageMask, DstStageMask;
     * RETURNS: None.
     */
    VOID ImageSetLayout( VkImage Image,
                         VkImageAspectFlags AspectMask,
                         VkImageLayout OldImageLayout, VkImageLayout NewImageLayout,
                         VkPipelineStageFlags SrcStageMask, VkPipelineStageFlags DstStageMask );

    /* Vulkan Image set layout (on first mip level and layer) function.
     * ARGUMENTS:
     *   - vulkan image handle:
     *       VkImage Image;
     *   - image size:
     *       INT W, H;
     *   - vulkan Image Format (see VK_FORMAT_***):
     *       VkFormat Format;
     *   - cube map flag (create 6 layers and 'PixelData' contain 6 joined same size images):
     *       BOOL IsCube;
     * RETURNS: None.
     */
    VOID ImageGenerateMipmaps( VkImage Image, INT W, INT H, UINT Levels, VkFormat Format, BOOL IsCube );

    //======================================
    //
    //  Sampler block
    // 
    //======================================
  
  public:
    
    /* Create vulkan image sampler function.
     * ARGUMENTS:
     *   - vulkan image sampler:
     *       VkSampler &Sampler;
     *   - vulkan image view:
     *       VkImageView ImageView;
     * RETURNS: None.
     */
    VOID SamplerCreate( VkSampler &Sampler, VkImageView ImageView );

    /* Delete vulkan image sampler function.
     * ARGUMENTS:
     *   - vulkan image sampler:
     *       VkSampler &Sampler;
     * RETURNS: None.
     */
    VOID SamplerFree( VkSampler &Sampler );

  }; /* End of 'vlk' class */

  /* Get pointer to vulkan core by render pointer function
   * ARGUMETNS:
   *   - pointer to render:
   *       render *Rnd;
   * RETURNS:
   *   (vlk *) pointer to vulkan core.
   */
  vlk * GetVlk( render *Rnd );

  /* Find supported format function.
   * ARGUMENTS:
   *   - physical device:
   *       VkPhysicalDevice PhysicalDevice;
   *   - format canditates:
   *       const std::vector<VkFormat> &Candidates;
   *   - tiling:
   *       VkImageTiling Tiling;
   *   - feature flags:
   *       VkFormatFeatureFlags Features;
   * RETURNS:
   *   (VkFormat) finded format.
   */
  inline VkFormat FindSupportedFormat( VkPhysicalDevice PhysicalDevice, const std::vector<VkFormat> &Candidates, VkImageTiling Tiling, VkFormatFeatureFlags Features )
  {
    for (VkFormat format : Candidates)
    {
      VkFormatProperties props;
      vkGetPhysicalDeviceFormatProperties(PhysicalDevice, format, &props);

      if (Tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & Features) == Features)
        return format;
      else if (Tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & Features) == Features)
        return format;
    }

    throw std::runtime_error("failed to find supported format!");
    return VK_FORMAT_UNDEFINED;
  } /* End of 'FindSupportedFormat' function */

} /* end of 'pivk' namespace */

#endif // !__vlk_core_h_

/* END OF 'vlk_core.h' FILE */
