/*HEAD fem_base HXX ONESTEP */

/*==============================================================================

                    Copyright 2006 Jilin Kingmesh Corp.
                           All rights reserved

================================================================================ 
File description:

    -- Please append file description informations here --
A dialog about check and modify overlap condition .
================================================================================
Date            Name                    Description of Change
                Zhang Xiang-kui 	      Create

$HISTORY$
================================================================================*/

#ifndef _KMAS_FEM_BASE_HXX
#define _KMAS_FEM_BASE_HXX

#include <set>
#include <vector>
#include <map>
#include <assert.h>
#include <algorithm>

using namespace std;

namespace fem{

//////////////////////////////////////////////////////////////////////////

#ifndef _KMAS_FEM_TEMPLATE_TYPE_DEFINE
#define _KMAS_FEM_TEMPLATE_TYPE_DEFINE

enum POSITION{
     BEGIN,
     END
};

template<typename N, typename E>
class FemGenericADTTemplate;

template<typename U>
class TElementTemplate;

//////////////////////////////////////////////////////////////////////////

template<typename U,typename P>
class TNodeTemplate{
public:
     TNodeTemplate(double x = 0., double y = 0., double z = 0.)
          :point(x,y,z),id(NULL){}
     ~TNodeTemplate(){}
public:
     typedef U		TUserData;
     typedef P		TPoint;
private:
     typedef int*       PtrOfIndex;
     typedef vector<PtrOfIndex>	TContainer;
public:
     typedef typename 	TContainer::iterator 		iterator;
     typedef typename 	TContainer::const_iterator 	const_iterator;
private:
     TUserData          user_data;
     TPoint             point;
     TContainer         adjoin_element_indexs;
     int                *id;
public:
     TUserData          &UserData(){return user_data;}
     TPoint             &Point(){return point;}	
     int                &Id()
     {
          if (id == NULL) id = new int(-1);
          return *id;
     }
     void               Dispose()
     {
          if (id != NULL)
          {
               delete id;
               id = NULL;
          }
     }
public:
     inline iterator	GetAdjoinElement(POSITION pos){
          switch(pos){
          case BEGIN:
               return adjoin_element_indexs.begin();
               break;
          case END:
          default:
               return adjoin_element_indexs.end();
               break;
          }
     }
     inline iterator    GetAdjoinElement(int pos){
          assert(pos >= 0 && pos < static_cast<int>(adjoin_element_indexs.size()));
          return GetAdjoinElement(BEGIN) + pos;
     }
     inline void        InsertAdjoinElement(int *Id){
          bool exist = false;
          for(int loop = 0 ; loop < GetAdjoinElementCount() ; loop ++)
          {
               if (adjoin_element_indexs[loop] == Id)
               {
                    exist = true;
                    break;
               }
          }
          if (!exist) {
               adjoin_element_indexs.push_back(Id);
          }
     }
     inline void	DeleteAdjoinElement(iterator pos){
          adjoin_element_indexs.erase(pos);
     }
     inline int	GetAdjoinElementCount(){return (int)adjoin_element_indexs.size();}
};

//////////////////////////////////////////////////////////////////////////

template<typename U>
class TElementTemplate{
public:
     typedef U		TUserData;
private:
     typedef int*				PtrOfIndex;
     typedef vector<PtrOfIndex>	TContainer;
public:
     TElementTemplate() : id(NULL){}
public:
     typedef typename TContainer::iterator iterator;
     typedef typename TContainer::const_iterator const_iterator;
private:
     TUserData	        user_data;
     TContainer	   node_indexs;
     int               *id;
public:
     TUserData 	   &UserData(){return user_data;}
     int               &Id()
     {
          if(id == NULL) id = new int(-1);
          return *id;
     }
     void              Dispose()
     {
          if (id != NULL)
          {
               delete id;
               id = NULL;
          }
     }
public:
     inline iterator GetNode(POSITION pos){
          switch(pos){
          case BEGIN:
               return node_indexs.begin();
               break;
          case END:
          default:
               return node_indexs.end();
               break;
          }
     }
     inline iterator GetNode(int pos){
          assert(pos >= 0 && pos < static_cast<int>(node_indexs.size()));
          return GetNode(BEGIN) + pos;
     }
     inline void InsertNode(int *Id){
          node_indexs.push_back(Id);
     }
     inline void DeleteNode(iterator pos){
          node_indexs.erase(pos);
     }
     inline int	GetNodeCount(){return (int)node_indexs.size();}
};

#endif

//////////////////////////////////////////////////////////////////////////

#ifndef _KMAS_FEM_POINT
#define _KMAS_FEM_POINT

class TPoint{
public:
     double x,y,z;
public:
     TPoint(double vx = 0. , double vy = 0. , double vz = 0.)
          :x(vx),y(vy),z(vz){}
};

inline bool operator<(const TPoint &left,const TPoint &right)
{
     return (left.x < right.x);
}

#endif

//////////////////////////////////////////////////////////////////////////

#ifndef _KMAS_FEM_NORMAL
#define _KMAS_FEM_NORMAL

class TNormal{
public:
     double x,y,z;
public:
     TNormal(double vx = 0. , double vy = 0. , double vz = 0.)
          :x(vx),y(vy),z(vz){}

};

#endif

//////////////////////////////////////////////////////////////////////////

#ifndef _KMAS_FEM_VECTOR
#define _KMAS_FEM_VECTOR

class TVector{
public:
     double x,y,z;
public:
     TVector(double vx = 0. , double vy = 0. , double vz = 0.)
          :x(vx),y(vy),z(vz){}
};

#endif

//////////////////////////////////////////////////////////////////////////

#ifndef _KMAS_FEM_SIDE_TYPE_DEFINE
#define _KMAS_FEM_SIDE_TYPE_DEFINE

class TSide{
public:
     int start,end;
public:
     TSide(int start_v,int end_v):start(start_v),end(end_v){assert(start_v<end_v);}
public:
     bool operator<(const TSide &right)const{
          if (start<right.start) {
               return true;
          }else if(start == right.start){
               if (end<right.end) {
                    return true;
               }
          }
          return false;
     }
     const bool operator==(const TSide &right)const{
          if (start == right.start && end == right.end) {
               return true;
          }
          return false;
     }
};
typedef vector<TSide>::iterator TSideIterator;

#endif

//////////////////////////////////////////////////////////////////////////

#ifndef _KMAS_FEM_TYPE_DEFINE
#define _KMAS_FEM_TYPE_DEFINE

typedef TNormal                                               TUserData;
typedef TNodeTemplate<TUserData,TPoint >                      TNode;
typedef TElementTemplate<TUserData >                          TElement;
typedef vector<TNode>::iterator                               TNodeIterator;
typedef vector<TElement>::iterator                            TElementIterator;

#endif

//////////////////////////////////////////////////////////////////////////

#ifndef _KMAS_FEM_TRAVEL_TEMPLATE_DEFINE
#define _KMAS_FEM_TRAVEL_TEMPLATE_DEFINE

template<typename iterator_t , typename functor_t>
inline void Travel(iterator_t begin, iterator_t end, functor_t functor)
{
     iterator_t iter;
     for (iter = begin ; iter != end ; iter++)
     {
          functor(iter);
     }
}

#endif

typedef unsigned int element_id_t;
typedef unsigned int node_id_t;

struct local_point_t{
     double x;
     double y;
     double z;
};

struct local_element_t{
     node_id_t n1;
     node_id_t n2;
     node_id_t n3;
     node_id_t n4;
};

struct side_t{
public:
     int first;
     int second;
public:
     bool operator<(const side_t &right) const
          {
               bool ret = true;

               if (first < right.first)
                    ret = true;
               else if(first == right.first)
                    ret = second < right.second;
               else
                    ret = false;

               return ret;
          }
public:
     side_t(const int &f = -1,const int &s = -1):first(f < s ? f : s),second(f < s ? s : f){}
};

typedef side_t local_side_t;

struct local_e2e_t{
     node_id_t first;
     node_id_t second;
     element_id_t elem_id;
public:
     bool operator<(const local_e2e_t &right) const
          {
               local_side_t l(first , second);
               local_side_t r(right.first , right.second);

               return l < r;
          }
};

struct TFemCoreCacheBase  
{
     double *nodes;
     int nodes_num;
     
