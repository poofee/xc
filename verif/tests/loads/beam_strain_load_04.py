# -*- coding: utf-8 -*-
# home made test
# Referencia:  Cálculo de estructuras por el método de los elementos finitos. E. Oñate, pg. 165, ejemplo 5.3

import xc_base
import geom
import xc
from solution import predefined_solutions
from model import predefined_spaces
from model import fix_node_6dof
from materials import typical_materials

L= 1.0 # Bar length (m)
E= 2.1e6*9.81/1e-4 # Módulo elástico
nu= 0.3 # Coeficiente de Poisson
G= E/(2*(1+nu)) # Módulo de elasticidad a cortante
alpha= 1.2e-5 # Coeficiente de dilatación del acero
A= 4e-4 # Área de la barra expresada en metros cuadrados
Iy= 80.1e-8 # Momento de inercia de la sección expresada en m4
Iz= 8.49e-8 # Momento de inercia de la sección expresada en m4
J= 0.721e-8 # Momento de inercia a torsión expresado en m4
AT= 10 # Incremento de temperatura expresado en grados centígrados

# Problem type
prueba= xc.ProblemaEF()
mdlr= prueba.getModelador
nodos= mdlr.getNodeLoader
predefined_spaces.gdls_resist_materiales3D(nodos)
nodos.defaultTag= 1 #First node number.
nod= nodos.newNodeXYZ(0.0,0.0,0.0)
nod= nodos.newNodeXYZ(L,0.0,0.0)


trfs= mdlr.getTransfCooLoader
lin= trfs.newLinearCrdTransf3d("lin")
lin.xzVector= xc.Vector([0,1,0])
# Materials definition
seccion= typical_materials.defElasticShearSection3d(mdlr,"seccion",A,E,G,Iz,Iy,J,1.0)

# Elements definition
elementos= mdlr.getElementLoader
elementos.defaultTransformation= "lin"
elementos.defaultMaterial= "seccion"
elementos.defaultTag= 1
beam= elementos.newElement("force_beam_column_3d",xc.ID([1,2]));
    
# Constraints
coacciones= mdlr.getConstraintLoader

fix_node_6dof.fixNode6DOF(coacciones,1)
fix_node_6dof.fixNode6DOF(coacciones,2)

# Loads definition
cargas= mdlr.getLoadLoader

casos= cargas.getLoadPatterns
ts= casos.newTimeSeries("linear_ts","ts")
casos.currentTimeSeries= "ts"
#Load case definition
lp0= casos.newLoadPattern("default","0")
#casos.currentLoadPattern= "0"
eleLoad= lp0.newElementalLoad("beam_strain_load")
eleLoad.elementTags= xc.ID([1])
defTermica= xc.PlanoDeformacion(alpha*AT)
eleLoad.planoDeformacionDorsal= defTermica
eleLoad.planoDeformacionFrontal= defTermica

#We add the load case to domain.
casos.addToDomain("0")

analisis= predefined_solutions.simple_static_linear(prueba)
result= analisis.analyze(1)


elem1= elementos.getElement(1)
elem1.getResistingForce()
scc0= elem1.getSections()[0]

axil= scc0.getStressResultantComponent("N")
momentoY= scc0.getStressResultantComponent("My")
momentoZ= scc0.getStressResultantComponent("Mz")
cortanteY= scc0.getStressResultantComponent("Vy")
cortanteZ= scc0.getStressResultantComponent("Vz")



N= (-E*A*alpha*AT)
ratio= ((axil-N)/N)

''' 
print "N= ",N
print "axil= ",axil
print "ratio= ",ratio
print "momentoY= ",momentoY
print "momentoZ= ",momentoZ
print "cortanteY= ",cortanteY
print "cortanteZ= ",cortanteZ
   '''

import os
fname= os.path.basename(__file__)
if (abs(ratio)<1e-10) & (abs(momentoY)<1e-10) & (abs(momentoZ)<1e-10) & (abs(cortanteY)<1e-10) & (abs(cortanteZ)<1e-10):
  print "test ",fname,": ok."
else:
  print "test ",fname,": ERROR."
