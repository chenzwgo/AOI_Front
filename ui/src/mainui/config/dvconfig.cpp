#include "filevariable/ifilevariable.h"
#include "g12common.h"
#include "stringcommon.h"
#include "rtdefine.h"
#include "common_data.h"

#include "dvconfig.h"
#include "ui_dvconfig.h"
#include "dvfilterconfig.h"
#include "workflow/InvokeArithmetic.h"

#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include "dlArithmetic/idlArithmetic.h"
#define DEFECT_MODEL_ITEM_NUMB    16

void dvconfig::initDvConfigUI()
{
	/*
    IFileVariable *pFile = getDVCfgFileVariable();
    if (pFile == NULL)
    {
        QMessageBox::information(this, "配置读取", QString::fromLocal8Bit("配置文件读取失败"));
    }
    string dv_config_path = pFile->getValue(DV_CONFIG_FILE, "./hconfig/config.json").toCString();
    char *json = NULL;
    QFile fileQ;
    bool bRet = false;
    int    iConfigValue = 0;
    double dConfigValue = 0;
    do
    {
        fileQ.setFileName(dv_config_path.c_str());
        if (!fileQ.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(this, "配置读取", QString::fromLocal8Bit("配置文件读取失败(%1)").arg(dv_config_path.c_str()));
            break;
        }
        //读取原有配置文件
        int iSize = fileQ.size();
        json = new char[iSize + 1];
        memset(json, 0, iSize + 1);
        if (iSize != fileQ.read(json, iSize))
        {
            QMessageBox::information(this, "配置读取", QString::fromLocal8Bit("配置文件读取失败(%1)").arg(dv_config_path.c_str()));
            break;
        }
        fileQ.close();

        //解析Json文件
        Document hConfigDoc;
        string strJson = SString::UTF8ToGBK(json);
        hConfigDoc.Parse(strJson.c_str());
        if (hConfigDoc.HasParseError())
        {
            QMessageBox::information(this, "配置读取", QString::fromLocal8Bit("配置文件解析失败,错误码%1, 位置：%2").arg(hConfigDoc.GetParseError()).arg(hConfigDoc.GetErrorOffset()));
            break;
        }

        Value::ConstMemberIterator itIndexItem = hConfigDoc.FindMember("glass_index");
        int index = 0;
        if (itIndexItem != hConfigDoc.MemberEnd() && itIndexItem->value.IsInt())
        {
            index = itIndexItem->value.GetInt();
        }

        Value::ConstMemberIterator itPruductItem;
        Value::ConstMemberIterator itParamItem;
        if (index == 0)
        {
            itPruductItem  = hConfigDoc.FindMember("white");
        }
        else
        {
            itPruductItem = hConfigDoc.FindMember("Silkprint");
        }

        if (itPruductItem != hConfigDoc.MemberEnd() && itPruductItem->value.IsObject())
        {
            itParamItem = itPruductItem->value.FindMember("iDelaytime");
            if (itParamItem != itPruductItem->value.MemberEnd() && itParamItem->value.IsInt())
            {
                iConfigValue = itParamItem->value.GetInt();
                ui->spinBox_timeout->setValue(iConfigValue);
            }
            itParamItem = itPruductItem->value.FindMember("bStart_timer");
            if (itParamItem != itPruductItem->value.MemberEnd() && itParamItem->value.IsInt())
            {
                ui->groupBox_timeout->setChecked(itParamItem->value.GetInt());
            }
            //heat_thresh
            itParamItem = itPruductItem->value.FindMember("edge_heat_thresh");
            if (itParamItem != itPruductItem->value.MemberEnd() && itParamItem->value.IsDouble())
            {
                dConfigValue = itParamItem->value.GetDouble();
                ui->doubleSpinBox_heat_thresh_edge->setValue(dConfigValue);
            }
            itParamItem = itPruductItem->value.FindMember("window_heat_thresh");
            if (itParamItem != itPruductItem->value.MemberEnd() && itParamItem->value.IsDouble())
            {
                dConfigValue = itParamItem->value.GetDouble();
                ui->doubleSpinBox_heat_thresh_window->setValue(dConfigValue);
            }

            //灰度值取值方式设置
            Value::ConstMemberIterator itParamTmpItem;
            itParamTmpItem = itPruductItem->value.FindMember("gray_val_control");
            if (itParamTmpItem != itPruductItem->value.MemberEnd() && itParamTmpItem->value.IsObject())
            {
                itParamItem = itParamTmpItem->value.FindMember("A");
                if (itParamItem != itParamTmpItem->value.MemberEnd() && itParamItem->value.IsInt())
                {
                    iConfigValue = itParamItem->value.GetInt();
                    ui->spinBox_gray_A->setValue(iConfigValue);
                }
                itParamItem = itParamTmpItem->value.FindMember("B");
                if (itParamItem != itParamTmpItem->value.MemberEnd() && itParamItem->value.IsInt())
                {
                    iConfigValue = itParamItem->value.GetInt();
                    ui->spinBox_gray_B->setValue(iConfigValue);
                }
                itParamItem = itParamTmpItem->value.FindMember("C");
                if (itParamItem != itParamTmpItem->value.MemberEnd() && itParamItem->value.IsInt())
                {
                    iConfigValue = itParamItem->value.GetInt();
                    ui->spinBox_gray_C->setValue(iConfigValue);
                }
                itParamItem = itParamTmpItem->value.FindMember("D");
                if (itParamItem != itParamTmpItem->value.MemberEnd() && itParamItem->value.IsInt())
                {
                    iConfigValue = itParamItem->value.GetInt();
                    ui->spinBox_gray_D->setValue(iConfigValue);
                }
            }

            itParamTmpItem = itPruductItem->value.FindMember("wlj_param");
            if (itParamTmpItem != itPruductItem->value.MemberEnd() && itParamTmpItem->value.IsObject())
            {
                Value::ConstMemberIterator itEdgeTmpItem;
                itEdgeTmpItem = itParamTmpItem->value.FindMember("edge");
                if (itEdgeTmpItem != itParamTmpItem->value.MemberEnd() && itEdgeTmpItem->value.IsObject())
                {
                    itParamItem = itEdgeTmpItem->value.FindMember("top");
                    if (itParamItem != itEdgeTmpItem->value.MemberEnd() && itParamItem->value.IsInt())
                    {
                        int iDomainOffset = itParamItem->value.GetInt();
                        ui->spinBox_edge_top->setValue(iDomainOffset);
                    }
                    itParamItem = itEdgeTmpItem->value.FindMember("right");
                    if (itParamItem != itEdgeTmpItem->value.MemberEnd() && itParamItem->value.IsInt())
                    {
                        int iDomainOffset = itParamItem->value.GetInt();
                        ui->spinBox_edge_right->setValue(iDomainOffset);
                    }
                    itParamItem = itEdgeTmpItem->value.FindMember("bottom");
                    if (itParamItem != itEdgeTmpItem->value.MemberEnd() && itParamItem->value.IsInt())
                    {
                        int iDomainOffset = itParamItem->value.GetInt();
                        ui->spinBox_edge_bottom->setValue(iDomainOffset);
                    }
                    itParamItem = itEdgeTmpItem->value.FindMember("left");
                    if (itParamItem != itEdgeTmpItem->value.MemberEnd() && itParamItem->value.IsInt())
                    {
                        int iDomainOffset = itParamItem->value.GetInt();
                        ui->spinBox_edge_left->setValue(iDomainOffset);
                    }
                }

                itParamItem = itParamTmpItem->value.FindMember("shiftX");
                if (itParamItem != itParamTmpItem->value.MemberEnd() && itParamItem->value.IsInt())
                {
                    iConfigValue = itParamItem->value.GetInt();
                    ui->spinBox_shiftX->setValue(iConfigValue);
                }
                itParamItem = itParamTmpItem->value.FindMember("shiftY");
                if (itParamItem != itParamTmpItem->value.MemberEnd() && itParamItem->value.IsInt())
                {
                    iConfigValue = itParamItem->value.GetInt();
                    ui->spinBox_shiftY->setValue(iConfigValue);
                }
                itParamItem = itParamTmpItem->value.FindMember("x_rate_zoom");
                if (itParamItem != itParamTmpItem->value.MemberEnd() && itParamItem->value.IsDouble())
                {
                    dConfigValue = itParamItem->value.GetDouble();
                    ui->doubleSpinBox_x_rate_zoom->setValue(dConfigValue);
                }
                itParamItem = itParamTmpItem->value.FindMember("y_rate_zoom");
                if (itParamItem != itParamTmpItem->value.MemberEnd() && itParamItem->value.IsDouble())
                {
                    dConfigValue = itParamItem->value.GetDouble();
                    ui->doubleSpinBox_y_rate_zoom->setValue(dConfigValue);
                }
                itParamItem = itParamTmpItem->value.FindMember("right_distortion");
                if (itParamItem != itParamTmpItem->value.MemberEnd() && itParamItem->value.IsInt())
                {
                    iConfigValue = itParamItem->value.GetInt();
                    ui->spinBox_right_distortion->setValue(iConfigValue);
                }
                itParamItem = itParamTmpItem->value.FindMember("left_distortion");
                if (itParamItem != itParamTmpItem->value.MemberEnd() && itParamItem->value.IsInt())
                {
                    iConfigValue = itParamItem->value.GetInt();
                    ui->spinBox_left_distortion->setValue(iConfigValue);
                }
            }

            itParamItem = itPruductItem->value.FindMember("contrast_val_control");
            if (itParamItem != itPruductItem->value.MemberEnd() && itParamItem->value.IsInt())
            {
                ui->spinBox_contrast_val_control->setValue(itParamItem->value.GetInt());
            }

            itParamTmpItem = itPruductItem->value.FindMember("detect_model_list");
            if (itParamTmpItem != itPruductItem->value.MemberEnd() && itParamTmpItem->value.IsObject())
            {
                Value::ConstMemberIterator itAreaTmpItem;
                itAreaTmpItem = itParamTmpItem->value.FindMember("silkArea");
                if (itAreaTmpItem != itParamTmpItem->value.MemberEnd() && itAreaTmpItem->value.IsObject())
                {
                    itParamItem = itAreaTmpItem->value.FindMember("symbol_file");
                    if (itParamItem != itAreaTmpItem->value.MemberEnd() && itParamItem->value.IsString())
                    {
                        ui->lineEdit_Edge_Defect_Symbol->setText(QString::fromLocal8Bit(itParamItem->value.GetString()));
                    }
                    itParamItem = itAreaTmpItem->value.FindMember("params_file");
                    if (itParamItem != itAreaTmpItem->value.MemberEnd() && itParamItem->value.IsString())
                    {
                        ui->lineEdit_Edge_Defect_Param->setText(QString::fromLocal8Bit(itParamItem->value.GetString()));
                    }
                }
                itAreaTmpItem = itParamTmpItem->value.FindMember("windowArea");
                if (itAreaTmpItem != itParamTmpItem->value.MemberEnd() && itAreaTmpItem->value.IsObject())
                {
                    itParamItem = itAreaTmpItem->value.FindMember("symbol_file");
                    if (itParamItem != itAreaTmpItem->value.MemberEnd() && itParamItem->value.IsString())
                    {
                        ui->lineEdit_Window_Defect_Symbol->setText(QString::fromLocal8Bit(itParamItem->value.GetString()));
                    }
                    itParamItem = itAreaTmpItem->value.FindMember("params_file");
                    if (itParamItem != itAreaTmpItem->value.MemberEnd() && itParamItem->value.IsString())
                    {
                        ui->lineEdit_Window_Defect_Param->setText(QString::fromLocal8Bit(itParamItem->value.GetString()));
                    }
                }
            }
            itParamTmpItem = itPruductItem->value.FindMember("classify_model_list");
            if (itParamTmpItem != itPruductItem->value.MemberEnd() && itParamTmpItem->value.IsObject())
            {
                Value::ConstMemberIterator itAreaTmpItem;
                itAreaTmpItem = itParamTmpItem->value.FindMember("silkArea");
                if (itAreaTmpItem != itParamTmpItem->value.MemberEnd() && itAreaTmpItem->value.IsObject())
                {
                    itParamItem = itAreaTmpItem->value.FindMember("symbol_file");
                    if (itParamItem != itAreaTmpItem->value.MemberEnd() && itParamItem->value.IsString())
                    {
                        ui->lineEdit_Edge_Classify_Symbol->setText(QString::fromLocal8Bit(itParamItem->value.GetString()));
                    }
                    itParamItem = itAreaTmpItem->value.FindMember("params_file");
                    if (itParamItem != itAreaTmpItem->value.MemberEnd() && itParamItem->value.IsString())
                    {
                        ui->lineEdit_Edge_Classify_Param->setText(QString::fromLocal8Bit(itParamItem->value.GetString()));
                    }
                }
                itAreaTmpItem = itParamTmpItem->value.FindMember("windowArea");
                if (itAreaTmpItem != itParamTmpItem->value.MemberEnd() && itAreaTmpItem->value.IsObject())
                {
                    itParamItem = itAreaTmpItem->value.FindMember("symbol_file");
                    if (itParamItem != itAreaTmpItem->value.MemberEnd() && itParamItem->value.IsString())
                    {
                        ui->lineEdit_Window_Classify_Symbol->setText(QString::fromLocal8Bit(itParamItem->value.GetString()));
                    }
                    itParamItem = itAreaTmpItem->value.FindMember("params_file");
                    if (itParamItem != itAreaTmpItem->value.MemberEnd() && itParamItem->value.IsString())
                    {
                        ui->lineEdit_Window_Classify_Param->setText(QString::fromLocal8Bit(itParamItem->value.GetString()));
                    }
                }
                itAreaTmpItem = itParamTmpItem->value.FindMember("twoClassification");
                if (itAreaTmpItem != itParamTmpItem->value.MemberEnd() && itAreaTmpItem->value.IsObject())
                {
                    itParamItem = itAreaTmpItem->value.FindMember("symbol_file");
                    if (itParamItem != itAreaTmpItem->value.MemberEnd() && itParamItem->value.IsString())
                    {
                        ui->lineEdit_Window_Classify_Symbol_Sec->setText(QString::fromLocal8Bit(itParamItem->value.GetString()));
                    }
                    itParamItem = itAreaTmpItem->value.FindMember("params_file");
                    if (itParamItem != itAreaTmpItem->value.MemberEnd() && itParamItem->value.IsString())
                    {
                        ui->lineEdit_Window_Classify_Param_Sec->setText(QString::fromLocal8Bit(itParamItem->value.GetString()));
                    }
                }
            }

            itParamTmpItem = itPruductItem->value.FindMember("default_control");
            if (itParamTmpItem != itPruductItem->value.MemberEnd() && itParamTmpItem->value.IsArray())
            {
                const Value& itemControlObj = itParamTmpItem->value;
                Value itemFilterObj;

                int index = 0;
                ui->tabWidget_edge->clear();
                ui->tabWidget_window->clear();
                QTabWidget *pTabWidget = NULL;
                for (int iS = 0; iS < itemControlObj.Size(); ++iS)
                {
                    if (0 == iS % 2)
                    {
                        pTabWidget = ui->tabWidget_edge;
                    }
                    else
                    {
                        pTabWidget = ui->tabWidget_window;
                    }
                    const Value& itemSeqObj = itemControlObj[iS];
                    if (itemSeqObj.IsArray())
                    {
                        stDefectItemInfo info[MAX_DEFECT_CLASS_NUMBS];
                        for (int iDefect = 0; iDefect < itemSeqObj.Size(); ++iDefect)
                        {
                            const Value& itemDefectObj = itemControlObj[iS][iDefect];
                            if (itemDefectObj.IsObject())
                            {
                                itParamItem = itemDefectObj.FindMember("name");
                                if (itParamItem != itemDefectObj.MemberEnd() && itParamItem->value.IsString())
                                {
                                    info[iDefect].name = itParamItem->value.GetString();
                                }
                                itParamItem = itemDefectObj.FindMember("length");
                                if (itParamItem != itemDefectObj.MemberEnd() && itParamItem->value.IsInt())
                                {
                                    info[iDefect].length = itParamItem->value.GetInt();
                                }
                                itParamItem = itemDefectObj.FindMember("level");
                                if (itParamItem != itemDefectObj.MemberEnd() && itParamItem->value.IsInt())
                                {
                                    info[iDefect].level = itParamItem->value.GetInt();
                                }
                                itParamItem = itemDefectObj.FindMember("enable");
                                if (itParamItem != itemDefectObj.MemberEnd() && itParamItem->value.IsBool())
                                {
                                    info[iDefect].bEnable = itParamItem->value.GetBool();
                                }
                            }
                        }

                        dvFilterConfig *pItem = new dvFilterConfig();
                        char strType = 'A' + iS / 2;
                        pTabWidget->addTab(pItem, QString("    %1    ").arg(strType));
                        pItem->initUI(info, itemSeqObj.Size());
                    }
                }
            }
        }
    } while (0);
    if (fileQ.isOpen())
    {
        fileQ.close();
    }
    if (NULL != json)
    {
        delete[]json;
        json = NULL;
    }
	*/
}

