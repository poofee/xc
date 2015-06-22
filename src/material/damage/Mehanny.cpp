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
/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in XC::main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */
                                                                        
// $Revision: 1.1 $
// $Date: 2004/09/01 03:54:28 $
// $Source: /usr/local/cvs/OpenSees/SRC/damage/Mehanny.cpp,v $

// Written: Arash Altoontash, Gregory Deierlein 
// Created: 10/02
// Revision: AA
//
// Description: This file contains the class implementation for XC::Mehanny 
// damage model. Mehanny damage model calculates the damage index based on
// primary half cycle and the summation of follower half cycles.
//

#include <material/damage/Mehanny.h>
#include <material/damage/DamageResponse.h>


#define DEBG   0

XC::Mehanny::Mehanny(int tag, double alpha, double beta, double gamma,
                 double ultimatePosValue , double ultimateNegValue, double abstol, double reltol, 
                 double posmodifier, double negmodifier)
  :DamageModel(tag,DMG_TAG_Mehanny),
   Alpha(alpha) , Beta(beta) , Gamma(gamma) , UltimatePosValue(ultimatePosValue) , UltimateNegValue(ultimateNegValue),
   PosModifier(posmodifier), NegModifier(negmodifier),AbsTol(abstol), RelTol(reltol)
  {
    if( UltimatePosValue<=0 || Alpha<0 || Beta<0 || Gamma<0 )
      std::cerr << "XC::CumulativePeak::CumulativePeak : Incorrect arguments for the damage model";
  
    if( UltimateNegValue == 0.0 )
      { UltimateNegValue = UltimatePosValue; }
    else
      { UltimateNegValue = fabs ( UltimateNegValue ) ; }
  
    if( AbsTol < 0.0 ) AbsTol = 1.0;
    if( RelTol < 0.0 ) RelTol = 1.0;
    if( PosModifier < 0.0 ) PosModifier = 1.0;
    if( NegModifier < 0.0 ) NegModifier = 1.0;
  
    this->revertToStart();

    /*
    if ( DEBG ==1 )
      {
        // Open an output file for debugging
        char FileName[20];                                                // debugging
        sprintf(FileName, "Mehanny%d.out", tag);
        OutputFile = fopen ( FileName , "w" );        // debugging
        fprintf( OutputFile , "\t dp\t\t pcyc\t\t pFHC\t\t pPHC\t\t ncyc\t\t nFHC\t\t nPHC\t\t dmg \n" ) ;
      }
    */
  }

XC::Mehanny::Mehanny(void)
  :DamageModel(0,DMG_TAG_Mehanny)
  {}

XC::Mehanny::~Mehanny(void)
  {
    /*
    if( DEBG == 1 )
      {
        fclose( OutputFile );                                                // debugging
      }
    */
  }


int XC::Mehanny::setTrial(const Vector &trialVector)
  {
    if(trialVector.Size() != 3)
      {
        std::cerr << "WARNING: XC::Mehanny::setTrial Wrong vector size for trial data" << std::endl;
        return -1;
      }
  
    double TrialDefo = trialVector(0);
    double TrialForce = trialVector(1);
    double TrialKU = trialVector(2);
  
    double TrialScalar = 0.0;
  
    // calculate the plastic deformation once the unloading stiffness is defined as non zero
    if(TrialKU!=0.0)
      { TrialScalar = TrialDefo - TrialForce/TrialKU; }
    else
      {
        // use the elastic deformation instead
        TrialScalar = TrialDefo;
      }
    return this->processData( TrialScalar );
  }


double XC::Mehanny::getDamage(void)
  {
  
    const double PosDamage= ( pow(TrialPosPHC,Alpha) + pow(TrialSumPosFHC,Beta) ) / ( pow(UltimatePosValue,Alpha) + pow(TrialSumPosFHC,Beta) ) ;
  
    const double NegDamage= ( pow(fabs(TrialNegPHC),Alpha) + pow(fabs(TrialSumNegFHC),Beta) ) / ( pow(fabs(UltimateNegValue),Alpha) + pow(fabs(TrialSumNegFHC),Beta) ) ;
  
    double OveralDamage= pow( ( pow(PosDamage,Gamma) + pow(NegDamage,Gamma) ) , 1/Gamma );
    if( OveralDamage < CommDamage ) OveralDamage = CommDamage;
    return OveralDamage;
  }


