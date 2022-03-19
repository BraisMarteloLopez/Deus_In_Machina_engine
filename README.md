# DeusEx Machina engine
C++/GL/Vulkan 3D graphic engine

This proyect should be an abstraction of the engine that I am using on BOC: Birth of Cultures, my intention is to be able to dedicate some time each month to update this openSource proyect, Deus Ex Machina is focused on performance, using most moderns indirect draw APIs, to avoiding draws API overdrive as a first mayor goal.

Shaders aren't a tarjet on this proyect (compiling/implement are out of this), but I still can recomend Book of Shaders https://thebookofshaders.com/, so far the best online resourCe that I can find.


First commit, hello world! :D
Reddit post about why I started with skeletal animation system and talking about future updates: 
https://www.reddit.com/r/BirthOfCivilization/comments/smbddl/deus_ex_machina_engine_first_public_repository/


```
Content menu
1. MultiDraw Elements Indirect
2. Multi-threading and asynchronous chunks sync
3. Interface Items system
4. Skeleton animation
5. +more content at large term
```


# MultiDraw Elements Indirect
-Upcoming NEXT update-

# Multi-threading and asynchronous chunks sync
-pending to organize-

# Interface Items system
-pending to organize-

# Skeleton Animation
Skeleton animation system should not have numbers reestrictions about how many skeletons, animations, keyframes or even how many game entities use each one of them, for this reason there are two separate buffer with different purposes, one static and other dynamic, is GPU who allows the interpolation between two final positions, CPU only take care to compute animations play time, there are not any kind of uniform over-load, is the uniform buffer object the only point to sync, (his size of 32kb that guarantee a fast cpu<->gpu update) all animations transforms are pre-computed inside static buffer to avoid extra CPU load on runtime.

![Atlas_animations](https://user-images.githubusercontent.com/5490676/152707323-daf85571-5b85-4b25-a434-c0bee2b82e67.jpg)




# Dependencies

- ASSIMP: `vcpkg install assimp`.
- GLM: `vcpkg install glm`.
- GLAD: `vcpkg install glad`.
- GLFW: `vcpkg install glfw3`.


