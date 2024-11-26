/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        shader.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 30.08.2024
 * PURPOSE:     Shader class header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __shader_h_
#define __shader_h_

#include "res.h"

/* Base project namespace */
namespace pivk
{
  /* Directory watcher class */
  class dir_watcher
  {
  private:
    // Overlapped structure for changing hook
    OVERLAPPED Ovr {};
    // Traching directory handle
    HANDLE hDir {};
    // Read directory changes data buffer
    BYTE Buffer[30 * 47] {};
    // Store path
    std::string Path;
 
  public:
    /* Start directory watching function.
     * ARGUMENTS:
     *   - directory path:
     *       const std::string &DirPath;
     * RETURNS: None.
     */
    VOID StartWatch( const std::string &DirPath )
    {
      StopWatch();
      ZeroMemory(&Ovr, sizeof(OVERLAPPED));
      Path = DirPath;
      // Open directory
      hDir =
        CreateFile(DirPath.c_str(),
          FILE_LIST_DIRECTORY,
          FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
          nullptr, OPEN_EXISTING,
          FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
          nullptr);
 
      // Start monitoring
      Ovr.hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
      ReadDirectoryChangesW(hDir,
        Buffer, sizeof(Buffer),
        TRUE,
        FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION |
          FILE_NOTIFY_CHANGE_FILE_NAME,
        nullptr,
        &Ovr,
        nullptr);
    } /* End of 'StartWatch' function */
 
    /* Stop directory watching function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID StopWatch( VOID )
    {
      if (Ovr.hEvent != nullptr)
        CloseHandle(Ovr.hEvent), Ovr.hEvent = nullptr;
      if (hDir != nullptr)
        CloseHandle(hDir), hDir = nullptr;
 
      ZeroMemory(&Ovr, sizeof(OVERLAPPED));
      hDir = nullptr;
      Path = "";
    } /* End of 'StopWatch' function */
 
    /* Check if directory is changed function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (BOOL) TRUE if directory contents is changed (and subfolders).
     */
    BOOL IsChanged( VOID )
    {
      if (Ovr.hEvent == nullptr || hDir == nullptr)
        return FALSE;
 
      INT id = WaitForSingleObject(Ovr.hEvent, 0);
      if (id == WAIT_OBJECT_0)
      {
        // Restart monitoring
        ResetEvent(Ovr.hEvent);
        ReadDirectoryChangesW(hDir,
          Buffer, sizeof(Buffer),
          TRUE,
          FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION |
            FILE_NOTIFY_CHANGE_FILE_NAME,
          nullptr,
          &Ovr,
          nullptr);
        return TRUE;
      }
      return FALSE;
    } /* End of 'IsChanged' function */
  }; /* End of 'dir_watcher' class */

  /* Vulkan shader class */
  class shader : public resource
  {
    friend class shader_manager;

  private:
    /* Save text to log file function.
     * ARGUMENTS:
     *   - text 1 to save:
     *       const std::string &Stage;
     *   - text 2 to save:
     *       const std::string &Text;
     * RETURNS: None.
     */
    VOID Log( const std::string &Stage, const std::string &Text ) const
    {
      CHAR Buf[_MAX_PATH];
      GetCurrentDirectory(sizeof(Buf), Buf);
      std::string Bufstr = Buf;
      std::ofstream(Bufstr + "/bin/shaders/IP5_30.LOG", std::ios_base::app) <<
        Name << "\n" << Stage << ":\n" << Text << "\n";
    } /* End of 'Log' function */
 
    /* Text file load to memory function.
     * ARGUMENTS:
     *   - file name:
     *       const std::string &FileName;
     * RETURNS:
     *   (std::string) load text.
     */
    std::string LoadTextFile( const std::string &FileName );
 
    /* Load shader function.
     * ARGUMENTS:
     *   - pointer to render:
     *       render *NewRnd;
     * RETURNS:
     *   (shader &) self reference.
     */
    shader & Load( render *NewRnd = nullptr );
 
  public:
    /* Free shader function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );
 
  private:
    // Shader watcher
    dir_watcher Watcher;

    // Type of shader
    enum TypeOfShader
    {
      FRAG = 0, // Fragment shader
      VERT = 1, // Vertex shader
      GEOM = 2, // Geometry shader
      EVAL = 3, // Evaluation shader
      CTRL = 4, // Control shader
    };

  public:
    // Vulkan shader modules
    VkShaderModule
      ShaderModuleVert = VK_NULL_HANDLE, // Vertex shader
      ShaderModuleCtrl = VK_NULL_HANDLE, // Control shader
      ShaderModuleEval = VK_NULL_HANDLE, // Evaluation shader
      ShaderModuleGeom = VK_NULL_HANDLE, // Geometry shader
      ShaderModuleFrag = VK_NULL_HANDLE; // Fragment shader

    //std::map<TypeOfShader, VkShaderModule> ShaderModules;
    // Vulkan shader pipeline
    VkPipeline Pipeline;
    //  Vulkan shader pipeline layout
    //VkPipelineLayout PipelineLayout;

  public:
    // Shader name
    std::string Name;
 
    /* Shader input attributes and uniforms information structure */
    struct ATTR_INFO
    {
      std::string SType; // String type like shader
      INT Components;    // Number of components
      BOOL IsFloat;      // Float value flag (otherwise integer)
      INT MatrRows;      // For matrix number of rows
      INT Loc;           // Shader attribute location
    }; /* End of 'ATTR_INFO' structure */
 
