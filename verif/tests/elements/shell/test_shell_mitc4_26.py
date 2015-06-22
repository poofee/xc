# -*- coding: utf-8 -*-
# home made test

L= 2.0 # Lado del elemento.
t= 0.2 # Canto del elemento.
E= 2.1e6 # Módulo de Young del acero.
nu= 0.3 # Coeficiente de Poisson.
# Load
F= 10 # Magnitud de la carga

import xc_base
import geom
import xc
from model import fix_node_6dof
from model import predefined_spaces
from materials import typical_materials
from solution import predefined_solutions

# Problem type
prueba= xc.ProblemaEF()
mdlr= prueba.getModelador
nodos= mdlr.getNodeLoader
predefined_spaces.gdls_resist_materiales3D(nodos)
nodos.newNodeIDXYZ(1,0,0,0)
nodos.newNodeIDXYZ(2,L,0,0)
nodos.newNodeIDXYZ(3,L,L,0)
nodos.newNodeIDXYZ(4,0,L,0)


# Materials definition
nmb1= typical_materials.defElasticMembranePlateSection(mdlr,"memb1",E,nu,0.0,t)

elementos= mdlr.getElementLoader
elementos.defaultMaterial= "memb1"
elementos.defaultTag= 1
elem= elementos.newElement("shell_mitc4",xc.ID([1,2,3,4]))


# Constraints
coacciones= mdlr.getConstraintLoader

fix_node_6dof.fixNode6DOF(coacciones,1)
fix_node_6dof.fixNode6DOF(coacciones,4)

# Loads definition
cargas= mdlr.getLoadLoader

casos= cargas.getLoadPatterns

#Load modulation.
ts= casos.newTimeSeries("constant_ts","ts")
casos.currentTimeSeries= "ts"
#Load case definition
lp0= casos.newLoadPattern("default","0")
lp0.newNodalLoad(2,xc.Vector([0,0,F,0,0,0]))
lp0.newNodalLoad(3,xc.Vector([0,0,F,0,0,0]))
#We add the load case to domain.
casos.addToDomain("0")



# Solution
analisis= predefined_solutions.simple_static_linear(prueba)
result= analisis.analyze(1)


q13MedioElem= 0.0
q23MedioElem= 0.0
elem.getResistingForce()
mats= elem.getPhysicalProperties.getVectorMaterials #Materials at gauss points.
#Gauss points iterator
for m in mats:
  q13MedioElem= q13MedioElem+m.getStressResultantComponent("q13")
  q23MedioElem= q23MedioElem+m.getStressResultantComponent("q23")

q13MedioElem= q13MedioElem/4
q23MedioElem= q23MedioElem/4




q13MedioElemTeor= 2*F/L
q23MedioElemTeor= 0.0
ratio1= abs((q13MedioElem-q13MedioElemTeor)/q13MedioElemTeor)
ratio2= abs((q23MedioElem-q23MedioElemTeor))

''' 
print "q13MedioElem= ",q13MedioElem
print "q13MedioElemTeor= ",q13MedioElemTeor
print "ratio1= ",ratio1
print "q23MedioElem= ",q23MedioElem
print "q23MedioElemTeor= ",q23MedioElemTeor
print "ratio2= ",ratio2
   '''

import os
fname= os.path.basename(__file__)
if (abs(ratio1)<1e-12) & (abs(ratio2)<1e-12) :
  print "test ",fname,": ok."
else:
  print "test ",fname,": ERROR."
