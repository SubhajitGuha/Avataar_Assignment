Build Instructions:-
* To build the project run the "GenerateProjectFiles.bat" file in the root directory and this will generate the "Visual Studio 2019" project files(.sln) for you.  
* Open Visual Studio 2019 and build/run the project using any configurations(Release/Debug).  
* Thats all.

In case if any build error occurs, I have included the build version of the application in "BuildEXE" folder, where you just have to run the "Renderer3D.exe" to launch the application.  

The Application:-  
When you run the application you will see 36 spheres with varying material properties, shaded using "Phong material model". From left to right the diffuse parameter decreases linearly and from bottom to top the specular parameter
increases linearly. I use 3-lights one directional light at direction [-3.0f,-5.0f,5.0f] and two point lights at locations [-15.0,-10.0,-5.0] and [-15.0,10.0,-5.0]. When you click on one of 
the spheres it changes its color to its surface normal, you can click it again to show the shaded color. If you click on any other location all the spheres changes its color to the original
shaded color. You can easily resize the application window as you need and to quit the application press "Esc" or close the application.  

  
The Assignment:-  
The code solution has a Renderer3D folder and it contains all the code and the thirdparty libraries. The "Application.cpp" file contains the render-loop and it manages all 
sub-systems (like window creation, events, creating and initializing a renderer, setup a camera...etc). Subsystems are contained in their respective folders like "Camera",
"Events", "Window". All OpenGL code is present in the "OpenGL folder", it contains a Renderer class that handles the buffer creation, shader creation, passing uniforms and drawing 
of spheres.Shaders are parsed from a file present in the Renderer3D/Shaders folder, it contains the "LightingShader.glsl" shader file and it contains the vertex and fragemnt shader.  

The shader is quite simple, the vertex shader just outputs the vertex positions in "Normalized Device Coordinate" space, and the fragment shader calculates the lighting using
Phong material model using this :- total_color = Kd * diff_color * NdotL + Ks * spec_color * pow(NdotH,alpha), where Kd and Ks are the diffuse and specular factors, NdotL is the dot product 
between normal and light vector, NdotH is the dot product between the normal and the half vector and alpha is the factor that controls the size of the specular lobe.
I have one directional Light and two point lights and I calculate the lighting accordingly.  

For highlighting the spheres (i.e changing the sphere color to sphere normal color) when I click on them, I do a "Ray-Sphere" intersection test for each of the spheres. If the intersection is true I set the isHit
variable to '1' and pass it into the shader as an uniform. When "Mouse Press Event" is triggered I call a function where I get the window-space mouse position and then I convert it to clip-space
using the screen size, then to view-space using inverse of the projection matrix, then to world-space using inverse of the camera view matrix. This world-space position becomes my ray_origin and my ray_direction is my view direction. So using these ray_origin, sphere position and ray_direction
I do the ray-sphere intersection test.
