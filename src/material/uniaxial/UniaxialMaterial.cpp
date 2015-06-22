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
// $Date: 2006/01/10 19:15:48 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/uniaxial/UniaxialMaterial.cpp,v $
                                                                        
                                                                        
// File: ~/material/UniaxialMaterial.C
//
// Written: fmk 
// Created: 05/98
// Revision: A
//
// Description: This file contains the class implementation for 
// UniaxialMaterial.
//
// What: "@(#) UniaxialMaterial.C, revA"

#include <material/uniaxial/UniaxialMaterial.h>
#include <cstring>
#include <domain/mesh/element/Information.h>
#include <utility/recorder/response/MaterialResponse.h>
#include <cfloat>
#include <utility/matrix/Vector.h>
#include "xc_utils/src/base/CmdStatus.h"
#include "xc_utils/src/base/any_const_ptr.h"
#include "utility/actor/actor/MovableVector.h"

//! @brief Constructor.
XC::UniaxialMaterial::UniaxialMaterial(int tag, int clasTag)
  :Material(tag,clasTag), rho(0.0) {}

//! @brief Lee un objeto XC::UniaxialMaterial desde archivo
bool XC::UniaxialMaterial::procesa_comando(CmdStatus &status)
  {
    const std::string cmd= deref_cmd(status.Cmd());
    if(verborrea>2)
      std::clog << "(UniaxialMaterial) Procesando comando: " << cmd << std::endl;
    if(cmd == "rho")
      {
        rho= interpretaDouble(status.GetString());
        return true;
      }
    else if(cmd == "set_trial_strain")
      {
        std::clog << "El comando: " << cmd << " está pensado para pruebas." << std::endl;
        const double trial_eps= interpretaDouble(status.GetString());
        setTrialStrain(trial_eps);
        return true;
      }
    else
      return Material::procesa_comando(status);
  }

int XC::UniaxialMaterial::setTrial(double strain, double &stress, double &tangent, double strainRate)
  {
    int res = this->setTrialStrain(strain, strainRate);
    if(res == 0)
      {
        stress = this->getStress();
        tangent = this->getTangent();
      }
    else
      {
        std::cerr << "XC::UniaxialMaterial::setTrial() - material failed in setTrialStrain()\n"; 
      }
    return res;
  }

//! @brief Devuelve la deformación inicial.
double XC::UniaxialMaterial::getInitialStrain(void) const
  { return 0.0; }

//! @brief default operation for strain rate is zero
double XC::UniaxialMaterial::getStrainRate(void) const
  { return 0.0; }


//! @brief Devuelve la tensión generalizada.
const XC::Vector &XC::UniaxialMaterial::getGeneralizedStress(void) const
  {
    static Vector retval(1);
    retval(0)= getStress();
    return retval;
  }

//! @brief Devuelve la deformación generalizada.
const XC::Vector &XC::UniaxialMaterial::getGeneralizedStrain(void) const
  {
    static Vector retval(1);
    retval(0)= getStrain();
    return retval;
  }

const XC::Vector &XC::UniaxialMaterial::getInitialGeneralizedStrain(void) const
  {
    static Vector retval(1);
    retval(0)= getInitialStrain();
    return retval;
  }

//! @brief default operation for damping tangent is zero
double XC::UniaxialMaterial::getDampTangent(void) const
  { return 0.0; }

//! @brief default operation for secant stiffness
double XC::UniaxialMaterial::getSecant(void) const
  {
    double strain = this->getStrain();
    double stress = this->getStress();

    if(strain != 0.0)
      return stress/strain;
    else
      return this->getTangent();
  }

//! @brief Devuelve la inversa de la rigidez.
double XC::UniaxialMaterial::getFlexibility(void) const
  {
    double retval;
    const double k= getTangent();
    if(k == 0.0)
      {
        std::cerr << "DqUniaxialMaterial::getFlexibility; singular material stiffness.\n";
	retval= 1.e14;
      }
    else
      retval= 1/k;
    return retval;
  }

//! @brief Devuelve la inversa de la rigidez inicial.
double XC::UniaxialMaterial::getInitialFlexibility(void) const
  {
    double retval;
    const double k= getInitialTangent();
    if(k == 0.0)
      {
        std::cerr << "DqUniaxialMaterial::getInitialFlexibility; singular material stiffness.\n";
	retval= 1.e14;
      }
    else
      retval= 1/k;
    return retval;
  }

//! @brief Devuelve la densidad del material.
double XC::UniaxialMaterial::getRho(void) const
  { return rho; }

//! @brief Assigns la densidad del material.
void XC::UniaxialMaterial::setRho(const double &r)
  { rho= r; }

//! @brief Devuelve una copia del material.
XC::UniaxialMaterial* XC::UniaxialMaterial::getCopy(SectionForceDeformation *s) const
  { return getCopy(); }

//! @brief Establece el valor de la deformación inicial.
int XC::UniaxialMaterial::setInitialStrain(double strain)
  {
    std::clog << "El material: " << nombre_clase() 
              << " no admite deformaciones iniciales." << std::endl;
    return 0;
  }

//! @brief Asigna a la deformación inicial el valor que se pasa como parámetro.
void XC::UniaxialMaterial::setInitialGeneralizedStrain(const Vector &iS)
  { setInitialStrain(iS[0]); }

