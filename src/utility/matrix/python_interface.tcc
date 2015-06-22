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


int &(XC::ID::*getItemID)(const int &)= &XC::ID::operator();
class_<XC::ID, bases<EntCmd> >("ID")
  .def(init<boost::python::list>())
  .def(init<std::set<int> >())
  .def(init<std::vector<int> >())
  .def("__getitem__",getItemID, return_value_policy<return_by_value>())
  .def(self_ns::str(self_ns::self))
  .def("size",&XC::ID::Size)
  .def("__len__",&XC::ID::Size)
  // .def(self + self)
  // .def(self - self)
  // .def(self += self)
  // .def(self -= self)
  // .def(self *= int())
  ;

implicitly_convertible<XC::ID,boost::python::list>();
implicitly_convertible<boost::python::list,XC::ID>();

def("id_to_py_list",XC::xc_id_to_py_list);

class_<CmdVectorBase, bases<EntCmd>, boost::noncopyable >("CmdVectorBase", no_init);

double &(XC::Vector::*getItemVector)(const size_t &)= &XC::Vector::at;
class_<XC::Vector, bases<CmdVectorBase> >("Vector")
  .def(init<boost::python::list>())
  .def("__getitem__",getItemVector, return_value_policy<return_by_value>())
//  .def( "__getitem__", getItemVector, boost::python::arg( "index" ), boost::python::return_internal_reference<>() )
  .def(self * double())
  .def(double() * self)
  .def(self / double())
  .def(self + self)
  .def(self - self)
  .def(self += self)
  .def(self -= self)
  .def(self *= double())
  .def(self /= double())
  .def(self_ns::str(self_ns::self))
  .def("dot",&XC::Vector::dot,"Returns dot product.")
  .def("Norm",&XC::Vector::Norm,"Returns vector norm.")
  .def("Norm2",&XC::Vector::Norm2,"Returns vector norm.")
  .def("size",&XC::Vector::Size)
  .def("__len__",&XC::Vector::Size)
  .def("getComponents",&XC::Vector::getComponents,"Return the specified set of vecto's components")
  .def("putComponents",&XC::Vector::putComponents,"Assigns the specified values to the specified set of vecto's components")
  .def("addComponents",&XC::Vector::addComponents,"Sums the specified values to the specified set of vecto's components")
  ;


implicitly_convertible<XC::Vector,boost::python::list>();
implicitly_convertible<boost::python::list,XC::Vector>();

double &(XC::Matrix::*at)(int,int)= &XC::Matrix::operator();
class_<XC::Matrix, bases<EntCmd> >("Matrix")
  .def(init<boost::python::list>())
  .def("__call__",at, return_value_policy<return_by_value>())
  .def(self * double())
  .def(double() * self)
  .def(self * XC::Vector())
  .def(self + self)
  .def(self - self)
  .def(self += self)
  .def(self -= self)
  .add_property("noRows",&XC::Matrix::noRows,"Returns number of rows.")
  .add_property("noCols",&XC::Matrix::noCols,"Returns number of columns.")
  .def("at",at, return_value_policy<return_by_value>())
  .def("getRow",&XC::Matrix::getRow)
  .def("getCol",&XC::Matrix::getCol)
  .def(self_ns::str(self_ns::self))
  .def("rowSum",&XC::Matrix::rowSum)
  .def("columnSum",&XC::Matrix::columnSum)
  .def("rowNorm",&XC::Matrix::rowNorm)
  .def("columnNorm",&XC::Matrix::columnNorm)
  .def("Norm2",&XC::Matrix::Norm2)
  .def("Norm",&XC::Matrix::Norm)
   ;


#include "nDarray/python_interface.tcc"
