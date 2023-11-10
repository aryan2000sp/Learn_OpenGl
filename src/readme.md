# How shaders work?
Watch this [video](https://youtube.com/watch?v=5W7JLgFCkwI&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=6) to understand how shaders work in general.

# Writing shaders
Shaders can be written as string or create a file by extension **.glsl**
Once you have shaders you have to compile them while running the program
Steps to compile and run shaders(has to be done for each shader used):
1. First create shader object by calling **glCreateShader()**.
2. Once you have created shader you add the source code that is string or **.glsl** (you have to read this file as string) to shader object by calling glShaderSource()
3. Then once you have added the source compile the shader by calling glCompileShader()
4. Then create the program object by calling glCreateProgram(). The program object is just container which contains multiple types of shader.
5. Now add the shaders created to the program object container by calling glAttachShader()
6. Once the shaders are added or attched to program object container then link and validate all the shaders togather by calling glLinkProgram() and glValidateProgram()
7. Once all the shaders are linked then we can use the program object container by calling glUseProgram().
