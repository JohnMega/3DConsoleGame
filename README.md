3D Windows Console Game Engine. All calculations are performed on the CPU. The game engine is written without using any graphics libraries.

Github:
3DConsoleGame: https://github.com/JohnMega/3DConsoleGame
wc: https://github.com/JohnMega/wc

Short Map editor tutorial:

To open the map editor, run the file wc.exe.
To move the camera around the scene, press the W A S D keys (make sure that you have the English keyboard layout enabled and the CapsLock is turned off).
To enter the menu, press Esc.
To exit the menu, press Esc.
You can scroll through by pressing the S key.
To select one of the functions, you need to navigate to it and press Enter.
To open the submenu of an item, hover over the item in the menu and press the RIGHT key.

1) How to download an existing map:
Select Load Map function and enter the name of the existing map (it has an extension.bsp):
![D__3d_wc exe-2024-08-22-11-40-31](https://github.com/user-attachments/assets/a1cb3ff0-8b85-4b46-a366-bba081cdcb11)

2) How to spawn objects:
You need to select one of the functions with the Spawn prefix and press Enter.
After that, to freeze the object, you need to press Enter again.
![D__3d_wc exe-2024-08-22-11-53-04](https://github.com/user-attachments/assets/fa23b794-c2ad-4cc7-b254-4eece5f88c8c)

3) How to select an existing object:
You need to press H, navigate to the desired object and press Enter.
![D__3d_wc exe-2024-08-22-12-07-39](https://github.com/user-attachments/assets/1d1a0dea-fd5a-4fdb-a29f-2cfb696bbb79)

4) How to move/rotate/resize a selected object:
You need to select some object, and then press:
Q - to change the position of the object;
E - to rotate the object;
R - to change the size of the object.

Then you need to go to the menu, point to the selected axis and click the left mouse button. Then you just need to move the mouse. To accept the changes, right-click.
![D__3d_wc exe-2024-08-22-12-15-40](https://github.com/user-attachments/assets/50804f47-719c-407e-935a-2ac7166911ff)

A brief summary of all the paragraphs from the menu:
Spawn Cube - spawn a regular 1/1/1 cube (it has a submenu);
Spawn Pyramid - spawn a regular pyramid;
Spawn Light - creates a light source (to change the color of the light, select the light object and press V; You can increase/decrease the brightness of the light source using the I/O keys);
Spawn Player - creates the player's position of appearance;
Spawn Model - creates a model or animation:
![D__3d_wc exe-2024-08-22-12-26-24](https://github.com/user-attachments/assets/4535870c-4392-4e60-9233-9dd4d09f6f84)

Spawn Env Fire - creates an animated fire sprite;
Spawn Sprite - creates a static sprite (it has a submenu);
Spawn Env Particles - creates smoke;
Spawn Env Cubemap - creates an object that needs to be attached to another object in order for light to act on it:
![D__3d_wc exe-2024-08-22-12-35-39](https://github.com/user-attachments/assets/9c527b2b-add8-43bd-a6cb-b4e919fdca80)

Spawn Clip Wall - creates an invisible object that the player cannot pass through;
Spawn Trigger - creates a trigger (it has a submenu);
Spawn Area Portal - creates an object that reduces the drawing distance (used for optimization).
Spawn Env Shake - creates an object that can be used to create the illusion of an earthquake (used only with a trigger);
Spawn Sky Camera - creates a camera for 3D skybox;
Spawn Volume Skybox - creates an object that is used as a material for 3D skybox;
Spawn Env Fade - Creates an object that is used to create a screen fade (used only with a trigger):
![D__3d_3dFullGameClean exe-2024-08-22-12-48-00 (1)](https://github.com/user-attachments/assets/5b05a4b4-0120-470b-8163-30a506883520)

The main game tutorial:
To open the main game, run the file 3dFullGameClean.exe.
You can scroll the manu through by pressing the S key.
To launch the map, select New Game and enter the name of the map (the name must be entered with the map extension. Example: testMap2.bsp).
In the Customization section, you can select the player model:
![D__3d_3dFullGameClean exe-2024-08-22-12-54-05](https://github.com/user-attachments/assets/e7905011-7bb5-4af2-8284-19cb5f46c37f)

To move the player around the map, press the W A S D keys.
To jump, press Space.
