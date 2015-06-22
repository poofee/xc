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
                                                                        
// $Revision: 1.6 $
// $Date: 2003/03/04 00:48:12 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/load/pattern/UniformExcitation.h,v $
                                                                        
                                                                        
// File: ~/domain/load/UniformExcitation.h
//
// Written: fmk 11/98
// Revised:
//
// Purpose: This file contains the class definition for UniformExcitation.
// UniformExcitation is a concrete class. It sets the R for a single
// ground motion acting on a structure.

#ifndef UniformExcitation_h
#define UniformExcitation_h

#include "EarthquakePattern.h"

namespace XC {
//! @ingroup LPatterns
//
//! @brief Pauta de carga correspondiente a un terremoto
//! en el que la excitación es igual en todos los apoyos.
class UniformExcitation: public EarthquakePattern
  {
  private:
    GroundMotion *theMotion; //!< the ground motion
    int theDof;      //!< the dof corrseponding to the ground motion
    double vel0;     //!< the initial velocity, should be neg of ug dot(0)

    UniformExcitation(const UniformExcitation &otro);
    UniformExcitation &operator=(const UniformExcitation &otro);
  protected:
    int sendData(CommParameters &);
    int recvData(const CommParameters &);
    bool procesa_comando(CmdStatus &status);
  public:
    UniformExcitation(int tag= 0);
    UniformExcitation(GroundMotion &theMotion, int dof, int tag, double vel0 = 0.0);

    GroundMotion &getGroundMotionRecord(void);
    
    void setDomain(Domain *theDomain);
    void applyLoad(double time);
    void Print(std::ostream &s, int flag =0);

    int sendSelf(CommParameters &);
    int recvSelf(const CommParameters &);

    // AddingSensitivity:BEGIN /////////////////////////////////
    void applyLoadSensitivity(double time);
    // AddingSensitivity:END ///////////////////////////////////
    any_const_ptr GetProp(const std::string &cod) const;
  };
} // fin namespace XC

#endif
