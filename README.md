# pupr_senior_proj
This is my Senior Project done at the Polytechnic University of Puerto Rico before I graduated. This project was done for their Plasma Laboratories.

Note that this code is old, dating back to August 2007 and may therefore have silly tricks and mistakes

The goal of this project is to find the minimum energy achievable using a system of 6 rings with 6 charges each. This is accomplished by the usage of a random algorithm, which places all the charges at random, repeating the algorithm until the requested iterations are done.

Improvements that could be done include making each iteration a thread of its own, or using GPU processing libraries.

Memory usage depends on grid size: The larger the grid size, the more precise it is, however it also takes more memory and time to calculate.

This project depends on Boost C++ for threading which can also be improved
