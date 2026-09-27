// Decompiled by Opus. Names are provisional.
// Text for the reason a player could not join a multiplayer game.

// FUNCTION: 0x452c40
char* __stdcall FUN_00452c40(int reason)
{
    switch (reason) {
    case 4:
        return "You did not have the correct password";
    case 3:
        return "The game is closed";
    case 5:
        return "The game is full";
    case 6:
        return "You have lost connection with the game";
    case 7:
        return "You need a unit you don't have for this game";
    case 8:
        return "You need a newer version of the game to enter";
    case 9:
        return "No watching is allowed for this game";
    case 10:
        return "The creator has left the game";
    default:
        return "You were rejected from the game";
    }
}
