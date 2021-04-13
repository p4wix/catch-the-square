#include "Game.h"

//Private fun
void Game::initVariables() {
	this->window = nullptr;

   //Game logic init
   this->endGame = 0;
   this->points = 0;
   this->health = 10;
   this->enemySpanTimerMax = 20.f;
   this->enemySpawnTimer = this->enemySpanTimerMax;
   this->maxEnemies = 10;
   this->mouseHeld = 0;
}

void Game::initFonts() {
   if (this->font.loadFromFile("Fonts/Dosis-Light.ttf")) {
      std::cout << "ERROR::GAME::INITFONTS::Faild to load fonts!\n";
   }
}

void Game::initText() {
   this->uiText.setFont(this->font);
   this->uiText.setCharacterSize(26);
   this->uiText.setFillColor(sf::Color::White);
   this->uiText.setString("None");
}

void Game::initWindow() {
	this->videoMode.height = 600;
	this->videoMode.width = 800;

	this->window = new sf::RenderWindow(this->videoMode, "Game 1", sf::Style::Titlebar | sf::Style::Close);

   this->window->setFramerateLimit(60);
}

void Game::initEnemies() {
   this->enemy.setPosition(sf::Vector2f(10.f, 10.f));
   this->enemy.setSize(sf::Vector2f(100.f, 100.f));
   this->enemy.setFillColor(sf::Color::Cyan);
   /*this->enemy.setOutlineColor(sf::Color::Green);
   this->enemy.setOutlineThickness(2.f);*/
}

void Game::spawnEnemy() {
   /*
      @return void

      spawns enemies and sets their types and colors spawns them at random positons positions.
      -sets random types and colors
      -set a random postion
      -set a random color
      -adds enemy to the vector
      -removes eneamies
   */

   //Randomize enemy type
   int type = rand() % 5;

   switch (type) {
   case 0:
      this->enemy.setSize(sf::Vector2f(10.f, 10.f));
      this->enemy.setFillColor(sf::Color(193,84,193, 255));
      break;
   case 1:
      this->enemy.setSize(sf::Vector2f(30.f, 30.f));
      this->enemy.setFillColor(sf::Color::Blue);
      break;
   case 2:
      this->enemy.setSize(sf::Vector2f(50.f, 50.f));
      this->enemy.setFillColor(sf::Color::Cyan);
      break;
   case 3:
      this->enemy.setSize(sf::Vector2f(70.f, 70.f));
      this->enemy.setFillColor(sf::Color::Red);
      break;
   case 4:
      this->enemy.setSize(sf::Vector2f(100.f, 100.f));
      this->enemy.setFillColor(sf::Color::Green);
      break;

   default:
      this->enemy.setSize(sf::Vector2f(100.f, 100.f));
      this->enemy.setFillColor(sf::Color::Yellow);
      break;
   }

   // SPRÓBOWAÆ Z TYM this->window->getSize().x 
   this->enemy.setPosition(
      static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
      0.f
   );

   //spawn the enemy
   this->enemies.push_back(this->enemy);

}

//Constructios / Destructors
Game::Game() {
	this->initVariables();
   this->initFonts();
   this->initText();
	this->initWindow();
   this->initEnemies();
}

Game ::~Game() {
	delete this->window;
}

//Accesors
const bool Game::running() const {
	return this->window->isOpen();
}

const bool Game::getEndGame() const {
   return this->endGame;
}

//Functions
void Game::pollEvents() {
   //event polling
   while (this->window->pollEvent(this->event)) {
      switch (this->event.type) {
         case sf::Event::Closed: {
            this->window->close();
            break;
         }
         case sf::Event::KeyPressed: {
            if (this->event.key.code == sf::Keyboard::Escape) {
               this->window->close();
            }
            break;
         }
      }
   }
}

void Game::updateMousePositions() {
   /*
      @return void

      Updates the mouse postions:
      - mouse postions relative to window (vector2i)
   */
   this->mousePosWindow = sf::Mouse::getPosition(*this->window);
   this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateText() {
   std::stringstream ss;

   ss << "Points: " << this->points << "\n" << "Health: " << this->health << std::endl;
   

   this->uiText.setString(ss.str());
}

void Game::updateEnemies() {

   /*
      @return void

      Updates the enemy spawn timer and spawns enemies
      when the total amount of enemies is smaller then the maxumim.
      moves the enemies downwards.
      removes the enemies at the edage of the screen //TODO
   */

   //Updateing the timer for enemy spawning
   if (this->enemies.size() < this->maxEnemies) {
      if (this->enemySpawnTimer >= this->enemySpanTimerMax) {
         //spawn the enemy and reset the timer
         this->spawnEnemy();
         this->enemySpawnTimer = 0.f;
      }
      else {
         this->enemySpawnTimer += 1.f;
      }
   }

   //Moving and updateing enemies
   for (int i = 0; i < this->enemies.size(); ++i) {
      bool deleted = 0;
      this->enemies[i].move(0.f, 2.f);

      // if thhe eneamy is past the bottom of the screen
      if (this->enemies[i].getPosition().y > this->window->getSize().y) {
         this->enemies.erase(this->enemies.begin() + i);
         this->health -= 1;
         std::cout << "Health: " << this->health << std::endl;
      }
   }

   //check if clicked upon
   if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      if (this->mouseHeld == 0) {
         this->mouseHeld = 1;
         bool deleted = 0;
         for (size_t i = 0; i < this->enemies.size() and deleted == 0; ++i)
         {
            if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
               //Gain points
               if (this->enemies[i].getFillColor() == sf::Color(193, 84, 193, 255)) {
                  this->points += 10;
               }
               else if (this->enemies[i].getFillColor() == sf::Color::Blue) {
                  this->points += 7;
               }
               else if(this->enemies[i].getFillColor() == sf::Color::Cyan) {
                  this->points += 5;
               }
               else if (this->enemies[i].getFillColor() == sf::Color::Red) {
                  this->points += 3;
               }
               else if (this->enemies[i].getFillColor() == sf::Color::Green) {
                  this->points += 1;
               }
               std::cout << "Points: " << this->points << std::endl;

               //Delete the enemy
               deleted = 1;
               this->enemies.erase(this->enemies.begin() + i);
            }
         }
      }
   }
   else {
      this->mouseHeld = 0;
   }


}

void Game::update() {
   this->pollEvents();

   if (this->endGame == 0) {
      this->updateMousePositions();
      this->updateText();
      this->updateEnemies();
   }

   //End game conditon
   if (this->health <= 0) {
      this->endGame = 1;
   }
}

void Game::renderText(sf::RenderTarget& target) {
   target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget& target) {
   //Rendering all the enemies
   for (auto& i : this->enemies) {
      target.draw(i);
   }
}

void Game::render() {

   /*
    -clear old frame
    -render new obj
    -display frame in window
    -renders the game obj
   */

   this->window->clear();

   //Draw game obj
   this->renderEnemies(*this->window);

   this->renderText(*this->window);

   //this->window->draw(this->enemy);

   this->window->display();
}