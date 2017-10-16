Jared Brown
CSE321 - Realtime and Embedded Systems
LAB1 - DCAS Readme

***************************************
Program Details
***************************************
Map details
  ~ - represent open space in map
  A - represents airport
  (0-9) - represents different drones
  Top of map is sky, bottom of map is the ground
  The map will print once every .05 seconds for smooth movement

Drones move an arbitrary speed along a random path generated at their creation.
Only one drone can leave the airport at a time due to the launch delay.
Only one drone can land at the airport at a time because of collision avoidance.

***************************************
Building the Project
***************************************
1. In terminal, navigate to the directory this README is located in.
2. Run the 'make' command in the terminal.
3. The directory should now contain an exe called 'dcas' and the .o files for the project.

***************************************
Cleaning the Project
***************************************
If you want to clean the project after running it
1. Navigate to the directory containing the project and make file
2. run 'make clean'
3. 'dcas' and the .o files should now be removed from the directory

***************************************
Running the Project
***************************************
After building the project and before cleaning it, do the following
1. Navigate to the project directory and run ./dcas
2. The program will begin print out the map every .05 seconds.

***************************************
Capturing the Project Output
***************************************
After bulding the project
1. Run './dcas > output.txt' to run the project (give it time, this will take ~1 minute)
2. Open output.txt in vim or some other text editor to view the capture
	2.1 if opening in vim, for best results:
		2.1.1 resize terminal so that only one map can be seen
		2.2.2 hold shift and press down or up to move forward or backwars in "time" (should look like a smooth animation)
	2.2 if opening in text editor:
		2.1.1 press pgdn button to move forward or backwards in "time" (won't be a smooth as vim)
