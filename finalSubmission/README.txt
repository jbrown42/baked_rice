Jared Brown
CSE321 - Realtime and Embedded Systems
LAB1 - DCAS Readme

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
2. The project will ask you how large of a map you would like, type the size (int) and press enter
3. The project will ask you how many drones you would like, type the amount (int) and press enter
*Note: if you don't type an int into the prompt, hit CTRL + C to kill the program and try again, following instructions this time*
4. The program will then print out every frame of movement.
5. The map uses 'A' to denote the airport and a number from 0-number of Drones to denote the drones.
6. If a collision occurs, the map will print out '<droneID> avoided vertical/horizontal collision' depending on the type.
7. The frame before this statement will the frame before the collision and the one after will be where the collision was avoided.

***************************************
Capturing the Project Output
***************************************
After bulding the project
1. Run './dcas > output.txt' to run the project
2. Follow steps 2, 3 and the Note in Running the Project
*Note: there will be no prompt when capturing the project so just type the values and press enter*
3. Open output.txt in vim or some other text editor to view the capture
