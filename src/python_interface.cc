//----------------------------------------------------------------------------
//  programa XC; cálculo mediante el método de los elementos finitos orientado
//  a la solución de problemas estructurales.
//
//  Copyright (C)  Luis Claudio Pérez Tato
//
//  Este software es libre: usted puede redistribuirlo y/o modificarlo 
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
//python_interface.cc


#include "ProblemaEF.h"
#include "python_interface.h"

void export_utility(void);
void export_material_base(void);
void export_material_uniaxial(void);
void export_material_nD(void);
void export_material_section(void);
void export_material_fiber_section(void);
void export_domain(void);
void export_domain_mesh(void);
void export_modelador_loaders(void);
void export_modelador_build_model(void);
void export_modelador_sets(void);
void export_modelador_main(void);
void export_solution(void);

BOOST_PYTHON_MODULE(xc)
  {
    using namespace boost::python;
    docstring_options doc_options;

    export_utility();
//Expose domain components.
#include "domain/component/python_interface.tcc"



    class_<XC::MeshComponent, bases<XC::ContinuaReprComponent>, boost::noncopyable >("MeshComponent", no_init)
       ;

    export_material_base(); //Exposición de los materiales.
    export_material_uniaxial(); //Exposición de los materiales.
    export_material_nD(); //Exposición de los materiales.
    export_material_section(); //Exposición de los materiales.
    export_material_fiber_section(); //Exposición de los materiales.
    export_domain_mesh(); //Exposición de la malla.
    export_domain(); //Exposición del dominio.
    export_modelador_loaders();//Exposicion del modelador.
    export_modelador_build_model();
    export_modelador_sets();
    export_modelador_main();
    export_solution(); //Solution routines exposition.

#include "post_process/python_interface.tcc"

    XC::Domain *(XC::ProblemaEF::*getDomainRef)(void)= &XC::ProblemaEF::getDomain;
    XC::Modelador &(XC::ProblemaEF::*getModeladorRef)(void)= &XC::ProblemaEF::getModelador;
    XC::ProcSolu &(XC::ProblemaEF::*getSoluProcRef)(void)= &XC::ProblemaEF::getSoluProc;
    class_<XC::ProblemaEF, bases<EntCmd>, boost::noncopyable>("ProblemaEF")
      .add_property("getDomain", make_function( getDomainRef, return_internal_reference<>() ))
      .add_property("getModelador", make_function( getModeladorRef, return_internal_reference<>() ))
      .add_property("getSoluProc", make_function( getSoluProcRef, return_internal_reference<>() ))
      .add_property("getDatabase", make_function( &XC::ProblemaEF::getDataBase, return_internal_reference<>() ))
      .def("newDatabase", make_function( &XC::ProblemaEF::defineDatabase, return_internal_reference<>() ))
      .add_property("getFields", make_function( &XC::ProblemaEF::getFields, return_internal_reference<>() ),"Returns fields definition (export).")
      .def("clearAll",&XC::ProblemaEF::clearAll)
   ;
  }

