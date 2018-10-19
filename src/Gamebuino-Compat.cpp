/*
This file is part of the Gamebuino-Meta library,
Copyright (c) Aadalie 2017

This is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License (LGPL)
as published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

This is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License (LGPL) for more details.

You should have received a copy of the GNU Lesser General Public
License (LGPL) along with the library.
If not, see <http://www.gnu.org/licenses/>.

Authors:
 - Sorunome
*/

#include "Gamebuino-Compat.h"

namespace Gamebuino_Compat {

const uint8_t gamebuinoLogo[] = {
	84,10, //width and height
	B00000011, B11100001, B10000001, B10000110, B01111111, B00111110, B00011000, B01101101, B10000011, B00001111, B00001111, 
	B00001110, B00000001, B10000011, B10000110, B01100000, B00110011, B00011000, B01101101, B11000011, B00011001, B10001111, 
	B00011000, B00000011, B11000011, B10001110, B01100000, B00110011, B00011000, B01101101, B11100011, B00110000, B11001111, 
	B00011000, B00000011, B11000011, B10011110, B01100000, B00110110, B00110000, B11001101, B11100011, B01100000, B11001111, 
	B00110000, B00000110, B11000111, B10011110, B01111110, B00111110, B00110000, B11001101, B10110011, B01100000, B11001111, 
	B00110000, B00001100, B11000110, B11110110, B01100000, B00110011, B00110000, B11011001, B10110110, B01100000, B11001111, 
	B00110011, B11001111, B11001100, B11110110, B01100000, B01100001, B10110000, B11011011, B00011110, B01100000, B11001111, 
	B00110000, B11011000, B01101100, B11100110, B11000000, B01100001, B10110000, B11011011, B00011110, B01100001, B10001111, 
	B00011001, B10011000, B01101100, B11000110, B11000000, B01100011, B10110001, B10011011, B00001110, B00110011, B00001111, 
	B00001111, B10110000, B01111000, B11000110, B11111111, B01111110, B00011111, B00011011, B00000110, B00011110, B00001111, 
};

void Gamebuino::begin() {
	Gamebuino_Meta::Gamebuino::begin();
	EEPROM.begin(1024);
	sound.begin();
}

void Gamebuino::getDefaultName(char* string) {
	char name[13];
	Gamebuino_Meta::Gamebuino::getDefaultName(name);
	memcpy(string, name, 10);
	string[10] = '\0';
}

bool Gamebuino::update() {
	bool u = Gamebuino_Meta::Gamebuino::update();
	if (u) {
		if (!display.persistence) {
			display.clear();
		}
		display.setColor(Color::black);
		return true;
	}
	return false;
}

void Gamebuino::titleScreen(const __FlashStringHelper* name, const uint8_t *logo) {
	display.fontSize = 1;
	display.persistence = false;
	
	
	Image buttonsIcons = Image(Gamebuino_Meta::buttonsIconsData);
	
	while(1) {
		while (!update());
		
		display.clear();
		display.setColor(Color::black);
		display.drawBitmap(-2, 0, gamebuinoLogo);
		if (logo){
			uint8_t lw = logo[0];
			uint8_t lh = logo[1];
			display.drawBitmap((display.width() - lw) / 2, (display.height() - lh) / 2, logo);
		}
		display.setCursor(0, 12);
		display.print(name);
		
		//blinking border
		if ((frameCount % 8) >= 4) {
			display.setColor(BROWN);
			display.drawRect(0, 0, display.width(), display.height());
		}
		
		//blinking A button icon
		if((frameCount%8) >= 4){
			buttonsIcons.setFrame(1); //button A pressed
		} else {
			buttonsIcons.setFrame(0); //button A released
		}
		uint8_t scale = display.width() == 80 ? 1 : 2;
		uint8_t w = buttonsIcons.width() * scale;
		uint8_t h = buttonsIcons.height() * scale;
		uint8_t x = display.width() - w;
		uint8_t y = display.height() - h;
		display.drawImage(x, y, buttonsIcons, w, h);
		
		if (buttons.pressed(Button::a)) {
			sound.playOK();
			break;
		}
	}
}

void Gamebuino::titleScreen(const __FlashStringHelper* name){
	titleScreen(name, 0);
}

void Gamebuino::titleScreen(const uint8_t* logo){
	titleScreen(F(""), logo);
}

void Gamebuino::titleScreen(){
	titleScreen(F(""));
}

void Gamebuino::keyboard(char* text, uint8_t length) {
	gui.keyboard("Please Enter:", text, length);
}

int8_t Gamebuino::menu(const char* const* items, uint8_t length) {
	return gui.menu("Please Select:", (const char**)items, length);
}

void Gamebuino::popup(const char* text, uint8_t duration) {
	gui.popup(text, duration);
}

}; // namespace Gamebuino_Compat

Gamebuino gb;
