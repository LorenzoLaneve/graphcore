# graphcore

Graphcore is a library that provides an interface for rendering complex and animated models. It aims to be lightweight and platform-independent.
Another abstraction layer will be added to allow rendering through graphics libraries other than OpenGL, to provide the best performance in different environments.

The project also defines a file format that is designed to be easy to read, and ready to be drawn. An application translating Collada models to this format is provided.


Graphcore has the following dependencies:
 - GLFW 3.2
 - GLEW
 - SOIL (to be replaced with our image loader)
 - GLM (to be replaced with our math libraries)
 
 Collada2bin has the following dependencies:
 - ASSIMP
