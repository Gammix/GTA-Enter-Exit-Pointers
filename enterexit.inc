/*
	GTA Enter-Exit Pointer (enterexit.inc)
	* Generate a enter exit teleport pointer in one line, the relevant moving pointer like in GTA SA singleplayer!

 	Author: (creator)
	* Gammix

 	Contributors:
	* Incognito - Streamer plugin, used dynamic areas

	(c) Copyright 2015
  	* This file is provided as is (no warranties).
*/
/*
FUNCTIONS:
native CreateEnterExit(Float:x, Float:y, Float:z, Float:enterx, Float:entery, Float:enterz, enterinterior, enterworld, modelid = 1559, type = 0, worldid = -1, interiorid = -1, playerid = -1, Float:streamdistance = 200.0, Float:drawdistance = 0.0);
native IsValidEnterExit(enterexitid);
native DestoryEnterExit(enterexitid);
native CountAllEnterExits();
native DestroyAllEnterExits();

CALLBACKS:
public OnPlayerEnterExit(playerid, enterexitid);
*/

#include <streamer>//thnx to Incognito

#define MAX_ENTER_EXITS 100//maximum enter exits your server can have, the limit is upto unlimited because of streamer plugin!
//Note: More the limit, more will be the loops, more time it will take!

#define SCREEN_FADE//if this is defined, there will be screen fade on picking every enterexit. Comment this if you don't want this effect

#define MOVEMENT_SPEED 1.0//the speed at which a pointer, or the arrow will move up and down.FixSet Default is 1.0, San Andreas's one is even slower than 1.0 units per second,
//its because the object(maybe its an object) is resized and made biger in length and made thin, so require less speed

#define HEIGHT 1.0//the height at which a pointer, or the arrow will reach, default is 1.0


enum EnterExitInfo
{
	E_object,
	E_pickup,
	bool:E_exist,
	Float:E_x,
	Float:E_y,
	Float:E_z,
	Float:E_enterx,
	Float:E_entery,
	Float:E_enterz,
	E_enterinterior,
	E_enterworld,
	bool:E_movedup,
	E_type
}
new gEnterExit[MAX_ENTER_EXITS][EnterExitInfo];

#if defined SCREEN_FADE
new PlayerText:gFader;
new gFadeLevel[MAX_PLAYERS];
new gFadeTimer[MAX_PLAYERS];
new gFadeType[MAX_PLAYERS];
new gPlayerPoint[MAX_PLAYERS];
new gFadeState[MAX_PLAYERS];
#endif

new gPlayerTimer[MAX_PLAYERS];

//Note: if you use this in a filterscript, please define "FILTERSCRIPT" in your script
#if defined FILTERSCRIPT// if used in a filterscript

	public OnFilterScriptInit()
	{
		DestroyAllEnterExits();//destory all enter exits

	    #if defined HOOk_OnFilterScriptInit
			HOOk_OnFilterScriptInit();
		#endif
		return 1;
	}
	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif
	#define OnFilterScriptInit HOOk_OnFilterScriptInit
	#if defined HOOk_OnFilterScriptInit
		forward HOOk_OnFilterScriptInit();
	#endif

	public OnFilterScriptExit()
	{
		DestroyAllEnterExits();//destory all enter exits

	    #if defined HOOk_OnFilterScriptExit
			HOOk_OnFilterScriptExit();
		#endif
		return 1;
	}
	#if defined _ALS_OnFilterScriptExit
		#undef OnFilterScriptExit
	#else
		#define _ALS_OnFilterScriptExit
	#endif
	#define OnFilterScriptExit HOOk_OnFilterScriptExit
	#if defined HOOk_OnFilterScriptExit
		forward HOOk_OnFilterScriptExit();
	#endif

