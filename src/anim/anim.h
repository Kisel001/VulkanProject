/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        anim.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 01.08.2024
 * PURPOSE:     Animation header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __anim_h_
#define __anim_h_

#include "win/win.h"
#include "input/input.h"
#include "input/timer.h"
#include "rnd/render.h"

/* Base project namespace */
namespace pivk
{ 
  /* Unit class */
  class unit;

  /* Animation class */
  class anim : public win, public input, public timer, public render
  {
  private:
    /* Animation units stock */
    stock<unit *> Units;
    /* Map with units */
    static inline std::map<std::string, unit * (*)( anim & )> UnitNames;
    BOOL CloseState = FALSE;

  public:
    /* Constructor by hinstance.
     * ARGUMENTS:
     *   - hinstance:
     *       HINSTANCE hInstance;
     */
    anim( VOID ) : win(), input(), timer(), render(win::hWnd, *this)
    {
    } /* End of 'anim' function */

    /* Default destuctor */
    ~anim( VOID ) override;

  public:
    /* Path of animation */
    std::string Path;

    /* Position data of user */
    vec3 Pos {};  // Position vector of user
    FLT Wheel {}; // Rotate wheel of user

    /* Obtain animation system instance reference function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (anim &) reference to instance.
     */
    static anim & GetRef( VOID )
    {
      static anim AnimInstance;

      return AnimInstance;
    } /* End of 'GetRef' function */

    /* Obtain animation system instance pointer function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (anim *) pointer to instance.
     */
    static anim * Get( VOID )
    {
      return &GetRef();
    } /* End of 'Get' function */

    /* Unit register class */
    template<typename unit_type>
      class unit_register
      {
      private:
        /* Create unit function
         * ARGUMENTS: None.
         * RETURNS:
         *   (unit *) pointer to register unit.
         */
        static unit * Create( anim &Ani )
        {
          return new unit_type(Ani);
        } /* End of 'Create' function */

      public:
        /* Constructor by name.
         * ARGUMENTS:
         *   - name:
         *       const std::string &UnitName;
         */
        unit_register( const std::string &UnitName )
        {
          anim::UnitNames[UnitName] = Create;
        } /* End of 'unit_register' function */
      }; /* End of 'unit_register' class */

    /* WM_CREATE window message handle function.
     * ARGUMENTS:
     *   - structure with creation data:
     *       CREATESTRUCT *CS;
     * RETURNS:
     *   (BOOL) TRUE to continue creation window, FALSE to terminate.
     */
    BOOL OnCreate( CREATESTRUCT *CS ) override
    {
      SetTimer(win::hWnd, 47, 0, nullptr);
      render::Init(win::hWnd);
      return TRUE;
    } /* End of 'win::OnCreate' function */

    /* WM_DESTROY window message handle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID OnDestroy( VOID ) override
    {
      CloseState = TRUE;
      KillTimer(win::hWnd, 47);
    } /* End of 'win::OnDestroy' function */

    /* WM_TIMER window message handle function.
     * ARGUMENTS:
     *   - specified the timer identifier.
     *       INT Id;
     * RETURNS: None.
     */    
    VOID OnTimer( INT Id ) override
    {
      anim::Render();

      InvalidateRect(win::hWnd, nullptr, 0);
    } /* End of 'OnTimer' function */

    /* WM_PAINT window message handle function.
     * ARGUMENTS:
     *   - window device context:
     *       HDC hDC;
     *   - paint message structure pointer:
     *       PAINTSTRUCT *PS;
     * RETURNS: None.
     */
    VOID OnPaint( HDC hDC, PAINTSTRUCT *Ps ) override
    {
    } /* End of 'OnPaint' function */

    /* WM_SIZE window message handle function.
     * ARGUMENTS:
     *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
     *       UINT State;
     *   - new width and height of client area:
     *       INT W, H;
     * RETURNS: None.
     */
    VOID OnSize( UINT State, INT W, INT H ) override
    {
      render::Resize(W, H);
    } /* End of 'OnSize' function */

    /* Animation render function
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Render( VOID );

    /* Adding unit function.
     * ARGUMENTS:
     *   - pointer to unit:
     *       unit *Uni;
     * RETURNS: None.
     */
    anim & operator<<( unit *Uni )
    {
      this->Units << Uni;

      return *this;
    } /* End of 'operator<<' function */

    /* Adding unit by name function.
     * ARGUMENTS:
     *   - name:
     *       const std::string &UnitName;
     * RETURNS:
     *   (anim &) reference to this animation.
     */
    anim & operator<<( const std::string &UnitName )
    {
      if (UnitNames.find(UnitName) != UnitNames.end())
        *this << UnitNames[UnitName](*this);
      return *this;
    } /* End of 'operator<<' function */

  }; /* End of 'anim' class */

  /* Unit class */
  class unit
  {
  public:

    /* Default constructor */
    unit()
    {
    } /* End of 'unit' function */

    /* Default destructor */
    virtual ~unit()
    {
    } /* End of '~unit' function */

    // Delete copy constructor and operator=
    // because using this methods for dynamic
    // unit objects are invalid.

    /* Delete copy constructor */
    unit( const unit &U ) = delete;

    /* Delete operator= */
    unit & operator=( const unit &U ) = delete;

    /* Unit response function.
     * ARGUMENTS:
     *   - animation reference:
     *       const anim &A;
     * RETURNS: NONE.
     */
    virtual VOID Response( anim &A )
    {
    } /* End of 'Response' function */

    /* Unit render function.
     * ARGUMENTS:
     *   - animation reference:
     *       const anim &A;
     * RETURNS: NONE.
     */
    virtual VOID Render( anim &A )
    {
    } /* End of 'Render' function */

  }; /* End of 'unit' class */

} /* end of 'pivk' file */

#endif // !__anim_h_

/* END OF 'anim.h' FILE */
