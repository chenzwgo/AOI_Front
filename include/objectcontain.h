#ifndef  __OBJECT_CONTAIN_H_
#define __OBJECT_CONTAIN_H_
#include <map>

using namespace std;

template <class mykey,class T > class objectContain
{
public:
    objectContain(void);
    ~objectContain(void);
    T FindObject(mykey vkey);
    void AddObject(mykey vkey,T pobject);
    void RemoveObject(mykey vkey);
    void RemoveObject(T pobject);
    void RemoveAllObject();
    static objectContain< mykey, T>* GetFacInstance();
    static void DestroyFacInstance();
private:
    map<mykey,T> objectMap;
    static objectContain< mykey, T>* FactInstance ;
};

 template <class mykey,class T > objectContain< mykey, T>* objectContain<mykey, T>::FactInstance= NULL;

 template <class mykey,class T>
 objectContain<mykey, T>::objectContain( void )
 {
     
 }

 template <class mykey,class T>
 objectContain<mykey, T>::~objectContain( void )
 {
     RemoveAllObject();    
 }



 template <class mykey,class T>
 void objectContain<mykey, T>::RemoveObject(T pobject)
 {
    if(pobject== NULL)
    {
        return ;
    }

    typedef typename std::map<mykey,T>::iterator TEMPLATE_MAP_ITER;
   TEMPLATE_MAP_ITER iter;
   for(iter = objectMap.begin();iter !=objectMap.end();iter++)
   {
       if (pobject == iter->second)
       {

           objectMap.erase(iter);
           return;
      }
   }
   return;
 }


 template <class mykey,class T>
 void objectContain<mykey, T>::RemoveObject(mykey vkey)
 {
    typedef typename std::map<mykey,T>::iterator TEMPLATE_MAP_ITER;
   TEMPLATE_MAP_ITER iter;

   iter = objectMap.find(vkey);
   if (iter != objectMap.end())
   {
       objectMap.erase(iter);
       return;
   }
   return;
 }



 template <class mykey,class T>
 void objectContain<mykey, T>::RemoveAllObject()
 {
     objectMap.clear();
 }

 template <class mykey,class T>
 T objectContain<mykey, T>::FindObject(mykey vkey)
 {
     typedef typename std::map<mykey,T>::iterator TEMPLATE_MAP_ITER;
     TEMPLATE_MAP_ITER iter;
     iter = objectMap.find(vkey); 
     if (iter != objectMap.end()) 
     { 
         return iter->second;
     } 
     else 
     { 
         NULL; 
     }
     return NULL;
 }
 
 
  template <class mykey,class T>
 void objectContain<mykey, T>::AddObject(mykey vkey,T pobject)
 {
    objectMap[vkey] = pobject;
 }
 

 template <class mykey,class T>
 void objectContain<mykey, T>::DestroyFacInstance()
 {
     if (NULL != FactInstance)
     {
         delete FactInstance ;
         FactInstance = NULL;
     }
 }

 template <class mykey,class T>
 objectContain<mykey, T>* objectContain<mykey, T>::GetFacInstance()
 {
     if (NULL == FactInstance)
     {
         FactInstance = new objectContain();
     }
     return FactInstance;
 }

#endif










