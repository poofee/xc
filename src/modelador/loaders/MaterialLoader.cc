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
//MaterialLoader.cc

#include "MaterialLoader.h"
#include "domain/domain/Domain.h"
#include "xc_utils/src/base/CmdStatus.h"
#include "xc_utils/src/base/Lista.h"

//Uniaxial material.
#include "material/uniaxial/BarSlipMaterial.h"
//#include "material/uniaxial/BoucWenMaterial.h"
#include "material/uniaxial/CableMaterial.h"
#include "material/uniaxial/concrete/Concrete01.h"
#include "material/uniaxial/concrete/Concrete04.h"
#include "material/uniaxial/ENTMaterial.h"
#include "material/uniaxial/EPPGapMaterial.h"
#include "material/uniaxial/ElasticMaterial.h"
#include "material/uniaxial/ElasticPPMaterial.h"
#include "material/uniaxial/FatigueMaterial.h"
#include "material/uniaxial/HardeningMaterial.h"
#include "material/uniaxial/HystereticMaterial.h"
#include "material/uniaxial/MinMaxMaterial.h"
#include "material/uniaxial/NewUniaxialMaterial.h"
#include "material/uniaxial/connected/ParallelMaterial.h"
#include "material/uniaxial/connected/SeriesMaterial.h"
#include "material/uniaxial/PathIndependentMaterial.h"
#include "material/uniaxial/Pinching4Material.h"
#include "material/uniaxial/ReinforcingSteel.h"
#include "material/uniaxial/steel/Steel01.h"
#include "material/uniaxial/steel/Steel02.h"
#include "material/uniaxial/steel/Steel03.h"
#include "material/uniaxial/ViscousMaterial.h"

#include "material/uniaxial/fedeas/FedeasBond1Material.h"
#include "material/uniaxial/fedeas/FedeasBond2Material.h"
#include "material/uniaxial/fedeas/FedeasConcr1Material.h"
#include "material/uniaxial/fedeas/FedeasConcr2Material.h"
#include "material/uniaxial/fedeas/FedeasConcr3Material.h"
#include "material/uniaxial/fedeas/FedeasHardeningMaterial.h"
#include "material/uniaxial/fedeas/FedeasHyster1Material.h"
#include "material/uniaxial/fedeas/FedeasHyster2Material.h"
#include "material/uniaxial/fedeas/FedeasSteel1Material.h"
#include "material/uniaxial/fedeas/FedeasSteel2Material.h"

#include "material/uniaxial/PY/PySimple1.h"
#include "material/uniaxial/PY/QzSimple1.h"
#include "material/uniaxial/PY/TzSimple1.h"
#include "material/uniaxial/PY/PyLiq1.h"
#include "material/uniaxial/PY/TzLiq1.h"

#include "material/uniaxial/snap/Bilinear.h"
#include "material/uniaxial/snap/Clough.h"
#include "material/uniaxial/snap/CloughDamage.h"
#include "material/uniaxial/snap/Pinching.h"
#include "material/uniaxial/snap/PinchingDamage.h"


//nDMaterial
#include "material/nD/NDMaterial.h"
#include "material/nD/Template3Dep/Template3Dep.h"

#include "material/nD/ElasticCrossAnisotropic.h"
#include "material/nD/elastic_isotropic/ElasticIsotropic3D.h"
#include "material/nD/elastic_isotropic/ElasticIsotropicBeamFiber.h"
#include "material/nD/elastic_isotropic/ElasticIsotropicPlateFiber.h"
#include "material/nD/elastic_isotropic/ElasticIsotropicPlaneStress2D.h"
#include "material/nD/elastic_isotropic/ElasticIsotropicPlaneStrain2D.h"
#include "material/nD/elastic_isotropic/PressureDependentElastic3D.h"

#include "material/nD/FeapMaterial.h"

#include "material/nD/j2_plasticity/J2AxiSymm.h"
#include "material/nD/j2_plasticity/J2PlaneStrain.h"
#include "material/nD/j2_plasticity/J2PlaneStress.h"
#include "material/nD/j2_plasticity/J2PlateFiber.h"
#include "material/nD/j2_plasticity/J2ThreeDimensional.h"

#include "material/nD/nd_adaptor/PlaneStressMaterial.h"
#include "material/nD/nd_adaptor/PlateFiberMaterial.h"
#include "material/nD/nd_adaptor/BeamFiberMaterial.h"

#include "material/nD/FiniteDeformation/FDdecoupledElastic3D.h"
#include "material/nD/FiniteDeformation/NeoHookeanCompressible3D.h"
#include "material/nD/FiniteDeformation/FiniteDeformationEP3D.h"

#include "material/nD/soil/cyclicSoil/MultiaxialCyclicPlasticity3D.h"
#include "material/nD/soil/cyclicSoil/MultiaxialCyclicPlasticityAxiSymm.h"
#include "material/nD/soil/cyclicSoil/MultiaxialCyclicPlasticityPlaneStrain.h"

#include "material/nD/soil/FluidSolidPorousMaterial.h"
#include "material/nD/soil/PressureDependMultiYield.h"
#include "material/nD/soil/PressureDependMultiYield02.h"
#include "material/nD/soil/PressureIndependMultiYield.h"

