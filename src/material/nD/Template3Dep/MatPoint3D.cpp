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
//=============================================================================
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite XC::Element Program
// FILE:              MatPoint3D.cpp
// CLASS:             MatPoint3D
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Material Point
// RETURN:
// VERSION:
// LANGUAGE:          C++.ver >= 3.0 (Borland.C++.ver=3.1||SUN.C++.ver=3.0.1)
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Boris Jeremic
// PROGRAMMER:        Boris Jeremic
// DATE:              17 October 1994.
// UPDATE HISTORY:
//
//
//#                    Aug 2000 porting to OpenSees                            #
//
//=============================================================================
//
#ifndef MATPOINT3D_CPP
#define MATPOINT3D_CPP

#include "material/nD/Template3Dep/MatPoint3D.h"
#include "utility/matrix/nDarray/stresst.h"
#include "utility/matrix/nDarray/straint.h"

#include "material/nD/Template3Dep/EPState.h"
#include "material/nD/NDMaterial.h"

//=============================================================================
// Constructor
XC::MatPoint3D::MatPoint3D(short int INr_direction_point_number,
                       short int INs_direction_point_number,
                       short int INt_direction_point_number,
                       double r_coord,
                       double s_coord,
                       double t_coord,
                       double r_weight,
                       double s_weight,
                       double t_weight,
                       XC::NDMaterial * p_INmatmodel
                       //XC::stresstensor * p_INstress,
                       //XC::stresstensor * p_INiterative_stress,
                       //double         IN_q_ast_iterative,
                       //XC::straintensor * p_INstrain,
                       //BJtensor * p_Tangent_E_tensor,
                      )
  {
    r_direction_point_number = INr_direction_point_number;
    s_direction_point_number = INs_direction_point_number;
    t_direction_point_number = INt_direction_point_number;
    set_coordinates(r_coord,s_coord,t_coord);
    set_weights(r_weight,s_weight,t_weight);

    //if( eps ) {
    //    gpEPS = eps->newObj();  // Elasto-plastic 3D
    //}
    //else 
    //    gpEPS = 0;
    
    if(p_INmatmodel)
        matmodel = p_INmatmodel->getCopy();
    else 
        matmodel = 0;		

    //p_stress   = p_INstress; 
    //p_iterative_stress   = p_INiterative_stress;
    //q_ast_iterative      = IN_q_ast_iterative;
    //p_strain   = p_INstrain;
    //TangentE   = p_Tangent_E_tensor;

  }

//! @brief Destructor.
XC::MatPoint3D::~MatPoint3D(void) 
  {
    if(matmodel)
      //delete [] matmodel; //bug found: Inconsistent delete
      delete matmodel;

    //if( GP )
    //  delete [] GP;
  }

////=============================================================================
//XC::MatPoint3D::MatPoint3D(EPState *eps) {
//
//    this->r_direction_point_number = 0;
//    this->s_direction_point_number = 0;
//    this->t_direction_point_number = 0;
//    this->r = 0;
//    this->s = 0;
//    this->t = 0;
//    this->rw = 0;
//    this->sw = 0;
//    this->tw = 0;
//	     
//    if( eps ) 
//        gpEPS = eps->newObj();      
//    else
//        //gpEPS = new EPState();  //otherwise use default EPState __Zhaohui 09-30-2000
//        gpEPS = 0;  //otherwise use default XC::EPState __Zhaohui 09-30-2000
//    
//}

//=============================================================================
void XC::MatPoint3D::Initialize( short int INr_direction_point_number,
                             short int INs_direction_point_number,
                             short int INt_direction_point_number,
                             double r_coord,
                             double s_coord,
                             double t_coord,
                             double r_weight,
                             double s_weight,
                             double t_weight,
                             //EPState *EPS,
			     //XC::stresstensor * p_INstress,
                             //XC::stresstensor * p_INiterative_stress,
                             //double         IN_q_ast_iterative,
                             //XC::straintensor * p_INstrain,
                             //BJtensor *       p_Tangent_E_tensor,
                             XC::NDMaterial * p_INmmodel
                           )
  {
    this->r_direction_point_number = INr_direction_point_number;
    this->s_direction_point_number = INs_direction_point_number;
    this->t_direction_point_number = INt_direction_point_number;
    set_coordinates(r_coord,s_coord,t_coord);
    set_weights(r_weight,s_weight,t_weight);

    //if(EPS)
    //   this->gpEPS = EPS->newObj();
    //else 
    //   this->gpEPS = 0;

    if( p_INmmodel )
      this->matmodel = p_INmmodel;
    else 
      this->matmodel = 0;

    //this->p_stress   = p_INstress;
    //this->p_iterative_stress   = p_INiterative_stress;
    //q_ast_iterative       = IN_q_ast_iterative;
    //this->p_strain   = p_INstrain;
    //this->TangentE   = p_Tangent_E_tensor;
  }


//=============================================================================
//return MatPoint
XC::MatPoint3D *XC::MatPoint3D::GP(void) 
  { return this; }

//=============================================================================
short int XC::MatPoint3D::GP_number_r(void) const
  { return r_direction_point_number; }


//=============================================================================
short int XC::MatPoint3D::GP_number_s(void) const
  { return s_direction_point_number; }

//=============================================================================
short int XC::MatPoint3D::GP_number_t(void) const
  { return t_direction_point_number; }

