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
                                                                        
#ifndef BrickSelfWeight_h
#define BrickSelfWeight_h

// Written: ZHYang, UCDavis

// Purpose: This file contains the class definition for 8 node brick self weigth load.

#include <domain/load/ElementBodyLoad.h>

namespace XC {
//! @ingroup ElemLoads
//
//! @brief Peso propio sobre elemento hexaédrico.
class BrickSelfWeight: public ElementBodyLoad
  {
  private:
    static Vector data;
  protected:
    int sendData(CommParameters &cp);
    int recvData(const CommParameters &cp);
    bool procesa_comando(CmdStatus &status);
  public:
    BrickSelfWeight(int tag, const ID &theElementTags);
    BrickSelfWeight(int tag);
    BrickSelfWeight(void);    

    int getType(void);

    int sendSelf(CommParameters &);  
    int recvSelf(const CommParameters &);
    void Print(std::ostream &s, int flag =0);       
  };
} // fin namespace XC

#endif