double XC::Mehanny::getPosDamage(void)
  {  
    const double PosDamage=( pow(TrialPosPHC,Alpha) + pow(TrialSumPosFHC,Beta) ) / ( pow(UltimatePosValue,Alpha) + pow(TrialSumPosFHC,Beta) ) ;
  
    const double NegDamage=( pow(fabs(TrialNegPHC),Alpha) + pow(fabs(TrialSumNegFHC),Beta) ) / ( pow(fabs(UltimateNegValue),Alpha) + pow(fabs(TrialSumNegFHC),Beta) ) ;;

    const double OveralDamage= pow( ( 1.0 * pow(PosDamage,Gamma) + NegModifier * pow(NegDamage,Gamma) ) , 1/Gamma );
  
    return OveralDamage;
  }


double XC::Mehanny::getNegDamage (void)
  {
    const double PosDamage= ( pow(TrialPosPHC,Alpha) + pow(TrialSumPosFHC,Beta) ) / ( pow(UltimatePosValue,Alpha) + pow(TrialSumPosFHC,Beta) ) ;
  
    const double NegDamage= ( pow(fabs(TrialNegPHC),Alpha) + pow(fabs(TrialSumNegFHC),Beta) ) / ( pow(fabs(UltimateNegValue),Alpha) + pow(fabs(TrialSumNegFHC),Beta) ) ;;
  
    const double OveralDamage= pow( ( PosModifier * pow(PosDamage,Gamma) + 1.0 * pow(NegDamage,Gamma) ) , 1/Gamma );
  
    return OveralDamage;
  }

    
int XC::Mehanny::commitState (void)
  {
    /*
    if ( DEBG ==1 )
      {                
        fprintf( OutputFile , "\t %f\t %f\t %f\t %f\t %f\t %f\t %f\t %f \n",        TrialPlasticDefo, TrialPosCycle, TrialSumPosFHC, TrialPosPHC, TrialNegCycle, TrialSumNegFHC, TrialNegPHC, this->getDamage() ) ;
      }
    */
    LCommPlasticDefo = CommPlasticDefo;
    LCommDefoIncr = CommDefoIncr;
    LCommTempPDefo = CommTempPDefo;
    LCommPosCycle = CommPosCycle;
    LCommNegCycle = CommNegCycle;        
    LCommSumPosFHC = CommSumPosFHC;
    LCommPosPHC = CommPosPHC;
    LCommSumNegFHC = CommSumNegFHC;
    LCommNegPHC = CommNegPHC;
    LCommDamage = CommDamage;
  
    CommPlasticDefo = TrialPlasticDefo;
    CommDefoIncr = TrialDefoIncr;
    CommTempPDefo = TrialTempPDefo;
    CommPosCycle = TrialPosCycle;
    CommNegCycle = TrialNegCycle;        
    CommSumPosFHC = TrialSumPosFHC;
    CommPosPHC = TrialPosPHC;
    CommSumNegFHC = TrialSumNegFHC;
    CommNegPHC = TrialNegPHC;
    CommDamage = TrialDamage;
    return 0;
  }

int XC::Mehanny::revertToLastCommit(void)
  {
    CommPlasticDefo = LCommPlasticDefo;
    CommDefoIncr = LCommDefoIncr;
    CommTempPDefo = LCommTempPDefo;
    CommPosCycle = LCommPosCycle;
    CommNegCycle = LCommNegCycle;        
    CommSumPosFHC = LCommSumPosFHC;
    CommPosPHC = LCommPosPHC;
    CommSumNegFHC = LCommSumNegFHC;
    CommNegPHC = LCommNegPHC;
    CommDamage = LCommDamage;
    return 0;
  }

int XC::Mehanny::revertToStart (void)
  {
    CommPlasticDefo = LCommPlasticDefo = 0.0;
    CommDefoIncr = LCommDefoIncr = 0.0;
    CommTempPDefo = LCommTempPDefo = 0.0;
    CommPosCycle = LCommPosCycle = 0.0;
    CommNegCycle = LCommNegCycle = 0.0;        
    CommSumPosFHC = LCommSumPosFHC = 0.0;
    CommPosPHC = LCommPosPHC = 0.0;
    CommSumNegFHC = LCommSumNegFHC = 0.0;
    CommNegPHC = LCommNegPHC = 0.0;
    CommDamage = LCommDamage = 0.0;
    return 0;
  }

