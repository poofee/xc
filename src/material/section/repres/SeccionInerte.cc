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
//SeccionInerte.cpp

#include "SeccionInerte.h"
#include "xc_basic/src/util/inercia.h"
#include "utility/matrix/Vector.h"
#include "utility/matrix/Matrix.h"
#include "xc_utils/src/geom/pos_vec/Pos2d.h"
#include "xc_utils/src/geom/sis_ref/Ref2d2d.h"
#include "xc_utils/src/geom/sis_ref/EjesPrincInercia2d.h"
#include "material/section/repres/ConstantesSecc3d.h"
#include "material/section/repres/ConstantesSecc2d.h"
#include "xc_utils/src/geom/d1/Segmento2d.h"
#include "xc_utils/src/base/CmdStatus.h"
#include "xc_utils/src/base/utils_any.h"

XC::SeccionInerte::SeccionInerte(EntCmd *owr)
  : EntCmd(owr) {}

//! @brief Devuelve el producto de inercia de la sección homogeneizada respecto al eje paralelo al x por el CDG.
//! @param E0: Módulo elástico de referencia.
double XC::SeccionInerte::getIxSeccHomogeneizada(const double &E0) const
  { return getIySeccHomogeneizada(E0)+getIzSeccHomogeneizada(E0); }



//! @brief Devuelve el ángulo que define un eje principal de inercia.
double XC::SeccionInerte::getTheta_pSeccHomogeneizada(void) const
  { return theta_inercia(getIySeccHomogeneizada(1.0),getIzSeccHomogeneizada(1.0),getPyzSeccHomogeneizada(1.0)); }

//! @brief Devuelve la dirección de un eje principal de inercia (no sabemos si
//! el mayor o el menor
//! @param E0: Módulo elástico de referencia.
XC::Vector XC::SeccionInerte::getDirEjeI_aSeccHomogeneizada(void) const
  {
    const Dir2d p(getTheta_pSeccHomogeneizada());
    Vector retval(2);
    retval[0]= p.dx();
    retval[1]= p.dy();
    return retval;
  }

//! @brief Devuelve la dirección del otro eje principal de inercia (no sabemos si
//! el mayor o el menor
//! @param E0: Módulo elástico de referencia.
XC::Vector XC::SeccionInerte::getDirEjeI_bSeccHomogeneizada(void) const
  {
    const Dir2d p= Dir2d(getTheta_pSeccHomogeneizada()).Perpendicular();
    Vector retval(2);
    retval[0]= p.dx();
    retval[1]= p.dy();
    return retval;
  }

//! @brief Devuelve el momento de inercia principal mayor.
double XC::SeccionInerte::getI1SeccHomogeneizada(const double &E0) const
  { 
    const double iy= getIySeccHomogeneizada(E0);
    const double iz= getIzSeccHomogeneizada(E0);
    const double pyz= getPyzSeccHomogeneizada(E0);
    return I1_inercia(iy,iz,pyz);
  }

//! @brief Devuelve el momento de inercia principal menor.
double XC::SeccionInerte::getI2SeccHomogeneizada(const double &E0) const
  {
    const double iy= getIySeccHomogeneizada(E0);
    const double iz= getIzSeccHomogeneizada(E0);
    const double pyz= getPyzSeccHomogeneizada(E0);
    return I2_inercia(iy,iz,pyz);
  }

//! @brief Devuelve los ejes principales de inercia de la sección homogeneizada.
EjesPrincInercia2d XC::SeccionInerte::getEjesInerciaSeccHomogeneizada(const double &E0) const
  {
    const Vector v= getCdgSeccHomogeneizada(E0);
    const Pos2d cdg(v[0],v[1]);
    return EjesPrincInercia2d(cdg,getIySeccHomogeneizada(E0),getIzSeccHomogeneizada(E0),getPyzSeccHomogeneizada(E0));
  }
//! @brief Devuelve el vector del eje principal I.
Vector2d XC::SeccionInerte::getVDirEje1SeccHomogeneizada(const double &E0) const
  { return getEjesInerciaSeccHomogeneizada(E0).getVDirEje1(); }