//Section
#include "material/section/Bidirectional.h"
#include "material/section/elastic_section/ElasticSection2d.h"
#include "material/section/elastic_section/ElasticShearSection2d.h"
#include "material/section/elastic_section/ElasticSection3d.h"
#include "material/section/elastic_section/ElasticShearSection3d.h"
#include "material/section/fiber_section/FiberSection2d.h"
#include "material/section/fiber_section/FiberSection3d.h"
#include "material/section/fiber_section/FiberSectionGJ.h"
#include "material/section/fiber_section/FiberSectionShear3d.h"
#include "material/section/GenericSection1d.h"
#include "material/section/GenericSectionNd.h"
#include "material/section/Isolator2spring.h"
#include "material/section/SectionAggregator.h"
#include "material/section/yieldSurface/YS_Section2D02.h"
#include "material/section/yieldSurface/YS_Section2D01.h"
#include "material/section/diag_interaccion/DatosDiagInteraccion.h"
#include "material/section/repres/geom_section/GeomSection.h"
#include "material/section/diag_interaccion/DiagInteraccion.h"

//Plate section.
#include "material/section/plate_section/ElasticPlateSection.h"
#include "material/section/plate_section/ElasticMembranePlateSection.h"
#include "material/section/plate_section/MembranePlateFiberSection.h"

//Yield surface
#include "material/yieldSurface/plasticHardeningMaterial/ExponReducing.h"
#include "material/yieldSurface/plasticHardeningMaterial/MultiLinearKp.h"
#include "material/yieldSurface/plasticHardeningMaterial/NullPlasticMaterial.h"



#include "xc_utils/src/base/any_const_ptr.h"
#include "xc_utils/src/base/utils_any.h"
#include "xc_utils/src/nucleo/InterpreteRPN.h"

//! @brief Constructor por defecto.
XC::MaterialLoader::MaterialLoader(Modelador *owr)
  : Loader(owr), tag_mat(0) {}

XC::Material *load_uniaxial_py_material(int tag_mat,const std::string &cmd)
  {
    XC::Material *retval= nullptr;
    if(cmd == "py_simple1")
      retval= new XC::PySimple1(tag_mat);
    else if(cmd == "qz_simple1")
      retval= new XC::QzSimple1(tag_mat);
    else if(cmd == "tz_simple1")
      retval= new XC::TzSimple1(tag_mat);
    else if(cmd == "py_liq1")
      retval= new XC::PyLiq1(tag_mat);
    else if(cmd == "tz_liq1")
      retval= new XC::TzLiq1(tag_mat);
    return retval;
  }

XC::Material *load_uniaxial_snap_material(int tag_mat,const std::string &cmd)
  {
    XC::Material *retval= nullptr;
    if(cmd == "snap_bilinear")
      retval= new XC::Bilinear(tag_mat);
    else if(cmd == "snap_clough")
      retval= new XC::Clough(tag_mat);
    else if(cmd == "snap_pinching")
      retval= new XC::Pinching(tag_mat);
    else if(cmd == "snap_clough_damage")
      retval= new XC::Clough(tag_mat);
    else if(cmd == "snap_pinching_damage")
      retval= new XC::Pinching(tag_mat);
    return retval;
  }

XC::Material *load_uniaxial_fedeas_material(int tag_mat,const std::string &cmd)
  {
    XC::Material *retval= nullptr;
    if(cmd == "fedeas_bond1_material")
      retval= new XC::FedeasBond1Material(tag_mat);
    else if(cmd == "fedeas_bond2_material")
      retval= new XC::FedeasBond2Material(tag_mat);
    else if(cmd == "fedeas_concr1_material")
      retval= new XC::FedeasConcr1Material(tag_mat);
    else if(cmd == "fedeas_concr2_material")
      retval= new XC::FedeasConcr2Material(tag_mat);
    else if(cmd == "fedeas_concr3_material")
      retval= new XC::FedeasConcr3Material(tag_mat);
    else if(cmd == "fedeas_hardening_material")
      retval= new XC::FedeasHardeningMaterial(tag_mat);
    else if(cmd == "fedeas_hyster1_material")
      retval= new XC::FedeasHyster1Material(tag_mat);
    else if(cmd == "fedeas_hyster2_material")
      retval= new XC::FedeasHyster2Material(tag_mat);
    else if(cmd == "fedeas_steel1_material")
      retval= new XC::FedeasSteel1Material(tag_mat);
    else if(cmd == "fedeas_steel2_material")
      retval= new XC::FedeasSteel2Material(tag_mat);
    return retval;
  }

