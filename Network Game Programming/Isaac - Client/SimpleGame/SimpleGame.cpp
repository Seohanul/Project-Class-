#include "stdafx.h"
#include "Framework.h"
#include "Scene.h"
#include "Logo.h"
#include "Gameplay.h"
#include "Network.h"
#include "World.h"


int main(int argc, char **argv)
{
	// TOFIX , Login, Loby Scenen here
	// Initialize network socket, make thread for recv func 

	World::SetScale(0.025f, 1);
	//n.create();
//	HANDLE hThread = CreateThread(NULL, 0, n.ProcessClient, (LPVOID)n.sock, 0, NULL);

	///* Game Loop */
	Fw.Initialize("The Binding of Isaac", 800, 800, argc, argv);
	Fw.ToScene(new Gameplay);
	Fw.Run();
	Fw.Close();

    return FALSE;
}

