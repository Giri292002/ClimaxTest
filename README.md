# ClimaxTest
A Top down RTS test

# Controls

- **W,A,S,D** - Pan Camera
- **Mouse X,Y** - Move to edge of screen to also pan around
- **Right Click** - Move Selected Unit(s) to point
- **Scroll Wheel** - Zoom in/out
- **Left Click** - Select a single Unit
- **Left Click Drag** - Select Multiple Unit

# Features and Explanations
## Camera
- The camera can be moved with the usual WASD but it also moves if you hit the edge of the screen with the mouse cursor. Initially the camera just moved if you hit the edge but I thought it would be more fun to add a smooth interpolation so the more closer you are to the screen edge the faster the camera moves. There is also a zoom in to get closer to the action or zoom out more to get the whole scene. All of the values are configurable in a data asset that can be found in: \
*ClimaxTest/Blueprints/CameraPawn/DA_DefaultCTCameraPawn.uasset*

## Unit Selection
- Currently you can select a single unit or you can select multiple units
- When I got to researching Unit Selection methods, most of the method was to draw rect in the HUD. I wasn't a huge fan of that since that limits the artists. I wanted a widget instead that we could control the style, color and effect and pretty much all the features of a widget blueprint.

# Things that could be better
## Camera
- The camera doesn't have any environment tracing. This means if we had a mountain or something in the middle it would clip. We could implement a line trace and have the end of our spring arm follow this.

