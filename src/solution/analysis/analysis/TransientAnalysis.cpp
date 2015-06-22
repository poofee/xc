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
** file 'COPYRIGHT'  in XC::main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */
                                                                        
// $Revision: 1.1.1.1 $
// $Date: 2000/09/15 08:23:16 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/analysis/TransientAnalysis.cpp,v $
                                                                        
                                                                        
// File: ~/analysis/analysis/TransientAnalysis.C
// 
// Written: fmk 
// Created: Sun Sept 15 11:47:47: 1996
// Revision: A
//
// Description: This file contains the implementation of XC::TransientAnalysis.
// TransientAnalysis is an abstract base class, i.e. no objects of it's
// type can be created. 
//
// What: "@(#) TransientAnalysis.C, revA"

#include <solution/analysis/analysis/TransientAnalysis.h>
#include <solution/analysis/integrator/TransientIntegrator.h>
#include "xc_utils/src/base/CmdStatus.h"

//! @brief Constructor.
XC::TransientAnalysis::TransientAnalysis(SoluMethod *metodo)
  :Analysis(metodo) {}


//! @brief Lee un objeto XC::TransientAnalysis desde archivo
bool XC::TransientAnalysis::procesa_comando(CmdStatus &status)
  {
    const std::string cmd= deref_cmd(status.Cmd());
    if(verborrea>2)
      std::clog << "(TransientAnalysis) Procesando comando: " << cmd << std::endl;
    if(cmd == "analyze") //Lanza el análisis del problema.
      {
        int numSteps= 1;
        std::vector<std::string> params= crea_vector_string(status.GetString());
        if(params.size()>0)
          numSteps= interpretaInt(params[0]);
        double dT= 1.0;
        if(params.size()>1)
          dT= interpretaDouble(params[1]);
        analysis_result= analyze(numSteps,dT);
        return true;
      }
    else
      return Analysis::procesa_comando(status);
  }











