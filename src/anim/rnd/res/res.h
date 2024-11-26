/***************************************************************
 * Copyright (C) 1992-2024
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE:        res.h
 * PROGRAMMER:  IP5
 * LAST UPDATE: 27.10.2024
 * PURPOSE:     Resource storage base class header file.
 * NOTE:        None.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum.
 */

#ifndef __res_h_
#define __res_h_

#include "../vulkan/vlk_core.h"

/* Base project namespace */
namespace pivk
{

  /* Base resouse class */
  class resource
  {
  protected:
    render *Rnd {}; // Pointer to render
  }; /* End of 'resource' class */

  /* Resource manager class */
  template<typename entry_type, typename index_type = INT>
    class resource_manager
    {
    protected:

      /* Entry reference structure */
      struct entry_ref : public entry_type
      {
        /* Entry reference index value */
        index_type EntryRef;

        /* Class constructor */
        entry_ref( void ) : EntryRef {}
        {
        } /* End of 'entry_ref' function */

        /* Class constructor.
         * ARGUMENTS:
         *   - entry reference to be attached:
         *       const entry_ref &Entry;
         *   - entry reference index value:
         *       const index_type &RefVal;
         */
        entry_ref( const entry_type &Entry, const index_type &RefVal = {} ) :
          entry_type(Entry), EntryRef(RefVal)
        {
        } /* End of 'entry_ref' function */
      }; /* End of 'entry_ref' structure */

      // Check for correct entry resource class based on resource
      static_assert(std::is_convertible_v<entry_type, resource>,
                    "resource store class should be derived from 'resource'");

      render &RndRef; // Reference to render

      /* resource manager constructor
       * ARGUMENTS:
       *   - reference to render:
       *       render &Rnd;
       */
      resource_manager( render &Rnd ) : RndRef(Rnd)
      {
      } /* End of 'resouse_manager' function */

      /* Resource entries stock */
      std::map<index_type, entry_ref> Stock;

      /* Manager reference counter */
      INT TotalRefCnt = 0;

      /* Add to stock function.
       * ARGUMENTS:
       *   - entry data reference:
       *       const entry_type &Entry;
       * RETURNS:
       *   (entry_type *) created entry interface.
       */
      entry_type * Add( const entry_type &Entry )
      {
        if constexpr (std::is_convertible_v<index_type, INT>)
        {
          entry_ref ref(Entry, TotalRefCnt);
          return &(Stock[TotalRefCnt++] = ref);
        }
        else
        {
          entry_ref ref(Entry, Entry.Name);
          TotalRefCnt++;
          return &(Stock[Entry.Name] = ref);
        }
      } /* End of 'Add' function */

      /* Clear manager stock function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (resource_manager &) self reference.
       */
      resource_manager & Clear( VOID )
      {
        for (auto &entry : Stock)
          entry.second.Free();
        Stock.clear();
        return *this;
      } /* End of 'Clear' function */

    public:
      /* Find resource at stock function.
       * ARGUMENTS:
       *   - resource name to find:
       *       const std::string &Name;
       * RETURNS:
       *   (type *) reference to found elememt.
       */
      entry_type * Find( const index_type &Name )
      {
        if (Stock.find(Name) == Stock.end())
          return nullptr;
        return &Stock[Name];
      } /* End of 'Find' function */

      /* Entry delete function.
       * ARGUMENTS:
       *   - entry interface pointer:
       *       entry_type *Entry;
       * RETURNS:
       *   (resource_manager &) self reference.
       */
      resource_manager & Delete( entry_type *Entry )
      {
        if (Entry == nullptr)
          return *this;

        Entry->Free();

        auto find_entry =
          Stock.find(reinterpret_cast<entry_ref *>(Entry)->EntryRef);

        if (find_entry != Stock.end())
          Stock.erase(find_entry);

        return *this;
      } /* End of 'Delete' function */
    }; /* End of 'resource_manager' class */

} /* end of 'pivk' namespace */

#endif // !__res_h_

/* END OF 'res.h' FILE */
