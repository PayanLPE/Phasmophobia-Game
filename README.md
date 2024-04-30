# Summary:
This program is a game simulation of the video game Phasmophobia. Using threads, we are able to simulate hunter behaviour which allows the hunter to communicate evidence with other hunters in the room, move to an adjacent room, or read evidence found in the current room. The ghost behaviour allows the ghost to move rooms, do nothing, leave ghostly evidence in the room. The simulation will end once the proper conditions are met which either ends with hunters winning or ghost to win.

# Follow the steps below to find out how to use the following program.

1. Before attempting to run the program, download the tar file if you have not already for the Cipher Encryptor/Decryptor 5000 and untar the tar file by moving into the directory using your terminal. To do that you can use a combination of 'ls' which lists the files and directory contained in your current directory and 'cd *insert directory name*' to enter a directory. If you enter the wrong directory, enter the command 'cd ..' to back track out of the current directory.
2. To untar the tar file, enter the command 'tar -xvf project.tar'. Once you have extracted the files from the tar file you will notice that there will be a new directory called 'project'. Enter the command 'cd project' to enter the directory.a
3. Next you want to enter the command 'make final' in the terminal. This will create an executable file of the name 'final' which allows you to run the program.
4. Run the program by entering './final'.
5. The program will ask you for a name for each of the hunters. Enter the names into the console.
6. You will notice the program running and simulating a game of Phasmophobia!

# Author:
- Peien Liu
- Louie Ma

# List of files
- building.c
- evidence.c
- ghost.c
- room.c
- hunter.c
- main.c
- defs.h
- Makefile
- README.txt
