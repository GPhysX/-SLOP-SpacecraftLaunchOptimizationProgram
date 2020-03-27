-SLOP- Spacecraft Launch Optimization Program

--Status--
Proof of Concepte in MATLAB finished May 2019
ReOpened for porting to Python/C++ January 2020


--!!UPDATE!!--
3/27/2020
This project has been changed from being entirely in C++ to now being primarily in Python with C++ called to handle the heavy lifting. This change was made due to complications with calling a python library in C++. I should have new files uploaded sometime this weekend to make the switch official. Also a proper devlog since I can't keep dumping these sort of updates in the README.

--Project Summary--
SLOP is a program which uses a combination of Monte Carlo methods, Verlet Integration, and nonlinear optimization to find initial launch vectors for a spacecraft traveling from Earth to a user specified destination. This project was originally completed in MATLAB as a proof of concept for my 2017 Computational Physics class final. I have recently reopened the project to port it to Python and to take it from a proof of concept state to a finished product state.

Planned improvements from the original proof of concepte are shown below. This list is subject to change at any time.
1. Dynamic timesteps for Verlet Integration
2. NASA JPL Horizons Interfacing
3. Multithreading the Monte Carlo and Optimization
4. GUI?

--My Contribution--
I wrote all the code showcased in this project. The MATLAB version was completed in approximatley 1 week as my final project for a computational physics class. The Python/C++ version is currently in development.

--Software--
This folder contains all the code I have written for this project. The MATLAB folder contains the original proof of concept version of this program. This version of the program is technically functionally but is extremely limited in what it can calculate. The C++ folder contains some of the current work I am doing on this project. 