//! @brief Devuelve el vector del eje principal I.
Vector2d XC::SeccionInerte::getVDirEjeFuerteSeccHomogeneizada(const double &E0) const
  { return getEjesInerciaSeccHomogeneizada(E0).getVDirEje1(); }
//! @brief Devuelve el vector del eje principal II.
Vector2d XC::SeccionInerte::getVDirEje2SeccHomogeneizada(const double &E0) const
  { return getEjesInerciaSeccHomogeneizada(E0).getVDirEje2(); }
//! @brief Devuelve el vector del eje principal II.
Vector2d XC::SeccionInerte::getVDirEjeDebilSeccHomogeneizada(const double &E0) const
  { return getEjesInerciaSeccHomogeneizada(E0).getVDirEje2(); }

//! @brief Devuelve la componente i,j del tensor de inercia calculado respecto al CDG.
double XC::SeccionInerte::getISeccHomogeneizada(const double &E0,const unsigned short int &i,const unsigned short int &j) const
  {
    unsigned short int k= i + (j-1)*2;
    double retval= 0.0;
    switch(k)
      {
        case 1: //(1,1)
          retval= getIySeccHomogeneizada(E0);
          break;
        case 2: //(2,1)
        case 3: //(1,2)
          retval= -getPyzSeccHomogeneizada(E0);
          break;
	case 4:
          retval= getIzSeccHomogeneizada(E0);
          break;
      }
    return retval;
  }

//! @brief Devuelve el momento de inercia respecto al eje que pasa por O con dirección la de e.
double XC::SeccionInerte::getISeccHomogeneizada(const double &E0,const Pos2d &O,const Vector &e) const
  {
    const Matrix Io(getISeccHomogeneizada(E0,O));
    return dot(e,Io*e)/e.Norm2();
  }

//! @brief Devuelve el momento de inercia respecto a la recta que se pasa
//! como parámetro.
double XC::SeccionInerte::getISeccHomogeneizada(const double &E0,const Recta2d &r) const
  { return getISeccHomogeneizada(E0,r.Punto(),Vector(r.VDir()));  }

//! @brief Devuelve la componente i,j del tensor de inercia calculado respecto al punto "o".
double XC::SeccionInerte::getISeccHomogeneizada(const double &E0,const unsigned short int &i,const unsigned short int &j,const Pos2d &o) const
  {
    const double Iij= getISeccHomogeneizada(E0,i,j);
    const Vector cdg= getCdgSeccHomogeneizada(E0);

    Pos2d pp(cdg[0],cdg[1]);
    Ref2d2d ejes(pp);
    Pos2d pos_local= ejes.GetPosLocal(o);
    return Iij + getAreaSeccHomogeneizada(E0) * pos_local(i) * pos_local(j);
  }

//! @brief Devuelve el momento polar de inercia respecto al punto o.
double XC::SeccionInerte::getIOSeccHomogeneizada(const double &E0,const Pos2d &o) const
  { return (getISeccHomogeneizada(E0,1,1,o)+getISeccHomogeneizada(E0,2,2,o)+getISeccHomogeneizada(E0,3,3,o))/2; }


//! @brief Devuelve el tensor de inercia calculado desde el centro de gravedad del objeto.
XC::Matrix XC::SeccionInerte::getISeccHomogeneizada(const double &E0) const
  {
    Matrix i(2,2);
    i(0,0)= getIySeccHomogeneizada(E0); i(0,1)= -getPyzSeccHomogeneizada(E0);
    i(1,0)= i(0,1);   i(1,1)= getIzSeccHomogeneizada(E0);
    return i;
  }

//! @brief Devuelve el tensor de inercia respector al punto o.
XC::Matrix XC::SeccionInerte::getISeccHomogeneizada(const double &E0,const Pos2d &o) const
  {
    Matrix retval(2,2);
    const Matrix Ig= getISeccHomogeneizada(E0);
    Vector O(2); O[0]= o.x(); O[1]= o.y();
    const Vector og= getCdgSeccHomogeneizada(E0) - O;
    const double m= getAreaSeccHomogeneizada(E0);
    retval= Ig+m*(og.Norm2()*identity(Ig)-(og & og));
    return retval;
  }

