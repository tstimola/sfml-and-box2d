# sfml-and-box2d
A simplistic, one-source program to demonstrate the coupling of SFML and Box2D in C++ 

To expand, user must create the "game framework" themselves. This might include creating an object type which contains data which may be accessed and/or modified by either SFML or Box2D
In example, a GameObject will contain Components, which should serve as a flywieght data structure which contains information specific to to "manager" that will update it
For timestep and threading, you could treat the "core engine" (the coupling of SFML and Box2D) as its own thread trading back and forth between graphics and physics steps, and then run one or more threads which are dedicated to higher-level game management.