XC::DamageModel *XC::Mehanny::getCopy(void) const
  {
    Mehanny *theCopy = new Mehanny(this->getTag(), Alpha , Beta , Gamma , UltimatePosValue , UltimateNegValue, AbsTol, RelTol, PosModifier, NegModifier );
  
    theCopy->TrialPlasticDefo = TrialPlasticDefo;
    theCopy->TrialDefoIncr = TrialDefoIncr;
    theCopy->TrialTempPDefo = TrialTempPDefo;
    theCopy->TrialPosCycle = TrialPosCycle;
    theCopy->TrialNegCycle = TrialNegCycle;        
    theCopy->TrialSumPosFHC = TrialSumPosFHC;
    theCopy->TrialPosPHC = TrialPosPHC;
    theCopy->TrialSumNegFHC = TrialSumNegFHC;
    theCopy->TrialNegPHC = TrialNegPHC;
    theCopy->TrialDamage = TrialDamage;
  
    // Commited state
    theCopy->CommPlasticDefo = CommPlasticDefo;
    theCopy->CommDefoIncr= CommDefoIncr;
    theCopy->CommTempPDefo = CommTempPDefo;
    theCopy->CommPosCycle = CommPosCycle;
    theCopy->CommNegCycle = CommNegCycle;        
    theCopy->CommSumPosFHC = CommSumPosFHC;
    theCopy->CommPosPHC = CommPosPHC;
    theCopy->CommSumNegFHC = CommSumNegFHC;
    theCopy->CommNegPHC = CommNegPHC;
    theCopy->CommDamage = CommDamage;
  
    // Last commit
    theCopy->LCommPlasticDefo = LCommPlasticDefo;
    theCopy->LCommDefoIncr = LCommDefoIncr;
    theCopy->LCommTempPDefo = LCommTempPDefo;
    theCopy->LCommPosCycle = LCommPosCycle;
    theCopy->LCommNegCycle = LCommNegCycle;        
    theCopy->LCommSumPosFHC = LCommSumPosFHC ;
    theCopy->LCommPosPHC = LCommPosPHC;
    theCopy->LCommSumNegFHC = LCommSumNegFHC;
    theCopy->LCommNegPHC = LCommNegPHC;
    theCopy->LCommDamage = LCommDamage;
    return theCopy;
  }

//! @brief Devuelve el identificador de la variable cuyo
//! nombre se pasa como parámetro.
int XC::Mehanny::setVariable(const std::string &argv)
  { return -1; }

//! @brief Devuelve el valor de la variable cuyo
//! identificador se pasa como parámetro.
int XC::Mehanny::getVariable(int variableID, double &info)
  { return -1; }

int XC::Mehanny::setParameter(const std::vector<std::string> &argv, Information &eleInformation)
  { return -1; }

int XC::Mehanny::updateParameter(int responseID, Information &eleInformation)
  { return -1; }

XC::Response *XC::Mehanny::setResponse(const std::vector<std::string> &argv, Information &info)
  {
    //
    // we compare argv[0] for known response types for the XC::Truss
    //
  
    if( argv[0] == "damage" || argv[0] == "damageindex" )
      return new XC::DamageResponse( this , 1 , 0.0 );
    else if (argv[0] == "Value" || argv[0] == "defo" || argv[0] == "deformation" )
      return new XC::DamageResponse( this , 2 , 0.0 );
    else if (argv[0] == "trial" || argv[0] == "trialinfo" )
      return new XC::DamageResponse( this , 3 , Vector(4) );
    else 
      return 0;
  }

int XC::Mehanny::getResponse(int responseID, Information &info)
  {
    switch (responseID)
     {
     case -1:
       return -1;
     case 1:
       return info.setDouble( this->getDamage() );
     case 2:
       return info.setDouble( TrialPlasticDefo );
     case 3:
      if(info.theVector!=0)
        {
          (*(info.theVector))(0) = TrialPosPHC;
          (*(info.theVector))(1) = TrialSumPosFHC;
          (*(info.theVector))(2) = TrialNegPHC;
          (*(info.theVector))(3) = TrialSumNegFHC;
        }
      return 0;
    default:
      return -1;
    }
  }


int XC::Mehanny::sendSelf(CommParameters &cp)
  { return 0; }


int XC::Mehanny::recvSelf(const CommParameters &cp)
  { return 0; }


void XC::Mehanny::Print(std::ostream &s, int flag )
  {
    s << "CumulativePeak tag: " << this->getTag() << std::endl;
    s << "  Alpha: " << Alpha << " Beta: " << Beta << "  Gamma: " << Gamma << std::endl;
    s << " UltimatePosValue: " << UltimatePosValue << " UltimateNegValue: " << UltimateNegValue << std::endl;
  }


