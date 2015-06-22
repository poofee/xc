# -*- coding: utf-8 -*-
# Test comprobación de perno de anclaje según EOTA TR 029.
# comparado en parte con los resultados que da Hilti en el documento
# Hilti HIT-HY 150 with rebar.

from materials.pernos_anclaje import EOTA_TR029
from materials.pernos_anclaje import EOTA_TR029_traccion
import math
import geom

def sqr(a):
  return a*a

# Datos
gammaMs= 1.4 # Minoración de la resistencia del acero.
gammaMc= 2.1 # Minoración de la resistencia del acero.
diamBarra= 25e-3 # Diámetro del perno en metros.
areaBarra= math.pi*sqr(diamBarra/2) # Área de la barra en metros cuadrados.
h= 274e-3 # Espesor de la pieza.
hef= 210e-3 # Profundidad efectiva del anclaje.
posAnc=  geom.Pos2d(.135,0) # Posición del anclaje
contornoPiezaSoporte=  geom.Poligono2d() # Contorno de la pieza.
contornoPiezaSoporte.agregaVertice(geom.Pos2d(0,-1))
contornoPiezaSoporte.agregaVertice(geom.Pos2d(1,-1))
contornoPiezaSoporte.agregaVertice(geom.Pos2d(1,1))
contornoPiezaSoporte.agregaVertice(geom.Pos2d(0,1))    


fuk= 550e6 # Resistencia a tracción del acero  del perno en Pa.
tauRk= 7.5e6 # Valor característico de la tensión de adherencia (tomado de ETA-05/0051 tabla 11).
tauRkUcr= 7.5e6 # Valor característico de la tensión de adherencia para hormigón no fisurado.
k1= 10.1 # 7.2 para hormigón fisurado y 10.1 para hormigón no fisurado.
fckCube= 25e6 # Resistencia característica a compresión del hormigón medida en probeta cúbica.
# Resistencia del propio perno
NRds= EOTA_TR029_traccion.axialResistanceSteelFailure(areaBarra,fuk)/gammaMs


# Influencia de la distancia al borde
CcrN= EOTA_TR029_traccion.getCcrNp(diamBarra,hef,tauRkUcr)
C= contornoPiezaSoporte.getRecubrimiento(posAnc) 
Cmin= 5*diamBarra+10e-3

if (C<Cmin):
  print "El recubrimiento del perno es insuficiente."

f1N= EOTA_TR029_traccion.getFactor1N(C,CcrN)

# Área bruta extracción
plgA0pN= EOTA_TR029_traccion.getA0pN(diamBarra,posAnc,hef,tauRkUcr)
A0pN= plgA0pN.getArea()
# Área neta pull-out
plgApN= plgA0pN
plgApN.recortaPorPoligono(contornoPiezaSoporte)
ApN= plgApN.getArea()
f2pN= EOTA_TR029_traccion.getFactor2pN(A0pN,ApN)
N0Rdp= EOTA_TR029_traccion.axialInitialResistancePullOut(diamBarra,hef,tauRk)/gammaMc
NRdp= N0Rdp*f1N*f2pN # Extracción

# Área bruta desprendimiento de cono
plgA0cN= EOTA_TR029_traccion.getA0cN(posAnc,hef)
A0cN= plgA0cN.getArea()
# Área neta extracción de cono.
plgAcN= plgA0cN
plgAcN.recortaPorPoligono(contornoPiezaSoporte)
AcN= plgAcN.getArea()
f2cN= EOTA_TR029_traccion.getFactor2cN(A0cN,AcN)
N0Rdc= EOTA_TR029_traccion.axialInitialResistanceConeFailure(k1,fckCube,hef)/gammaMc
NRdc= N0Rdc*f1N*f2cN # Desprendimiento de cono.

# Splitting
CcrSp= EOTA_TR029_traccion.getCcrSpHiltiHY150(h,hef)
ScrSp= 2*CcrSp
f1Nsp= EOTA_TR029_traccion.getFactor1N(C,CcrSp)
# Área bruta splitting
plgA0spN= EOTA_TR029_traccion.getA0spN(posAnc,CcrSp)
A0spN= plgA0spN.getArea()
# Area neta splitting
plgAspN= plgA0spN
plgAspN.recortaPorPoligono(contornoPiezaSoporte)
AspN= plgAspN.getArea()
f2spN= EOTA_TR029_traccion.getFactor2spN(A0spN,AspN)
N0RdSp= N0Rdc
NRdSp= N0RdSp*f1Nsp*f2spN # Desprendimiento de cono.

# Resultados

''' Resistencia a tracción del perno aislado sin
   tener en cuenta el efecto favorable de la presencia
   de armadura. '''
NRd= min(min(min(NRdSp,NRdc),NRdp),NRds)

ratio1= abs(NRds-192900)/192900
ratio2= abs(N0Rdp-58.9e3)/58.9e3
ratio3= abs(N0Rdc-73.2e3)/73.2e3
ratio4= abs(f1N-0.828571)/0.828571
f2pNTeor= (sqr(2*CcrN)-(CcrN-C)*2*CcrN)/sqr(2*CcrN)
ratio5= abs(f2pN-f2pNTeor)/f2pNTeor
CcrNc= EOTA_TR029_traccion.getScrN(hef)/2
f2cNTeor= (sqr(2*CcrNc)-(CcrNc-C)*2*CcrNc)/sqr(2*CcrNc)
ratio6= abs(f2cN-f2cNTeor)/f2cNTeor
ratio7= abs(NRd-34.8621e3)/34.8621e3

'''
print "A_s= ",areaBarra*1e6," mm2\n"
print "C_{cr,N}= ",CcrN," m\n"
print "C= ",C," m\n"
print "C/C_{cr,N}= ",C/CcrN
print "Área bruta extracción A0pN= ",A0pN," m\n"
print "Área neta extracción ApN= ",ApN," m\n"
print "Área bruta extracción cono A0cN= ",A0cN," m\n"
print "Área neta extracción cono AcN= ",AcN," m\n"
print "Área bruta splitting A0spN= ",A0spN," m\n"
print "Área neta splitting AcN= ",AspN," m\n"
print "f1N= ",f1N
print "f2pNTeor= ",f2pNTeor
print "f2pN= ",f2pN
print "f2cN= ",f2cN
print "C_{cr,sp}= ",CcrSp," m\n"
print "N_{Rd,s}= ",NRds/1e3," kN\n"
print "N_{Rd,p}= ",NRdp/1e3," kN\n"
print "N_{Rd,c}= ",NRdc/1e3," kN\n"
print "N_{Rd,sp}= ",NRdSp/1e3," kN\n"
print "N_{Rd}= ",NRd/1e3," kN\n"
print "ratio1= ",(ratio1)
print "ratio2= ",(ratio2)
print "ratio3= ",(ratio3)
print "ratio4= ",(ratio4)
print "ratio5= ",(ratio5)
print "ratio6= ",(ratio6)
print "ratio7= ",(ratio7)
'''

import os
fname= os.path.basename(__file__)
if (ratio1<5e-4) & (ratio2<5e-4) & (ratio3<5e-4) & (ratio4<5e-5) & (ratio5<0.03) & (ratio6<0.03) & (ratio6<1e-5):
  print "test ",fname,": ok."
else:
  print "test ",fname,": ERROR."