#else// if used in a gamemode

	public OnGameModeInit()
	{
		DestroyAllEnterExits();//destory all enter exits

	    #if defined HOOk_OnGameModeInit
			HOOk_OnGameModeInit();
		#endif
		return 1;
	}
	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	#define OnGameModeInit HOOk_OnGameModeInit
	#if defined HOOk_OnGameModeInit
		forward HOOk_OnGameModeInit();
	#endif

	public OnGameModeExit()
	{
		DestroyAllEnterExits();//destory all enter exits

	    #if defined HOOk_OnGameModeExit
			HOOk_OnGameModeExit();
		#endif
		return 1;
	}
	#if defined _ALS_OnGameModeExit
		#undef OnGameModeExit
	#else
		#define _ALS_OnGameModeExit
	#endif
	#define OnGameModeExit HOOk_OnGameModeExit
	#if defined HOOk_OnGameModeExit
		forward HOOk_OnGameModeExit();
	#endif

#endif

//if color is '-1', the arrow or the pointer color will be default that SAMP offers(yellow)!
//NOTE: you can also use your own models, just edit the parameter 'modelid'. Its will work same as pointer and works perfect!
stock CreateEnterExit(Float:x, Float:y, Float:z, Float:enterx, Float:entery, Float:enterz, enterinterior, enterworld, modelid = 1559, type = 0, worldid = -1, interiorid = -1, playerid = -1, Float:streamdistance = 200.0, Float:drawdistance = 0.0)
{
	for(new enterexitid = 0; enterexitid < MAX_ENTER_EXITS; enterexitid++)
	{
    	if(! gEnterExit[enterexitid][E_exist])
		{
            gEnterExit[enterexitid][E_object] = CreateDynamicObject(modelid, x, y, z, 0.0, 0.0, 0.0, worldid, interiorid, playerid, streamdistance, drawdistance);
	    	gEnterExit[enterexitid][E_type] = type;
	    	switch(type)
			{
			    case 0: gEnterExit[enterexitid][E_pickup] = CreateDynamicPickup(1, 1, x, y, z, worldid, interiorid, playerid, streamdistance);
			    case 1: gEnterExit[enterexitid][E_pickup] = -1;
			    case 2: gEnterExit[enterexitid][E_pickup] = CreateDynamicPickup(1, 1, x, y, z, worldid, interiorid, playerid, streamdistance);
    		}
			gEnterExit[enterexitid][E_exist] = true;
	    	gEnterExit[enterexitid][E_x] = x;
	    	gEnterExit[enterexitid][E_y] = y;
	    	gEnterExit[enterexitid][E_z] = z;
	    	gEnterExit[enterexitid][E_enterx] = enterx;
	    	gEnterExit[enterexitid][E_entery] = entery;
	    	gEnterExit[enterexitid][E_enterz] = enterz;
	    	gEnterExit[enterexitid][E_enterinterior] = enterinterior;
	    	gEnterExit[enterexitid][E_enterworld] = enterworld;

			MoveDynamicObject(gEnterExit[enterexitid][E_object], x, y, z + HEIGHT, MOVEMENT_SPEED);
    		gEnterExit[enterexitid][E_movedup] = true;
			return enterexitid;
		}
	}
	return -1;
}

stock IsValidEnterExit(enterexitid)
{
	if(enterexitid >= MAX_ENTER_EXITS || enterexitid < 0)
	{
	    printf("Error::IsValidEnterExit()::Out Of Bounds::%i", enterexitid);
	    return false;//if invalid enterexit id
	}
	return gEnterExit[enterexitid][E_exist];
}

