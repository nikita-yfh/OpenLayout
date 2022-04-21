#include "File.h"
#include "ColorScheme.h" 

#define PATH_LENGTH 256

struct Settings {
	Settings();
	void SetDefault();
	void SetDefaultMacroPath();


	uint8_t undoCount;

	bool sameDir;
	char layExport[PATH_LENGTH];
	char gbrExport[PATH_LENGTH];
	char bmpExport[PATH_LENGTH];
	char hpglExport[PATH_LENGTH];
	char scannedCopies[PATH_LENGTH];

	char macroDir[PATH_LENGTH];

	uint8_t units;
	uint8_t drill;
	uint8_t gridStyle;

	ColorScheme colors[3]; //user color schemes
	uint8_t selectedColorScheme; //0-Standart; 1-User1; 2-User2; 3-User3;

	const ColorScheme &GetCurrentColors();

	uint16_t copperThickness;
	uint16_t tempEnhance;

	char toolKeys[TOOL_COUNT]; //key bindings for tools

	static const char *toolNames[TOOL_COUNT];

	bool measure45Lines;
	bool measureShow;
	bool measureBig;
	bool measureTp;
	bool measureLight;

	bool autosave;
	uint16_t autosaveTimer;

	vector<Vec2>padSizes;
	vector<Vec2>smdSizes;
	vector<float>trackSizes;

	vector<double>grids;
	double gridsBind[9]; //for keys 1-9


	uint8_t sub_grid;
	bool show_grid;

	float customRotationAngle;
	float rotationAngle;

	uint8_t circleQuality;

};
