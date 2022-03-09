INFO6044 Game Engine Frameworks - Project 2
Evan Sinasac - 1081418

CONTROLS
WASD		- Move Camera
QE		- Raise/Lower Camera
Mouse		- Aim Camera
1/2/3		- Activate different scripts (described below)
Pressing Enter will cause the "SpaceInvaders" code to start running, which may allow some control to show things like the triggers, but will also break the scripts run from 2 and 3, especially the 3 which will never be able to finish and prevent other scripts/commands from running.

This is a modified version of the Final Exam that has had a large chunk of it commented out and the Animation Controller and Commands and Command Groups added in.  Using it in either mode will screw up the other, so either avoid hitting Enter or be prepared to reset to see the other non-trigger scripts in action.

SCRIPTS/COMMAND GROUPS
LoadTheAnimationCommands - This was the base command group/script that I used to test each command as I made it.  The submitted version of it runs a move to serial command, followed by a rotate to serial command.  During the move to, a parallel rotate to command is executed.  A second command group is then added to the animation controller, this group has a single serial waypoint command.

PermanentCommandGroup - I used this script to test creating a permanent command.  It runs the follow object command, which is permanently running in the animation controller once 2 is pressed.

TriggerCommand - This script is for another permanent command, the location based trigger.

LocationTriggerCommand (yeah, I probably should've made clearer names for these) - This script is created and added when any ship gets within a certain distance of the point (line 689 in main).  Only one ship of each type will be affected at a time, and because the command groups are serial, the command will be stored until all preceding command groups have finished.  (Note to future self, figure out how to make parallel command groups without them being permanent).

WaypointCommand - Added during demo video, this script makes a single command group that causes the new alien to follow a waypoint path.

COMMANDS
Follow Object - Takes two ship pointers for which is the target and which is the follower, and then continues to follow the target forever.

Move To (Relative Time) - Takes the ship you want to move, and the location you want it to move to, and calculates the velocity it has to move at to reach it in the time entered.

Rotate To (Relative Time) - Same as Move To but with Euler Angles and radians, fun!

Follow Waypoints (Relative Time) - first vec4 is the start point and the time the ship has to travel along all of the waypoints.  Then it goes through every vec4 after and takes it as a vec3 in world space as a waypoint.  In Update, it checks if it's reached the waypoint and calculates it's new velocity to reach the next one.

Location Based Trigger - takes a ship and the location, and the acceptable distance from the location, and when the ship reaches the location, turns light 0 on.

DEMO VIDEOs
https://youtu.be/NXySGJ9WojY	(shorter)
https://youtu.be/ri2v6_3Bqf4