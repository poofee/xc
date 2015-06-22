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
//DqFibras.h

#ifndef DqFibras_h
#define DqFibras_h

#include "xc_utils/src/nucleo/EntCmd.h"
#include "xc_utils/src/geom/GeomObj.h"
#include <deque>

class Ref3d3d;
class Pos2d;
class Recta2d;
class Segmento2d;
class Poligono2d;
class BND2d;
class Semiplano2d;

namespace XC {
class Fiber;
class Vector;
class FiberSection2d;
class FiberSectionBase;
class FiberSection3dBase;
class FiberSection3d;
class FiberSectionGJ;
class Matrix;
class Response;
class Parameter;
class Information;
class KRSeccion;
class GeomSection;
class PlanoDeformacion;

enum ClaseEsfuerzo {TRACCION_SIMPLE,TRACCION_COMPUESTA,FLEXION_SIMPLE,FLEXION_COMPUESTA,COMPRESION_SIMPLE,COMPRESION_COMPUESTA,ERROR};


//! \ingroup MATSCCFibras
//
//! @brief Contenedor de fibras.
class DqFibras : public EntCmd, public std::deque<Fiber *>
  {
  public:
    typedef std::deque<Fiber *> dq_ptr_fibras;
  protected:

    double yCDG; //!< Coordenada y del centro de gravedad.
    double zCDG; //!< Coordenada z del centro de gravedad.

    mutable std::deque<std::list<Poligono2d> > dq_ac_eficaz; //!< (En su caso) áreas eficaces para cada fibra.
    mutable std::deque<double> recubs; //! Recubrimiento de cada fibra.
    mutable std::deque<double> seps; //! Separación de cada fibra.

    Fiber *inserta(const Fiber &f);
    inline void resize(const size_t &nf)
      { dq_ptr_fibras::resize(nf,nullptr); }


    bool procesa_comando(CmdStatus &status);
  private:
    friend class FiberSectionBase;
    
  public:
    DqFibras(const size_t &num= 0);
    DqFibras(const DqFibras &otro);
    DqFibras &operator=(const DqFibras &otro);

    void push_back(Fiber *f);
     inline size_t getNumFibers(void) const
      { return size(); }

    const Fiber *buscaFibra(const int &tag) const;
    Fiber *buscaFibra(const int &tag);

    inline const double &getYCdg(void) const
      { return yCDG; }
    inline const double &getZCdg(void) const
      { return zCDG; }
    Pos2d getCdg(void) const;
    inline double getYRelativa(const double &y) const
      { return y-yCDG; }
    inline double getZRelativa(const double &z) const
      { return z-zCDG; }

    GeomObj::list_Pos2d getPosiciones(void) const;

    bool hayMomento(const double &tol=1e-4) const;
    double Resultante(void) const;
    double getMz(const double &y0= 0.0) const;
    double getMy(const double &z0= 0.0) const;
    double getExcentricidadMz(const double &y0= 0.0) const;
    double getExcentricidadMy(const double &z0= 0.0) const;
    Vector2d getVectorMomento(const double &y0=0.0,const double &z0=0.0) const;
    Pos2d getPosResultante(const double &y0=0.0,const double &z0=0.0) const;
    Recta2d getFibraNeutra(void) const;

    double ResultanteComp(void) const;
    double getMzComp(const double &y0= 0.0) const;
    double getMyComp(const double &z0= 0.0) const;
    const Vector &baricentroCompresiones(void) const;
    const Vector &baricentroDefMenores(const double &epsRef) const;

    double ResultanteTracc(void) const;
    double getMzTracc(const double &y0= 0.0) const;
    double getMyTracc(const double &z0= 0.0) const;
    const Vector &baricentroTracciones(void) const;
    const Vector &baricentroDefMayores(const double &epsRef) const;

    int commitState(void);

    double getStrainMin(void) const;
    double getStrainMax(void) const;
    double getStrainMed(void) const;
    double getStressMin(void) const;
    double getStressMax(void) const;
    double getStressMed(void) const;
    PlanoDeformacion getPlanoDeformacion(void) const;
    const Vector &getDeformation(void) const;
    ClaseEsfuerzo getClaseEsfuerzo(const double &tol= 1e-4) const;
    bool enTraccion(void) const;
    bool enFlexion(void) const;
    bool enCompresion(void) const;
    std::string getStrClaseEsfuerzo(const double &tol= 1e-4) const;
    double getNeutralAxisDepth(const FiberSectionBase &) const;
    Vector getVectorBrazoMecanico(void) const;
    Segmento2d getSegmentoBrazoMecanico(void) const;
    double getBrazoMecanico(void) const;
    Recta2d getTrazaPlanoFlexion(void) const;
    Recta2d getTrazaPlanoTraccion(void) const;
    Recta2d getTrazaPlanoCompresion(void) const;
    double calcAcEficazFibras(const std::list<Poligono2d> &,const double &factor= 15) const;
    const std::list<Poligono2d> &getContornoAcEficazFibra(const size_t &i) const;
    double getAcEficazFibra(const size_t &i) const;
    double getAcEficazFibras(void) const;
    void calcRecubrimientos(const GeomSection &) const;
    void calcSeparaciones(void) const;
    const double &getRecubrimientoFibra(const size_t &i) const;
    const double &getSeparacionFibra(const size_t &i) const;
    double getDiamEqFibra(const size_t &i) const;
    double getSigmaSRFibra(const size_t &,const double &,const double &,const double &) const;
    double getDistMediaFibras(void) const;
    int updateCDG(void);
    
