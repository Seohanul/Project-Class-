

#include "InterfaceControl.h"

int main(void)
{
	UserInterface UI;
	cout << endl << endl << endl;
	cout << "  ==============================================" << endl;
	cout << "||\t\t 1. 게임 시작\t\t\t|| " << endl;
	cout << "||\t\t 2. 리플레이\t\t\t|| " << endl;
	int temp = 0;
	cin >> temp;
	switch (temp)
	{
	case 1 :
		system("cls");
		UI.GamePlay();
		UI.Save();
		break;
	case 2:
		system("cls");
		UI.Load();
		system("cls");
		UI.Replay();
		break;
	case 3:
		UI.Save();
		break;
	case 4:
		UI.Load();
		break;
	default:
		break;
	}
	
	

    return 0;
}

/* end of file */