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
//MatrizPtrBase.h
//Matriz de puntos

#ifndef MATRIZPTRBASE_H
#define MATRIZPTRBASE_H

#include "xc_utils/src/nucleo/EntCmd.h"
#include "xc_basic/src/matrices/matrizT.h"
#include <vector>
#include "boost/any.hpp"
#include "boost/lexical_cast.hpp"
#include "xc_utils/src/base/utils_any.h"
#include "xc_utils/src/nucleo/InterpreteRPN.h"
#include "xc_utils/src/base/CmdStatus.h"
#include "xc_basic/src/matrices/m_int.h"

namespace XC{


//! @ingroup CadMR
//
//! @brief Base para las matrices de punteros a nodos, elementos y puntos.
template <class T>
class MatrizPtrBase: public MatrizT<T *,std::vector<T *> >, public EntCmd
  {
  public:
    typedef MatrizT<T *,std::vector<T *> > m_ptr;
    typedef typename m_ptr::iterator iterator;
    typedef typename m_ptr::const_iterator const_iterator;
    typedef typename m_ptr::value_type value_type;
    typedef typename m_ptr::reference reference;
    typedef typename m_ptr::const_reference const_reference;
  protected:
    virtual bool procesa_comando(CmdStatus &status);

    //! @brief Constructor.
    MatrizPtrBase(const size_t &f=0,const size_t &c=0)
      : m_ptr(f,c,NULL), EntCmd() {}
  public:
    bool Null(void) const;
    bool HasNull(void) const;

    void dim(const size_t &,const size_t &);

