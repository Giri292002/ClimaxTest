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
- You can select a single unit or you can marquee select multiple units. Units remain outlined even when obscured from view, ensuring that you can still track and identify selected units.
- When I got to researching Unit Selection methods, most of the method was to draw rect in the HUD. I wasn't a huge fan of that since that limits the artists. I wanted a widget instead that we could control the style of.
- You can click anywhere to deselect unit(s).

# Things that could be better
## Camera
- The camera doesn't have any environment tracing. This means if we had a mountain or something in the middle it would clip. We could implement a line trace and have the end of our spring arm follow this.

## Selecing Units
- Unreal has a built in GetActorsInRectangle but the caveat is we can only use it in the draw hud event, so we need to pass on info to HUD and get back the selected actors. I am not a huge fan of how this going out of one class and then coming back in with the actors info. If this was a proper production setting I would look into decoupling that HUD function from draw HUD and research how we can use it normally. All that functionally does is create a bounding volume and checks if actors of a selected type are within that bounds. But to do this it needs to project the 2D rect onto the world which unfortunately runs only on DrawHUD event since it needs the Canvas. Another method to go about this would be to calculate the 4 points of the box in the world space with the start mouse position and current mouse position and then do a box trace, but I probably wouldn't do this since the above approach is much cleaner.

