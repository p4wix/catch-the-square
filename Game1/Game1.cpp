#include "Game.h"

int main() {
   //init srand
   srand(static_cast<unsigned>(time(NULL)));

   //init game engine
   Game game;
   
   //Game loop
   while (game.running() and !game.getEndGame()) {
      //Update
      game.update();

      //Render
      game.render();
   }

   //End of app
   return 1;
}