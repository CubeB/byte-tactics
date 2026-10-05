// Decompiled by space-bunny-free. Names are provisional.
// HAPINET_GetDPErrorString: maps a DirectPlay HRESULT (the DPERR_* values of
// dplay.h, 0x88770000 + the enum ordinal) to the message TA prints. DP_OK and
// the undocumented default are Cavedog's own additions. HapinetTrace is the
// trace stub, so the argument is re-read from the stack after the call.

extern void __cdecl HapinetTrace(const char*);

// FUNCTION: 0x4c9530
char* __stdcall HAPINET_GetDPErrorString(int error)
{
    HapinetTrace("HAPINET_GetDPErrorString\n");
    switch (error)
    {
    case 0x80004001:
        return "DPERR_UNSUPPORTED - The function is not available in this implementation. ";
    case 0x80004005:
        return "DPERR_GENERIC - An undefined error condition occurred. ";
    case 0x8007000e:
        return "DPERR_OUTOFMEMORY - There is insufficient memory to perform the requested operation. ";
    case 0x80070057:
        return "DPERR_INVALIDPARAMS - One or more of the parameters passed to the function are invalid.";
    case 0x88770005:
        return "DPERR_ALREADYINITIALIZED - This object is already initialized. ";
    case 0x8877000a:
        return "DPERR_ACCESSDENIED - The session is full or an incorrect password was supplied.";
    case 0x88770014:
        return "DPERR_ACTIVEPLAYERS - The requested operation cannot be performed because there are existing active players.";
    case 0x8877001e:
        return "DPERR_BUFFERTOOSMALL - The supplied buffer is not large enough to contain the requested data.";
    case 0x88770028:
        return "DPERR_CANTADDPLAYER - The player cannot be added to the session.";
    case 0x8877003c:
        return "DPERR_CANTCREATEPLAYER - A new player cannot be created. ";
    case 0x88770050:
        return "DPERR_CAPSNOTAVAILABLEYET - The capabilities of the DirectPlay object have not been determined yet.";
    case 0x8877005a:
        return "DPERR_EXCEPTION - An exception occurred when processing the request. ";
    case 0x88770078:
        return "DPERR_INVALIDFLAGS - The flags passed to this function are invalid. ";
    case 0x88770082:
        return "DPERR_INVALIDOBJECT - The DirectPlay object pointer is invalid. ";
    case 0x88770096:
        return "DPERR_INVALIDPLAYER - The player ID is not recognized as a valid player ID for this game session.";
    case 0x887700a0:
        return "DPERR_NOCAPS - The communication link underneath DirectPlay is not capable of this function.";
    case 0x887700aa:
        return "DPERR_NOCONNECTION - No communication link was established. ";
    case 0x887700be:
        return "DPERR_NOMESSAGES - There are no messages to be received. ";
    case 0x887700c8:
        return "DPERR_NONAMESERVERFOUND - No name server could be found or created. A name server must exist in order to create a player. ";
    case 0x887700d2:
        return "DPERR_NOPLAYERS - There are no active players in the session. ";
    case 0x887700dc:
        return "DPERR_NOSESSIONS - There are no existing sessions for this game. ";
    case 0x887700e6:
        return "DPERR_SENDTOOBIG - The message buffer passed to the IDirectPlay::Send method is larger than allowed. ";
    case 0x887700f0:
        return "DPERR_TIMEOUT - The operation could not be completed in the specified time. ";
    case 0x887700fa:
        return "DPERR_UNAVAILABLE - The requested service provider or session is not available. ";
    case 0x8877010e:
        return "DPERR_BUSY - The DirectPlay message queue is full. ";
    case 0x88770118:
        return "DPERR_USERCANCEL - The user canceled the connection process during a call to the IDirectPlay::Open method.";
    case 0x88770136:
        return "DPERR_SESSIONLOST - The session was lost. ";
    case 0x88770140:
        return "DPERR_UNINITIALIZED - An object was not initialized. ";
    case 0x00000000:
        return "DP_OK - The request completed successfully.";
    }
    return "DPERR_IHAVENOBLOODYIDEA - DirectPlay returned an undocumented return value.";
}
