# SFMLight
SFMLight is a simple to use library that can be used to add lighting to your 2D game.


## Screenshots
![example](https://i.imgur.com/Cn0rHCl.png?1)


## Getting started
These steps will show you how to build SFMLight on your machine.

### Dependencies
This project uses:
* [SFML](https://www.sfml-dev.org/download.php)

### Building
First, get the SFMLight source code
```
git clone https://github.com/dfranx/SFMLight.git SFMLight
cd SFMLight
```

Then build it
```
cmake .
make
```

If you get error message which says that SFML was not found on your computer you
need to specify SFML_ROOT variable in your cmake command:
```
cmake -DSFML_ROOT=path/to/your/sfml .
```

## Usage
Include the library
```c++
#include <SFMLight.h>
```

Declare a LightScene
```c++
sfl::LightScene scene;
scene.SetDebugDraw(true); // just for debugging
```

Declare an object/obstacle and define it's boundaries
```c++
sfl::Object obj;
obj.Add(50, 50);
obj.Add(100, 50);
obj.Add(100, 100);
obj.Add(50, 100);
```

Add the object to the scene
```c++
scene.Add(obj);
```

Create a light and define some of its properties (radius and position)
```c++
sfl::Light light;
light.SetRadius(300);
light.SetPosition(sf::Vector2f(150, 150));
```

Update light's vertices
```c++
scene.Update(light);
```

Render everything
```c++
light.Render(window);
scene.Render(window);
```

Result:

![test](https://i.imgur.com/eEVs1hD.png?1)

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