     int *elements;
     int elements_num;

     double *node_normals;
     double *element_normals;

     int *adjt_elements_of_nodes;
     int *adjt_elements_of_nodes_index;

     int *adjt_nodes_of_nodes;
     int *adjt_nodes_of_nodes_index;

     int *adjt_elements_of_elements;
     int *adjt_elements_of_elements_index;
};

class TFemCoreCachePoint
{
     TFemCoreCacheBase *_cache_base;
public:
     struct Item
     {
          double x;
          double y;
          double z;
     };
public:
     TFemCoreCachePoint(TFemCoreCacheBase *cache_base) : _cache_base(cache_base)
     {
          assert(cache_base);
     }
	 TFemCoreCachePoint() : _cache_base(NULL){}
	 void Attach(TFemCoreCacheBase *cache_base)
	 {
		 _cache_base = cache_base;
	 }
public:
     const Item &operator[](const int &index)
     {
          assert(index >= 0 && index < _cache_base->nodes_num);
          Item *ptr_pt = (Item*)(_cache_base->nodes + index * 3);
          return *ptr_pt;
     }
     int Count(){return _cache_base->nodes_num;}
};

class TFemCoreCacheNNormal
{
     TFemCoreCacheBase *_cache_base;
public:
     struct Item
     {
          double x;
          double y;
          double z;
     };
public:
     TFemCoreCacheNNormal(TFemCoreCacheBase *cache_base) : _cache_base(cache_base)
     {
          assert(cache_base);
     }
	 TFemCoreCacheNNormal() : _cache_base(NULL){}
	 void Attach(TFemCoreCacheBase *cache_base)
	 {
		 _cache_base = cache_base;
		 assert(cache_base);
	 }
public:
     const Item &operator[](const int &index)
     {
          assert(index >= 0 && index < _cache_base->nodes_num);
          Item *ptr_pt = (Item*)(_cache_base->node_normals + index * 3);
          return *ptr_pt;
     }
     int Count(){return _cache_base->nodes_num;}
};

class TFemCoreCacheElement
{
     TFemCoreCacheBase *_cache_base;
public:
     struct Item
     {
          int n1;
          int n2;
          int n3;
          int n4;
     };
public:
     TFemCoreCacheElement(TFemCoreCacheBase *cache_base) : _cache_base(cache_base)
     {
          assert(cache_base);
     }
	 TFemCoreCacheElement() : _cache_base(NULL){}
	 void Attach(TFemCoreCacheBase *cache_base)
	 {
		 _cache_base = cache_base;
		 assert(cache_base);
	 }
public:
     const Item &operator[](const int &index)
     {
          assert(index >= 0 && index < _cache_base->elements_num);
          Item *ptr_elem = (Item*)(_cache_base->elements + index * 4);
          return *ptr_elem;
     }
     int Count(){return _cache_base->elements_num;}
};

class TFemCoreCacheENormal
{
     TFemCoreCacheBase *_cache_base;
public:
     struct Item
     {
          double x;
          double y;
          double z;
     };
public:
     TFemCoreCacheENormal(TFemCoreCacheBase *cache_base) : _cache_base(cache_base)
     {
          assert(cache_base);
     }
	 TFemCoreCacheENormal() : _cache_base(NULL){}
	 void Attach(TFemCoreCacheBase *cache_base)
	 {
		 _cache_base = cache_base;
		 assert(cache_base);
	 }
public:
     const Item &operator[](const int &index)
     {
          assert(index >= 0 && index < _cache_base->elements_num);
          Item *ptr_pt = (Item*)(_cache_base->element_normals + index * 3);
          return *ptr_pt;
     }
     int Count(){return _cache_base->elements_num;}
};

class TFemCoreCacheN2E
{
     TFemCoreCacheBase *_cache_base;
public:
     TFemCoreCacheN2E(TFemCoreCacheBase *cache_base) : _cache_base(cache_base)
     {
          assert(cache_base);
     }
	 TFemCoreCacheN2E() : _cache_base(NULL){}
	 void Attach(TFemCoreCacheBase *cache_base)
	 {
		 _cache_base = cache_base;
		 assert(cache_base);
	 }
public:
     class Item
     {
     public:
          typedef int* Iterator;
          typedef int  value_t;
     private:
          Iterator _begin;
          Iterator _end;
     public:
          Item(Iterator vbegin, Iterator vend) : _begin(vbegin), _end(vend){}
     public:
          int size(){return static_cast<int>(_end - _begin);}
          Iterator const begin(){return _begin;}
          Iterator const end(){return _end;}
          const value_t &operator[](const int &index)
          {
               assert(index >= 0 && index < size());
               return *(_begin + index);
          }
     };
public:
     Item operator[](const int &index)
     {
          assert(index >= 0 && index < _cache_base->nodes_num);

          const int &base_addr = _cache_base->adjt_elements_of_nodes_index[index * 2 + 0];
          const int &number = _cache_base->adjt_elements_of_nodes_index[index * 2 + 1];

          return Item(_cache_base->adjt_elements_of_nodes + base_addr,
               _cache_base->adjt_elements_of_nodes + base_addr + number);
     }
     int Count(){return _cache_base->nodes_num;}
};

class TFemCoreCacheN2N
{
     TFemCoreCacheBase *_cache_base;
public:
     TFemCoreCacheN2N(TFemCoreCacheBase *cache_base) : _cache_base(cache_base)
     {
          assert(cache_base);
     }
	 TFemCoreCacheN2N() : _cache_base(NULL){}
	 void Attach(TFemCoreCacheBase *cache_base)
	 {
		 _cache_base = cache_base;
		 assert(cache_base);
	 }
public:
     class Item
     {
     public:
          typedef int* Iterator;
          typedef int  value_t;
     private:
          Iterator _begin;
          Iterator _end;
     public:
          Item(Iterator vbegin, Iterator vend) : _begin(vbegin), _end(vend){}
     public:
          int size(){return static_cast<int>(_end - _begin);}
          Iterator const begin(){return _begin;}
          Iterator const end(){return _end;}
          const value_t &operator[](const int &index)
          {
               assert(index >= 0 && index < size());
               return *(_begin + index);
          }
     };
public:
     Item operator[](const int &index)
     {
          assert(index >= 0 && index < _cache_base->nodes_num);

          const int &base_addr = _cache_base->adjt_nodes_of_nodes_index[index * 2 + 0];
          const int &number = _cache_base->adjt_nodes_of_nodes_index[index * 2 + 1];

          return Item(_cache_base->adjt_nodes_of_nodes + base_addr,
               _cache_base->adjt_nodes_of_nodes + base_addr + number);
     }
     int Count(){return _cache_base->nodes_num;}
};

class TFemCoreCacheE2E
{
     TFemCoreCacheBase *_cache_base;
public:
     TFemCoreCacheE2E(TFemCoreCacheBase *cache_base) : _cache_base(cache_base)
     {
          assert(cache_base);
     }
	 TFemCoreCacheE2E() : _cache_base(NULL){}
	 void Attach(TFemCoreCacheBase *cache_base)
	 {
		 _cache_base = cache_base;
		 assert(cache_base);
	 }
public:
     class Item
     {
     public:
          struct  value_t
          {
               int first;
               int second;
               int elem_id;
          };
          typedef value_t* Iterator;
     private:
          Iterator _begin;
          Iterator _end;
     public:
          Item(Iterator vbegin, Iterator vend) : _begin(vbegin), _end(vend){}
     public:
          int size(){return static_cast<int>(_end - _begin);}
          Iterator const begin(){return _begin;}
          Iterator const end(){return _end;}
          const value_t &operator[](const int &index)
          {
               assert(index >= 0 && index < size());
               return *(_begin + index);
          }
     };
public:
     Item operator[](const int &index)
     {
          assert(index >= 0 && index < _cache_base->elements_num);

          const int &base_addr = _cache_base->adjt_elements_of_elements_index[index * 2 + 0];
          const int &number = _cache_base->adjt_elements_of_elements_index[index * 2 + 1];

          return Item((Item::Iterator)(_cache_base->adjt_elements_of_elements + base_addr),
               (Item::Iterator)(_cache_base->adjt_elements_of_elements + base_addr + number * 3));
     }
     int Count(){return _cache_base->elements_num;}
};

struct TFemCore1dCacheBase  
{
     double *nodes;
     int nodes_num;
     
