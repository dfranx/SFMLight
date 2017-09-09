# SFMLight
A simple library for adding lights to your 2D game.
![example](https://i.imgur.com/Cn0rHCl.png?1)

## How to use this library
Include necessary file
```c++
#include <SFMLight.h>
```

&nbsp;

Declare a scene instance
```c++
sfl::LightScene scene;
scene.SetDebugDraw(true); // just for debugging
```

&nbsp;

Declare an object/obstacle and its boundaries
```c++
sfl::Object obj;
obj.Add(50, 50);
obj.Add(100, 50);
obj.Add(100, 100);
obj.Add(50, 100);
```

&nbsp;

Add the object to the scene
```c++
scene.Add(obj);
```

&nbsp;

Create a light and define some of its properties
```c++
sfl::Light light;
light.SetRadius(300);
light.SetPosition(sf::Vector2f(150, 150));
```

&nbsp;

Update light's vertices
```c++
scene.Update(light);
```

&nbsp;

Render everything
```c++
light.Render(window);
scene.Render(window);
```

&nbsp;

Result:
![test](https://i.imgur.com/eEVs1hD.png?1)

## Building
[COMING SOON]