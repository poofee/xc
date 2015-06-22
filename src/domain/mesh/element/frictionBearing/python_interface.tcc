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
//python_interface.tcc

#include "frictionModel/python_interface.tcc"

class_<XC::ElemFriccionBase, bases<XC::Element0D>, boost::noncopyable >("ElemFriccionBase", no_init);

class_<XC::FlatSliderSimple2d, bases<XC::ElemFriccionBase>, boost::noncopyable >("FlatSliderSimple2d", no_init);

class_<XC::FlatSliderSimple3d, bases<XC::ElemFriccionBase>, boost::noncopyable >("FlatSliderSimple3d", no_init);

class_<XC::SingleFPSimple2d , bases<XC::ElemFriccionBase>, boost::noncopyable >("SingleFPSimple2d", no_init);

class_<XC::SingleFPSimple3d , bases<XC::ElemFriccionBase>, boost::noncopyable >("SingleFPSimple3d", no_init);
