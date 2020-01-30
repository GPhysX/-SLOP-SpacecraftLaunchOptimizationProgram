# This file is to test if matplotlib is fast enough for me to use it in testing of the nonlinear optimization of spacecraft launch parameters C++ program

import numpy as np
import matplotlib.pyplot as plt
import pylab as py
from matplotlib import animation
import astropy as aspy
from astroquery.jplhorizons import Horizons

# For some the location defaults to Earth @ my input. Not sure if I can get around this or not. Leaving it blank will use the sun as the center which is probably fine
obj = Horizons(id='499',location='500@0',epochs={'start':'2020-01-01', 'stop':'2020-01-05','step':'1d'},id_type='majorbody')
#eph = obj.ephemerides()
vec = obj.vectors()
print(obj)
print(vec['targetname','datetime_str','x','y'])
print(obj.uri)
#TableColumns names=('x','y','z','vx','vy','vz')


# First I'll need to load that data in from the Output.txt file
RawData = np.loadtxt(fname = "/Users/4lius/Documents/C++/NonlinLaunchFinder/Output.txt")
dims = np.shape(RawData)
print(dims)

py.figure(1)
plt.plot(RawData[:,0],RawData[:,1]) # Mercury
plt.plot(RawData[:,2],RawData[:,3]) # Venus
plt.plot(RawData[:,4],RawData[:,5]) # Earth
plt.plot(RawData[:,6],RawData[:,7]) # Mars
#plt.plot(RawData[:,8],RawData[:,9]) # Jupiter
#plt.plot(RawData[:,10],RawData[:,11]) # Saturn
#plt.plot(RawData[:,12],RawData[:,13]) # Uranus
#plt.plot(RawData[:,14],RawData[:,15]) # Neptune
plt.plot(RawData[:,16],RawData[:,17]) # Sun
plt.plot(RawData[:,18],RawData[:,19]) # Spaceship
py.show(1)


