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
                                                                        
// $Revision: 1.1 $
// $Date: 2005/12/06 22:04:33 $
// $Source: /usr/local/cvs/OpenSees/SRC/system_of_eqn/linearSOE/bandSPD/DistributedBandSPDLinSOE.h,v $
                                                                        
#ifndef DistributedBandSPDLinSOE_h
#define DistributedBandSPDLinSOE_h

// Written: fmk 
// Description: This file contains the class definition for DistributedBandSPDLinSOE
// DistributedBandSPDLinSOE is a subclass of LinearSOE. It uses the LAPACK storage
// scheme to store the components of the A matrix, which is a banded 
// unsymmetric matrix.
//
// What: "@(#) DistributedBandSPDLinSOE.h, revA"


#include <solution/system_of_eqn/linearSOE/bandSPD/BandSPDLinSOE.h>
#include "solution/system_of_eqn/linearSOE/DistributedBandLinSOE.h"
#include "utility/actor/channel/ChannelQueue.h"

namespace XC {
class DistributedBandSPDLinSolver;

//! @ingroup SOE
//
//! @brief Clase base para los sistemas de ecuaciones
//! con matriz en banda distribuída.
class DistributedBandSPDLinSOE: public BandSPDLinSOE, public DistributedBandLinSOE
  {
  protected:
    void calcBand(Graph &);

    friend class SoluMethod;
    DistributedBandSPDLinSOE(SoluMethod *);
    SystemOfEqn *getCopy(void) const;
  public:
    // these methods need to be rewritten
    int addA(const Matrix &, const ID &, double fact = 1.0);
    int addB(const Vector &, const ID &,const double &fact= 1.0);    
    int setB(const Vector &, const double &fact = 1.0);            
    int setSize(Graph &theGraph);
    int solve(void);
    const Vector &getB(void) const;

    int sendSelf(CommParameters &);
    int recvSelf(const CommParameters &);
    friend class BandSPDLinLapackSolver;

  };
inline SystemOfEqn *DistributedBandSPDLinSOE::getCopy(void) const
  { return new DistributedBandSPDLinSOE(*this); }
} // fin namespace XC


#endif