void dvconfig::saveDvConfig()
{
	/*
    IFileVariable *pFile = getDVCfgFileVariable();
    if (pFile == NULL)
    {
        QMessageBox::information(this, "配置读取", QString::fromLocal8Bit("配置文件读取失败"));
    }
    string dv_config_path = pFile->getValue(DV_CONFIG_FILE, "./hconfig/config.json").toCString();
    int iRet = 0;
    char *json = NULL;
    QFile fileQ;
    int iUIValue = 0;
    double dUIValue = 0;
    const char *pConfigPath[DEFECT_MODEL_ITEM_NUMB] = {0};
    QString strItem[DEFECT_MODEL_ITEM_NUMB];
    QByteArray byteData[DEFECT_MODEL_ITEM_NUMB];
    int iLength = 0;
    int index = 0;
    do
    {
        fileQ.setFileName(dv_config_path.c_str());
        if (!fileQ.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(this, "配置读取", QString::fromLocal8Bit("配置文件读取失败(%1)").arg(dv_config_path.c_str()));
            iRet = -1;
            break;
        }
        //读取原有配置文件
        int iSize = fileQ.size();
        json = new char[iSize + 1];
        memset(json, 0, iSize + 1);
        if (iSize != fileQ.read(json, iSize))
        {
            QMessageBox::information(this, "配置读取", QString::fromLocal8Bit("配置文件读取失败(%1)").arg(dv_config_path.c_str()));
            iRet = -1;
            break;
        }
        fileQ.close();

        //解析Json文件
        Document hConfigDoc;
        string strJson = SString::UTF8ToGBK(json);
        hConfigDoc.Parse(strJson.c_str());
        if (hConfigDoc.HasParseError())
        {
            QMessageBox::information(this, "配置读取", QString::fromLocal8Bit("配置文件解析失败,错误码%1, 位置：%2").arg(hConfigDoc.GetParseError()).arg(hConfigDoc.GetErrorOffset()));
            iRet = -1;
            break;
        }
        Document::AllocatorType &allocator = hConfigDoc.GetAllocator(); //获取分配器  

        Value::ConstMemberIterator itIndexItem = hConfigDoc.FindMember("glass_index");
        int index = 0;
        if (itIndexItem != hConfigDoc.MemberEnd() && itIndexItem->value.IsInt())
        {
            index = itIndexItem->value.GetInt();;
        }

        Value::MemberIterator itProductItem;
        if (index == 0)  //白玻璃
        {
            itProductItem = hConfigDoc.FindMember("white");
        }
        else //丝印玻璃
        {
            itProductItem = hConfigDoc.FindMember("Silkprint");
        }
        BREAK_IF(itProductItem == hConfigDoc.MemberEnd());
        Value& itemProduct = itProductItem->value;

        {
            Value::MemberIterator itParamItem;
            updateIntValue("iDelaytime", itemProduct, ui->spinBox_timeout->value());
			updateIntValue("bStart_timer", itemProduct, ui->groupBox_timeout->isChecked());
            //heat_thresh
            updateDoubleValue("edge_heat_thresh", itemProduct, ui->doubleSpinBox_heat_thresh_edge->value());
            updateDoubleValue("window_heat_thresh", itemProduct, ui->doubleSpinBox_heat_thresh_window->value());

            ////灰度值取值方式设置
            Value::MemberIterator itGrayValControlItem;
            itGrayValControlItem = itemProduct.FindMember("gray_val_control");
            if (itGrayValControlItem != itemProduct.MemberEnd() && itGrayValControlItem->value.IsObject())
            {
                updateIntValue("A", itGrayValControlItem->value, ui->spinBox_gray_A->value());

                updateIntValue("B", itGrayValControlItem->value, ui->spinBox_gray_B->value());

                updateIntValue("C", itGrayValControlItem->value, ui->spinBox_gray_C->value());

                updateIntValue("D", itGrayValControlItem->value, ui->spinBox_gray_D->value());
            }

            updateIntValue("contrast_val_control", itemProduct, ui->spinBox_contrast_val_control->value());

            //位置信息设置
            Value::MemberIterator itImageParamItem;
            itImageParamItem = itemProduct.FindMember("wlj_param");
            if (itImageParamItem != itemProduct.MemberEnd() && itImageParamItem->value.IsObject())
            {
                Value::MemberIterator itEdgeParamItem;
                itEdgeParamItem = itImageParamItem->value.FindMember("edge");
                if (itEdgeParamItem != itImageParamItem->value.MemberEnd() && itEdgeParamItem->value.IsObject())
                {
                    updateIntValue("top", itEdgeParamItem->value, ui->spinBox_edge_top->value());

                    updateIntValue("right", itEdgeParamItem->value, ui->spinBox_edge_right->value());

                    updateIntValue("bottom", itEdgeParamItem->value, ui->spinBox_edge_bottom->value());

                    updateIntValue("left", itEdgeParamItem->value, ui->spinBox_edge_left->value());
                }

                updateIntValue("shiftX", itImageParamItem->value, ui->spinBox_shiftX->value());

                updateIntValue("shiftY", itImageParamItem->value, ui->spinBox_shiftY->value());

                updateDoubleValue("x_rate_zoom", itImageParamItem->value, ui->doubleSpinBox_x_rate_zoom->value());

                updateDoubleValue("y_rate_zoom", itImageParamItem->value, ui->doubleSpinBox_y_rate_zoom->value());

                updateIntValue("right_distortion", itImageParamItem->value, ui->spinBox_right_distortion->value());

                updateIntValue("left_distortion", itImageParamItem->value, ui->spinBox_left_distortion->value());
            }

            Value::MemberIterator itModelParamItem;
            //检测模型配置
            itModelParamItem = itemProduct.FindMember("detect_model_list");
            if (itModelParamItem != itemProduct.MemberEnd() && itModelParamItem->value.IsObject())
            {
                Value::MemberIterator itPosParamItem;
                itPosParamItem = itModelParamItem->value.FindMember("silkArea");
                if (itPosParamItem != itModelParamItem->value.MemberEnd() && itPosParamItem->value.IsObject())
                {
                    strItem[index] = ui->lineEdit_Edge_Defect_Symbol->text();
                    byteData[index] = strItem[index].toLocal8Bit();
                    pConfigPath[index] = byteData[index].data();
                    iLength = byteData[index].size();
                    updateStringValue("symbol_file", itPosParamItem->value, pConfigPath[index], iLength);
                    index++;

                    strItem[index] = ui->lineEdit_Edge_Defect_Param->text();
                    byteData[index] = strItem[index].toLocal8Bit();
                    pConfigPath[index] = byteData[index].data();
                    iLength = byteData[index].size();
                    updateStringValue("params_file", itPosParamItem->value, pConfigPath[index], iLength);
                    index++;
                }
                itPosParamItem = itModelParamItem->value.FindMember("windowArea");
                if (itPosParamItem != itModelParamItem->value.MemberEnd() && itPosParamItem->value.IsObject())
                {
                    strItem[index] = ui->lineEdit_Window_Defect_Symbol->text();
                    byteData[index] = strItem[index].toLocal8Bit();
                    pConfigPath[index] = byteData[index].data();
                    iLength = byteData[index].size();
                    updateStringValue("symbol_file", itPosParamItem->value, pConfigPath[index], iLength);
                    index++;

                    strItem[index] = ui->lineEdit_Window_Defect_Param->text();
                    byteData[index] = strItem[index].toLocal8Bit();
                    pConfigPath[index] = byteData[index].data();
                    iLength = byteData[index].size();
                    updateStringValue("params_file", itPosParamItem->value, pConfigPath[index], iLength);
                    index++;
                }
            }
            //分类模型配置
            itModelParamItem = itemProduct.FindMember("classify_model_list");
            if (itModelParamItem != itemProduct.MemberEnd() && itModelParamItem->value.IsObject())
            {
                Value::MemberIterator itPosParamItem;
                itPosParamItem = itModelParamItem->value.FindMember("silkArea");
                if (itPosParamItem != itModelParamItem->value.MemberEnd() && itPosParamItem->value.IsObject())
                {
                    strItem[index] = ui->lineEdit_Edge_Classify_Symbol->text();
                    byteData[index] = strItem[index].toLocal8Bit();
                    pConfigPath[index] = byteData[index].data();
                    iLength = byteData[index].size();
                    updateStringValue("symbol_file", itPosParamItem->value, pConfigPath[index], iLength);
                    index++;

                    strItem[index] = ui->lineEdit_Edge_Classify_Param->text();
                    byteData[index] = strItem[index].toLocal8Bit();
                    pConfigPath[index] = byteData[index].data();
                    iLength = byteData[index].size();
                    updateStringValue("params_file", itPosParamItem->value, pConfigPath[index], iLength);
                    index++;
                }
                itPosParamItem = itModelParamItem->value.FindMember("windowArea");
                if (itPosParamItem != itModelParamItem->value.MemberEnd() && itPosParamItem->value.IsObject())
                {
                    strItem[index] = ui->lineEdit_Window_Classify_Symbol->text();
                    byteData[index] = strItem[index].toLocal8Bit();
                    pConfigPath[index] = byteData[index].data();
                    iLength = byteData[index].size();
                    updateStringValue("symbol_file", itPosParamItem->value, pConfigPath[index], iLength);
                    index++;

                    strItem[index] = ui->lineEdit_Window_Classify_Param->text();
                    byteData[index] = strItem[index].toLocal8Bit();
                    pConfigPath[index] = byteData[index].data();
                    iLength = byteData[index].size();
                    updateStringValue("params_file", itPosParamItem->value, pConfigPath[index], iLength);
                    index++;
                }
                itPosParamItem = itModelParamItem->value.FindMember("twoClassification");
                if (itPosParamItem != itModelParamItem->value.MemberEnd() && itPosParamItem->value.IsObject())
                {
                    strItem[index] = ui->lineEdit_Window_Classify_Symbol_Sec->text();
                    byteData[index] = strItem[index].toLocal8Bit();
                    pConfigPath[index] = byteData[index].data();
                    iLength = byteData[index].size();
                    updateStringValue("symbol_file", itPosParamItem->value, pConfigPath[index], iLength);
                    index++;

                    strItem[index] = ui->lineEdit_Window_Classify_Param_Sec->text();
                    byteData[index] = strItem[index].toLocal8Bit();
                    pConfigPath[index] = byteData[index].data();
                    iLength = byteData[index].size();
                    updateStringValue("params_file", itPosParamItem->value, pConfigPath[index], iLength);
                    index++;
                }
            }

            //过滤参数
            Value::MemberIterator itControlParamItem;
            itControlParamItem = itemProduct.FindMember("default_control");
            if (itControlParamItem != itemProduct.MemberEnd() && itControlParamItem->value.IsArray())
            {
                QTabWidget *pTabWidget = NULL;
                for (int iS = 0; iS < itControlParamItem->value.Size(); ++iS)
                {
                    if (0 == iS % 2)
                    {
                        pTabWidget = ui->tabWidget_edge;
                    }
                    else
                    {
                        pTabWidget = ui->tabWidget_window;
                    }
                    Value& itemSeqObj = itControlParamItem->value[iS];
                    BREAK_IF(!itemSeqObj.IsArray());
                    int widIndex = iS / 2;
                    dvFilterConfig *pItem = (dvFilterConfig *)pTabWidget->widget(widIndex);
                    int iCount = 0;
                    const stDefectItemInfo *pInfo = pItem->getItemsValue(iCount);
                    for (int iDefect = 0; iDefect < itemSeqObj.Size(); ++iDefect)
                    {
                        Value& itemDefectObj = itControlParamItem->value[iS][iDefect];
                        BREAK_IF(!itemDefectObj.IsObject());

                        updateIntValue("length", itemDefectObj, pInfo[iDefect].length);

                        updateIntValue("level", itemDefectObj, pInfo[iDefect].level);

                        updateBoolValue("enable", itemDefectObj, pInfo[iDefect].bEnable);
                    }
                }
            }
        }

        //更新Json Buffer内存
        StringBuffer buffer;
        PrettyWriter<StringBuffer> writer(buffer);
        hConfigDoc.Accept(writer);
        strJson = SString::GBKToUTF8(buffer.GetString());

        if (!fileQ.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, "配置更新", QString::fromLocal8Bit("配置文件更新失败"));
            break;
        }

        iSize = strJson.size();
        if (iSize != fileQ.write(strJson.c_str(), iSize))
        {
            QMessageBox::information(this, "配置更新", QString::fromLocal8Bit("配置文件更新失败"));
            break;
        }
        fileQ.close();
    } while (0);
    if (fileQ.isOpen())
    {
        fileQ.close();
    }
    if (NULL != json)
    {
        delete[]json;
        json = NULL;
    }
	*/
}