    /* Shader buffer block information structure */
    struct BLOCK_INFO
    {
      std::string SType; // String type like shader
      INT Index;         // Buffer block index
      INT Bind;          // Buffer bind point
    }; /* End of 'BLOCK_INFO' structure */
 
    // Attributes and uniforms information
    std::map<std::string, ATTR_INFO> Attributes, Uniforms;
    // Shader storage blocks informations (type, index, bind point)
    std::map<std::string, BLOCK_INFO> SSBOBuffers;
 
//#if 0
    /* Class default constructor */
    shader( VOID )
    {
    } /* End of 'shader' function */
//#endif

    /* Class constructor.
     * ARGUMENTS:
     *   - shader file name prefix:
     *       const std::string &FileNamePrefix;
     */
    shader( const std::string &FileNamePrefix ) : Name(FileNamePrefix)
    {
      //Watcher.StopWatch();
      //Watcher.StartWatch("bin/shaders/" + Name);
    } /* End of 'shader' function */
 
    /* Class destructor */
    ~shader( VOID )
    {
      //Free();
      //Watcher.StopWatch();
    } /* End of '~shader' function */
 
    /* Init pipeline function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */


    /* Apply shader function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (UINT) shader program Id;
     */
    VOID Apply( VOID );
 
    /* Update shader function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Update( VOID )
    {
      //if (Watcher.IsChanged())
      //{
      //  Free();
      //  Load();
      //  Watcher.StartWatch("bin/shaders/" + Name);
      //}
    } /* End of 'Update' function */
 
    /* Update shader internal data info function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID UpdateInfo( VOID )
    {
    } /* End of 'UpdateInfo' function */

  }; /* End of 'shader' class */

  /* Shader manager class */
  class shader_manager : public resource_manager<shader, std::string>
  {
  private:
    dir_watcher Watcher;

  public:
    /* Shader manager constructor.
     * ARGUMENTS:
     *   - ref to render:
     *       render &Rnd;
     */
    shader_manager( render &Rnd ) : resource_manager(Rnd)
    {
      //Watcher.StartWatch("bin/shaders");
    } /* End of 'shader_manager' function */

    /* Create shader function.
     * ARGUMENTS:
     *   - shader file name prefix:
     *       const std::string &FileNamePrefix;
     * RETURNS:
     *   (shader *) created primitive interface.
     */
    shader * ShdCreate( const std::string &FileNamePrefix )
    {
      return resource_manager::Add(shader(FileNamePrefix).Load(&RndRef));
      //if (resource_manager::Find(FileNamePrefix) == nullptr)
      //  return resource_manager::Add(shader(FileNamePrefix).Load());
      //return resource_manager::Find(FileNamePrefix);
      //return resource_manager::Add(shader(FileNamePrefix).Load());
    } /* End of 'ShdCreate' function */

    /* Create shader function.
     * ARGUMENTS:
     *   - pointer to shader:
     *       shader *Shd;
     * RETURNS:
     *   (shader *) this shader.
     */
    shader * ShdLoad( shader *Shd )
    {
      Shd->Load(&RndRef);
      return Shd;
    } /* End of 'ShdLoad' function */

    /* Find shader by name.
     * ARGUMENTS:
     *   - shader name:
     *       const std::string &Name;
     * RETURNS:
     *   (shader *) pointer to shader, if shader finded, or nullptr, if no.
     */
    shader * ShdFind( const std::string &Name )
    {
      shader *S = resource_manager::Find(Name);

      if (S != nullptr)
        return S;
      return ShdCreate(Name);
    } /* End of 'ShdFind' function */
 
    ///* Class constructor.
    // * ARGUMENTS: None.
    // */
    //shader_manager() : resource_manager()
    //{
    //   Watcher.StartWatch("bin/shaders");
    //} /* End of 'shader_manager' function */
 
    /* Class destructor */
    ~shader_manager( VOID )
    {
      //Watcher.StopWatch();
    } /* End of '~shader_manager' function */

    /* Close shader manager function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID ShdClose( VOID )
    {
      for (auto &i : resource_manager::Stock)
        i.second.Free();
    } /* End of 'ShdClose' function */
 
    /* Update all shaders in stock function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID ShdUpdate( VOID )
    {
      //if (Watcher.IsChanged())
      //{
      //  for (auto &shd : Stock)
      //    shd.second.Update();
      //}
    } /* End of 'Update' function */
  }; /* End of 'shader_manager' class */

} /* end of 'pivk' class */

#endif // !__shader_h_

/* END OF 'shader.h' FILE */
