#ifndef UIBUSINESSCOMMDEFINE
#define UIBUSINESSCOMMDEFINE
#include <stdio.h>
#include <string>
#include <QString>
#include <QMap>

using namespace std;

#define LOGOUT				"logout"
#define PRODUCTION_MODELS	"production_models"
#define DEBUG_MODELS		"debug_models"
#define ENGINEER_MODELS		"engineer_models"
#define ROOT_MODELS			"root_models"

enum ST_LOGIN_MODEL
{
	MODE_LOGOUT=0,
	MODE_PRODUCTION,
	MODE_DEBUG,
	MODE_ENGINEER,
	MODE_ROOT
};

struct STWidgetItem
{
	QString	strObjectName;
	QString	strDesc;
	QString	strPluginName;
	bool bDisable[5];

	STWidgetItem()
	{
		strObjectName = "";
		strDesc = "";
		strPluginName = "";
		memset(bDisable, 0, sizeof(bDisable));
	}

};

struct STWidgetInfo
{
	STWidgetItem stWidgetItem;
	QMap<QString, STWidgetInfo*>	mapItem;

	STWidgetInfo()
	{
		mapItem.clear();
	}

};

#endif // UIBUSINESSCOMMDEFINE