XC::ConstantesSecc3d XC::SeccionInerte::getConstantesSecc3d(const ConstantesSecc3d &base) const
  {
    ConstantesSecc3d retval;
    retval= base; //Asignamos E, G y alpha.
    retval.A()= getAreaSeccHomogeneizada(base.E()); 
    retval.Iy()= getIySeccHomogeneizada(base.E());
    retval.Iz()= getIzSeccHomogeneizada(base.E());
    retval.J()= getIxSeccHomogeneizada(base.E());
    return retval;
  }

XC::ConstantesSecc2d XC::SeccionInerte::getConstantesSecc2d(const ConstantesSecc2d &base) const
  {
    static ConstantesSecc2d retval;
    retval= base; //Asignamos E, G y alpha.
    retval.A()= getAreaSeccHomogeneizada(base.E()); 
    retval.I()= getIzSeccHomogeneizada(base.E());
    return retval;
  }

//! @brief Devuelve el producto de inercia de la sección bruta respecto al eje paralelo al x por el CDG.
double XC::SeccionInerte::getIxSeccBruta(void) const
  { return getIySeccBruta()+getIzSeccBruta(); }

//! @brief Devuelve el ángulo que define un eje principal de inercia.
double XC::SeccionInerte::getTheta_pSeccBruta(void) const
  { return theta_inercia(getIySeccBruta(),getIzSeccBruta(),getPyzSeccBruta()); }

//! @brief Devuelve la dirección de un eje principal de inercia (no sabemos si
//! el mayor o el menor
XC::Vector XC::SeccionInerte::getDirEjeI_aSeccBruta(void) const
  {
    const Dir2d p(getTheta_pSeccBruta());
    Vector retval(2);
    retval[0]= p.dx();
    retval[1]= p.dy();
    return retval;
  }

//! @brief Devuelve la dirección del otro eje principal de inercia (no sabemos si
//! el mayor o el menor
XC::Vector XC::SeccionInerte::getDirEjeI_bSeccBruta(void) const
  {
    const Dir2d p= Dir2d(getTheta_pSeccBruta()).Perpendicular();
    Vector retval(2);
    retval[0]= p.dx();
    retval[1]= p.dy();
    return retval;
  }

//! @brief Devuelve el momento de inercia principal mayor.
double XC::SeccionInerte::getI1SeccBruta(void) const
  {
    const double iy= getIySeccBruta();
    const double iz= getIzSeccBruta();
    const double pyz= getPyzSeccBruta();
    return I1_inercia(iy,iz,pyz);
  }

//! @brief Devuelve el momento de inercia principal menor.
double XC::SeccionInerte::getI2SeccBruta(void) const
  {
    const double iy= getIySeccBruta();
    const double iz= getIzSeccBruta();
    const double pyz= getPyzSeccBruta();
    return I2_inercia(iy,iz,pyz);
  }

//! @brief Devuelve los ejes principales de inercia de la sección bruta.
EjesPrincInercia2d XC::SeccionInerte::getEjesInerciaSeccBruta(void) const
  {
    const Vector v= getCdgSeccBruta();
    const Pos2d cdg(v[0],v[1]);
    return EjesPrincInercia2d(cdg,getIySeccBruta(),getIzSeccBruta(),getPyzSeccBruta());
  }
//! @brief Devuelve el vector del eje principal I.
Vector2d XC::SeccionInerte::getVDirEje1SeccBruta(void) const
  { return getEjesInerciaSeccBruta().getVDirEje1(); }
//! @brief Devuelve el vector del eje principal I.
Vector2d XC::SeccionInerte::getVDirEjeFuerteSeccBruta(void) const
  { return getEjesInerciaSeccBruta().getVDirEje1(); }
