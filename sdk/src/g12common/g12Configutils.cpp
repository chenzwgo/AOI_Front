#include "g12Configutils.h"
#include "g12common.h"
#include "filevariable/ifilevariable.h"
stMutilCamCfg* g_stMutilCamCfg = NULL;
ChannelCamCfg* g_upCamCfg = NULL;
ChannelCamCfg* g_downCamCfg = NULL;

stMutilCamCfg *getMutilCamCfg()
{
    if(g_stMutilCamCfg == NULL)
    {
        g_stMutilCamCfg = new stMutilCamCfg();
		IFileVariable* pFileVariable = getAcquisitionFileVariable();
		if (pFileVariable != NULL)
		{
			g_stMutilCamCfg->iCamResolutionX = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_RESOLUTIONX).toInt();
			g_stMutilCamCfg->iCamResolutionY = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_RESOLUTIONY).toInt();
			g_stMutilCamCfg->iCamCnt = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_CNT).toInt();
			g_stMutilCamCfg->iOverlapX = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_OVERLAPX).toInt();
			g_stMutilCamCfg->iOverlapY = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_OVERLAPY).toInt();
			g_stMutilCamCfg->iImageWidth = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEWIDTH).toInt();
			g_stMutilCamCfg->iImageHeight = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEHEIGHT).toInt();
			g_stMutilCamCfg->bEnableBlockGrap = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_BLOCKENABLE).toBool();
			g_stMutilCamCfg->iBlockLines = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_BLOCKLINES).toInt();
			g_stMutilCamCfg->iBlockOverlapLines = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_BLOCKOVERLAPLINES).toInt();
			g_stMutilCamCfg->iBlockCnt = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_BLOCKCNT).toInt();

			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_RESOLUTIONX, &g_stMutilCamCfg->iCamResolutionX);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_RESOLUTIONY, &g_stMutilCamCfg->iCamResolutionY);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_CNT, &g_stMutilCamCfg->iCamCnt);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_OVERLAPX, &g_stMutilCamCfg->iOverlapX);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_OVERLAPY, &g_stMutilCamCfg->iOverlapY);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEWIDTH, &g_stMutilCamCfg->iImageWidth);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEHEIGHT, &g_stMutilCamCfg->iImageHeight);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG,ACQUISITION_BLOCKENABLE, &g_stMutilCamCfg->bEnableBlockGrap);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG,ACQUISITION_BLOCKLINES, &g_stMutilCamCfg->iBlockLines);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG,ACQUISITION_BLOCKOVERLAPLINES, &g_stMutilCamCfg->iBlockOverlapLines);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG,ACQUISITION_BLOCKCNT, &g_stMutilCamCfg->iBlockCnt);
		}
    }
    return g_stMutilCamCfg;
}

ChannelCamCfg* getUpCamCfg()
{
	if (g_upCamCfg == NULL)
	{
		g_upCamCfg = new ChannelCamCfg();
		IFileVariable* pFileVariable = getAcquisitionFileVariable();
		if (pFileVariable != NULL)
		{
			g_upCamCfg->bCam1Enable = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_UPCAM1_ENABLE).toBool();
			g_upCamCfg->bCam2Enable = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_UPCAM2_ENABLE).toBool();
			g_upCamCfg->bCam3Enable = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_UPCAM3_ENABLE).toBool();
			g_upCamCfg->bCam4Enable = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_UPCAM4_ENABLE).toBool();

			g_upCamCfg->strCam1Sn = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_UPCAM1_SN).toCString("");
			g_upCamCfg->strCam2Sn = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_UPCAM2_SN).toCString("");
			g_upCamCfg->strCam3Sn = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_UPCAM3_SN).toCString("");
			g_upCamCfg->strCam4Sn = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_UPCAM4_SN).toCString("");

			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_UPCAM1_ENABLE, &g_upCamCfg->bCam1Enable);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_UPCAM2_ENABLE, &g_upCamCfg->bCam2Enable);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_UPCAM3_ENABLE, &g_upCamCfg->bCam3Enable);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_UPCAM4_ENABLE, &g_upCamCfg->bCam4Enable);

			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_UPCAM1_SN, &g_upCamCfg->strCam1Sn);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_UPCAM2_SN, &g_upCamCfg->strCam2Sn);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_UPCAM3_SN, &g_upCamCfg->strCam3Sn);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_UPCAM4_SN, &g_upCamCfg->strCam4Sn);

		}
	}
	return g_upCamCfg;
}


ChannelCamCfg* getDownCamCfg()
{
	if (g_downCamCfg == NULL)
	{
		g_downCamCfg = new ChannelCamCfg();
		IFileVariable* pFileVariable = getAcquisitionFileVariable();
		if (pFileVariable != NULL)
		{
			g_downCamCfg->bCam1Enable = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_DOWNCAM1_ENABLE).toBool();
			g_downCamCfg->bCam2Enable = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_DOWNCAM2_ENABLE).toBool();
			g_downCamCfg->bCam3Enable = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_DOWNCAM3_ENABLE).toBool();
			g_downCamCfg->bCam4Enable = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_DOWNCAM4_ENABLE).toBool();

			g_downCamCfg->strCam1Sn = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_DOWNCAM1_SN).toCString("");
			g_downCamCfg->strCam2Sn = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_DOWNCAM2_SN).toCString("");
			g_downCamCfg->strCam3Sn = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_DOWNCAM3_SN).toCString("");
			g_downCamCfg->strCam4Sn = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_DOWNCAM4_SN).toCString("");

			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_DOWNCAM1_ENABLE, &g_downCamCfg->bCam1Enable);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_DOWNCAM2_ENABLE, &g_downCamCfg->bCam2Enable);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_DOWNCAM3_ENABLE, &g_downCamCfg->bCam3Enable);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_DOWNCAM4_ENABLE, &g_downCamCfg->bCam4Enable);

			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_DOWNCAM1_SN, &g_downCamCfg->strCam1Sn);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_DOWNCAM2_SN, &g_downCamCfg->strCam2Sn);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_DOWNCAM3_SN, &g_downCamCfg->strCam3Sn);
			pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_DOWNCAM4_SN, &g_downCamCfg->strCam4Sn);

		}
	}
	return g_downCamCfg;
}