//! @brief Procesa los comandos que se emplean para definir
//! materiales uniaxiales para su empleo en el modelo de
//! elementos finitos. Interpreta los siguientes comandos:
//!
//! - elastic_material: Define un material uniaxial elástico lineal.
//! - elasticpp_material: Define un material uniaxial elastoplástico perfecto.
//! - epp_gap_material: Define una material uniaxial elastoplástico perfecto con «gap».
//! - cable_material: Define un material uniaxial de tipo «cable» sin resistencia a compresión.
//! - concrete01_material: Define un material uniaxial para representar el comportamiento del hormigón.
//! - concrete04_material: Define un material uniaxial para representar el comportamiento del hormigón.
//! - steel01: Define un material uniaxial para representar el comportamiento del acero.
//! - steel02: Define un material uniaxial para representar el comportamiento del acero.
//! - steel03: Define un material uniaxial para representar el comportamiento del acero.
XC::Material *load_uniaxial_material(int tag_mat,const std::string &cmd)
  {
    XC::Material *retval= nullptr;
    if(cmd == "bar_slip_material")
      retval= new XC::BarSlipMaterial(tag_mat);
    else if(cmd == "cable_material")
      retval= new XC::CableMaterial(tag_mat);
    else if(cmd == "concrete01_material")
      retval= new XC::Concrete01(tag_mat);
    else if(cmd == "concrete04_material")
      retval= new XC::Concrete04(tag_mat);
    else if(cmd == "elast_no_trac_material")
      retval= new XC::ENTMaterial(tag_mat);
    else if(cmd == "epp_gap_material")
      retval= new XC::EPPGapMaterial(tag_mat);
    else if(cmd == "elastic_material")
      retval= new XC::ElasticMaterial(tag_mat);
    else if(cmd == "elasticpp_material")
      retval= new XC::ElasticPPMaterial(tag_mat);
    else if(cmd == "fatigue_material")
      retval= new XC::FatigueMaterial(tag_mat);
    else if(cmd == "hardening_material")
      retval= new XC::HardeningMaterial(tag_mat);
    else if(cmd == "hysteretic_material")
      retval= new XC::HystereticMaterial(tag_mat);
    else if(cmd == "min_max_material")
      retval= new XC::MinMaxMaterial(tag_mat);
    else if(cmd == "parallel_material")
      retval= new XC::ParallelMaterial(tag_mat);
    else if(cmd == "path_independent_material")
      retval= new XC::PathIndependentMaterial(tag_mat);
    else if(cmd == "pinching4_material")
      retval= new XC::Pinching4Material(tag_mat);
    else if(cmd == "series_material")
      retval= new XC::SeriesMaterial(tag_mat);
    else if(cmd == "reinforcing_steel")
      retval= new XC::ReinforcingSteel(tag_mat);
    else if(cmd == "steel01")
      retval= new XC::Steel01(tag_mat);
    else if(cmd == "steel02")
      retval= new XC::Steel02(tag_mat);
    else if(cmd == "steel03")
      retval= new XC::Steel03(tag_mat);
    else if(cmd == "viscous_material")
      retval= new XC::ViscousMaterial(tag_mat);
    return retval;
  }

//! @brief Procesa los comandos que se emplean para definir
//! materiales nD para su empleo en el modelo de
//! elementos finitos. Interpreta los siguientes comandos:
//!
//! - elastic_cross_anisotropic: Define un material elástico anisótropo.
//! - elastic_isotropic_3d: Define un material elástico isótropo.
//! - elastic_isotropic_plane_stress_2d: Define un material
//!   elástico isótropo para problemas de tensión plana.
//! - elastic_isotropic_plane_strain_2d: Define un material
//!   elástico isótropo para problemas de deformación plana.
XC::Material *load_nD_material(int tag_mat,const std::string &cmd)
  {
    XC::Material *retval= nullptr;
    if(cmd == "elastic_cross_anisotropic")
      retval= new XC::ElasticCrossAnisotropic(tag_mat);
    else if(cmd == "elastic_isotropic_3d")
      retval= new XC::ElasticIsotropic3D(tag_mat);
    else if(cmd == "elastic_isotropic_plate_fiber")
      retval= new XC::ElasticIsotropicPlateFiber(tag_mat);
    else if(cmd == "elastic_isotropic_plane_stress_2d")
      retval= new XC::ElasticIsotropicPlaneStress2D(tag_mat);
    else if(cmd == "elastic_isotropic_plane_strain_2d")
      retval= new XC::ElasticIsotropicPlaneStrain2D(tag_mat);
    else if(cmd == "elastic_isotropic_beam_fiber")
      retval= new XC::ElasticIsotropicBeamFiber(tag_mat);
    else if(cmd == "J2_axy_symm")
      retval= new XC::J2AxiSymm(tag_mat);
    else if(cmd == "J2_plane_strain")
      retval= new XC::J2PlaneStrain(tag_mat);
    else if(cmd == "J2_plane_stress")
      retval= new XC::J2PlaneStress(tag_mat);
    else if(cmd == "J2_plate_fiber")
      retval= new XC::J2PlateFiber(tag_mat);
    else if(cmd == "J2_three_dimensional")
      retval= new XC::J2ThreeDimensional(tag_mat);
    else if(cmd == "plane_stress_material")
      retval= new XC::PlaneStressMaterial(tag_mat);
    else if(cmd == "plate_fiber_material")
      retval= new XC::PlateFiberMaterial(tag_mat);
    else if(cmd == "pressure_dependent_elastic_3d")
      retval= new XC::PressureDependentElastic3D(tag_mat);
    else if(cmd == "template_3d_ep")
      retval= new XC::Template3Dep(tag_mat);
    return retval;
  }

XC::Material *load_nD_finite_deformation_material(int tag_mat,const std::string &cmd)
  {
    XC::Material *retval= nullptr;
    if(cmd == "neo_hookean_compressible_3d")
      retval= new XC::NeoHookeanCompressible3D(tag_mat);
    else if(cmd == "fd_decoupled_elastic_3d")
      retval= new XC::FDdecoupledElastic3D(tag_mat);
    else if(cmd == "finite_deformation_ep_3d")
      retval= new XC::FiniteDeformationEP3D(tag_mat);
    return retval;
  }

XC::Material *load_nD_cyclic_soil_material(int tag_mat,const std::string &cmd)
  {
    XC::Material *retval= nullptr;
    if(cmd == "multiaxial_cyclic_plasticity_3D")
      retval= new XC::MultiaxialCyclicPlasticity3D(tag_mat);
    else if(cmd == "multiaxial_cyclic_plasticity_axi_symm")
      retval= new XC::MultiaxialCyclicPlasticityAxiSymm(tag_mat);
    else if(cmd == "multiaxial_cyclic_plasticity_plane_strain")
      retval= new XC::MultiaxialCyclicPlasticityPlaneStrain(tag_mat);
    return retval;
  }