     int *elements;
     int elements_num;

     int *adjt_elements_of_nodes;
     int *adjt_elements_of_nodes_index;

     int *adjt_nodes_of_nodes;
     int *adjt_nodes_of_nodes_index;

     int *adjt_elements_of_elements;
     int *adjt_elements_of_elements_index;
};

class TFemCore1dCachePoint
{
     TFemCore1dCacheBase *_cache_base;
public:
     struct Item
     {
          double x;
          double y;
          double z;
     };
public:
     TFemCore1dCachePoint(TFemCore1dCacheBase *cache_base) : _cache_base(cache_base)
     {
          assert(cache_base);
     }
	 TFemCore1dCachePoint() : _cache_base(NULL){}
	 void Attach(TFemCore1dCacheBase *cache_base)
	 {
		 _cache_base = cache_base;
	 }
public:
     const Item &operator[](const int &index)
     {
          assert(index >= 0 && index < _cache_base->nodes_num);
          Item *ptr_pt = (Item*)(_cache_base->nodes + index * 3);
          return *ptr_pt;
     }
     int Count(){return _cache_base->nodes_num;}
};

class TFemCore1dCacheElement
{
     TFemCore1dCacheBase *_cache_base;
public:
     struct Item
     {
          int n1;
          int n2;
     };
public:
     TFemCore1dCacheElement(TFemCore1dCacheBase *cache_base) 
		 : _cache_base(cache_base)
     {
          assert(cache_base);
     }
	 TFemCore1dCacheElement() : _cache_base(NULL){}
	 void Attach(TFemCore1dCacheBase *cache_base)
	 {
		 _cache_base = cache_base;
		 assert(cache_base);
	 }
public:
     const Item &operator[](const int &index)
     {
          assert(index >= 0 && index < _cache_base->elements_num);
          Item *ptr_elem = (Item*)(_cache_base->elements + index * 2);
          return *ptr_elem;
     }
     int Count(){return _cache_base->elements_num;}
};

class TFemCore1dCacheN2E
{
     TFemCore1dCacheBase *_cache_base;
public:
     TFemCore1dCacheN2E(TFemCore1dCacheBase *cache_base) : _cache_base(cache_base)
     {
          assert(cache_base);
     }
	 TFemCore1dCacheN2E() : _cache_base(NULL){}
	 void Attach(TFemCore1dCacheBase *cache_base)
	 {
		 _cache_base = cache_base;
		 assert(cache_base);
	 }
public:
     class Item
     {
     public:
          typedef int* Iterator;
          typedef int  value_t;
     private:
          Iterator _begin;
          Iterator _end;
     public:
          Item(Iterator vbegin, Iterator vend) : _begin(vbegin), _end(vend){}
     public:
          int size(){return static_cast<int>(_end - _begin);}
          Iterator const begin(){return _begin;}
          Iterator const end(){return _end;}
          const value_t &operator[](const int &index)
          {
               assert(index >= 0 && index < size());
               return *(_begin + index);
          }
     };
public:
     Item operator[](const int &index)
     {
          assert(index >= 0 && index < _cache_base->nodes_num);

          const int &base_addr = _cache_base->adjt_elements_of_nodes_index[index * 2 + 0];
          const int &number = _cache_base->adjt_elements_of_nodes_index[index * 2 + 1];

          return Item(_cache_base->adjt_elements_of_nodes + base_addr,
               _cache_base->adjt_elements_of_nodes + base_addr + number);
     }
     int Count(){return _cache_base->nodes_num;}
};

class TFemCore1dCacheN2N
{
     TFemCore1dCacheBase *_cache_base;
public:
     TFemCore1dCacheN2N(TFemCore1dCacheBase *cache_base) : _cache_base(cache_base)
     {
          assert(cache_base);
     }
	 TFemCore1dCacheN2N() : _cache_base(NULL){}
	 void Attach(TFemCore1dCacheBase *cache_base)
	 {
		 _cache_base = cache_base;
		 assert(cache_base);
	 }
public:
     class Item
     {
     public:
          typedef int* Iterator;
          typedef int  value_t;
     private:
          Iterator _begin;
          Iterator _end;
     public:
          Item(Iterator vbegin, Iterator vend) : _begin(vbegin), _end(vend){}
     public:
          int size(){return static_cast<int>(_end - _begin);}
          Iterator const begin(){return _begin;}
          Iterator const end(){return _end;}
          const value_t &operator[](const int &index)
          {
               assert(index >= 0 && index < size());
               return *(_begin + index);
          }
     };
public:
     Item operator[](const int &index)
     {
          assert(index >= 0 && index < _cache_base->nodes_num);

          const int &base_addr = _cache_base->adjt_nodes_of_nodes_index[index * 2 + 0];
          const int &number = _cache_base->adjt_nodes_of_nodes_index[index * 2 + 1];

          return Item(_cache_base->adjt_nodes_of_nodes + base_addr,
               _cache_base->adjt_nodes_of_nodes + base_addr + number);
     }
     int Count(){return _cache_base->nodes_num;}
};

class TFemCore1dCacheE2E
{
     TFemCore1dCacheBase *_cache_base;
public:
     TFemCore1dCacheE2E(TFemCore1dCacheBase *cache_base) : _cache_base(cache_base)
     {
          assert(cache_base);
     }
	 TFemCore1dCacheE2E() : _cache_base(NULL){}
	 void Attach(TFemCore1dCacheBase *cache_base)
	 {
		 _cache_base = cache_base;
		 assert(cache_base);
	 }
public:
     class Item
     {
     public:
          struct  value_t
          {
               int first;
               int second;
               int elem_id;
          };
          typedef value_t* Iterator;
     private:
          Iterator _begin;
          Iterator _end;
     public:
          Item(Iterator vbegin, Iterator vend) : _begin(vbegin), _end(vend){}
     public:
          int size(){return static_cast<int>(_end - _begin);}
          Iterator const begin(){return _begin;}
          Iterator const end(){return _end;}
          const value_t &operator[](const int &index)
          {
               assert(index >= 0 && index < size());
               return *(_begin + index);
          }
     };
public:
     Item operator[](const int &index)
     {
          assert(index >= 0 && index < _cache_base->elements_num);

          const int &base_addr = _cache_base->adjt_elements_of_elements_index[index * 2 + 0];
          const int &number = _cache_base->adjt_elements_of_elements_index[index * 2 + 1];

          return Item((Item::Iterator)(_cache_base->adjt_elements_of_elements + base_addr),
               (Item::Iterator)(_cache_base->adjt_elements_of_elements + base_addr + number * 3));
     }
     int Count(){return _cache_base->elements_num;}
};

}//namespace fem

#endif
