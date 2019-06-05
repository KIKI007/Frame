# Frame
C++ Framework for Assembly Research

# Problems
1. inconsistent data type. (three different 3-dim vector classes mix together) 
2. unorganized input and output. (hard code output hiden in class)
3. limited code reusability. (copy and paste a function in a class usually does not work. The function have too many dependecy of the class).
4. unoriganized data exchange with python(compas).
5. duplicate rendering code.
6. OpenGL depended GUI.
7. fail to compile in server.
8. unorganized unit test.
9. unorganized documentation.
10. lack of automatically cross plateform compiling check.

# Core Features
## Dataflow Programming
- battery/link and graphical representation. (like grasshopper)
- dataflow control and autoparallelization.
- automatical generate input and output .xml file.
- battery group which combines each battery's I/O into one Unified I/O.
## Unified Data Type
- number, vector, matrix, ...
- triangle mesh, polygonal mesh, ...
- data conversion from C++ to python. (for compas)
## User Interface
- imgui, which can be seperated from rendering system.
## Render System
- OpenGL >= 3.x
## Server and Cross Platform
- libraries can be compile in common server.
- should support Windows.
## Unit Test
- Catch2
## Documentation
- ?
## Automatically Compiling Check
- Travis CI
