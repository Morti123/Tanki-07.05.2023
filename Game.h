#pragma once
#include "SFML/Graphics.hpp"
#include "settings.h"
#include <vector>
#include "Fonobj.h"
#include "Player1.h"
#include "Player2.h"
#include "HpBar_Player_1.h"
#include "HpBar_Player_2.h"
#include "Bullet_Player_1.h"
#include "Bullet_Player_2.h"

class Game {
private:
	sf::RenderWindow window;
	Fonobj fon;
	Player1 player1;
	Player2 player2;
	HpBar_1 hpBar1;
	HpBar_2 hpBar2;
public:
	Game() {
		window.create(sf::VideoMode{ (size_t)WINDOW_WIDTH, (size_t)WINDOW_HEIGHT }, WINDOW_TITLE);
		window.setFramerateLimit(FPS);
	}
	void checkEvents() {
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed) window.close();
	}
	void update() {
		fon.update();
		player1.update();
		player2.update();
		hpBar1.update(player1.getHp());
		hpBar2.update(player2.getHp());
	}
	void checkCollisions() {
		sf::FloatRect player_1_Bounds = player1.getHitBox();
		sf::FloatRect player_2_Bounds = player2.getHitBox();
		auto bullets_1_Sprites = player1.getBullets();
		for (auto& bullet_1 : *bullets_1_Sprites) {
			sf::FloatRect bullet_1_Bounds = bullet_1->getHitBox();
			if (bullet_1_Bounds.intersects(player_2_Bounds)) {
				player2.decreaseHp(bullet_1->getDamage());
				bullet_1->hit();
			}
		}
		auto bullets_2_Sprites = player2.getBullets();
		for (auto& bullet_2 : *bullets_2_Sprites) {
			sf::FloatRect bullet_2_Bounds = bullet_2->getHitBox();
			if (bullet_2_Bounds.intersects(player_1_Bounds)) {
				player1.decreaseHp(bullet_2->getDamage());
				bullet_2->hit();;
			}
		}
		(*bullets_1_Sprites).remove_if([](Bullet_1* bullet) {return bullet->getHit(); });
		(*bullets_2_Sprites).remove_if([](Bullet_2* bullet) {return bullet->getHit(); });
	}
	void draw() {
		window.clear();
		window.draw(fon.getSprite());
		hpBar1.draw(window);
		hpBar2.draw(window);
		player1.draw(window);
		player2.draw(window);
		window.display();
	}
	void play() {
		while (window.isOpen()) {
			checkEvents();
			update();
			checkCollisions();
			draw();
		}
	}
};