XC::Material *load_nD_soil_material(int tag_mat,const std::string &cmd)
  {
    XC::Material *retval= nullptr;
    if(cmd == "fluid_solid_porous_material")
      retval= new XC::FluidSolidPorousMaterial(tag_mat);
    else if(cmd == "pressure_depend_multi_yield")
      retval= new XC::PressureDependMultiYield(tag_mat);
    else if(cmd == "pressure_depend_multi_yield02")
      retval= new XC::PressureDependMultiYield02(tag_mat);
    else if(cmd == "pressure_independ_multi_yield")
      retval= new XC::PressureIndependMultiYield(tag_mat);
    return retval;
  }

//! @brief Procesa los comandos que se emplean para definir
//! materiales para láminas. Interpreta los siguientes comandos:
//!
//! - elastic_membrane_plate_section: Define un material elástico para problemas de láminas.
//! - elastic_plate_section: Define un material elástico para problemas de placas.
XC::Material *load_plate_section_material(int tag_mat,const std::string &cmd,XC::MaterialLoader *mloader)
  {
    XC::Material *retval= nullptr;
    if(cmd == "elastic_membrane_plate_section")
      retval= new XC::ElasticMembranePlateSection(tag_mat);
    else if(cmd == "elastic_plate_section")
      retval= new XC::ElasticPlateSection(tag_mat);
    else if(cmd == "membrane_plate_fiber_section")
      retval= new XC::MembranePlateFiberSection(tag_mat);
    return retval;
  }

//! @brief Procesa los comandos que se emplean para definir
//! materiales de tipo sección para su empleo en el modelo de
//! elementos finitos. Interpreta los siguientes comandos:
//!
//! - elastic_section_2d: Define una sección de material elástico para problemas planos.
//! - elastic_shear_section_2d: Define una sección de material elástico
//!   con rigidez a cortante para problemas planos.
//! - elastic_section_3d: Define una sección de material elástico para problemas tridimensionales.
//! - elastic_shear_section_3d: Define una sección de material elástico
//!   con rigidez a cortante para problemas tridimensionales.
//! - fiber_section_2d: Define un modelo de fibras para una sección (problemas planos).
//! - fiber_section_3d: Define un modelo de fibras para una sección (problemas tridimensionales).
//! - fiber_section_GJ: Define un modelo de fibras para una sección con rigidez
//!    a torsión (problemas tridimensionales).
//! - section_aggregator: Agrega propiedades mecánicas a una sección.
XC::Material *load_section_material(int tag_mat,const std::string &cmd,XC::MaterialLoader *mloader)
  {
    XC::Material *retval= nullptr;
    if(cmd == "bidirectional_section")
      retval= new XC::Bidirectional(tag_mat);
    else if(cmd == "elastic_section_2d")
      retval= new XC::ElasticSection2d(tag_mat,mloader);
    else if(cmd == "elasticShearSection2d")
      retval= new XC::ElasticShearSection2d(tag_mat,mloader);
    else if(cmd == "elastic_section_3d")
      retval= new XC::ElasticSection3d(tag_mat,mloader);
    else if(cmd == "elasticShearSection3d")
      retval= new XC::ElasticShearSection3d(tag_mat,mloader);
    else if(cmd == "fiber_section_2d")
      retval= new XC::FiberSection2d(tag_mat,mloader);
    else if(cmd == "fiber_section_3d")
      retval= new XC::FiberSection3d(tag_mat,mloader);
    else if(cmd == "fiber_section_GJ")
      retval= new XC::FiberSectionGJ(tag_mat,mloader);
    else if(cmd == "fiberSectionShear3d")
      retval= new XC::FiberSectionShear3d(tag_mat,mloader);
    else if(cmd == "generic_section_1d")
      retval= new XC::GenericSection1d(tag_mat);
    else if(cmd == "isolator2spring")
      retval= new XC::Isolator2spring(tag_mat);
    else if(cmd == "section_aggregator")
      retval= new XC::SectionAggregator(tag_mat,mloader);
    else if(cmd == "ys_section_2d_02")
      retval= new XC::YS_Section2D02(tag_mat);
    else if(cmd == "ys_section_2d_01")
      retval= new XC::YS_Section2D01(tag_mat);
    return retval;
  }

XC::Material *load_yield_surf_plastic_hardening(int tag_mat,const std::string &cmd)
  {
    XC::Material *retval= nullptr;
    if(cmd == "expon_reducing")
      retval= new XC::ExponReducing(tag_mat);
    else if(cmd == "multi_linear_kp")
      retval= new XC::MultiLinearKp(tag_mat);
    else if(cmd == "null_plastic_material")
      retval= new XC::NullPlasticMaterial(tag_mat);
    return retval;
  }