    void EjecutaBloqueForEach(CmdStatus &,const std::string &);
    std::vector<int> getTagsObjsFila(const size_t &);
    std::vector<int> getTagsObjsColumna(const size_t &);
    std::vector<int> getTagsObjsInterioresFila(const size_t &);
    std::vector<int> getTagsObjsInterioresColumna(const size_t &);
    std::vector<int> getTagsInteriorObjs(void);
    std::vector<int> getTagsObjs(void);
  };

//! @brief Matrix dimensions.
template <class T>
void XC::MatrizPtrBase<T>::dim(const size_t &nRows,const size_t &nCols)
  { this->resize(nRows,nCols,NULL); }

//! @brief Procesa comandos desde archivo
template <class T>
bool XC::MatrizPtrBase<T>::procesa_comando(CmdStatus &status)
  {
    const std::string cmd= deref_cmd(status.Cmd());
    if(verborrea>2)
      std::clog << "(MatrizPtrBase) Procesando comando: " << cmd << std::endl;

    if(cmd == "for_each")
      {
        const std::string &bloque= status.GetBloque();
        EjecutaBloqueForEach(status,bloque);
        return true;
      }
    else if(cmd == "dim")
      {
	std::vector<size_t> dim(crea_vector_size_t(status.GetString()));
        if(dim.size()<2)
          {
            const std::string posLectura= get_ptr_status()->getPosicionLecturaActual();
	    std::clog << "(MatrizPtrBase) Procesando comando: '" << cmd
                      << "'; se esperaban dos valores."
                      << posLectura << std::endl;
          }
        this->resize(dim[0],dim[1],NULL);
        return true;
      }
    else if(cmd == "obj")
      {
        const CmdParser &parser= status.Parser();
        if(parser.TieneIndices())
          {
            interpreta(parser.GetIndices());
            if(InterpreteRPN::HayArgumentos(2,cmd))
              {
                const size_t k= convert_to_size_t(InterpreteRPN::Pila().Pop()); 
                const size_t j= convert_to_size_t(InterpreteRPN::Pila().Pop()); 
                T *ptr= (*this)(j,k);
                if(ptr)
                  ptr->LeeCmd(status);
                else
	          std::clog << "(MatrizPtrBase) Procesando comando: '" << cmd
                            << "'; no se encontró el objeto de índices: (" << j << ',' << k << ").\n";
              }
          }
        else
	  std::cerr << "(MatrizPtrBase) Procesando comando: '" << cmd
                            << "'; faltan los índices del punto.\n";
        return true;
      }
    else if(cmd == "objs_fila")
      {
	std::cerr << "deprecated." << std::endl;
        /* const std::string &bloque= status.GetBloque(); */
        /* const CmdParser &parser= status.Parser(); */
        /* if(parser.TieneIndices()) */
        /*   { */
        /*     interpreta(parser.GetIndices()); */
        /*     if(InterpreteRPN::HayArgumentos(1,cmd)) */
        /*       { */
        /*         const size_t f= convert_to_size_t(InterpreteRPN::Pila().Pop()); */
        /*         this->EjecutaBloqueObjsFila(f,status,bloque); */
        /*       } */
        /*   } */
        /* else */
	/*   std::cerr << "(MatrizPtrBase) Procesando comando: '" << cmd */
        /*                     << "'; faltan los índices del objeto.\n"; */
        return true;
      }
    else if(cmd == "objs_columna")
      {
	std::cerr << "deprecated." << std::endl;
        /* const std::string &bloque= status.GetBloque(); */
        /* const CmdParser &parser= status.Parser(); */
        /* if(parser.TieneIndices()) */
        /*   { */
        /*     interpreta(parser.GetIndices()); */
        /*     if(InterpreteRPN::HayArgumentos(1,cmd)) */
        /*       { */
        /*         const size_t c= convert_to_size_t(InterpreteRPN::Pila().Pop()); */
        /*         this->EjecutaBloqueObjsColumna(c,status,bloque); */
        /*       } */
        /*   } */
        /* else */
	/*   std::cerr << "(MatrizPtrBase) Procesando comando: '" << cmd */
        /*                     << "'; faltan los índices del objeto.\n"; */
        return true;
      }
    else if(cmd == "objs_interiores")
      {
	std::cerr << "deprecated. Use Python." << std::endl;
        //this->ForEachInteriorObj(status,status.GetBloque());
        return true;
      }
    else
      return EntCmd::procesa_comando(status);
  }

//! @brief Devuelve verdadero si está vacía o si los punteros son nulos.
template <class T>
bool MatrizPtrBase<T>::Null(void) const
  {
    if(this->empty())
      return true;
    else
      return (this->operator()(1,1)==NULL);
  }

//! @brief Devuelve verdadero si está vacía o si algún puntero es nulo.
template <class T>
bool MatrizPtrBase<T>::HasNull(void) const
  {
    bool retval= false;
    if(Null())
      retval= true;
    else
      {
        const size_t nfilas= this->getNumFilas();
        const size_t ncols= this->getNumCols();
        for(size_t j= 1;j<=nfilas;j++)
          for(size_t k= 1;k<=ncols;k++)
            if(this->operator()(j,k)==NULL)
              {
                retval= true;
                break;
              }
      }
    return retval;
  }

//! @brief Solicita a cada una de las componentes que ejecute el bloque que
//! se pasa como parámetro.
template <class T>
void MatrizPtrBase<T>::EjecutaBloqueForEach(CmdStatus &status,const std::string &blq)
  {
    const std::string nmbBlq= nombre_clase()+":for_each";
    for(iterator i= this->begin();i!= this->end();i++)
      {
        T *ptr= *i;
        if(ptr)
          ptr->EjecutaBloque(status,blq,nmbBlq);
      }
  }

//! @brief Pide a cada uno de los objetos de la fila que ejecuten el código que se pasa como
//! parámetro.
template <class T>
std::vector<int> XC::MatrizPtrBase<T>::getTagsObjsFila(const size_t &f)
  {
    const std::string nmbBlq= nombre_clase()+":objetos_fila:"+boost::lexical_cast<std::string>(f);
    const size_t numCols= this->getNumCols();
    std::vector<int> retval(numCols);
    for(size_t i= 1;i<=numCols;i++)
      retval[i-1]= (*this)(f,i)->getTag();
    return retval;
  }

//! @brief Pide a cada uno de los objetos de la columna que ejecuten el código que se pasa como
//! parámetro.
template <class T>
std::vector<int>  XC::MatrizPtrBase<T>::getTagsObjsColumna(const size_t &c)
  {
    const std::string nmbBlq= nombre_clase()+":objetos_columna:"+boost::lexical_cast<std::string>(c);
    const size_t numFilas= this->getNumFilas();
    std::vector<int> retval(numFilas);
    for(size_t i= 1;i<=numFilas;i++)
      retval[i-1]= (*this)(i,c)->getTag();
    return retval;
  }

//! @brief Pide a cada uno de los objetos interiores de la fila que ejecuten el código que se pasa como
//! parámetro.
template <class T>
std::vector<int> XC::MatrizPtrBase<T>::getTagsObjsInterioresFila(const size_t &f)
  {
    const std::string nmbBlq= nombre_clase()+":objetos_interiores_fila:"+boost::lexical_cast<std::string>(f);
    const size_t numCols= this->getNumCols();
    std::vector<int> retval(numCols-2);
    for(size_t i= 2;i<numCols;i++)
      retval[i-2]= (*this)(f,i)->getTag();
    return retval;
  }

//! @brief Pide a cada uno de los objetos interiores de la columna que ejecuten el código que se pasa como
//! parámetro.
template <class T>
std::vector<int> XC::MatrizPtrBase<T>::getTagsObjsInterioresColumna(const size_t &c)
  {
    const std::string nmbBlq= nombre_clase()+":objetos_interiores_columna:"+boost::lexical_cast<std::string>(c);
    const size_t numFilas= this->getNumFilas();
    std::vector<int> retval(numFilas-2);
    for(size_t i= 2;i<numFilas;i++)
      retval[i-2]= (*this)(i,c)->getTag();
    return retval;
  }

//! @brief Pide a cada uno de los objetos interiores que ejecuten el código que se pasa como
//! parámetro.
template <class T>
std::vector<int> XC::MatrizPtrBase<T>::getTagsInteriorObjs(void)
  {
    const size_t numFilas= this->getNumFilas();
    const size_t numCols= this->getNumCols();
    std::vector<int> retval((numFilas-1)*(numCols-1));
    if(numFilas==1)
      retval= this->getTagsObjsInterioresFila(1);
    if(numCols==1)
      retval= this->getTagsObjsInterioresColumna(1);
    else
      {
        m_int tmp(numFilas-2,numCols-2);
        for(size_t i= 2;i<numFilas;i++)
          for(size_t j= 2;j<numCols;j++)
    	    tmp(i-1,j-1)= (*this)(i,j)->getTag();
        retval= tmp.getVector();
      }
    return retval;
  }

template <class T>
std::vector<int> XC::MatrizPtrBase<T>::getTagsObjs(void)
  {
    const size_t numFilas= this->getNumFilas();
    const size_t numCols= this->getNumCols();
    std::vector<int> retval(numFilas*numCols);
    m_int tmp(numFilas,numCols);
    for(size_t i= 1;i<=numFilas;i++)
      for(size_t j= 1;j<=numCols;j++)
        tmp(i,j)= (*this)(i,j)->getTag();
    retval= tmp.getVector();
    return retval;
  }

} //fin namespace XC

#endif
