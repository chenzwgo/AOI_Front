#ifndef SETTING_DEFINE_H
#define SETTING_DEFINE_H

// 控件类型
typedef enum _CONTROL_SETTING_TYPE
{
    SETTING_LINE_EDIT,		// line edit
    SETTING_CHECK_BOX,		// check box
    SETTING_COMBO_BOX,		// combo box
    SETTING_RADIO_BTN,		// radio button
    SETTING_SPIN_BOX,		// spin box
    SETTING_DSPIN_BOX,		// double spin box
    SETTING_WIDGET,			// widget
    SETTING_STACKED_WIDGET,	// stacked widget
    SETTING_FRAME,			// frame
    SETTING_GROUP_BOX,		// group box

}E_CONTROL_SETTING_TYPE;

typedef struct _ST_CONTROL_SETTING
{
    QString strOjectName;
    QString strClassName;
    QString strKey;
    QString strValue;
    QObject* pOject;
    E_CONTROL_SETTING_TYPE eType;

    _ST_CONTROL_SETTING()
    {
        strOjectName = "";
        strClassName = "";
        strKey = "";
        strValue = "";
        pOject = NULL;
        eType = SETTING_GROUP_BOX;
    }

    _ST_CONTROL_SETTING& operator=(const _ST_CONTROL_SETTING &other)
    {
        if (this == &other)
        {
            return *this;
        }
        strOjectName = other.strOjectName;
        strClassName = other.strClassName;
        strKey = other.strKey;
        strValue = other.strValue;
        eType = other.eType;
        pOject = other.pOject;
        return *this;
    }
}ControlSetting;

typedef struct _ST_SETTING_VALUE
{
    QString strKey;			// 键值
    QString strValue;			// 数据
    QString strDes;			// 描述
    QString strCtrlType;		// 控件类型
    QString strDisable;		// 是否显示
    _ST_SETTING_VALUE(QString key = "")
    {
        strKey = key;
        strValue = "";
        strDes = "";
        strDisable = "";
        strCtrlType = "";
    }

    _ST_SETTING_VALUE& operator=(const _ST_SETTING_VALUE &other)
    {
        if (this == &other)
        {
            return *this;
        }
        strKey = other.strKey;
        strValue = other.strValue;
        strDes = other.strDes;
        strDisable = other.strDisable;
        strCtrlType = other.strCtrlType;
        return *this;
    }
}SettingValue;

typedef struct _ST_SETTING_INFO
{
    QString strBeginGroup;
    QList<SettingValue> listSetting;

    _ST_SETTING_INFO(QString beginGroup)
    {
        strBeginGroup = beginGroup;
        listSetting.clear();
    }

    _ST_SETTING_INFO()
    {
        strBeginGroup = "";
    }

    _ST_SETTING_INFO& operator=(const _ST_SETTING_INFO& other)
    {
        if (this == &other)
        {
            return *this;
        }
        strBeginGroup = other.strBeginGroup;
        listSetting = other.listSetting;
        return *this;
    }
}SettingInfo;


typedef struct _ST_UI_VIEW_SETTTING
{
    QString strBeginGroup;
    QMap<QString, QList<QString>> mapChild;

    _ST_UI_VIEW_SETTTING(QString beginGroup)
    {
        strBeginGroup = beginGroup;
        mapChild.clear();
    }

    _ST_UI_VIEW_SETTTING()
    {
        strBeginGroup = "";
    }

    _ST_UI_VIEW_SETTTING& operator=(const _ST_UI_VIEW_SETTTING& other)
    {
        if (this == &other)
        {
            return *this;
        }
        strBeginGroup = other.strBeginGroup;
        mapChild = other.mapChild;
        return *this;
    }
}UIViewSetting;

#endif