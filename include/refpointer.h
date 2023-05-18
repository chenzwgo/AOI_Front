#ifndef  __REF_POINTER_H_
#define  __REF_POINTER_H_

// 注意：处于性能和实用方面考虑，此类不是线程安全的
template<class T>  
class RefPointer
{  
    public:  
        RefPointer(T* t=NULL):pt(NULL),count(0)
        {   
            pt = t;
        }
        ~RefPointer()
        {
            if(pt != NULL)
            {
                delete pt;
                pt = NULL; 
            } 
        }  

         RefPointer(RefPointer& rhs) :pt(NULL),count(0)
         {
             pt = rhs.Get();
         }

         RefPointer(const RefPointer& rhs) :pt(NULL),count(0)
         {

             pt = rhs.Get();
         }

        RefPointer& operator=(RefPointer& rhs)
        {
            if (&rhs == this)
            {
                return this;
            }
            reset(rhs.Get());
            return (*this);
        }

        RefPointer& operator=(const RefPointer& rhs) 
        {
            if (&rhs == this)
            {
                return this;
            }
            reset(rhs.Get());
            return (*this);
        }

        template<class T> RefPointer& operator=(T* t)
        {
            reset(NULL);
            pt = t;
            return (*this);
        }

        template<class T> RefPointer& operator=(const T* t) 
        {
            reset(NULL);
            pt = t;
            return (*this);
        }

        T* get()
        {
			if (pt== NULL)
			{
				return NULL;
			}
            count++;
            return pt;
        } 
        void realse()
        {
            count--;
            if (count <=0)
            {
                if(pt != NULL)
                {
                    delete pt;
                    pt = NULL;
                }
                count =0;
            }
        }
        bool empty()
        {
            return pt == NULL;
        }

        template<class T>
        void reset(T* t)
        {
            if(pt != NULL)
            {
                delete pt;
                pt = NULL;
            }
            count =0;
            pt =t;
        }
		int getRefCount()
		{
			return count;
		}
    private:  
        T *pt;
        int count ; 
}; 

#endif










