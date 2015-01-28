# H1Z1Tool
H1Z1 Map / Tool modification

![preview](http://i.imgur.com/QdGSgfA.jpg)

![preview](http://i.imgur.com/eyPFodi.jpg)

## Requirements in order to build on Windows
- Visual Studio 2015 Preview or CTP
- Visual Studio 2015 Redistributables/Runtime
- x64 Windows 8/8.1/10 Preview
- DirectX SDK 2010
- (If you're trying to run it with visual studio in DEBUG/RELEASE mode, make sure to copy the data folder (dist/data) to (build/DEBUG , build/RELEASE))

## H1Z1Tool includes the following features
```
- General DirectX9 external overlay
- World Position
- Heading including a compass
- Distance to Pleasent Valley
- General Minimap
- Zombie/Player attention/warning if they have focused you/are attacking you
- World items & entity (3D) to 2D screen conversion
```
**UPCOMING**
- Minimap(with texture overlay) + Fullscreen Map
- Navigation system which leeds you with an arrow on your wanted direction
- Lightning improvements(night & day) + longer day cycles
- Spawn protection so bugged objects won't kill you
- Automatic computed direction runner(relax & lay back while the game runs to your direction itself)
- Other player loot ability & preview (see what loot other players have)
- Timers to show you when you have to eat/drink again
- Vehicle stability improvements(including physics)

## H1Z1Tool is using following methods (at the moment)
- RPM (ReadProcessMemory)
- DirectX9 / WIN32 overlay creation

## Development Ideas
```
Usage of hidden DLL injection (x64)
CreateRemoteThread
Anticheat reverse engineering of H1Z1 to research cheat protection
```
