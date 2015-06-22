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
// $Date: 2000/09/15 08:23:20 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/feap/fElmt05.cpp,v $
                                                                        
                                                                        
// File: ~/element/fortran/fElmt05.C
// 
// Written: fmk 
// Created: 07/98
// Revision: A
//
// Description: This file contains the implementation for the XC::fElmt05 class.
//
// What: "@(#) fElement.C, revA"

#include "domain/mesh/element/feap/fElmt05.h"
#include <utility/matrix/ID.h>
#include <utility/matrix/Vector.h>


XC::fElmt05::fElmt05(int tag, int nd1, int nd2, double E, double A, double rho)
:fElement(tag, ELE_TAG_fElmt05, 3, 2, 0, 0)
{
    (*data)(0) = A;
    (*data)(1) = E;
    (*data)(2) = rho;
    
    (*connectedNodes)(0) = nd1; 
    (*connectedNodes)(1) = nd2;   
}
    
XC::fElmt05::fElmt05()
:fElement(ELE_TAG_fElmt05)    
{
    // does nothing
}

extern "C" int elmt05_(double *d, double *ul, double *xl, int *ix, double *tl, 
                       double *s, double *r, int *ndf, int *ndm, int *nst, int *isw, 
		       double *dm, int *nen, int *n, int *nh1, int *nh2, int *nh3, 
		       double *h, double *ctan, int *ior, int *iow);

int
XC::fElmt05::invokefRoutine(double *d, double *ul, double *xl, int *ix, double *tl, 
			double *s, double *r, int ndf, int ndm, int nst, int isw, 
			double dm, int nen, int n, int nh1, int nh2, int nh3, 
			double *h, double *ctan, int ior, int iow)
{
    // check that the values are acceptable to the fortran subroutine
    if (nst != 4 || nen != 2 || dm != 2)
	return 0;
    
    elmt05_(d, ul, xl, ix, tl, s, r, ndf, ndm, nst, isw, dm,
            nen, n, nh1, nh2, nh3, h, ctan, ior, iow);
        
    return nst;
}

