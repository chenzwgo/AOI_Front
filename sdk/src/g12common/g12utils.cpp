#include "g12utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include "motion/imotion.h"
#include "card/icardcontinterp.h"
#include "station/istation.h"
#include "rtdefine.h"
#include "proxymanager.h"
#include "sdkcore.h"

#define  CONTROLCARD_NAME  "card"

#include <Windows.h>

int usSleep(int us)
{
    LARGE_INTEGER fre;
    if (QueryPerformanceFrequency(&fre))
    {
        LARGE_INTEGER run, priv, curr, res;
        run.QuadPart = fre.QuadPart * us / 1000000;
        QueryPerformanceCounter(&priv);
        do
        {
            QueryPerformanceCounter(&curr);
        } while (curr.QuadPart - priv.QuadPart < run.QuadPart);
        curr.QuadPart -= priv.QuadPart;
        int nres = (curr.QuadPart * 1000000 / fre.QuadPart);
        return nres;
    }
    return -1;//
}

unsigned char* loadImage( const string& strFileName)
{
    FILE *pf=fopen(strFileName.c_str(),"rb");
    if (pf==0) return NULL;
    fseek(pf,0,SEEK_END);
    long len=ftell(pf);
    fseek(pf,0,SEEK_SET);

    unsigned char* ucImageData = new unsigned char[len];
    if (!ucImageData)
    {
        fclose(pf);
        return NULL;
    }
    int result =fread(ucImageData,1,len,pf);
    if (result != len) 
    {
        delete []ucImageData;
		fclose(pf);
        return NULL;
    }
    fclose(pf);
    return ucImageData;
}

bool getImageSize( const string& strFileName, unsigned int& iWidth, unsigned int& iHeight )
{
#if 0
    string suffixStr = strFileName.substr(strFileName.find_last_of('.') + 1);

    ifstream ffin(strFileName, std::ios::binary);

    if (!ffin.is_open()){
        return false;
    }   
    char s1[2] = {0}, s2[2] = {0};
    if (suffixStr.compare("gif") == 0)
    {   // gif    
        ffin.seekg(6);         
        ffin.read(s1, 2);
        ffin.read(s2, 2);       
        iWidth = (unsigned int)(s1[1])<<8|(unsigned int)(s1[0]);
        iHeight = (unsigned int)(s2[1])<<8|(unsigned int)(s2[0]);    
    }
    else if (suffixStr.compare("jpg") == 0)
    {   // jpg
        ffin.seekg(164);        
        ffin.read(s1, 2);
        ffin.read(s2, 2);       
        iWidth = (unsigned int)(s1[1])<<8|(unsigned int)(s1[0]);
        iHeight = (unsigned int)(s2[1])<<8|(unsigned int)(s2[0]);          
    }
    else if (suffixStr.compare("png") == 0)
    {     // png
        ffin.seekg(17);        
        ffin.read(s1, 2);
        ffin.seekg(2, std::ios::cur);
        ffin.read(s2, 2);     
        iWidth = (unsigned int)(s1[1])<<8|(unsigned int)(s1[0]);
        iHeight = (unsigned int)(s2[1])<<8|(unsigned int)(s2[0]);    
    }
    else if (suffixStr.compare("bmp") == 0)
    {    // bmp       
        ffin.seekg(18);        
        ffin.read(s1, 2);
        ffin.seekg(2, std::ios::cur);
        ffin.read(s2, 2);       
        iWidth = (unsigned int)(s1[1])<<8|(unsigned int)(s1[0]);
        iHeight = (unsigned int)(s2[1])<<8|(unsigned int)(s2[0]);    
    }
 
    ffin.close();

#endif
    string suffixStr = strFileName.substr(strFileName.find_last_of('.') + 1);
    const char *fn = strFileName.c_str();

    FILE *f=fopen(fn,"rb");
    if (f==0) return false;
    fseek(f,0,SEEK_END);
    long len=ftell(f);
    fseek(f,0,SEEK_SET);
    if (len<24) {
        fclose(f);
        return false;
    }
    // Strategy:
    // reading GIF dimensions requires the first 10 bytes of the file
    // reading PNG dimensions requires the first 24 bytes of the file
    // reading JPEG dimensions requires scanning through jpeg chunks
    // In all formats, the file is at least 24 bytes big, so we'll read that always
    unsigned char buf[54] = {0};
    fread(buf,1,54,f);

   
    // JPEG: (first two bytes of buf are first two bytes of the jpeg file; rest of buf is the DCT frame
    if (suffixStr.compare("jpg") == 0)
    { 
        long pos=2;
        while (buf[2]==0xFF)
        { 
            if (buf[3]==0xC0 || buf[3]==0xC1 || buf[3]==0xC2 || buf[3]==0xC3 || buf[3]==0xC9 || buf[3]==0xCA || buf[3]==0xCB) 
                break;
            pos += 2+(buf[4]<<8)+buf[5];
            if (pos+12>len) 
                break;
            fseek(f,pos,SEEK_SET); fread(buf+2,1,12,f);
        }
        iHeight = (buf[7]<<8) + buf[8];
        iWidth = (buf[9]<<8) + buf[10];
    }
    // GIF: first three bytes say "GIF", next three give version number. Then dimensions
    else if (suffixStr.compare("gif") == 0)
    { 
        iWidth = buf[6] + (buf[7]<<8);
        iHeight = buf[8] + (buf[9]<<8);
    }
    // PNG: the first frame is by definition an IHDR frame, which gives dimensions
    else if (suffixStr.compare("png") == 0)
    { 
        iWidth = (buf[16]<<24) + (buf[17]<<16) + (buf[18]<<8) + (buf[19]<<0);
        iHeight = (buf[20]<<24) + (buf[21]<<16) + (buf[22]<<8) + (buf[23]<<0);
    }
    else if (suffixStr.compare("bmp") == 0)
    {
         iWidth = *(int*)&buf[18];
         iHeight = *(int*)&buf[22];
    }
    else
    {
		fclose(f);
        return false;
    }
    fclose(f);
    return true;
}

