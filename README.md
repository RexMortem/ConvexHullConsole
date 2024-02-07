# Running the program

You can run the program directly using the exe file. 

You can execute it from the GUI or use the command:

./VectricConvexHull

You can also build the exe using a standard C++ compiler. Using the GNU compiler, you can do:

g++ VectricConvexHull.cpp -o VectricConvexHull

# What is the program

This is a console application written in C++ that lets you add/remove points to a plane, 
display the plane, and run an algorithm to generate the convex hull of the points. 

It implements a console-like navigation system where you use commands to navigate, 
execute state changes, and display states. 

# Using the program

Whilst running the program, you can use viewMenu to list the commands. 
It provides brief descriptions of the commands. 
An excerpt of a version of the menu is written below:

viewMenu - prints this menu out
dimensions - edit the number of rows and columns
addPoints - add points to the plane
removePoints - remove points from the plane
clearPoints - clear all points from the plane
viewSpace - view the plane of points (no hull)
viewHull - view the convex hull generated

quit/q - quit the program
