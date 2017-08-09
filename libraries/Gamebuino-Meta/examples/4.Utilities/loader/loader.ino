#include <Gamebuino-Meta.h>

#define MAX_FOLDER_NAME_LENGTH 40
#define GRID_WIDTH 4
#define GRID_HEIGHT 2
#define PAGE_SIZE (GRID_WIDTH * GRID_HEIGHT)
#define PAGES_PER_BLOCK 8
#define BLOCK_LENGTH (PAGE_SIZE * PAGES_PER_BLOCK)
char gameFolders[BLOCK_LENGTH][MAX_FOLDER_NAME_LENGTH];
uint32_t totalGames;
uint8_t filesInBlock;
uint8_t pageInBlock = 0;
bool lastBlock = false;
char folderName[512];
char nameBuffer[512];
uint32_t currentGame = 0;
uint32_t gameFolderBlock = 0;


const uint16_t startupSound[] = {0x0005,0x3089,0x208,0x238,0x7849,0x1468,0x0000};

void clearEmptyFolders() {
	File dir_walk = SD.open("/");
	File entry;
	while (entry = dir_walk.openNextFile()) {
		if (!entry.isDirectory()) {
			continue;
		}
		folderName[0] = '/';
		entry.getName(folderName+1, 512-1);
		if (!strstr(folderName, "loader")) {
			SD.rmdir(folderName); // this already checks if the folder is empty
		}
	}
	entry.close();
	dir_walk.close();
}

bool getBinPath(char* name) {
	File dir_walk = SD.open(folderName);
	File entry;
	strcpy(name, folderName);
	strcpy(name + strlen(folderName), folderName);
	strcpy(name + strlen(name), ".bin");
	if (SD.exists(name)) {
		return true;
	}
	strcpy(name, folderName);
	strcpy(name + strlen(name), "/");
	uint16_t i = strlen(name);
	while (entry = dir_walk.openNextFile()) {
		if (!entry.isFile()) {
			continue;
		}
		entry.getName(name + i, 512 - i);
		if (strstr(name, ".bin") || strstr(name, ".BIN")) {
			return true;
		}
	}
	return false;
}

void loadNumberOfGames() {
	File root = SD.open("/");
	File entry;
	totalGames = 0;
	while (entry = root.openNextFile()) {
		if (!entry.isDirectory()) {
			continue;
		}
		folderName[0] = '/';
		entry.getName(folderName + 1, MAX_FOLDER_NAME_LENGTH - 1);
		if (!getBinPath(nameBuffer)) {
			continue;
		}
		totalGames++;
	}
}

void loadGameFolderBlock() {
	File root = SD.open("/");
	File entry;
	filesInBlock = 0;
	uint32_t i = 0;
	uint32_t j = 0;
	bool searchForLast = false;
	lastBlock = true;
	while (entry = root.openNextFile()) {
		if (!entry.isDirectory()) {
			continue;
		}
		gameFolders[filesInBlock][0] = '/';
		entry.getName(gameFolders[filesInBlock] + 1, MAX_FOLDER_NAME_LENGTH - 1);
		strcpy(folderName, gameFolders[filesInBlock]);
		
		if (!getBinPath(nameBuffer)) {
			continue;
		}
		if (searchForLast) {
			lastBlock = false;
			break;
		}
		if (i < gameFolderBlock) {
			if (j < BLOCK_LENGTH) {
				j++;
				continue;
			}
			i++;
			j = 0;
			if (i < gameFolderBlock) {
				continue;
			}
		}
		filesInBlock++;
		if (filesInBlock >= BLOCK_LENGTH) {
			searchForLast = true;
		}
	}
}


void setup() {
	gb.begin();
//	SerialUSB.begin(115200)
	gb.sound.play(startupSound);
	gb.display.setCursors(0, 0);
	gb.display.println("Loading...");
	gb.updateDisplay();
	clearEmptyFolders();
	loadNumberOfGames();
	loadGameFolderBlock();
	
	
	
	gridView();
}

void loop() {
	
}
