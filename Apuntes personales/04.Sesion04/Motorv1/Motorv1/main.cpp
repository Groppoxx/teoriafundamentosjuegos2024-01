#include <SDL/SDL.h>
#include "MainGame.h"
using namespace std;

int main(int argc, char** argv) {
	MainGame mainGame;
	mainGame.run();

	system("pause");
	return 0;
}