XC::Material *load_material(int tag_mat,const std::string &cmd,XC::MaterialLoader *mloader)
  {
    XC::Material *retval= load_uniaxial_material(tag_mat,cmd);
    if(retval)
      return retval;
    retval= load_uniaxial_fedeas_material(tag_mat,cmd);
    if(retval)
      return retval;
    retval= load_uniaxial_py_material(tag_mat,cmd);
    if(retval)
      return retval;
    retval= load_uniaxial_snap_material(tag_mat,cmd);
    if(retval)
      return retval;
    retval= load_nD_material(tag_mat,cmd);
    if(retval)
      return retval;
    retval= load_nD_finite_deformation_material(tag_mat,cmd);
    if(retval)
      return retval;
    retval= load_nD_cyclic_soil_material(tag_mat,cmd);
    if(retval)
      return retval;
    retval= load_nD_soil_material(tag_mat,cmd);
    if(retval)
      return retval;
    retval= load_section_material(tag_mat,cmd,mloader);
    if(retval)
      return retval;
    retval= load_plate_section_material(tag_mat,cmd,mloader);
    if(retval)
      return retval;
    retval= load_yield_surf_plastic_hardening(tag_mat,cmd);
    if(retval)
      return retval;
    return retval;
  }

//! @brief Define un nuevo material.
XC::Material *XC::MaterialLoader::nuevoMaterial(const std::string &cmd,const std::string &cod_mat)
  {
    Material *retval= load_material(tag_mat,cmd,this);
    if(retval)
      {
        retval->set_owner(this);
        if(materiales.find(cod_mat)!=materiales.end()) //El material existe.
          {
	    std::clog << "MaterialLoader::nuevo_material; ¡ojo! se redefine el material: '"
                      << cod_mat << "'." << std::endl;
            delete materiales[cod_mat];
          }
        materiales[cod_mat]= retval;
        tag_mat++;
      }
    return retval;
  }

//! @brief Define un nuevo material.
XC::GeomSection *XC::MaterialLoader::newSectionGeometry(const std::string &cod)
  {
    XC::GeomSection *retval= nullptr;
    if(geom_secciones.find(cod)!=geom_secciones.end()) //La geometria de la sección existe.
      {
	std::cerr << "MaterialLoader::procesa_comando; warning! section: '"
                      << cod << "' already exists. "<< '.' << std::endl;
        retval= geom_secciones[cod];
       }
    else
      {
        retval= new GeomSection(this);
        retval->set_owner(this);
        geom_secciones[cod]= retval;
      }
    return retval;
  }

//! @brief New interaction diagram
XC::DiagInteraccion *XC::MaterialLoader::newInteractionDiagram(const std::string &cod_diag)
  {
    DiagInteraccion *retval= nullptr;
    if(diagramas_interaccion.find(cod_diag)!=diagramas_interaccion.end()) //El diagrama existe.
      {
         std::clog << "MaterialLoader::procesa_comando; ¡ojo! el diagrama de interacción de nombre: '"
                   << cod_diag << "' ya existe. " << std::endl;
         retval= diagramas_interaccion[cod_diag];
      }
    else
      {
        retval= new DiagInteraccion();
        diagramas_interaccion[cod_diag]= retval;
      }
    return retval;
  }

//! @brief New interaction diagram
XC::DiagInteraccion *XC::MaterialLoader::calcInteractionDiagram(const std::string &cod_scc,const DatosDiagInteraccion &datos_diag)
  {
    iterator mat= materiales.find(cod_scc);
    DiagInteraccion *diagI= nullptr;
    if(mat!=materiales.end())
      {
        const FiberSectionBase *tmp= dynamic_cast<const FiberSectionBase *>(mat->second);
        if(tmp)
          {
            const std::string cod_diag= "diagInt"+cod_scc;
            if(diagramas_interaccion.find(cod_diag)!=diagramas_interaccion.end()) //Diagram exists.
              {
	        std::clog << "MaterialLoader::procesa_comando; ¡ojo! se redefine el diagrama de interacción de nombre: '"
                          << cod_diag << "'." << std::endl;
                delete diagramas_interaccion[cod_diag];
              }
            else
              {
                diagI= new DiagInteraccion(calc_diag_interaccion(*tmp,datos_diag));
                diagramas_interaccion[cod_diag]= diagI;
              }
          }
        else
          std::cerr << "El material: '" << cod_scc
                    << "' no corresponde a una sección de fibras." << std::endl;
      }
    else
      std::cerr << "No se encontró el material : '"
                      << cod_scc << "' se ignora la entrada.\n";
    return diagI;     
  }

