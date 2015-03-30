README.TXT

TEAM STTD
Trevor Barnett 
Daniel He 
Tony Quach
Seth Speaks

PROJECT 1 

ROBOT ARM WITH PAINTING CAPABILITIES
In order to compile our project, you will be required to install extra libraries. These libraries are the Glut working directory, and the GLUI working directory.

INSTALLING OPENGL & GLUT
This program was written using OpenGL and Glut within Visual Studios. These libraries were used to implement the graphics. To install OpenGL and Glut onto your system use the following link:
http://aschultz.us/blog/archives/176 

INSTALLING GLUI
Glui is an extension of Glut and was used to implement the user interface. We mostly used Glui for the implementation of buttons. 

Due to issues with installing Glui, we have included the .lib and .h file necessary to run Glui. These files can be found in the installation folder labeled Glui Install from the main zip folder.
Put the glui32.lib file into the C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib directory
Put the glui.h file into the C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\gl directory

You may also install Glui manually from the following link:
https://masdel.wordpress.com/2010/06/13/installing-glui-using-vc-on-windows/

The dependencies for your VS project have to be altered as well. Under Linker -> dependencies (glut32.lib, glui32.lib, opengl.lib) must all be added.
NOTE: this is already done in the included project from our webpage

With all of these in place, open the file and run the debugger, the project will run as instructed.
