# -*- coding: utf-8 -*-

def ListaCargaUniforme(mdlr,loadPattern, archivo, carga, fmt):
  elemTags= loadPattern.getElementTags()
  for i in elemTags:
    e= mdlr.getElementLoader.getElement(i)
    archivo.write(e.tag," & ",loadPattern," & unif. & & ",fmt.format(carga[0]/1e3)," & ",fmt.format(carga[1]/1e3)," & ",fmt.format(carga[2]/1e3),"\\\\\n")

def ListaCargaPuntual(mdlr,loadPattern, archivo, carga, fmt):
  xCarga= x
  elemTags= loadPattern.getElementTags()
  for i in elemTags:
    e= mdlr.getElementLoader.getElement(i)
    archivo.write(e.tag," & ",loadPattern," & punt. & ",xCarga," & ",fmt.format(carga[0]/1e3)," & ",fmt.format(carga[1]/1e3)," & ",fmt.format(carga[2]/1e3),"\\\\\n")

def ListaCargasElementos(loadPattern, archivo, fmt):
  caption= "Cargas sobre elementos"
  defCampos= "|r|c|c|r|r|r|r|"
  idsCampos= "Id & Acc. & Tipo & x & Fx & Fy &Fz \\\\\n - & - & - &  & kN/m & kN/m & kN/m "
  cabeceraSupertabular(archivo,7,defCampos,idsCampos,caption)
  loads= mdlr.getLoadPatterns
  print "Número de patrones de carga: ",numLoadPatterns
  print "Número de cargas en elementos: ", loadPattern.getNumEleLoads()
  eleLoads= loadPattern.getEleLoads()
  for l in eleLoads:
    if(l.category=="nodef"):
      carga= l.getGlobalForces
    elif(l.category=="uniforme"):
      ListaCargaUniforme(loadPattern,archivo,carga,fmt)
    elif(l.category=="puntual"):
      ListaCargaPuntual(loadPattern,archivo,carga,fmt)
  cierraSupertabular(archivo) 

def ListaCargasNodos(loadPattern, archivo, fmt):
  caption= "Cargas sobre nodos"
  defCampos= "|r|c|c|r|r|r|r|"
  idsCampos= "Id & Acc. & Tipo & x & Fx & Fy &Fz \\\\\n - & - & - &  & kN & kN & kN "
  cabeceraSupertabular(archivo,7,defCampos,idsCampos,caption) 
  print "Número de cargas en nodos: ", loadPattern.getNumNodalLoads()
  nodalLoads= loadPattern.getNodalLoads()
  for l in nodalLoads:
    carga= l.load
    archivo.write(l.nod.tag," & ",loadPattern," & nod. &  & ",fmt.format(carga[0]/1e3)," & ",fmt.format(carga[1]/1e3)," & ",fmt.format(carga[2]/1e3),"\\\\\n")
  cierraSupertabular(archivo) 