//! @brief Procesa los comandos que se emplean para definir
//! los materiales del modelo de elementos finitos. Interpreta
//! los siguientes comandos:
//!
//! - for_each: Solicita que cada uno de los materiales del modelo
//!   ejecute el bloque de código que se pasa como argumento.
//! - geom_secc: Define la geometría de una sección transversal.
//! - define_diagrama_interaccion: Define el diagrama de interacción de una sección transversal.
bool XC::MaterialLoader::procesa_comando(CmdStatus &status)
  {
    const std::string cmd= deref_cmd(status.Cmd());
    const std::string str_log= "(MaterialLoader) Procesando comando: '" + cmd + "'";
    if(verborrea>2)
      std::clog << str_log << std::endl;
    if(cmd == "for_each")
      { 
        const std::string bloque= status.GetBloque();
        ejecuta_bloque_for_each(status,bloque);
        return true;
      }
    const CmdParser &parser= status.Parser();
    std::deque<boost::any> fnc_indices;
    std::string cod_mat= "nil";
    size_t nind= 0; // Número de índices leídos.
    if(parser.TieneIndices())
      {
	fnc_indices= status.Parser().SeparaIndices(this);
        nind= fnc_indices.size();
        if(nind>0)
          cod_mat= convert_to_string(fnc_indices[0]); //Código del material.
        if(nind>1)
          tag_mat= convert_to_int(fnc_indices[1]); //Tag del material.
        else
          tag_mat++;
      }
    if(materiales.find(cmd)!=materiales.end())
      {
        materiales[cmd]->LeeCmd(status);
        return true;
      }
    if(geom_secciones.find(cmd)!=geom_secciones.end())
      {
        geom_secciones[cmd]->LeeCmd(status);
        return true;
      }
    if(diagramas_interaccion.find(cmd)!=diagramas_interaccion.end())
      {
        diagramas_interaccion[cmd]->LeeCmd(status);
        return true;
      }
    Material *material= load_material(tag_mat,cmd,this);
    if(material)
      {
        if(nind<1)
          std::cerr << str_log << " - uso: " << cmd << "[code,tag] " << std::endl;
        material->set_owner(this);
        material->LeeCmd(status);
        if(materiales.find(cod_mat)!=materiales.end()) //El material existe.
          {
            const std::string posLectura= status.GetEntradaComandos().getPosicionLecturaActual();
	    std::clog << "MaterialLoader::procesa_comando; ¡ojo! se redefine el material: '"
                      << cod_mat << "'. "<< posLectura << '.' << std::endl;
            delete materiales[cod_mat];
          }
        materiales[cod_mat]= material;
        return true;
      }
    else if(cmd == "mat_with_tag")
      {
        int tag= -1;
        int nargs= 0;
        if(parser.TieneIndices())
          std::cerr << str_log << " - uso: " << cmd << "(tag)." << std::endl;
        if(parser.TieneArgs())
          {
	    std::deque<boost::any> fnc_args= status.Parser().SeparaArgs(this);
            nargs= fnc_args.size();
            if(nargs>0)
              tag= convert_to_int(fnc_args[0]); //Tag del material.
          }
        if(nargs<1)
          std::cerr << str_log << " - uso: " << cmd << "(tag)." << std::endl;
        Material *mat= find_ptr(tag);
        if(mat)
          mat->LeeCmd(status);
        else
          {
            status.GetBloque(); //Ignoramos entrada.
	    std::cerr << str_log << " no se encontró el material de tag: "
                      << tag << " se ignora la entrada.\n";
          }
        return true;
      }
    else if(cmd == "geom_secc")
      {
        if(nind<1)
          std::cerr << cmd << " - uso: " << cmd << "[code] " << std::endl;
        GeomSection *gmSecc= new GeomSection(this);
        gmSecc->set_owner(this);
        gmSecc->LeeCmd(status);
        if(geom_secciones.find(cod_mat)!=geom_secciones.end()) //La geometria de la sección existe.
          {
            const std::string posLectura= status.GetEntradaComandos().getPosicionLecturaActual();
	    std::clog << "MaterialLoader::procesa_comando; ¡ojo! se redefine la geometría de sección: '"
                      << cod_mat << "'. "<< posLectura << '.' << std::endl;
            delete geom_secciones[cod_mat];
          }
        geom_secciones[cod_mat]= gmSecc;
        return true;
      }
    else if(cmd == "define_diagrama_interaccion")
      {
        if(nind<1)
          std::cerr << cmd << " - uso: " << cmd << "[code] " << std::endl;
	std::cerr << "DEPRECATED; use Python." << std::endl;
        //DiagInteraccion *diagI= calcInteractionDiagram(cod_mat);
        return true;
      }
    else if(cmd == "crea_diagrama_interaccion")
      {
	std::cerr << "DEPRECATED; use Python" << std::endl;
	// const std::string cod_diag= interpretaString(status.GetString());
        // DiagInteraccion *diagI= newInteractionDiagram(cod_diag);
        return true;
      }
   else if(cmd == "beam_fiber_material")
      {
        if(nind<3)
          {
            std::cerr << cmd << " - uso: " << cmd 
                      << "[code,NDMaterial,tag] " << std::endl;
            return true;
          }
        const std::string cod_ndmat= convert_to_string(fnc_indices[1]); //Código del material.
        NDMaterial *ptr_ndmat= dynamic_cast<NDMaterial *>(find_ptr(cod_ndmat));
        Material *mat= nullptr;
        if(ptr_ndmat)
          mat= new BeamFiberMaterial(tag_mat,*ptr_ndmat);
        else
          {
            const std::string posLectura= status.GetEntradaComandos().getPosicionLecturaActual();
	    std::cerr << cmd 
                      << " errr; no pudo obtenerse un puntero al NDMaterial: " 
                      << cod_ndmat << "'. "<< posLectura << '.' << std::endl;
          }
        mat->set_owner(this);
        if(mat) mat->LeeCmd(status);
        materiales[cod_mat]= mat;
        return true;
      }
    return Loader::procesa_comando(status);
  }

void XC::MaterialLoader::clearAll(void)
  {
    for(iterator i= begin();i!= end();i++)
      delete (*i).second;
    materiales.erase(begin(),end());
    for(geom_secc_iterator i= geom_secciones.begin();i!= geom_secciones.end();i++)
      delete (*i).second;
    geom_secciones.erase(geom_secciones.begin(),geom_secciones.end());
    tag_mat= 0;
  }

XC::MaterialLoader::~MaterialLoader(void)
  { clearAll(); }

//! @brief Devuelve una referencia al mapa de materiales.
const XC::MaterialLoader::map_materiales &XC::MaterialLoader::Map(void) const
  { return materiales; }

//! @brief Devuelve un iterador apuntando al principio de la lista.
XC::MaterialLoader::const_iterator XC::MaterialLoader::begin(void) const
  { return materiales.begin(); }
