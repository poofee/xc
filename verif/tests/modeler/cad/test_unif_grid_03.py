# -*- coding: utf-8 -*-

import xc_base
import geom
import xc
import math
import os
from model import predefined_spaces
from materials import typical_materials

ndivX= 5
ndivY= 8
ndivZ= 2

# Problem type
prueba= xc.ProblemaEF()
mdlr= prueba.getModelador
# Definimos materiales
elast3d= typical_materials.defElasticIsotropic3d(mdlr,"elast3d",30e6,0.3,0.0)

nodos= mdlr.getNodeLoader
predefined_spaces.gdls_elasticidad3D(nodos)


nodos.newSeedNode()
seedElemLoader= mdlr.getElementLoader.seedElemLoader
seedElemLoader.defaultMaterial= "elast3d"
seedElemLoader.dimElem= 3
seedElemLoader.defaultTag= 1 #Tag for the next element.
brick= seedElemLoader.newElement("brick",xc.ID([0,0,0,0,0,0,0,0]));

unifGrids= mdlr.getCad.getUniformGrids
uGrid= unifGrids.newUniformGrid()

uGrid.Lx= 10
uGrid.Ly= 10
uGrid.Lz= 10
uGrid.nDivX= ndivX
uGrid.nDivY= ndivY
uGrid.nDivZ= ndivZ


setTotal= mdlr.getSets.getSet("total")
setTotal.genMesh(xc.meshDir.I)

numNodos= setTotal.getNodes.size
numElem= setTotal.getElements.size

numNodosTeor= (ndivX+1)*(ndivY+1)*(ndivZ+1)
numElemTeor= ndivX*ndivY*ndivZ

import os
fname= os.path.basename(__file__)
if (abs(numNodosTeor-numNodos)<1e-15) & (abs(numElemTeor-numElem)<1e-15):
  print "test ",fname,": ok."
else:
  print "test ",fname,": ERROR."
