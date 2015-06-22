# -*- coding: utf-8 -*-
# Simple beam formulas

from __future__ import division
import math

class SimpleBeam(object):
  E= 1.0
  I= 1.0
  l= 1.0

  def getShearUnderUniformLoad(self,q,x):
    return -q*(self.l/2.0-x)
  def getReactionUnderUniformLoad(self,q):
    return self.getShearUnderUniformLoad(self,q,0.0)
  def getBendingMomentUnderUniformLoad(self,q,x):
    return q*x/2.0*(self.l-x)
  def getDeflectionUnderUniformLoad(self,q,x):
    return q*x/(24.0*self.E*self.I)(self.l**3-2*self.l*x**2+x**3)

#          l
# |<---------------->|
#    a      b     c
# |<-->|<------>|<--->|
#      ||||||||||
#  ___________________
# ^                   ^
  def getReaction1UnderUniformLoadPartiallyDistributed(self,q,a,b):
    c= self.l-a-b
    retval= q*b/2.0/self.l*(2*c+b)
    return -retval

  def getReaction2UnderUniformLoadPartiallyDistributed(self,q,a,b):
    c= self.l-a-b
    retval= q*b/2.0/self.l*(2*a+b)
    return -retval

  def getShearUnderUniformLoadPartiallyDistributed(self,q,a,b,x):
    R1= getReaction1UnderUniformLoadPartiallyDistributed(self,q,a,b)
    retval= R1
    if(x>a):
      if(x<(a+b)):
        retval-= q*(x-a)
      else:
        retval= getReaction2UnderUniformLoadPartiallyDistributed(self,q,a,b)
    return retval

  def getBendingMomentUnderUniformLoadPartiallyDistributed(self,q,a,b,x):
    R1= getReaction1UnderUniformLoadPartiallyDistributed(self,q,a,b)
    retval= R1*x
    if(x>a):
      if(x<(a+b)):
        retval-= q/2.0*(x-a)**2
      else:
        retval= getReaction2UnderUniformLoadPartiallyDistributed(self,q,a,b)*(self.l-x)
    return retval

#          l
# |<---------------->|
#    a         b
# |<--->|<---------->|
#       |
#  _____|______________
# ^                   ^
  def getReaction1UnderConcentratedLoad(self,P,a):
    b= self.l-a
    return -P*b/self.l

  def getReaction2UnderConcentratedLoad(self,P,a):
    return -P*a/self.l

  def getShearUnderConcentratedLoad(self,P,a,x):
    R1= getReaction1UnderConcentratedLoad(self,P,a)
    retval= R1
    if(x>a):
      retval= getReaction2UnderConcentratedLoad(self,P,a)
    return retval

  def getBendingMomentUnderConcentratedLoad(self,P,a,x):
    R1= getReaction1UnderConcentratedLoad(self,P,a)
    retval= R1*x
    if(x>a):
      retval= getReaction2UnderConcentratedLoad(self,P,a)*(l-x)
    return retval

  def getDeflectionUnderConcentratedLoad(self,P,a,x):
    b= self.l-a
    retval= P*b*x/(6.0*self.E*self.I*self.l)(self.l**2-b**2-x**2)
    if(x>a):
      retval= P*b*(self.l-x)/(6.0*self.E*self.I*self.l)(2*self.l*x-x*2-a**2)
    return retval