//! @brief Devuelve un iterador apuntando después del final de la lista.
XC::MaterialLoader::const_iterator XC::MaterialLoader::end(void) const
  { return materiales.end(); }
//! @brief Devuelve un iterador apuntando al principio de la lista.
XC::MaterialLoader::iterator XC::MaterialLoader::begin(void)
  { return materiales.begin(); }
//! @brief Devuelve un iterador apuntando después del final de la lista.
XC::MaterialLoader::iterator XC::MaterialLoader::end(void)
  { return materiales.end(); }

//! @brief Si encuentra el material cuyo nombre se pasa como parámetro devuelve un iterador apuntando al mismo.
XC::MaterialLoader::const_iterator XC::MaterialLoader::find(const std::string &nmb) const
  { return materiales.find(nmb); }

//! @brief Si encuentra el material cuyo nombre se pasa como parámetro devuelve un iterador apuntando al mismo.
XC::MaterialLoader::iterator XC::MaterialLoader::find(const std::string &nmb)
  { return materiales.find(nmb); }

//! @brief Si encuentra el material cuyo nombre se pasa como parámetro devuelve un puntero al mismo,
//! en otro caso devuelve nullptr.
XC::Material *XC::MaterialLoader::find_ptr(const std::string &nmb)
  {
    XC::MaterialLoader::iterator i= find(nmb);
    if(i!= materiales.end())
      return (*i).second;
    else
      return nullptr; 
  }

//! @brief Si encuentra el material cuyo nombre se pasa como parámetro devuelve un puntero al mismo,
//! en otro caso devuelve nullptr.
const XC::Material *XC::MaterialLoader::find_ptr(const std::string &nmb) const
  {
    XC::MaterialLoader::const_iterator i= find(nmb);
    if(i!= materiales.end())
      return (*i).second;
    else
      return nullptr; 
  }

//! @brief Si encuentra el material cuyo nombre se pasa como parámetro devuelve un puntero al mismo,
//! en otro caso devuelve nullptr.
XC::Material *XC::MaterialLoader::find_ptr(const int &tag)
  {
    Material *retval= nullptr;
    iterator i= begin();
    for(;i!= end();i++)
      if((*i).second->getTag() == tag)
        {
          retval= (*i).second;
          break;
        }
    return retval;
  }

//! @brief Si encuentra el material cuyo nombre se pasa como parámetro devuelve un puntero al mismo,
//! en otro caso devuelve nullptr.
const XC::Material *XC::MaterialLoader::find_ptr(const int &tag) const
  {
    const Material *retval= nullptr;
    const_iterator i= begin();
    for(;i!= end();i++)
      if((*i).second->getTag() == tag)
        {
          retval= (*i).second;
          break;
        }
    return retval;
  }

//! @brief Si encuentra el material cuyo nombre se pasa como parámetro devuelve un puntero al mismo,
//! en otro caso devuelve nullptr.
XC::GeomSection *XC::MaterialLoader::find_ptr_geom_section(const std::string &nmb)
  {
    geom_secc_iterator i= geom_secciones.find(nmb);
    if(i!= geom_secciones.end())
      return (*i).second;
    else
      return nullptr; 
  }

//! @brief Si encuentra el material cuyo nombre se pasa como parámetro devuelve un puntero al mismo,
//! en otro caso devuelve nullptr.
const XC::GeomSection *XC::MaterialLoader::find_ptr_geom_section(const std::string &nmb) const
  {
    const_geom_secc_iterator i= geom_secciones.find(nmb);
    if(i!= geom_secciones.end())
      return (*i).second;
    else
      return nullptr; 
  }

//! @brief Si encuentra el material cuyo nombre se pasa como parámetro devuelve un puntero al mismo,
//! en otro caso devuelve nullptr.
XC::DiagInteraccion *XC::MaterialLoader::find_ptr_diag_interaccion(const std::string &nmb)
  {
    diag_interacc_iterator i= diagramas_interaccion.find(nmb);
    if(i!= diagramas_interaccion.end())
      return (*i).second;
    else
      return nullptr; 
  }

//! @brief Si encuentra el material cuyo nombre se pasa como parámetro devuelve un puntero al mismo,
//! en otro caso devuelve nullptr.
const XC::DiagInteraccion *XC::MaterialLoader::find_ptr_diag_interaccion(const std::string &nmb) const
  {
    const_diag_interacc_iterator i= diagramas_interaccion.find(nmb);
    if(i!= diagramas_interaccion.end())
      return (*i).second;
    else
      return nullptr; 
  }

//! @brief Devuelve una referencia al material cuyo nombre se pasa como parámetro.
XC::Material &XC::MaterialLoader::getMaterial(const std::string &nmb)
  {
    Material *retval= find_ptr(nmb);
    assert(retval);
    return *retval;
  }

//! @brief Devuelve una referencia a la geometría de sección cuyo nombre se pasa como parámetro.
XC::GeomSection &XC::MaterialLoader::getGeomSection(const std::string &nmb)
  {
    GeomSection *retval= find_ptr_geom_section(nmb);
    assert(retval);
    return *retval;
  }

//! @brief Devuelve una referencia al diagrama de interacción cuyo nombre se pasa como parámetro.
XC::DiagInteraccion &XC::MaterialLoader::getDiagInteraccion(const std::string &nmb)
  {
    DiagInteraccion *retval= find_ptr_diag_interaccion(nmb);
    assert(retval);
    return *retval;
  }

