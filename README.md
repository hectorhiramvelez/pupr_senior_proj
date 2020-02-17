# pupr_senior_proj
This is my Senior Project done at the Polytechnic University of Puerto Rico before I graduated. This project was done for the Plasma Laboratories.

Note that this code is old, dating back to August 2007 and may therefore have silly tricks and mistakes

The goal of this project is to find the minimum energy achievable using a system of 6 rings with 6 charges each. This is accomplished by the usage of a random algorithm, which places all the charges at random, repeating the algorithm until the requested iterations are done.

Improvements that could be done include making each iteration a thread of its own, or even using GPU processing libraries.

Memory usage depends on grid size

This project depends on Boost C++ for threading but that can also be improved
