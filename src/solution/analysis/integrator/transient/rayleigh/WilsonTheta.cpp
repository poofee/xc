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
                                                                        
// $Revision: 1.4 $
// $Date: 2003/02/14 23:00:49 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/integrator/WilsonTheta.cpp,v $
                                                                        
                                                                        
// File: ~/analysis/integrator/WilsonTheta.C
// 
// Written: fmk 
// Created: 11/98
// Revision: A
//
// Description: This file contains the implementation of XC::WilsonTheta.
//
// What: "@(#) WilsonTheta.C, revA"

#include <solution/analysis/integrator/transient/rayleigh/WilsonTheta.h>
#include <solution/analysis/model/fe_ele/FE_Element.h>
#include <solution/system_of_eqn/linearSOE/LinearSOE.h>
#include <solution/analysis/model/AnalysisModel.h>
#include <utility/matrix/Vector.h>
#include <solution/analysis/model/dof_grp/DOF_Group.h>
#include <solution/analysis/model/DOF_GrpIter.h>
#include <solution/analysis/model/AnalysisModel.h>

//! @brief Constructor.
XC::WilsonTheta::WilsonTheta(SoluMethod *owr)
:RayleighBase(owr,INTEGRATOR_TAGS_WilsonTheta),
 theta(0), c1(0.0), c2(0.0), c3(0.0) {}

//! @brief Constructor.
XC::WilsonTheta::WilsonTheta(SoluMethod *owr,double _theta)
:RayleighBase(owr,INTEGRATOR_TAGS_WilsonTheta),
 theta(_theta), c1(0.0), c2(0.0), c3(0.0) {}

//! @brief Constructor.
XC::WilsonTheta::WilsonTheta(SoluMethod *owr,double _theta,const RayleighDampingFactors &rF)
  :RayleighBase(owr,INTEGRATOR_TAGS_WilsonTheta,rF), theta(_theta), c1(0.0), c2(0.0), c3(0.0) {}

int XC::WilsonTheta::newStep(double _deltaT)
  {
    if(theta <= 0.0 )
      {
        std::cerr << "XC::Newton::newStep() - error in variable\n";
        std::cerr << "theta: " << theta << " <= 0.0\n";
        return -1;
      }

  
    if(_deltaT <= 0.0)
      {
        std::cerr << "XC::Newton::newStep() - error in variable\n";
        std::cerr << "dT = " << deltaT << std::endl;
        return -2;	
      }

  deltaT = _deltaT;
  c1 = 1.0;
  c2 = 3.0/(theta*deltaT);
  c3 = 2*c2/(theta*deltaT);
    
  if (U.get().Size() == 0) {
    std::cerr << "XC::Newton::newStep() - domainChange() failed or hasn't been called\n"; 
    return -3;	
  }

  // set response at t to be that at t+delta t of previous step
  Ut= U;        
    
  // set new_ velocity and accelerations at t + theta delta t
  //  (U.getDot()) *= -2.0;
  double a1 = -0.5*theta*deltaT; 
  U.getDot().addVector(-2.0,Ut.getDotDot(),a1);

  //  (U.getDotDot()) *= -2.0;  
  double a2 = -6.0/theta/deltaT; 
  U.getDotDot().addVector(-2.0, Ut.getDot(), a2);
    

  // set the new_ trial response quantities
  AnalysisModel *theModel = this->getAnalysisModelPtr();
  theModel->setResponse(U.get(),U.getDot(),U.getDotDot());        

  // increment the time and apply the load
  double time = getCurrentModelTime();
  time+= theta * deltaT;
  applyLoadModel(time);
  updateModel();
  
  return 0;
}

int XC::WilsonTheta::formEleTangent(FE_Element *theEle)
{
  theEle->zeroTangent();
  theEle->addKtToTang(c1);
  theEle->addCtoTang(c2);
  theEle->addMtoTang(c3);


  return 0;
}    

int XC::WilsonTheta::formNodTangent(DOF_Group *theDof)
{
  theDof->zeroTangent();
  theDof->addMtoTang(c3);
  theDof->addCtoTang(c2);        

  return(0);
}    

int XC::WilsonTheta::domainChanged()
  {
    AnalysisModel *myModel = this->getAnalysisModelPtr();
    LinearSOE *theLinSOE = this->getLinearSOEPtr();
    const XC::Vector &x = theLinSOE->getX();
    int size = x.Size();

    setRayleighDampingFactors();
  
    if(Ut.get().Size() != size)
      {
        Ut.resize(size);
        U.resize(size);
      }
  
    // now go through and populate U, Udot and Udotdot by iterating through
    // the DOF_Groups and getting the last committed velocity and accel

  DOF_GrpIter &theDOFGroups = myModel->getDOFGroups();
  DOF_Group *dofGroupPtr= nullptr;
    
    while((dofGroupPtr = theDOFGroups()) != 0)
      {
        const ID &id = dofGroupPtr->getID();
        
        const Vector &disp = dofGroupPtr->getCommittedDisp();	
        U.setDisp(id,disp);
        
        const Vector &vel = dofGroupPtr->getCommittedVel();
        U.setVel(id,vel);
        
        const XC::Vector &accel = dofGroupPtr->getCommittedAccel();	
        U.setAccel(id,accel);
        // NOTE WE CAN't DO TOGETHER BECAUSE DOF_GROUPS USING SINGLE VECTOR
      }
    return 0;
  }