    int updateKRCDG(FiberSection2d &,KRSeccion &);
    Fiber *addFiber(FiberSection2d &,Fiber &,KRSeccion &);
    int setInitialSectionDeformation(const FiberSection2d &);
    int setTrialSectionDeformation(const FiberSection2d &,KRSeccion &);
    int revertToLastCommit(FiberSection2d &,KRSeccion &);
    int revertToStart(FiberSection2d &,KRSeccion &);
    const Matrix &getInitialTangent(const FiberSection2d &) const;
    const Vector &getStressResultantSensitivity(int gradNumber, bool conditional);
    int commitSensitivity(const XC::Vector& defSens, int gradNumber, int numGrads);

    int updateKRCDG(FiberSection3d &,KRSeccion &);
    Fiber *addFiber(FiberSection3d &,Fiber &,KRSeccion &);
    int setInitialSectionDeformation(const FiberSection3d &);
    int setTrialSectionDeformation(FiberSection3d &,KRSeccion &);
    int revertToLastCommit(FiberSection3d &,KRSeccion &);
    int revertToStart(FiberSection3d &,KRSeccion &);
    const Matrix &getInitialTangent(const FiberSection3d &) const;

    int updateKRCDG(FiberSectionGJ &,KRSeccion &);
    Fiber *addFiber(FiberSectionGJ &,Fiber &,KRSeccion &);
    int setInitialSectionDeformation(const FiberSectionGJ &);
    int setTrialSectionDeformation(FiberSectionGJ &,KRSeccion &);
    int revertToLastCommit(FiberSectionGJ &,KRSeccion &);
    int revertToStart(FiberSectionGJ &,KRSeccion &);
    const Matrix &getInitialTangent(const FiberSectionGJ &) const;

    Response *setResponse(const std::vector<std::string> &argv, Information &sectInfo);

    void Cumplen(const std::string &cond,DqFibras &,bool clear= true);
    void SelMatTag(const int &matTag,DqFibras &,bool clear= true);
    void ejecuta_bloque_for_each(CmdStatus &,const std::string &);
    virtual any_const_ptr GetProp(const std::string &cod) const;
    size_t IMaxProp(const std::string &nmb_prop) const;
    size_t IMinProp(const std::string &nmb_prop) const;
    size_t getFibraCooMax(const Ref3d3d &r,const size_t &iCoo) const;
    size_t getFibraCooMin(const Ref3d3d &r,const size_t &iCoo) const;

    int setParameter(const int &,const std::vector<std::string> &, Parameter &);
    int updateParameter(const int &,int parameterID, Information &info);
    int activateParameter(int passedParameterID);

    double GetYMin(void) const;
    double GetZMin(void) const;
    double GetYMax(void) const;
    double GetZMax(void) const;
    Pos2d GetPMax(void) const;
    Pos2d GetPMin(void) const;
    BND2d Bnd(void) const;
    double getSumaAreas(const double &factor= 1.0) const;
    double getAreaSeccHomogeneizada(const double &E0) const;
    const Vector &getCdgSeccHomogeneizada(const double &E0) const;
    //Momento de inercia.
    double getIz(const double &factor= 1.0,const double &y0= 0.0) const;
    double getIy(const double &factor= 1.0,const double &z0= 0.0) const;
    double getPyz(const double &factor= 1.0,const double &y0= 0.0,const double &z0= 0.0) const;
    double getI1(const double &factor= 1.0,const double &y0= 0.0,const double &z0= 0.0) const;
    double getI2(const double &factor= 1.0,const double &y0= 0.0,const double &z0= 0.0) const;
    double getTh1(const double &y0= 0.0,const double &z0= 0.0) const;
    Vector getEje1(const double &y0= 0.0,const double &z0= 0.0) const;
    Vector getEje2(const double &y0= 0.0,const double &z0= 0.0) const;
    double getIySeccHomogeneizada(const double &) const;
    double getIzSeccHomogeneizada(const double &) const;
    double getPyzSeccHomogeneizada(const double &) const;
    double getISeccHomogeneizada(const double &,const unsigned short int &,const unsigned short int &) const;
    Matrix &getISeccHomogeneizada(const double &E0) const;
    Matrix &getISeccHomogeneizada(const double &E0,const Pos2d &o) const;
    double getISeccHomogeneizada(const double &E0,const Pos2d &O,const Vector &e) const;
    double getISeccHomogeneizada(const double &E0,const Recta2d &r) const;
    //Momento estático.
    double getSzPos(const double &yf,const double &y0,const double &factor= 1.0) const;
    double getSzNeg(const double &yf,const double &y0,const double &factor= 1.0) const;
    double getSyPos(const double &zf,const double &z0,const double &factor= 1.0) const;
    double getSyNeg(const double &zf,const double &z0,const double &factor= 1.0) const;
    double getSPosSeccHomogeneizada(const double &E0,const Semiplano2d &sp) const;
    double getSNegSeccHomogeneizada(const double &E0,const Semiplano2d &sp) const;
    
    //! @brief Devuelve el momento de inercia polar respecto a G.
    inline double getIpolar(const double &factor= 1.0) const
      { return getIz(factor,yCDG)+getIy(factor,zCDG); }
    //! @brief Devuelve el radio de giro respecto al eje paralelo al z que pasa por el CDG.
    inline double getiz(const double factor= 1.0) const
      { return getIz(factor,yCDG)/getSumaAreas(factor); }
    //! @brief Devuelve el radio de giro respecto al eje paralelo al y que pasa por el CDG.
    inline double getiy(const double factor= 1.0) const
      { return getIy(factor,zCDG)/getSumaAreas(factor); }
    size_t nearest_fiber(const double &y,const double &z) const;

    void Print(std::ostream &s,const int &flag);
  };
} // fin namespace XC

#endif
