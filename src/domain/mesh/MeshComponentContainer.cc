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
//MeshComponentContainer.cpp


#include "MeshComponentContainer.h"
#include "xc_utils/src/base/CmdStatus.h"
#include "domain/domain/Domain.h"
#include "xc_utils/src/base/any_const_ptr.h"

//! @brief Constructor.
XC::MeshComponentContainer::MeshComponentContainer(EntCmd *owr,int classTag, int dbTag)
  :EntCmd(owr), MovableObject(classTag, dbTag) {}

//! @brief Lee un objeto MeshComponentContainer desde archivo
bool XC::MeshComponentContainer::procesa_comando(CmdStatus &status)
  {
    const std::string cmd= deref_cmd(status.Cmd());
    if(verborrea>2)
      std::clog << "(MeshComponentContainer) Procesando comando: " << cmd << std::endl;

    return EntCmd::procesa_comando(status);
  }

//! @brief Devuelve un puntero al dominio en el que se define la malla.
const XC::Domain *XC::MeshComponentContainer::getDomain(void) const
  {
    const Domain *retval= nullptr;
    const EntProp *owr= Owner();
    if(owr)
      retval= dynamic_cast<const Domain *>(owr);
    return retval;
  }

//! @brief Devuelve un puntero al dominio en el que se define la malla.
XC::Domain *XC::MeshComponentContainer::getDomain(void)
  {
    Domain *retval= nullptr;
    EntProp *owr= Owner();
    if(owr)
      retval= dynamic_cast<Domain *>(owr);
    return retval;
  }

//! @brief Devuelve la propiedad del objeto cuyo código (de la propiedad) se pasa
any_const_ptr XC::MeshComponentContainer::GetProp(const std::string &cod) const
  {
    return EntCmd::GetProp(cod);
  }