dvconfig* dvconfig::m_pInstance = NULL;

dvconfig::dvconfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dvconfig),
    m_bIsNeedSave(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog);// set move and close crl;
    initDvConfigUI();
}

dvconfig::~dvconfig()
{
    delete ui;
}

dvconfig* dvconfig::GetInstance(QWidget* pParant /*= NULL*/)
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new dvconfig(pParant);
    }
    return m_pInstance;
}

void dvconfig::ReleaseInstance()
{
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

void dvconfig::on_pushButton_recover_clicked()
{
    initDvConfigUI();
}

void dvconfig::on_pushButton_save_clicked()
{
    saveDvConfig();
    //重新初始化H算法

   //CInvokeArithmetic::getInstance()->getDlArithmetic()->SetUnInitDeepLearn();
}

void dvconfig::on_pushButton_cancel_clicked()
{
    m_bIsNeedSave = false;
    close();
}

void dvconfig::on_pushButton_Edge_Defect_Symbol_clicked()
{
    QString strName = ui->lineEdit_Edge_Defect_Symbol->text();
    strName = GetFileName(strName, QString::fromLocal8Bit("边缘检测模型"));
    if (!strName.isEmpty())
    {
        ui->lineEdit_Edge_Defect_Symbol->setText(strName);
    }
}

void dvconfig::on_pushButton_Edge_Defect_Param_clicked()
{
    QString strName = ui->lineEdit_Edge_Defect_Param->text();
    strName = GetFileName(strName, QString::fromLocal8Bit("边缘检测模型参数"));
    if (!strName.isEmpty())
    {
        ui->lineEdit_Edge_Defect_Param->setText(strName);
    }
}

void dvconfig::on_pushButton_Edge_Classify_Symbol_clicked()
{
    QString strName = ui->lineEdit_Edge_Classify_Symbol->text();
    strName = GetFileName(strName, QString::fromLocal8Bit("边缘分类模型"));
    if (!strName.isEmpty())
    {
        ui->lineEdit_Edge_Classify_Symbol->setText(strName);
    }
}

void dvconfig::on_pushButton_Edge_Classify_Param_clicked()
{
    QString strName = ui->lineEdit_Edge_Classify_Param->text();
    strName = GetFileName(strName, QString::fromLocal8Bit("边缘分类模型参数"));
    if (!strName.isEmpty())
    {
        ui->lineEdit_Edge_Classify_Param->setText(strName);
    }
}

void dvconfig::on_pushButton_Window_Defect_Symbol_clicked()
{
    QString strName = ui->lineEdit_Window_Defect_Symbol->text();
    strName = GetFileName(strName, QString::fromLocal8Bit("视窗检测模型"));
    if (!strName.isEmpty())
    {
        ui->lineEdit_Window_Defect_Symbol->setText(strName);
    }
}

void dvconfig::on_pushButton_Window_Defect_Param_clicked()
{
    QString strName = ui->lineEdit_Window_Defect_Param->text();
    strName = GetFileName(strName, QString::fromLocal8Bit("视窗检测模型参数"));
    if (!strName.isEmpty())
    {
        ui->lineEdit_Window_Defect_Param->setText(strName);
    }
}

void dvconfig::on_pushButton_Window_Classify_Symbol_clicked()
{
    QString strName = ui->lineEdit_Window_Classify_Symbol->text();
    strName = GetFileName(strName, QString::fromLocal8Bit("视窗分类模型"));
    if (!strName.isEmpty())
    {
        ui->lineEdit_Window_Classify_Symbol->setText(strName);
    }
}

void dvconfig::on_pushButton_Window_Classify_Param_clicked()
{
    QString strName = ui->lineEdit_Window_Classify_Param->text();
    strName = GetFileName(strName, QString::fromLocal8Bit("视窗分类模型参数"));
    if (!strName.isEmpty())
    {
        ui->lineEdit_Window_Classify_Param->setText(strName);
    }
}

void dvconfig::on_pushButton_Window_Classify_Symbol_Sec_clicked()
{
    QString strName = ui->lineEdit_Window_Classify_Symbol_Sec->text();
    strName = GetFileName(strName, QString::fromLocal8Bit("视窗分类模型"));
    if (!strName.isEmpty())
    {
        ui->lineEdit_Window_Classify_Symbol_Sec->setText(strName);
    }
}

void dvconfig::on_pushButton_Window_Classify_Param_Sec_clicked()
{
    QString strName = ui->lineEdit_Window_Classify_Param_Sec->text();
    strName = GetFileName(strName, QString::fromLocal8Bit("视窗分类模型参数"));
    if (!strName.isEmpty())
    {
        ui->lineEdit_Window_Classify_Param_Sec->setText(strName);
    }
}

void dvconfig::closeEvent(QCloseEvent *event)
{
    if (m_bIsNeedSave)
    {
        if (QMessageBox::Yes == QMessageBox::information(this, QString::fromLocal8Bit("保存"),
            QString::fromLocal8Bit("是否保存配置"), QMessageBox::Yes | QMessageBox::No))
        {
            on_pushButton_save_clicked();
        }
    }
}

void dvconfig::updateBoolValue(const char *pKey, Value& itemParent, bool bUIValue)
{
    int bConfigValue = 0;
    Value::MemberIterator itParamItem;
    //heat_thresh
    itParamItem = itemParent.FindMember(pKey);
    if (itParamItem != itemParent.MemberEnd())
    {
        if (itParamItem->value.IsBool())
        {
            bConfigValue = itParamItem->value.GetBool();
        }
        if (bUIValue != bConfigValue)
        {
            itParamItem->value.SetBool(bUIValue);
        }
    }
}
void dvconfig::updateIntValue(const char *pKey, Value& itemParent, int iUIValue)
{
    int iConfigValue = 0;
    Value::MemberIterator itParamItem;
    //heat_thresh
    itParamItem = itemParent.FindMember(pKey);
    if (itParamItem != itemParent.MemberEnd())
    {
        if (itParamItem->value.IsInt())
        {
            iConfigValue = itParamItem->value.GetInt();
        }
        if (iUIValue != iConfigValue)
        {
            itParamItem->value.SetInt(iUIValue);
        }
    }
}

void dvconfig::updateDoubleValue(const char *pKey, Value& itemParent, double dUIValue)
{
    double dConfigValue = 0;
    Value::MemberIterator itParamItem;
    //heat_thresh
    itParamItem = itemParent.FindMember(pKey);
    if (itParamItem != itemParent.MemberEnd())
    {
        if (itParamItem->value.IsDouble())
        {
            dConfigValue = itParamItem->value.GetDouble();
        }
        if (abs(dUIValue - dConfigValue) > DOUBLE_ACCURACY)
        {
            itParamItem->value.SetDouble(dUIValue);
        }
    }
}

void dvconfig::updateStringValue(const char *pKey, Value& itemParent, const char * szUIValue, int length)
{
    string strConfigValue = "";
    Value::MemberIterator itParamItem;
    itParamItem = itemParent.FindMember(pKey);
    if (itParamItem != itemParent.MemberEnd())
    {
        if (itParamItem->value.IsString())
        {
            strConfigValue = itParamItem->value.GetString();
        }
        if (0 != strConfigValue.compare(szUIValue))
        {
            itParamItem->value.SetString(szUIValue, length);
        }
    }
}

QString dvconfig::GetFileName(QString strName, QString strDes)
{
    QString fileName = "";
    QString filePath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择模型路径[%1]...").arg(strDes), strName);
    return filePath;
}