stock DestoryEnterExit(enterexitid)
{
	if(! IsValidEnterExit(enterexitid)) return false;

	DestroyDynamicObject(gEnterExit[enterexitid][E_object]);
	if(gEnterExit[enterexitid][E_type] == 0 || gEnterExit[enterexitid][E_type] ==2)
	{
		DestroyDynamicPickup(gEnterExit[enterexitid][E_pickup]);
	}
	gEnterExit[enterexitid][E_exist] = false;
	gEnterExit[enterexitid][E_x] = 0.0;
	gEnterExit[enterexitid][E_y] = 0.0;
	gEnterExit[enterexitid][E_z] = 0.0;
	gEnterExit[enterexitid][E_enterx] = 0.0;
	gEnterExit[enterexitid][E_entery] = 0.0;
	gEnterExit[enterexitid][E_enterz] = 0.0;
	gEnterExit[enterexitid][E_enterinterior] = -1;
	gEnterExit[enterexitid][E_enterworld] = -1;
	gEnterExit[enterexitid][E_type] = -1;
	return true;
}

stock CountAllEnterExits()
{
	new count = 0;
    for(new enterexit = 0; enterexit < MAX_ENTER_EXITS; enterexit++)
	{
	    if(IsValidEnterExit(enterexit))
	    {
	        count ++;
		}
	}
	return count;
}

stock DestroyAllEnterExits()
{
	for(new enterexit = 0; enterexit < MAX_ENTER_EXITS; enterexit++)
	{
	    DestoryEnterExit(enterexit);
	}
	return true;
}

public OnPlayerPickUpDynamicPickup(playerid, pickupid)
{
	for(new enterexitid; enterexitid < MAX_ENTER_EXITS; enterexitid++)
	{
    	if(gEnterExit[enterexitid][E_exist])
		{
		    if(pickupid == gEnterExit[enterexitid][E_pickup])
			{
			    if(CallLocalFunction("OnPlayerEnterExit", "ii", playerid, enterexitid))
		 		{
		 		    #if defined SCREEN_FADE//if fading is enabled

		 		    	FadePlayerScreen(playerid);
		 		    	TogglePlayerControllable(playerid, false);
    					gPlayerPoint[playerid] = enterexitid;
    					gFadeState[playerid] = 1;

					#else//if fading is disabled

						SetPlayerPos(playerid, gEnterExit[enterexitid][E_enterx], gEnterExit[enterexitid][E_entery], gEnterExit[enterexitid][E_enterz]);
						SetPlayerInterior(playerid, gEnterExit[enterexitid][E_enterinterior]);
						SetPlayerVirtualWorld(playerid, gEnterExit[enterexitid][E_enterworld]);

					#endif
					return 1;
				}
			}
		}
	}
 	#if defined HOOk_OnPlayerPickUpDynamicP
		HOOk_OnPlayerPickUpDynamicP(playerid, pickupid);
	#endif
	return 1;
}
#if defined _ALS_OnPlayerPickUpDynamicP
	#undef OnPlayerPickUpDynamicPickup
#else
	#define _ALS_OnPlayerPickUpDynamicP
#endif
#define OnPlayerPickUpDynamicPickup HOOk_OnPlayerPickUpDynamicP
#if defined HOOk_OnPlayerPickUpDynamicP
	forward HOOk_OnPlayerPickUpDynamicP(playerid, pickupid);
#endif

forward OnPlayerTimeUpdate(playerid);
public OnPlayerTimeUpdate(playerid)
{
    if(! IsPlayerInAnyVehicle(playerid)) return false;

	for(new enterexitid; enterexitid < MAX_ENTER_EXITS; enterexitid++)
	{
    	if(gEnterExit[enterexitid][E_exist])
		{
		    if(gEnterExit[enterexitid][E_type] == 1 || gEnterExit[enterexitid][E_type] == 2)
		    {
		        if(IsPlayerInRangeOfPoint(playerid, 1.5, gEnterExit[enterexitid][E_x], gEnterExit[enterexitid][E_y], gEnterExit[enterexitid][E_z]))
          		{
				    if(CallLocalFunction("OnPlayerEnterExit", "ii", playerid, enterexitid))
			 		{
						KillTimer(gFadeTimer[playerid]);

			 		    #if defined SCREEN_FADE//if fading is enabled

			 		    	FadePlayerScreen(playerid);
			 		    	TogglePlayerControllable(playerid, false);
	    					gPlayerPoint[playerid] = enterexitid;
	    					gFadeState[playerid] = 2;

						#else//if fading is disabled

							SetVehiclePos(GetPlayerVehicleID(playerid), gEnterExit[enterexitid][E_enterx], gEnterExit[enterexitid][E_entery], gEnterExit[enterexitid][E_enterz]);
							SetPlayerInterior(playerid, gEnterExit[enterexitid][E_enterinterior]);
							LinkVehicleToInterior(GetPlayerVehicleID(playerid), gEnterExit[enterexitid][E_enterinterior]);
							SetPlayerVirtualWorld(playerid, gEnterExit[enterexitid][E_enterworld]);
							SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), gEnterExit[enterexitid][E_enterworld]);

						#endif
		            	return 1;
					}
		        }
		    }
		}
	}
	return 1;
}

