//----------------------------------------------------------------------------
//  programa XC; cálculo mediante el método de los elementos finitos orientado
//  a la solución de problemas estructurales.
//
//  Copyright (C)  Luis Claudio Pérez Tato
//
//  El programa deriva del denominado OpenSees <http://opensees.berkeley.edu>
//  desarrollado por el «Pacific earthquake engineering research center».
//
//  Salvo las restricciones que puedan derivarse del copyright del
//  programa original (ver archivo copyright_opensees.txt) este
//  software es libre: usted puede redistribuirlo y/o modificarlo 
//  bajo los términos de la Licencia Pública General GNU publicada 
//  por la Fundación para el Software Libre, ya sea la versión 3 
//  de la Licencia, o (a su elección) cualquier versión posterior.
//
//  Este software se distribuye con la esperanza de que sea útil, pero 
//  SIN GARANTÍA ALGUNA; ni siquiera la garantía implícita
//  MERCANTIL o de APTITUD PARA UN PROPÓSITO DETERMINADO. 
//  Consulte los detalles de la Licencia Pública General GNU para obtener 
//  una información más detallada. 
//
// Debería haber recibido una copia de la Licencia Pública General GNU 
// junto a este programa. 
// En caso contrario, consulte <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------
/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */
                                                                        
// $Revision: 1.1.1.1 $
// $Date: 2000/09/15 08:23:19 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/load/Load.h,v $
                                                                        
                                                                        
#ifndef Load_h
#define Load_h

// File: ~/domain/load/Load.h
//
// Written: fmk 
// Created: 11/96
// Revision: A
//
// Purpose: This file contains the class definition for Load.
// Load is an abstract class. A Load object is used to add load
// to the model. 
//
// What: "@(#) Load.h, revA"

#include <domain/component/DomainComponent.h>

namespace XC {

//! \ingroup Dom
//!
//!
//! @defgroup Loads Condiciones de contorno en fuerzas.
//
//! @ingroup Loads
//
//! @brief Carga sobre nodos o elementos (clase base abstracta).
class Load: public DomainComponent
  {
  private:
    int loadPatternTag; //!< Identificador de la hipótesis a la que pertenece la carga.
  protected:
    int sendData(CommParameters &cp);
    int recvData(const CommParameters &cp);
    bool procesa_comando(CmdStatus &status);
  public:
    Load(int tag, int classTag);

    // pure virtual functions
    virtual void applyLoad(double loadfactor) =0;
    
    virtual void setLoadPatternTag(int loadPaternTag);
    virtual int  getLoadPatternTag(void) const;
    any_const_ptr GetProp(const std::string &cod) const;
  };
} // fin namespace XC

#endif
