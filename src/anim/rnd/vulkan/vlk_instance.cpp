/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        vlk_instance.cpp
 * PROGRAMMER:  IP5
 * LAST UPDATE: 28.10.2024
 * PURPOSE:     Vulkan core instance file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#include "pivk.h"

/* Base project namespace */
namespace pivk
{
  /* Create instance of vulkan function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID vlk::CreateInstance( VOID )
  {
    // Information structure of application.
    VkApplicationInfo AppInfo
    {
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pApplicationName = "Project Name",
      .applicationVersion = 1,
      .apiVersion = VK_MAKE_VERSION(1, 3, 0), // Target vulkan version.
    };
    
    // Information structure for creating instance.
    VkInstanceCreateInfo InstanceCreateInfo 
    {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &AppInfo,
    };

    // Setting extensions of instance.
    std::vector<const CHAR *> InstanceExtensionNames 
    {
      //...
      VK_KHR_SURFACE_EXTENSION_NAME,
      VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
      "VK_EXT_debug_utils",  // Debug extenstion
      "VK_EXT_debug_report",
    };
    InstanceCreateInfo.enabledExtensionCount = (UINT32)InstanceExtensionNames.size();
    InstanceCreateInfo.ppEnabledExtensionNames = InstanceExtensionNames.data();

    // Settiing layers of instance.
    std::vector<const CHAR *> EnabledLayersNames
    {
      // ...
      "VK_LAYER_KHRONOS_validation"   // Layer of validation.
    };

    InstanceCreateInfo.enabledLayerCount = (UINT32)EnabledLayersNames.size();
    InstanceCreateInfo.ppEnabledLayerNames = EnabledLayersNames.data();

    // Create opportunity of validation.
    // For details check specification (For version 1.3: paragraph 4.2, structure VkValidationFeaturesEXT).
    VkValidationFeaturesEXT ValidationFeatures 
    {
      .sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT,
    };
    std::vector<VkValidationFeatureEnableEXT> ValidationFeaturesEnable
    {
      VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT,    // Using for validate shaders.
      VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT   // Provides additional output of warnings.
    };
    ValidationFeatures.enabledValidationFeatureCount = (UINT32)ValidationFeaturesEnable.size();
    ValidationFeatures.pEnabledValidationFeatures = ValidationFeaturesEnable.data();

    InstanceCreateInfo.pNext = &ValidationFeatures;

#ifdef _DEBUG
    // Create debug callbacks.
    VkDebugReportCallbackCreateInfoEXT DebugReportCallbackInfo {};
    DebugReportCallbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;

    DebugReportCallbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    DebugReportCallbackInfo.pNext = nullptr;
    DebugReportCallbackInfo.flags =
      VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
      VK_DEBUG_REPORT_WARNING_BIT_EXT |
      VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
      VK_DEBUG_REPORT_ERROR_BIT_EXT |
      VK_DEBUG_REPORT_DEBUG_BIT_EXT;
    DebugReportCallbackInfo.pfnCallback = [](
      VkDebugReportFlagsEXT Flags,
      VkDebugReportObjectTypeEXT ObjectType,
      UINT64 Object,
      UINT_PTR Location,
      INT32 MessageCode,
      const CHAR *LayerPrefix,
      const CHAR *Message,
      VOID *UserData) -> VkBool32
      {
        INT TypeOfError = 0;

        if ((Flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) != 0)
          return VK_FALSE;
        std::string Res = "DEBUG REPORT: (";


        if ((Flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) != 0)
          Res += "WARNING", TypeOfError = 1;
        else if ((Flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) != 0)
          Res += "ERROR", TypeOfError = 2;
        else if ((Flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) != 0)
          Res += "PERFORMANCE", TypeOfError = 3;
        else if ((Flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) != 0)
          Res += "INFO", TypeOfError = 0;

        Res += ")";
        Res += std::string("{") + LayerPrefix + "} " + Message + "\n";
        MessageLog(Res, TypeOfError);
        return VK_FALSE;
      };
    DebugReportCallbackInfo.pUserData = nullptr;

    ValidationFeatures.pNext = &DebugReportCallbackInfo;
#endif

    VkResult res;

    //VkInstance TmpInstance;
    if ((res = vkCreateInstance(&InstanceCreateInfo, nullptr, &Instance)) != VK_SUCCESS)
    {
      //InstanceCreateInfo.
      MessageBox(this->hWndRef, "Error of creating vulkan instance", "T53VLK :: ERROR", MB_OK | MB_ICONERROR);
      exit(3); // Processing error.
    }
  } /* End of 'vlk::CreateInstance' function */

} /* end of 'pivk' namespace */

/* END OF 'vlk_instance.cpp' FILE */