//! @brief Devuelve el vector del eje principal II.
Vector2d XC::SeccionInerte::getVDirEje2SeccBruta(void) const
  { return getEjesInerciaSeccBruta().getVDirEje2(); }
//! @brief Devuelve el vector del eje principal II.
Vector2d XC::SeccionInerte::getVDirEjeDebilSeccBruta(void) const
  { return getEjesInerciaSeccBruta().getVDirEje2(); }

//! @brief Devuelve la componente i,j del tensor de inercia calculado respecto al CDG.
double XC::SeccionInerte::getISeccBruta(const unsigned short int &i,const unsigned short int &j) const
  {
    unsigned short int k= i + (j-1)*2;
    double retval= 0.0;
    switch(k)
      {
        case 1: //(1,1)
          retval= getIySeccBruta();
          break;
        case 2: //(2,1)
        case 3: //(1,2)
          retval= -getPyzSeccBruta();
          break;
	case 4:
          retval= getIzSeccBruta();
          break;
      }
    return retval;
  }

//! @brief Devuelve el momento de inercia respecto al eje que pasa por O con dirección la de e.
double XC::SeccionInerte::getISeccBruta(const Pos2d &O,const Vector &e) const
  {
    const Matrix Io(getISeccBruta(O));
    return dot(e,Io*e)/e.Norm2();
  }

//! @brief Devuelve el momento de inercia respecto a la recta que se pasa
//! como parámetro.
double XC::SeccionInerte::getISeccBruta(const Recta2d &r) const
  { return getISeccBruta(r.Punto(),Vector(r.VDir()));  }

//! @brief Devuelve la componente i,j del tensor de inercia calculado respecto al punto "o".
double XC::SeccionInerte::getISeccBruta(const unsigned short int &i,const unsigned short int &j,const Pos2d &o) const
  {
    const double Iij= getISeccBruta(i,j);
    const Vector cdg= getCdgSeccBruta();

    Pos2d pp(cdg[0],cdg[1]);
    Ref2d2d ejes(pp);
    Pos2d pos_local= ejes.GetPosLocal(o);
    return Iij + getAreaSeccBruta() * pos_local(i) * pos_local(j);
  }

//! @brief Devuelve el momento polar de inercia respecto al punto o.
double XC::SeccionInerte::getIOSeccBruta(const Pos2d &o) const
  { return (getISeccBruta(1,1,o)+getISeccBruta(2,2,o)+getISeccBruta(3,3,o))/2; }


//! @brief Devuelve el tensor de inercia calculado desde el centro de gravedad del objeto.
XC::Matrix XC::SeccionInerte::getISeccBruta(void) const
  {
    Matrix i(2,2);
    i(0,0)= getIySeccBruta(); i(0,1)= -getPyzSeccBruta();
    i(1,0)= i(0,1);   i(1,1)= getIzSeccBruta();
    return i;
  }

//! @brief Devuelve el tensor de inercia respector al punto o.
XC::Matrix XC::SeccionInerte::getISeccBruta(const Pos2d &o) const
  {
    Matrix retval(2,2);
    const Matrix Ig= getISeccBruta();
    Vector O(2); O[0]= o.x(); O[1]= o.y();
    const Vector og= getCdgSeccBruta() - O;
    const double m= getAreaSeccBruta();
    retval= Ig+m*(og.Norm2()*identity(Ig)-(og & og));
    return retval;
  }

