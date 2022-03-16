# DeusEx Machina engine
C++/GL/Vulkan 3D graphic engine
```
First commit, hello world! :D
Reddit post about why I started with skeletal animation system and talking about future updates: 
```
https://www.reddit.com/r/BirthOfCivilization/comments/smbddl/deus_ex_machina_engine_first_public_repository/


# Skeletal Animation
The skeletal animation system should not have numbers reestrictions about how many skeletons, animations, keyframes or even how many game entities use each one of them, for this reason there are two separate buffer with different purposes, one static and other dynamic, is GPU who allows the interpolation between two final positions, CPU only take care to compute animations play time, there are not any kind of uniform over-load, is the uniform buffer object the only point to sync, (his size of 32kb that guarantee a fast cpu<->gpu update) all animations transforms are pre-computed inside static buffer to avoid extra CPU load on runtime.

![Atlas_animations](https://user-images.githubusercontent.com/5490676/152707323-daf85571-5b85-4b25-a434-c0bee2b82e67.jpg)



# Dependencies

- ASSIMP: `vcpkg install assimp`.
- GLM: `vcpkg install glm`.
- GLAD: `vcpkg install glad`.
- GLFW: `vcpkg install glfw3`.


