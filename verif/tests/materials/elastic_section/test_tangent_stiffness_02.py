# -*- coding: utf-8 -*-
''' Verifica la formación de la matriz de rigidez tangente de la sección.
   Elaboración propia. '''

import xc_base
import geom
import xc
from solution import predefined_solutions
from model import predefined_spaces
from model import fix_node_6dof
from materials import typical_materials
from materials import parametrosSeccionRectangular
from materials import defSeccAggregation


h= 0.30 # Canto de la viga.
b= 0.2 # Ancho de la viga.
E= 2e6 # Módulo elástico

prueba= xc.ProblemaEF()
mdlr= prueba.getModelador   
nodos= mdlr.getNodeLoader

seccPrueba= parametrosSeccionRectangular.RectangularSection("prueba",b,h,E,0.3)

# Definimos materiales
defSeccAggregation.defSeccAggregation2d(mdlr,seccPrueba)
matPrueba= mdlr.getMaterialLoader.getMaterial("prueba")
tang= matPrueba.getTangentStiffness()
EI= tang.at(1,1)
EA= tang.at(0,0)




EITeor= (1/12.0*b*h**3*E)
EITeor2= seccPrueba.E*seccPrueba.Iz()
ratio1= ((EI-EITeor)/EITeor)
EATeor= (b*h*E)
ratio2= ((EA-EATeor)/EATeor)

''' 
print "EI= ",EI
print "E= ", seccPrueba.E
print "I= ", seccPrueba.Iz()
print "EITeor= ",EITeor
print "EITeor2= ",EITeor2
print "ratio1= ",ratio1
print "EA= ",EA
print "EATeor= ",EATeor
print "ratio2= ",ratio2
   '''

import os
fname= os.path.basename(__file__)
if (abs(ratio1)<1e-2) & (abs(ratio2)<1e-2):
  print "test ",fname,": ok."
else:
  print "test ",fname,": ERROR."
