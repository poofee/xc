//----------------------------------------------------------------------------
//  XC program; finite element analysis code
//  for structural analysis and design.
//
//  Copyright (C)  Luis Claudio Pérez Tato
//
//  XC is free software: you can redistribute it and/or modify 
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or 
//  (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful, but 
//  WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details. 
//
//
// You should have received a copy of the GNU General Public License 
// along with this program.
// If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------
//MEDMapNumCellsByType.h
//Wrapper for the MESHING object of MED (to export files to «salome»).

#ifndef MEDMAPNUMCELLSBYTYPE_H
#define MEDMAPNUMCELLSBYTYPE_H

#include <map>
#include <vector>
#include "xc_basic/src/med_xc/MEDMEM_define.hxx"

namespace XC {
//! @ingroup MED
//
//!  @brief Number of cells o each type.
class MEDMapNumCellsByType
  {
    std::map<MED_EN::medGeometryElement,int> types;
  public:
    typedef std::map<MED_EN::medGeometryElement,int>::const_iterator const_iterator;
    typedef std::map<MED_EN::medGeometryElement,int>::iterator iterator;
    typedef std::vector<MED_EN::medGeometryElement> type_vector;

    void add_cell(const MED_EN::medGeometryElement &);

    size_t getNumberOfTypes(void) const;
    size_t getNumCeldas(void) const;
    std::vector<int> getNumberOfCellsByType(void) const;
    type_vector getTypes(void) const;
    inline const_iterator begin(void) const
      { return types.begin(); } 
    inline const_iterator end(void) const
      { return types.end(); } 
    inline size_t size(void) const
      { return types.size(); }

    void clear(void);

  };
} // end of XC namespace
#endif