bool startTrigger(int iNumber, int iplusWidth/* = 100*/)
{
    iMotionCfg *pMotionCfg = (iMotionCfg*)GetSDKInstance("motion", iMotionCfg::IID_MOTION_CONFIG);
    RETURN_V_IF(NULL == pMotionCfg, false);
    iMotion *pMotion = (iMotion*)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);
    RETURN_V_IF(NULL == pMotion, false);
    ICard *pCard = (ICard *)pMotion->GetCardInstance(CONTROLCARD_NAME);
    RETURN_V_IF(NULL == pCard, false);
    ICardContInterp *pCardConInterp = dynamic_cast<ICardContInterp*>(pCard);
    RETURN_V_IF(NULL == pCardConInterp, false);

    bool res = false;
    StCardInfo cardInfo;
    pMotionCfg->getCardInfo("Card", cardInfo);
    if (0 == strcmp(cardInfo.cardType, "cardgg"))
    {
        res = pCardConInterp->SetComparePluse(2, 0, iplusWidth);
    }
    else
    {
        res = pCardConInterp->SetComparePluse(0, 1, iNumber);
    }
    return 0;
}

bool isTriggerFinished()
{
    iMotion *pMotion = (iMotion*)sdkcore::queryPriInterface("motion", iMotion::IDD_MOTION_MANAGE);
    RETURN_V_IF(NULL == pMotion, false);
    ICard *pCard = (ICard *)pMotion->GetCardInstance(CONTROLCARD_NAME);
    RETURN_V_IF(NULL == pCard, false);
    ICardContInterp *pCardConInterp = dynamic_cast<ICardContInterp*>(pCard);
    RETURN_V_IF(NULL == pCardConInterp, false);
    short istu = 0;//触发是否完成 值1为完成 值 0为正在进行
    long  ldata[2] = { 0 };
    bool res = false;
    Sleep(150);
    pCardConInterp->GetCompareStatus(istu, ldata);
    if (istu == 1)
    {
        res = true;
    }
    else
    {
        res = false;
    }
    return res;
}
