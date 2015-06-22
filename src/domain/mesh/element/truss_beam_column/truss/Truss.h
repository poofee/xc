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
                                                                        
// $Revision: 1.13 $
// $Date: 2003/03/17 19:19:49 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/truss/Truss.h,v $
                                                                        
                                                                        
#ifndef Truss_h
#define Truss_h

// File: ~/element/truss/Truss.h
// 
// Written: fmk 
// Created: 07/98
// Revision: A
//
// Description: This file contains the class definition for Truss. A Truss object
// provides the abstraction of the small deformation bar element. Each truss
// object is assocaited with a material object. This Truss element will work
// in 1d, 2d or 3d problems.
//
// What: "@(#) Truss.h, revA"

#include "TrussBase.h"

namespace XC {
class Channel;
class UniaxialMaterial;

//! \ingroup ElemBarraArt
//
//! @brief Barra articulada.
class Truss : public TrussBase
  {
  private:
    double computeCurrentStrain(void) const;
    double computeCurrentStrainRate(void) const;
    
    // private attributes - a copy for each object of the class
    UniaxialMaterial *theMaterial;  //!< pointer to a material
    double A; //!< area of truss

// AddingSensitivity:BEGIN //////////////////////////////////////////
    int parameterID;
    Vector *theLoadSens;
// AddingSensitivity:END ///////////////////////////////////////////

    void inicializa(void);
  protected:
    DbTagData &getDbTagData(void) const;
    int sendData(CommParameters &cp);
    int recvData(const CommParameters &cp);
    void libera_material(void);
    void libera_load_sens(void);
    void libera(void);
    void set_material(const UniaxialMaterial &);
    void set_load_sens(const Vector &);
    bool procesa_comando(CmdStatus &status);
  public:
    Truss(int tag, int dimension, int Nd1, int Nd2, UniaxialMaterial &theMaterial, double A);
    Truss(int tag,int dimension,const Material *ptr_mat);
    Truss(void);
    Truss(const Truss &);
    Truss &operator=(const Truss &);
    Element *getCopy(void) const;
    ~Truss(void);

    // public methods to obtain inforrmation about dof & connectivity    
    void setDomain(Domain *theDomain);

    // public methods to set the state of the element    
    int commitState(void);
    int revertToLastCommit(void);        
    int revertToStart(void);        
    int update(void);
    
    const Material *getMaterial(void) const;
    Material *getMaterial(void);
    virtual double getRho(void) const;
    inline const double &getArea(void) const
      { return A; }
    inline void setArea(const double &a)
      { A= a; }

    // public methods to obtain stiffness, mass, damping and residual information    
    const Matrix &getKi(void);
    const Matrix &getTangentStiff(void) const;
    const Matrix &getInitialStiff(void) const;
    const Matrix &getDamp(void) const;    
    const Matrix &getMass(void) const; 

    int addLoad(ElementalLoad *theLoad, double loadFactor);
    int addInertiaLoadToUnbalance(const Vector &accel);

    double getAxil(void) const;
    const Vector &getResistingForce(void) const;
    const Vector &getResistingForceIncInertia(void) const;            

    // public methods for element output
    int sendSelf(CommParameters &);
    int recvSelf(const CommParameters &);
    void Print(std::ostream &s, int flag =0);    
    virtual any_const_ptr GetProp(const std::string &cod) const;

    Response *setResponse(const std::vector<std::string> &argv, Information &eleInfo);
    int getResponse(int responseID, Information &eleInformation);

    // AddingSensitivity:BEGIN //////////////////////////////////////////
    int		   addInertiaLoadSensitivityToUnbalance(const Vector &accel, bool tag);
    int            setParameter(const std::vector<std::string> &argv, Parameter &param);
    int            updateParameter(int parameterID, Information &info);
    int            activateParameter(int parameterID);
    const Vector & getResistingForceSensitivity(int gradNumber);
    const Matrix & getKiSensitivity(int gradNumber);
    const Matrix & getMassSensitivity(int gradNumber);
    int            commitSensitivity(int gradNumber, int numGrads);
    // AddingSensitivity:END ///////////////////////////////////////////
  };
} // fin namespace XC

#endif