//! @brief Ejecuta el bloque que se pasa como parámetro.
void XC::MaterialLoader::ejecuta_bloque_for_each(CmdStatus &status,const std::string &bloque)
  {
    const std::string &nmbBlq= nombre_clase()+":for_each";
    iterator i= begin();
    for(;i!= end();i++)
      (*i).second->EjecutaBloque(status,bloque,nmbBlq);
  }

//! @brief Devuelve verdadero si existe el material cuyo código se pasa como parámetro.
bool XC::MaterialLoader::existeMaterial(const std::string &nmb) const
  { return (materiales.find(nmb)!=materiales.end()); }

//! @brief Devuelve verdadero si existe la geometría de la sección cuyo código se pasa como parámetro.
bool XC::MaterialLoader::existeGeomSection(const std::string &nmb) const
  { return (geom_secciones.find(nmb)!=geom_secciones.end()); }

//! @brief Devuelve verdadero si existe el diagrama de interacción cuyo código se pasa como parámetro.
bool XC::MaterialLoader::existeDiagInteraccion(const std::string &nmb) const
  { return (diagramas_interaccion.find(nmb)!=diagramas_interaccion.end()); }

//! \brief Devuelve la propiedad del objeto cuyo código (de la propiedad) se pasa
//! como parámetro.
//!
//! Soporta los códigos:
//!
//! - numMateriales: Devuelve el número de materiales definidos en el modelo.
//! - numGeomSecc: Devuelve el número de definiciones geométricas de sección definidas en el modelo.
//! - numDiagsInteraccion: Devuelve el número de diagramas de interacción definidos en el modelo.
//! - getFactorCapacidad: Devuelve el factor de capacidad que corresponde a la terna de
//!   esfuerzos que se pasan como parámetro.
//! - getTag: Devuelve el tag (identificador numérico) de un material a partir de su nombre.
any_const_ptr XC::MaterialLoader::GetProp(const std::string &cod) const
  {
    if(verborrea>4)
      std::clog << "MaterialLoader::GetProp (" << nombre_clase() 
                << "::GetProp) Buscando propiedad: " << cod << std::endl;
    if(cod=="numMateriales")
      {
        tmp_gp_szt= materiales.size();
        return any_const_ptr(tmp_gp_szt);
      }
    else if(cod=="getListaNombresMateriales")
      {
        tmp_gp_lista.clear();
        for(const_iterator i= begin();i!= end();i++)
          tmp_gp_lista.Inserta((*i).first);
        return any_const_ptr(tmp_gp_lista);
      }
    else if(cod=="getListaTagsMateriales")
      {
        tmp_gp_lista.clear();
        for(const_iterator i= begin();i!= end();i++)
          tmp_gp_lista.Inserta((*i).second->getTag());
        return any_const_ptr(tmp_gp_lista);
      }
    else if(cod=="numGeomSecc")
      {
        tmp_gp_szt= geom_secciones.size();
        return any_const_ptr(tmp_gp_szt);
      }
    else if(cod=="numDiagsInteraccion")
      {
        tmp_gp_szt= diagramas_interaccion.size();
        return any_const_ptr(tmp_gp_szt);
      }
    else if(cod=="getFactorCapacidad") //Devuelve factor de capacidad
      {                                //que corresponde a una terna de esfuerzos.
        tmp_gp_dbl= -1;
	std::string nmb_diag= "";
        double n= 0.0;
        double my= 0.0;
        double mz= 0.0;
        if(InterpreteRPN::Pila().size()>3)
          {
            mz= convert_to_double(InterpreteRPN::Pila().Pop());
            my= convert_to_double(InterpreteRPN::Pila().Pop());
            n= convert_to_double(InterpreteRPN::Pila().Pop());
            nmb_diag= convert_to_string(InterpreteRPN::Pila().Pop());
          }
        else
          err_num_argumentos(std::cerr,4,"GetProp",cod);
        const DiagInteraccion *ptr_diag= find_ptr_diag_interaccion(nmb_diag);
        if(ptr_diag)
          tmp_gp_dbl= ptr_diag->FactorCapacidad(Pos3d(n,my,mz));
        else
          std::cerr << "MaterialLoader::GetProp; getTag no se encontró el diagrama de interacción: '"
                    << nmb_diag << "'\n";
        return any_const_ptr(tmp_gp_dbl);
        
      }
    else if(cod=="getTag") //Devuelve el tag de un material a partir de su nombre.
      {
        tmp_gp_int= -1;
	const std::string nmb_material= popString(cod);
        const Material *ptr_mat= find_ptr(nmb_material);
        if(ptr_mat)
          tmp_gp_int= ptr_mat->getTag();
        else
          std::cerr << "MaterialLoader::GetProp; getTag no se encontró el material: '"
                    << nmb_material << "'\n";
        return any_const_ptr(tmp_gp_int);
      }
    else if(cod=="existeMaterial")
      {
        tmp_gp_bool= false;
	const std::string nmb= popString(cod);
        tmp_gp_bool= existeMaterial(nmb);
        return any_const_ptr(tmp_gp_bool);
      }
    else if(cod=="existeGeomSection")
      {
        tmp_gp_bool= false;
	const std::string nmb= popString(cod);
        tmp_gp_bool= existeGeomSection(nmb);
        return any_const_ptr(tmp_gp_bool);
      }
    else if(cod=="existeDiagInteraccion")
      {
        tmp_gp_bool= false;
	const std::string nmb= popString(cod);
        tmp_gp_bool= existeDiagInteraccion(nmb);
        return any_const_ptr(tmp_gp_bool);
      }
    else
      return Loader::GetProp(cod);
  }