XC::Response *XC::UniaxialMaterial::setResponse(const std::vector<std::string> &argv, Information &matInfo)
  {
    const size_t argc= argv.size();
    if(argc == 0) return 0;

    // stress
    if(argv[0] == "stress")
      return new MaterialResponse(this, 1, this->getStress());
    // tangent
    else if (argv[0] == "tangent")
      return new MaterialResponse(this, 2, this->getTangent());
    // strain
    else if (argv[0] == "strain")
      return new MaterialResponse(this, 3, this->getStrain());
    // strain
    else if ((argv[0] == "stressStrain") || 
             (argv[0] == "stressANDstrain"))
      { return new MaterialResponse(this, 4, XC::Vector(2)); }
    // otherwise unknown
    else
      { return nullptr; }
  }

int XC::UniaxialMaterial::getResponse(int responseID, Information &matInfo)
  {
    static XC::Vector stressStrain(2);
    // each subclass must implement its own stuff    
    switch(responseID)
      {
      case 1:
        matInfo.setDouble(this->getStress());
        return 0;
      case 2:
        matInfo.setDouble(this->getTangent());
        return 0;      
      case 3:
        matInfo.setDouble(this->getStrain());
        return 0;      
      case 4:
        stressStrain(0) = this->getStress();
        stressStrain(1) = this->getStrain();
        matInfo.setVector(stressStrain);
        return 0;
      default:      
        return -1;
      }
  }


// AddingSensitivity:BEGIN ////////////////////////////////////////
int XC::UniaxialMaterial::setParameter(const std::vector<std::string> &argv, Parameter &param)
  { return -1; }

int XC::UniaxialMaterial::updateParameter(int parameterID, Information &info)
  { return -1; }

int XC::UniaxialMaterial::activateParameter(int parameterID)
  { return -1; }

double XC::UniaxialMaterial::getStressSensitivity(int gradNumber, bool conditional)
  { return 0.0; }

double XC::UniaxialMaterial::getStrainSensitivity(int gradNumber)
  { return 0.0; }

double XC::UniaxialMaterial::getInitialTangentSensitivity(int gradNumber)
  { return 0.0; }

double XC::UniaxialMaterial::getRhoSensitivity(int gradNumber)
  { return 0.0; }

double XC::UniaxialMaterial::getDampTangentSensitivity(int gradNumber)
  { return 0.0; }

int XC::UniaxialMaterial::commitSensitivity(double strainSensitivity, int gradNumber, int numGrads)
  { return -1; }

double XC::UniaxialMaterial::getInitialTangent(void) const
  {
    std::cerr << "XC::UniaxialMaterial::getInitialTangent() -- this mehtod " << std::endl
              << " is not implemented for the selected material. " << std::endl;
    return 0.0;
  }

// AddingSensitivity:END //////////////////////////////////////////

//! @brief Envía los miembros del objeto a través del canal que se pasa como parámetro.
int XC::UniaxialMaterial::sendData(CommParameters &cp)
  {
    setDbTagDataPos(0,getTag());
    int res= cp.sendDouble(rho,getDbTagData(),CommMetaData(1));
    return res;
  }

//! @brief Recibe los miembros del objeto a través del canal que se pasa como parámetro.
int XC::UniaxialMaterial::recvData(const CommParameters &cp)
  {
    setTag(getDbTagDataPos(0));
    int res= cp.receiveDouble(rho,getDbTagData(),CommMetaData(1));
    return res;
  }

//! \brief Devuelve la propiedad del objeto cuyo código (de la propiedad) se pasa
//! como parámetro.
//!
//! Soporta los códigos:
//! nnod: Devuelve el número de nodos del dominio.
any_const_ptr XC::UniaxialMaterial::GetProp(const std::string &cod) const
  {
    if(verborrea>4)
      std::clog << "UniaxialMaterial::GetProp (" << nombre_clase() << "::GetProp) Buscando propiedad: " << cod << std::endl;
    if(cod=="dens")
      return any_const_ptr(rho);
    else if(cod=="strain")
      {
        tmp_gp_dbl= getStrain();
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="strain_rate")
      {
        tmp_gp_dbl= getStrainRate();
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="stress")
      {
        tmp_gp_dbl= getStress();
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="getTangent")
      {
        tmp_gp_dbl= getTangent();
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="getInitialTangent")
      {
        tmp_gp_dbl= getInitialTangent();
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="getDampTangent")
      {
        tmp_gp_dbl= getDampTangent();
        return any_const_ptr(tmp_gp_dbl);
      }
    else
      return Material::GetProp(cod);
  }

//! @brief Recibe un puntero a material a través del canal que se pasa como parámetro.
//! @param posClassTag: Posición de ID del identificador de la clase del material.
//! @param posDbTag: Posición de ID en la que se guarda el dbTag.
XC::UniaxialMaterial *XC::receiveUniaxialMaterialPtr(UniaxialMaterial *ptr,DbTagData &dt,const CommParameters &cp,const BrokedPtrCommMetaData &md)
  {
    UniaxialMaterial *retval= nullptr;
    Material *tmp= nullptr;
    tmp= receiveMaterialPtr(ptr,dt,cp,md);
    if(tmp)
      {
        retval= dynamic_cast<UniaxialMaterial *>(tmp);
        if(!retval)
          {
            std::cerr <<"WARNING receiveUniaxialMaterialPtr - "
                      << " failed to get material." << std::endl;
            delete tmp;
          }
      }
    return retval;
  }
