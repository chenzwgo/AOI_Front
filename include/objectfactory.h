#ifndef  __OBJECT_FACTORY_H_
#define __OBJECT_FACTORY_H_
#include <map>

using namespace std;

template <class mykey,class T > class objectFactory
{
public:
    objectFactory(void);
    ~objectFactory(void);
    T*  CreateObject(mykey vkey); 

	template <class param1,class param2 >
	T*  CreateObject(mykey vkey,param1 p1,param2 p2)
	{
		T* p_newobject = NULL;
		p_newobject = FindObject(vkey);
		if (NULL == p_newobject)
		{
			p_newobject = new T(p1,p2);
			if (NULL  == p_newobject)
			{
				return NULL;
			}
			objInfo obj(p_newobject,0);
			objectMap[vkey] = obj;
		}
		objectMap[vkey].m_iRef = objectMap[vkey].m_iRef + 1;
		return p_newobject;
	};

	template <class param1>
	T*  CreateObject(mykey vkey,param1 p1)
	{
		T* p_newobject = NULL;
		p_newobject = FindObject(vkey);
		if (NULL == p_newobject)
		{
			p_newobject = new T(p1);
			if (NULL  == p_newobject)
			{
				return NULL;
			}
			objInfo obj(p_newobject,0);
			objectMap[vkey] = obj;
		}
		objectMap[vkey].m_iRef = objectMap[vkey].m_iRef + 1;
		return p_newobject;
	};

    T* FindObject(mykey vkey);
    void DestroyObject(mykey vkey);
    void DestroyObject(T* pobject);
    static objectFactory< mykey, T>* GetFacInstance();
    static void DestroyFacInstance();
	void RemoveAllObject();
private:
    struct objInfo
    {
        T* pObj;
        int m_iRef;

        objInfo(T* obj ,int ref = 0)
        {
            pObj = obj;
            m_iRef = ref;
        }
        objInfo()
        {
            pObj = NULL;
            m_iRef = 0;
        }

    };
    map<mykey,objInfo> objectMap;
    
    static objectFactory< mykey, T>* FactInstance ;
};

 template <class mykey,class T > objectFactory< mykey, T>* objectFactory<mykey, T>::FactInstance= NULL;

 template <class mykey,class T>
 objectFactory<mykey, T>::objectFactory( void )
 {
     
 }

 template <class mykey,class T>
 objectFactory<mykey, T>::~objectFactory( void )
 {
     RemoveAllObject();    
 }

 template <class mykey,class T>
 T* objectFactory<mykey, T>::CreateObject(mykey vkey)
 {
     T* p_newobject = NULL;
     p_newobject = FindObject(vkey);
     if (NULL == p_newobject)
     {
         p_newobject = new T();
         if (NULL  == p_newobject)
         {
             return NULL;
         }
         objInfo obj(p_newobject,0);
         objectMap[vkey] = obj;
     }

     objectMap[vkey].m_iRef = objectMap[vkey].m_iRef + 1;
     return p_newobject;
 }

 template <class mykey,class T>
 void objectFactory<mykey, T>::DestroyObject(T *pobject)
 {
    if(pobject== NULL)
    {
        return ;
    }

    typedef typename std::map<mykey,objInfo>::iterator TEMPLATE_MAP_ITER;
   TEMPLATE_MAP_ITER iter;
   for(iter = objectMap.begin();iter !=objectMap.end();iter++)
   {
       if (pobject == iter->second.pObj)
       {
           iter->second.m_iRef = iter->second.m_iRef -1;
           if(iter->second.m_iRef <= 0 && iter->second.pObj != NULL)
           {
               delete (iter->second.pObj);
               iter->second.pObj = NULL;
               iter->second.m_iRef = 0;
               objectMap.erase(iter);
           }

           return;
       }
   }
   return;
 }


 template <class mykey,class T>
 void objectFactory<mykey, T>::DestroyObject(mykey vkey)
 {
    typedef typename std::map<mykey,objInfo>::iterator TEMPLATE_MAP_ITER;
   TEMPLATE_MAP_ITER iter;

   iter = objectMap.find(vkey);
   if (iter != objectMap.end())
   {
       iter->second.m_iRef = iter->second.m_iRef -1;
       if(iter->second.m_iRef <= 0 && iter->second.pObj != NULL)
       {
           delete (iter->second.pObj);
           iter->second.pObj = NULL;
           iter->second.m_iRef = 0;
           objectMap.erase(iter);
       }
       return;
   }
   return;
 }



 template <class mykey,class T>
 void objectFactory<mykey, T>::RemoveAllObject()
 {
      typedef typename std::map<mykey,objInfo>::iterator TEMPLATE_MAP_ITER;
     TEMPLATE_MAP_ITER iter;

     for(iter = objectMap.begin();iter !=objectMap.end();iter++)
     {
         if (NULL != iter->second.pObj )
         {
             delete (iter->second.pObj);
             iter->second.pObj = NULL;
             iter->second.m_iRef = 0;
         }
     }
     objectMap.clear();
 }

 template <class mykey,class T>
 T* objectFactory<mykey, T>::FindObject(mykey vkey)
 {
     typedef typename std::map<mykey,objInfo>::iterator TEMPLATE_MAP_ITER;
     TEMPLATE_MAP_ITER iter;
     iter = objectMap.find(vkey); 
     if (iter != objectMap.end()) 
     { 
         return iter->second.pObj;
     } 
     else 
     { 
         NULL; 
     }
     return NULL;
 }

 template <class mykey,class T>
 void objectFactory<mykey, T>::DestroyFacInstance()
 {
     if (NULL != FactInstance)
     {
         delete FactInstance ;
         FactInstance = NULL;
     }
 }

 template <class mykey,class T>
 objectFactory<mykey, T>* objectFactory<mykey, T>::GetFacInstance()
 {
     if (NULL == FactInstance)
     {
         FactInstance = new objectFactory();
     }
     return FactInstance;
 }

#endif