//! @brief Coordenada r.
double XC::MatPoint3D::r_coordinate() const
  { return r; }

//! @brief Coordenada s.
double XC::MatPoint3D::s_coordinate() const
  { return s; }

//! @brief Coordenada t.
double XC::MatPoint3D::t_coordinate() const
  { return t; }

//! @brief Peso r.
double XC::MatPoint3D::r_weight() const
  { return rw; }

//! @brief Peso s.
double XC::MatPoint3D::s_weight() const
  { return sw; }

//! @brief Peso t.
double XC::MatPoint3D::t_weight() const
  { return tw; }

////=============================================================================
//void XC::MatPoint3D::setEPS(EPState *eps) {
//
//    if( eps ) 
//        gpEPS = eps->newObj();  
//    else 
//        gpEPS = 0;
//	//g3ErrorHandler->warning("MatPoint3D::setEPS  No initial values for XC::EPState, using default");
//
//}


////=============================================================================
//EPState* XC::MatPoint3D::getEPS() const {
//
//    return gpEPS;
//}
//


//=============================================================================
XC::NDMaterial* XC::MatPoint3D::getNDMat() const
  { return matmodel; }

//=============================================================================
double XC::MatPoint3D::getrho(void) const 
  { return matmodel->getRho(); }

const std::string &XC::MatPoint3D::getType(void)const
  { return matmodel->getType(); }

int XC::MatPoint3D::getTag(void) const
  { return matmodel->getTag(); }

//! @brief Devuelve el tensor de tensiones.
const XC::stresstensor &XC::MatPoint3D::getStressTensor(void) const
  { return matmodel->getStressTensor(); }


//! @brief Devuelve el tensor de deformaciones.
const XC::straintensor &XC::MatPoint3D::getStrainTensor() const
  { return matmodel->getStrainTensor(); }

//=============================================================================
const XC::straintensor &XC::MatPoint3D::getPlasticStrainTensor() const
  { return matmodel->getPlasticStrainTensor(); }

//=============================================================================
double XC::MatPoint3D::getpsi() const 
{
    return matmodel->getpsi();
}


//================================================================================
int XC::MatPoint3D::commitState(void)
{
	int err;
	err = matmodel->commitState();
	return err;
}

//================================================================================
int XC::MatPoint3D::revertToLastCommit(void)
{
	int err;
	err = matmodel->revertToLastCommit();
	return err;
}

//================================================================================
int XC::MatPoint3D::revertToStart(void)
{
	int err;
	err = matmodel->revertToStart();
	return err;
}

//=============================================================================
void XC::MatPoint3D::report(const std::string &msg) const
  {
    //if( msg ) ::printf("%s",msg);
    std::cerr << msg;
    
    std::cerr << "\n\n\n---------------------------------------------------- \n";
    std::cerr << "Gauss point " << GP_number_r() << " #r "
       << GP_number_s() << " #s "<< GP_number_t() << " #t \n";

    std::cerr << "\tr-> " << r_coordinate() << " s->"
       << s_coordinate() << " t->"<< t_coordinate() << std::endl;

    ::printf("\tr_weight->%.8e   s_weight->%.8e   t_weight->%.8e  \n",
     r_weight(),s_weight(),t_weight());


    if( matmodel ) 
      {
        std::cerr << (*matmodel);

        XC::stresstensor tmpstress = matmodel->getStressTensor();
        tmpstress.report("stress at this Gauss-Legendre point\n");

        XC::straintensor tmpstrain = matmodel->getStrainTensor();
        tmpstrain.report("strain at this Gauss-Legendre point\n");
      }
    else
      std::cerr << "Empty XC::Material Model\n"; 

    //p_stress->report("stress at this Gauss-Legendre point\n");
    //p_iterative_stress->reportshortpqtheta("ITERATIVE stress at this Gauss-Legendre point\n");
    //::printf("ITERATIVE q_ast_iterative = %.8e  \n",q_ast_iterative);
    //p_strain->report("strain at this Gauss-Legendre point\n");
    //matmodel->report("material model at this Gauss-Legendre point\n");
    
    //if(gpEPS) 
    //  std::cerr << (*gpEPS);
    //else
    //  std::cerr << "Empty XC::EPState\n"; 
    
  }

//=============================================================================
void XC::MatPoint3D::reportpqtheta(const std::string &msg) const
  {
    //if( msg ) ::printf("%s",msg);
    //p_stress->reportshortpqtheta("");
    //p_stress->reportshortpqtheta("");
    std::cerr << msg;
    
    //if( gpEPS ) {
    //   ( gpEPS->getStress() ).reportshortpqtheta("");
    //   ( gpEPS->getStress() ).reportSHORTs1s2s3("");
    //}
   
    if( matmodel ) {
       XC::stresstensor tmp = matmodel->getStressTensor();
       tmp.reportshortpqtheta("");
       tmp.reportSHORTs1s2s3("");
    }

  }

//=============================================================================
void XC::MatPoint3D::reportTensor(const std::string &msg) const
  {
    //if( msg ) ::printf("%s",msg);
    //p_stress->reportTensor("");

    std::cerr << msg;
    
    //if(gpEPS )
    //   ( gpEPS->getStress() ).reportTensor("");

    if( matmodel ) {
       XC::stresstensor tmp = matmodel->getStressTensor();
       tmp.reportTensor("");
    }

  }


#endif