//! @brief Devuelve la propiedad del objeto cuyo código (de la propiedad)
//! se pasa como parámetro.
any_const_ptr XC::SeccionInerte::GetProp(const std::string &cod) const
  {
    if(cod=="getAreaSeccHomogeneizada")
      {
        const double E0= popDouble(cod);
        tmp_gp_dbl= getAreaSeccHomogeneizada(E0);
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="getIySeccHomogeneizada")
      {
        const double E0= popDouble(cod);
        tmp_gp_dbl= getIySeccHomogeneizada(E0);
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="getIzSeccHomogeneizada")
      {
        const double E0= popDouble(cod);
        tmp_gp_dbl= getIzSeccHomogeneizada(E0);
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="getPyzSeccHomogeneizada")
      {
        const double E0= popDouble(cod);
        tmp_gp_dbl= getPyzSeccHomogeneizada(E0);
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="getCdgSeccHomogeneizada")
      {
        static m_double tmp_m_double(1,2);
        const double E0= popDouble(cod);
        const Vector tmp= getCdgSeccHomogeneizada(E0);
        tmp_m_double(1,1)= tmp[0];
        tmp_m_double(1,2)= tmp[1];
        return any_const_ptr(tmp_m_double);
      }
   else if(cod=="getI1SeccHomogeneizada")
      {
        const double E0= popDouble(cod);
        tmp_gp_dbl= getI1SeccHomogeneizada(E0);
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="getI2SeccHomogeneizada")
      {
        const double E0= popDouble(cod);
        tmp_gp_dbl= getI2SeccHomogeneizada(E0);
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="getVDirEje1SeccHomogeneizada")
      {
        const double E0= popDouble(cod);
        tmp_gp_vector2d= getEjesInerciaSeccHomogeneizada(E0).getVDirEje1();
        return any_const_ptr(tmp_gp_vector2d);
      }
    else if(cod=="getVDirEje2SeccHomogeneizada")
      {
        const double E0= popDouble(cod);
        tmp_gp_vector2d= getEjesInerciaSeccHomogeneizada(E0).getVDirEje2();
        return any_const_ptr(tmp_gp_vector2d);
      }
    else if(cod=="getVDirEjeFuerteSeccHomogeneizada")
      {
        const double E0= popDouble(cod);
        tmp_gp_vector2d= getEjesInerciaSeccHomogeneizada(E0).getVDirEje1();
        return any_const_ptr(tmp_gp_vector2d);
      }
    else if(cod=="getVDirEjeDebilSeccHomogeneizada")
      {
        const double E0= popDouble(cod);
        tmp_gp_vector2d= getEjesInerciaSeccHomogeneizada(E0).getVDirEje2();
        return any_const_ptr(tmp_gp_vector2d);
      }
    else if(cod=="getAreaSeccBruta")
      {
        tmp_gp_dbl= getAreaSeccBruta();
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="getIySeccBruta")
      {
        tmp_gp_dbl= getIySeccBruta();
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="getIzSeccBruta")
      {
        tmp_gp_dbl= getIzSeccBruta();
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="getPyzSeccBruta")
      {
        tmp_gp_dbl= getPyzSeccBruta();
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="getCdgSeccBruta")
      {
        static m_double tmp_m_double(1,2);
        const Vector tmp= getCdgSeccBruta();
        tmp_m_double(1,1)= tmp[0];
        tmp_m_double(1,2)= tmp[1];
        return any_const_ptr(tmp_m_double);
      }
    else if(cod=="getI1SeccBruta")
      {
        tmp_gp_dbl= getI1SeccBruta();
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="getI2SeccBruta")
      {
        tmp_gp_dbl= getI2SeccBruta();
        return any_const_ptr(tmp_gp_dbl);
      }
    else if(cod=="getVDirEje1SeccBruta")
      {
        tmp_gp_vector2d= getEjesInerciaSeccBruta().getVDirEje1();
        return any_const_ptr(tmp_gp_vector2d);
      }
    else if(cod=="getVDirEje2SeccBruta")
      {
        tmp_gp_vector2d= getEjesInerciaSeccBruta().getVDirEje2();
        return any_const_ptr(tmp_gp_vector2d);
      }
    else if(cod=="getVDirEjeFuerteSeccBruta")
      {
        tmp_gp_vector2d= getEjesInerciaSeccBruta().getVDirEje1();
        return any_const_ptr(tmp_gp_vector2d);
      }
    else if(cod=="getVDirEjeDebilSeccBruta")
      {
        tmp_gp_vector2d= getEjesInerciaSeccBruta().getVDirEje2();
        return any_const_ptr(tmp_gp_vector2d);
      }
    else
      return EntCmd::GetProp(cod);
  }

