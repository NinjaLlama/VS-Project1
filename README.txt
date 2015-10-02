Comp 465 Warbird Simulation Phase 1
Therese Horey
Jesus Moran Perez


-----------------------------------

Summary:
	Project 1 creates a small solar system consisting of a sun (Ruber), 
	two planets (Unum and Duo), and two moons which orbit Duo (Primus and Secundus).
	One spaceship (Warbird) and 1 missile also exist in this system.

	The sun, planets, and moons are initially placed along the X axis at:

	Ruber	  0	(the origin)
	Unum	  4000	
	Duo       9000	
	Primus	  900	
	Secundus  1750	

	Since Primus and Secundus orbit Duo, these values represent their distance from Duo.
	Also, Duo should be 180 degrees opposite Unum at -9000, but to keep the planets'
	relative axes the same they are loaded in the above manner.  Duo is given its own
	rotation matrix to move it 180 degrees.  All objects rotate counter-clockwise.
	
	There are 5 camera views: Front, Top, Ship, Unum, and Duo.
	
	The program runs at a fixed 25 fps.

How to run:
	- Place file in folder where 'includes465' folder is
	- Unzip Project folder
	- Use Visual Studio to open the Project solution file
	- Build and run solution

Controls:
	- v to toggle camera
	- d to toggle debug options *not fully implemented*
	- q to exit program

-----------------------------------
	
See Project1.cpp file comments for more detailed info regarding program structure and code.
