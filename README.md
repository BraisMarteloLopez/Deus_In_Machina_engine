# DeusEx Machina engine
C++/GL/Vulkan 3D graphic engine
```
First commit, hello world! :D
Reddit post about why I started with skeletal animation system and talking about future updates: 
```


# Skeletical Animations loader
The skeletical animation system should not have numbers reestrictions about number of skeletons, animations, keyframes or even number of game entities for each one of them, for this reason there are two separate buffer with different purposes, one static and other dynamic, is GPU that allows the interpolation between two final vec3 positions, CPU only take care to direct animation durations and current keyframes, there are not any kind of uniform over-load, is the uniform buffer object the only point to sync, (his small size 32kb that guarantee a faster cpu<->gpu sync.

![Atlas_animations](https://user-images.githubusercontent.com/5490676/152705358-25342061-576e-4ad1-83c9-c67bb173c9ab.jpg)


# Dependencies

-ASSIMP: `vcpkg install assimp`.
-GLM: `vcpkg install glm`.
-GLAD: `vcpkg install glad`.
-GLFW: `vcpkg install glfw3`.


