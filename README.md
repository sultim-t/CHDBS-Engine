# CHDBS Engine
CHDBS Engine is a game engine that provides rendering 3D objects, simple physics simulation, basic scripting system.

# Graphics
Rendering is implemented using OpenGL.

- drawing static meshes with specified material
- drawing dynamic meshes with animation
- particle systems
- skybox
- frustum culling
- ui

# Physics
Engine uses custom phycics engine that supports:

- checking collision between AABB, spheres and meshes (except mesh-mesh)
- checking ray
- rigidbody simulation
- kinematic rigidbody
- trigger colliders

# Component system
Each object has a list of components (scripts) which define behavior of this object. 

# References
- Mike McShaffry, David Graham (2013). Game Coding Complete. Boston, MA: Course Technology.
- Joey de Vries (2017). Learn OpenGL. (n.p.).
- Jason Gregory (2009). Game Engine Architecture. Boca Raton, FL: Taylor and Francis Group, LLC.
- Christer Ericson (2005). Real-Time Collision Detection. San Francisco, CA: Elsevier Inc.

# Screenshots
![Screenshot #0](https://github.com/SultimTsyrendashiev/CHDBS-Engine/blob/master/GithubImages/0.jpg)
![Screenshot #1](https://github.com/SultimTsyrendashiev/CHDBS-Engine/blob/master/GithubImages/1.jpg)
