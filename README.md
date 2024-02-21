# ClimaxTest
A Top down RTS test

## Unreal Engine 5.3.2

# Note
- Before playing please let the navigation finish building. The units dont move otherwise.

# Controls

- **W,A,S,D** - Pan Camera
- **Mouse X,Y** - Move to edge of screen to also pan around
- **Scroll Wheel** - Zoom in/out
- **Left Click** - Select a single Unit
- **Left Click Drag** - Select Multiple Unit
- **F** - Toggle Formation Shape Selection Widget
- **Right Click** - Move Selected Unit(s) to point


# Features and Explanations
## Camera
- The camera can be moved with the usual WASD but it also moves if you hit the edge of the screen with the mouse cursor. Initially the camera just moved if you hit the edge but I thought it would be more fun to add a smooth interpolation so the more closer you are to the screen edge the faster the camera moves. There is also a zoom in to get closer to the action or zoom out more to get the whole scene. All of the values are configurable in a data asset that can be found in: \
*ClimaxTest/Blueprints/CameraPawn/DA_DefaultCTCameraPawn.uasset*

## Unit Selection
- You can select a single unit or you can marquee select multiple units. Units remain outlined even when obscured from view, ensuring that you can still track and identify selected units.
- When I got to researching Unit Selection methods, most of the method was to draw rect in the HUD. I wasn't a huge fan of that since that limits the artists. I wanted a widget instead that we could control the style of and thats what I ended up implementing.
- You can click anywhere to deselect unit(s).

## Unit(s) Movement
- Press 'F' to show the formation menu and select a formation. If you choose a group of units, it will adhere to that formation. I had initially made it so that the units just reach the spot and not collide into each other at the goal, (there should still be a commit for this in the *Better-Unit-Movement* branch if you want to check it out. :P), but I wanted to make something cooler so I ended making this formation system. It is extendable to support different formations, but we need to create new EQS shape generators. More on this later.
- Currently I am spawning a formation actor that runs an EQS generator and tells all the selects units to move to the generated points, once this is done it destroys itself.
- Theres a formation data where we can specify EQS Query params and the type of EQS Query.
- We can have different shapes with new EQS generators. 
- EQS Generators have distance test and pathing test so they still try their best to maintain the shape while also conforming to the environment.
- Detour Crowd Control has been enabled in the AI Controller so they avoid each other when moving.

# Things that could be better
## Camera
- The camera doesn't have any environment tracing. This means if we had a mountain or something in the middle it would clip. We could implement a line trace and have the end of our spring arm follow this.
- Camera bounds so you stay within the level

## Selecing Units
- Unreal has a built in GetActorsInRectangle but the caveat is we can only use it in the draw hud event, so we need to pass on info to HUD and get back the selected actors. I am not a huge fan of how this going out of one class and then coming back in with the actors info. If this was a proper production setting I would look into decoupling that HUD function from draw HUD and research how we can use it normally. All that functionally does is create a bounding volume and checks if actors of a selected type are within that bounds. But to do this it needs to project the 2D rect onto the world which unfortunately runs only on DrawHUD event since it needs the Canvas. Another method to go about this would be to calculate the 4 points of the box in the world space with the start mouse position and current mouse position and then do a box trace, but I probably wouldn't do this since the above approach is much cleaner.

## Unit Formation
- Currently the FormationActor creates and destroys itself, we could object pool it so we can reuse this, to save on resources.
- I really wanted to implement a radial menu that pops up and you can select the formation type, but I realized that is not the priority now :P.
- Currently we are allowing the player to move the units even if a formation may not be valid. For eg trying to make a line in a tight space. There is no way 10 units are fitting in a line in that corner. \
If this is the case, currently only some number of units move. If by design this is okay then this works but if we want all the troops we selected to be in a formation we need to implement some kind of formation preview that will show where our troops will end up, if that doesn't exist we shouldn't even call move on any of the units.

- Right now the formation data is doing some weird conversions to scale the params for the generators with the players. For example if we had a grid generator we could just have 2 params called width/height and we can just have a map of those values in the data. Right now its scaled by the number of units we have, so the int part of the map pair refers to the base value we need to multiply our number of units by. 
For eg: If its a circle formation the number of points on that circle is determined by int (which is 1) * Number of units, so n points will be generated. \
I am ***really*** not happy with this implementation as its hard to understand and overengineered. If this was production we would have our own generators with standardized user friendly values.

- AI should have the ability to repath mid pathing. If its goal is obscured by some other pawn, it should break out of the formation and go to somewhere close.
- AI should "push" other AI out of the way. Currently if we try to move an AI that is the middle of a circle formation, it can't get out.

