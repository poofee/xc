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
** ****************************************************************** */
                                                                        
// $Revision: 1.5 $
// $Date: 2003/02/14 23:01:25 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/nD/J2ThreeDimensional.h,v $

// Written: Ed "C++" Love

//
// J2ThreeDimensional isotropic hardening material class
// 
//  Elastic Model
//  sigma = K*trace(epsilion_elastic) + (2*G)*dev(epsilon_elastic)
//
//  Yield Function
//  phi(sigma,q) = || dev(sigma) ||  - sqrt(2/3)*q(xi) 
//
//  Saturation Isotropic Hardening with linear term
//  q(xi) = simga_0 + (sigma_infty - sigma_0)*exp(-delta*xi) + H*xi 
//
//  Flow Rules
//  \dot{epsilon_p} =  gamma * d_phi/d_sigma
//  \dot{xi}        = -gamma * d_phi/d_q 
//
//  Linear Viscosity 
//  gamma = phi / eta  ( if phi > 0 ) 
//
//  Backward Euler Integration Routine 
//  Yield condition enforced at time n+1 
//
//  Send strains in following format :
// 
//     strain_vec = {   eps_00
//                      eps_11
//	                eps_22 		      
//                    2 eps_01   
//            	      2 eps_12   
//		      2 eps_20    }   <--- note the 2
// 
//  set eta := 0 for rate independent case
//

#ifndef J2THREEDIMENSIONAL_H
#define J2THREEDIMENSIONAL_H

#include "material/nD/J2Plasticity.h"

namespace XC{
//! @ingroup J2NDMat
//
//! @brief J2 Isotropic hardening material class
//! para problemas tridimensionales.
class J2ThreeDimensional : public J2Plasticity {

//-------------------Declarations-------------------------------

  public : 

  J2ThreeDimensional(int tag);

 //null constructor
  J2ThreeDimensional( );

  //full constructor
  J2ThreeDimensional(   int    tag, 
                   double K,
                   double G,
                   double yield0,
                   double yield_infty,
                   double d,
                   double H,
                   double viscosity = 0 );


  //elastic constructor
  J2ThreeDimensional( int tag, double K, double G );

  //make a clone of this material
  NDMaterial* getCopy(void) const;

  //send back type of material
  const std::string &getType( ) const ;

  //send back order of strain in vector form
  int getOrder( ) const ;

  //get the strain and integrate plasticity equations
  int setTrialStrain( const Vector &strain_from_element);

  //unused trial strain functions
  int setTrialStrain( const Vector &v, const Vector &r );
  int setTrialStrainIncr( const Vector &v );
  int setTrialStrainIncr( const Vector &v, const Vector &r );

  //send back the strain
  const Vector& getStrain(void) const;

  //send back the stress 
  const Vector& getStress(void) const;

  //send back the tangent 
  const Matrix& getTangent(void) const;
  const Matrix& getInitialTangent(void) const;

  //this is mike's problem
  int setTrialStrain(const Tensor &v);
  int setTrialStrain(const Tensor &v, const Tensor &r);    
  int setTrialStrainIncr(const Tensor &v);
  int setTrialStrainIncr(const Tensor &v, const Tensor &r);
  const Tensor& getTangentTensor(void) const;
//jeremic@ucdavis.edu 22jan2001  const Tensor& getStressTensor( );
//jeremic@ucdavis.edu 22jan2001  const Tensor& getStrainTensor( );  

  private :

  //static vectors and matrices
  static Vector strain_vec ;     //strain in vector notation
  static Vector stress_vec ;     //stress in vector notation
  static Matrix tangent_matrix ; //material tangent in matrix notation

} ; //end of J2ThreeDimensional declarations


} //fin namespace XC

#endif