public OnPlayerConnect(playerid)
{
	#if defined SCREEN_FADE
	gFadeLevel[playerid] = -1;
	gFadeTimer[playerid] = -1;
	gFadeType[playerid] = -1;
	gPlayerPoint[playerid] = -1;
	gFadeState[playerid] = -1;
	#endif

	gPlayerTimer[playerid] = SetTimerEx("OnPlayerTimeUpdate", 1000, true, "i", playerid);

	#if defined HOOk_OnPlayerConnect
		HOOk_OnPlayerConnect(playerid);
	#endif
	return 1;
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect HOOk_OnPlayerConnect
#if defined HOOk_OnPlayerConnect
	forward HOOk_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
	#if defined SCREEN_FADE
	gFadeLevel[playerid] = -1;
	gFadeTimer[playerid] = -1;
	gFadeType[playerid] = -1;
	gPlayerPoint[playerid] = -1;
	gFadeState[playerid] = -1;
	#endif

	KillTimer(gPlayerTimer[playerid]);

	#if defined HOOk_OnPlayerDisconnect
		HOOk_OnPlayerDisconnect(playerid, reason);
	#endif
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect HOOk_OnPlayerDisconnect
#if defined HOOk_OnPlayerDisconnect
	forward HOOk_OnPlayerDisconnect(playerid, reason);
#endif

#if defined SCREEN_FADE
stock FadePlayerScreen(playerid)
{
	gFadeLevel[playerid] = 0;
	gFadeType[playerid] = 1;
	gFadeTimer[playerid] = SetTimerEx("OnPlayerFadeUpdate", 1, true, "i", playerid);
	return true;
}

forward OnPlayerFadeUpdate(playerid);
public OnPlayerFadeUpdate(playerid)
{
	if(gFadeType[playerid] == 1)
	{
    	gFadeLevel[playerid] += 2;

	    PlayerTextDrawDestroy(playerid, gFader);
	    gFader = CreatePlayerTextDraw(playerid,0.000000, 1.000000, "blankscreen");
		PlayerTextDrawBackgroundColor(playerid,gFader, 255);
		PlayerTextDrawLetterSize(playerid,gFader, 0.000000, 51.099998);
		PlayerTextDrawColor(playerid,gFader, -1);
		PlayerTextDrawUseBox(playerid,gFader, 1);
		PlayerTextDrawBoxColor(playerid,gFader, gFadeLevel[playerid]);
		PlayerTextDrawTextSize(playerid,gFader, 640.000000, 0.000000);
		PlayerTextDrawShow(playerid,gFader);

	    if(gFadeLevel[playerid] > 255)
	    {
	        gFadeType[playerid] = 0;
			gFadeLevel[playerid] = 255;
	    }
    }
    else if(gFadeType[playerid] == 0)
	{
	    if(gPlayerPoint[playerid] != -1)
	    {
	        new enterexitid = gPlayerPoint[playerid];
	    	if(gFadeState[playerid] == 1)
	        {
		        SetPlayerPos(playerid, gEnterExit[enterexitid][E_enterx], gEnterExit[enterexitid][E_entery], gEnterExit[enterexitid][E_enterz]);
				SetPlayerInterior(playerid, gEnterExit[enterexitid][E_enterinterior]);
				SetPlayerVirtualWorld(playerid, gEnterExit[enterexitid][E_enterworld]);
	    	}
	    	else
	    	{
	    	    SetVehiclePos(GetPlayerVehicleID(playerid), gEnterExit[enterexitid][E_enterx], gEnterExit[enterexitid][E_entery], gEnterExit[enterexitid][E_enterz]);
				SetPlayerInterior(playerid, gEnterExit[enterexitid][E_enterinterior]);
				LinkVehicleToInterior(GetPlayerVehicleID(playerid), gEnterExit[enterexitid][E_enterinterior]);
				SetPlayerVirtualWorld(playerid, gEnterExit[enterexitid][E_enterworld]);
				SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), gEnterExit[enterexitid][E_enterworld]);
	    	}
		    gPlayerPoint[playerid] = -1;
    	}

    	gFadeLevel[playerid] -= 2;

	    PlayerTextDrawDestroy(playerid, gFader);
	    gFader = CreatePlayerTextDraw(playerid,0.000000, 1.000000, "blankscreen");
		PlayerTextDrawBackgroundColor(playerid,gFader, 255);
		PlayerTextDrawLetterSize(playerid,gFader, 0.000000, 51.099998);
		PlayerTextDrawColor(playerid,gFader, -1);
		PlayerTextDrawUseBox(playerid,gFader, 1);
		PlayerTextDrawBoxColor(playerid,gFader, gFadeLevel[playerid]);
		PlayerTextDrawTextSize(playerid,gFader, 640.000000, 0.000000);
		PlayerTextDrawShow(playerid,gFader);

		if(gFadeLevel[playerid] < 0)
	    {
	        PlayerTextDrawDestroy(playerid, gFader);
			gFadeLevel[playerid] = -1;
			gFadeType[playerid] = -1;
			gFadeState[playerid] = -1;
		 	TogglePlayerControllable(playerid, true);
	        KillTimer(gFadeTimer[playerid]);
	    }
	}
    return 1;
}
#endif