int XC::WilsonTheta::update(const Vector &deltaU)
{
  AnalysisModel *theModel = this->getAnalysisModelPtr();
  if (theModel == 0) {
    std::cerr << "WARNING XC::WilsonTheta::update() - no XC::AnalysisModel set\n";
    return -1;
  }	

  // check domainChanged() has been called, i.e. Ut will not be zero
  if (Ut.get().Size() == 0) {
    std::cerr << "WARNING XC::WilsonTheta::update() - domainChange() failed or not called\n";
    return -2;
  }	
  
  // check deltaU is of correct size
  if (deltaU.Size() != U.get().Size()) {
    std::cerr << "WARNING XC::WilsonTheta::update() - Vectors of incompatable size ";
    std::cerr << " expecting " << U.get().Size() << " obtained " << deltaU.Size() << std::endl;
    return -3;
  }

  // determine the response at t + theta * deltaT
  (U.get())+= deltaU;
  U.getDot().addVector(1.0, deltaU,c2);
  U.getDotDot().addVector(1.0, deltaU,c3);
  
  // update the responses at the DOFs
  theModel->setResponse(U.get(),U.getDot(),U.getDotDot());        
  updateModel();
  
  return 0;
}    


int XC::WilsonTheta::commit(void)
  {

    AnalysisModel *theModel = this->getAnalysisModelPtr();
    if(theModel == 0) {
    std::cerr << "WARNING XC::WilsonTheta::commit() - no XC::AnalysisModel set\n";
    return -1;
  }	  
  
  // determine the quantities at t + delta t 
  /* THIS IS WHAT IS IN BATHE'S BOOK - SEE BELOW FOR QUICKER SOLN
  double a1 = c3/theta;
  double a2 = c3*deltaT;	
  double a3 = (1 - 3.0/theta);
  (U.getDotDot()) = *U;
  (U.getDotDot()) -= Ut.get();
  (U.getDotDot()) *= a1;
  U.getDotDot().addVector(Ut.getDot(), -a2);
  U.getDotDot().addVector(Ut.getDotDot(), a3);
  */
  
  /* REVISION BASED ON WHAT IS IN CHOPRA's BOOK - MAKES SENSE - LINEAR ACCEL */

  // determine response quantities at Ut+dt given Ut and Ut + theta dt
  double a1,a2;
  (U.getDotDot()) -= Ut.getDotDot();
  (U.getDotDot()) *= 1/theta;
  (U.getDotDot()) += Ut.getDotDot();
  
  (U.getDot()) = Ut.getDot();
  a1 = 0.5*deltaT;
  U.getDot().addVector(1.0, U.getDotDot(), a1);
  U.getDot().addVector(1.0, Ut.getDotDot(), a1);


  (U.get()) = Ut.get();
  U.get().addVector(1.0, Ut.getDot(), deltaT);
  a2 = deltaT*deltaT/6.0;
  U.get().addVector(1.0, U.getDotDot(), a2);
  U.get().addVector(1.0, Ut.getDotDot(), 2*a2);
  // update the t + delta t responses at the DOFs 
  theModel->setResponse(U.get(),U.getDot(),U.getDotDot());        
  updateModel();

  // set the time to be t+delta t
  double time = getCurrentModelTime();
  time -= (theta -1) * deltaT;
  setCurrentModelTime(time);

  // now invoke commit on the XC::AnalysisModel
  return commitModel();
}

//! @brief Envía los miembros del objeto a través del canal que se pasa como parámetro.
int XC::WilsonTheta::sendData(CommParameters &cp)
  {
    int res= RayleighBase::sendData(cp);
    res+= cp.sendDoubles(theta,c1,c2,c3,getDbTagData(),CommMetaData(4));
    res+= cp.sendMovable(Ut,getDbTagData(),CommMetaData(5));
    res+= cp.sendMovable(U,getDbTagData(),CommMetaData(6));
    return res;
  }

//! @brief Recibe los miembros del objeto a través del canal que se pasa como parámetro.
int XC::WilsonTheta::recvData(const CommParameters &cp)
  {
    int res= RayleighBase::recvData(cp);
    res+= cp.receiveDoubles(theta,c1,c2,c3,getDbTagData(),CommMetaData(4));
    res+= cp.receiveMovable(Ut,getDbTagData(),CommMetaData(5));
    res+= cp.receiveMovable(U,getDbTagData(),CommMetaData(6));
    return res;
  }

//! @brief Envía el objeto a través del canal que se pasa como parámetro.
int XC::WilsonTheta::sendSelf(CommParameters &cp)
  {
    setDbTag(cp);
    const int dataTag= getDbTag();
    inicComm(7);
    int res= sendData(cp);

    res+= cp.sendIdData(getDbTagData(),dataTag);
    if(res < 0)
      std::cerr << nombre_clase() << "sendSelf() - failed to send data\n";
    return res;
  }

//! @brief Recibe el objeto a través del canal que se pasa como parámetro.
int XC::WilsonTheta::recvSelf(const CommParameters &cp)
  {
    inicComm(7);
    const int dataTag= getDbTag();
    int res= cp.receiveIdData(getDbTagData(),dataTag);

    if(res<0)
      std::cerr << nombre_clase() << "::recvSelf - failed to receive ids.\n";
    else
      {
        //setTag(getDbTagDataPos(0));
        res+= recvData(cp);
        if(res<0)
          std::cerr << nombre_clase() << "::recvSelf - failed to receive data.\n";
      }
    return res;
  }

void XC::WilsonTheta::Print(std::ostream &s, int flag)
  {
    RayleighBase::Print(s,flag);
    s << " theta: " << theta << std::endl;
  }