// Private functions
int XC::Mehanny::processData(double PDefo)
  {
    TrialPlasticDefo = PDefo;
    TrialDefoIncr = PDefo - CommPlasticDefo;
    TrialTempPDefo = CommTempPDefo;
    TrialPosCycle = CommPosCycle;
    TrialNegCycle = CommNegCycle;
    TrialSumPosFHC = CommSumPosFHC;
    TrialPosPHC = CommPosPHC;
    TrialSumNegFHC = CommSumNegFHC;
    TrialNegPHC = CommNegPHC;
    TrialDamage = CommDamage;
    
    if(TrialDefoIncr != 0.0) 
      {
        // For a non-zero step
        if ( ( (TrialDefoIncr >= AbsTol) && (TrialDefoIncr >= RelTol*TrialPosPHC) ) ||
           ( (TrialDefoIncr+TrialTempPDefo) >= AbsTol && (TrialDefoIncr+TrialTempPDefo) >= RelTol*TrialPosPHC )  ||
           ( (TrialDefoIncr <= -AbsTol) && (TrialDefoIncr >= -RelTol*TrialPosPHC) ) ||
           ( (TrialDefoIncr+TrialTempPDefo) <= -AbsTol && (TrialDefoIncr+TrialTempPDefo) <= -RelTol*TrialPosPHC )         )
          {
            // in case the plastic deformation increament is significant enough
            // or the current increment is larger than the tolerence
          
            if( TrialPosCycle == 0.0 && TrialNegCycle == 0.0 )
              {
                // For a brand new cycle
                if( TrialDefoIncr > 0.0 )
                  { TrialPosCycle = TrialDefoIncr; } 
                else
                  { TrialNegCycle = TrialDefoIncr; }
              }
            else if( TrialPosCycle > 0.0 && TrialNegCycle == 0.0 )
              {
                // Check the status for a positive half cycle
                // Determine the status of this step, if a new cycle has started
                // or still on the last cycle
                //
                if(TrialDefoIncr + TrialTempPDefo >= 0.0 )
                  {
                    // just add the temporarily saved and recent cycle to the current positive cycle
                    TrialPosCycle = TrialPosCycle + TrialDefoIncr + TrialTempPDefo;                        
                  }
                else
                  {
                    // end this positive half cycle and initiate a new negative half cycle
                    TrialPosCycle = 0.0;
                    TrialNegCycle = TrialDefoIncr + TrialTempPDefo;        
                  }
              }
            else if( TrialPosCycle == 0.0 && TrialNegCycle < 0.0 )
              {
                // Check the status for a negative half cycle
                // Determine the status of this step, if a new cycle has started
                // or still on the last cycle
                //
              
                if( TrialDefoIncr+TrialTempPDefo <= 0.0 )
                  {
                    // just add the temporarily saved and recent cycle to the current negative cycle
                    TrialNegCycle = TrialNegCycle + TrialDefoIncr + TrialTempPDefo;                        
                  }
                else
                  {
                    // end this negative half cycle and initiate a new positive half cycle
                    TrialNegCycle = 0.0;
                    TrialPosCycle = TrialDefoIncr + TrialTempPDefo;        
                  }
              }
            else 
              {
                // This is an internal error case
                std::cerr << "XC::Mehanny::processData :Error, Can not detect a half cycle" << std::endl;
                return -1;
              }
          
            // reset the temporary plastic deformation
            TrialTempPDefo = 0.0;
          }
        else 
          {
            // for very small increments, the increment is only added to the temporary plastic deformation
            TrialTempPDefo = TrialTempPDefo + TrialDefoIncr;
            
          }
        
        // Process the current step
        // now detect the peaks
        if( TrialPosCycle > 0.0 && TrialNegCycle == 0.0 )
          {
            // deal with a positive half cycle
            if( TrialPosCycle > TrialPosPHC)
              { TrialPosPHC = TrialPosCycle; }
            else
              { TrialSumPosFHC = TrialSumPosFHC - CommPosCycle + TrialPosCycle; }
          }
        else if(TrialPosCycle == 0.0 && TrialNegCycle < 0.0)
          {
            // deal with a negative half cycle
            if( TrialNegCycle < TrialNegPHC)
              { TrialNegPHC = TrialNegCycle; }
            else 
              { TrialSumNegFHC = TrialSumNegFHC - CommNegCycle + TrialNegCycle; }
          }
      }
    return 0;
  }


int XC::Mehanny::setInputResponse ( Element *elem , const std::vector<std::string> &argv, int ndof )
  { return -1; }