public OnDynamicObjectMoved(objectid)
{
	for(new enterexitid; enterexitid < MAX_ENTER_EXITS; enterexitid++)
	{
    	if(gEnterExit[enterexitid][E_exist])
		{
		    if(objectid == gEnterExit[enterexitid][E_object])
		    {
		        if(gEnterExit[enterexitid][E_movedup])//moving down
		        {
		            gEnterExit[enterexitid][E_movedup] = false;
					return MoveDynamicObject(gEnterExit[enterexitid][E_object], gEnterExit[enterexitid][E_x], gEnterExit[enterexitid][E_y], gEnterExit[enterexitid][E_z], MOVEMENT_SPEED);
		        }
		        else if(! gEnterExit[enterexitid][E_movedup])//moving up
		        {
		            gEnterExit[enterexitid][E_movedup] = true;
					return MoveDynamicObject(gEnterExit[enterexitid][E_object], gEnterExit[enterexitid][E_x], gEnterExit[enterexitid][E_y], gEnterExit[enterexitid][E_z] + HEIGHT, MOVEMENT_SPEED);
		        }
		        else return 0;
		    }
		}
	}
	#if defined HOOk_OnPlayerClickPlayerTD
		HOOk_OnPlayerClickPlayerTD(playerid, PlayerText:playertextid);
	#endif
	return 1;
}
#if defined _ALS_OnPlayerClickPlayerTD
	#undef OnPlayerClickPlayerTextDraw
#else
	#define _ALS_OnPlayerClickPlayerTD
#endif
#define OnPlayerClickPlayerTextDraw HOOk_OnPlayerClickPlayerTD
#if defined HOOk_OnPlayerClickPlayerTD
	forward HOOk_OnPlayerClickPlayerTD(playerid, PlayerText:playertextid);
#endif
