//=============================================================================
#include <Class_Tamplate.h>
//=================================================
//*****************************************************************************
//*** Class TFifo
//*****************************************************************************
//=============================================================================
// Constructor
//=======================================
template <class Type>
TFifo<Type>::TFifo(Word sz):max_size(sz),size(0),front_ind(0),end_ind(0)
{
  Buf = new Type[sz];
}
//=============================================================================
// Destructor
//=======================================
template <class Type>
TFifo<Type>::~TFifo()
{
  clear();
  delete[] Buf;
}
//=============================================================================
// 
//=======================================
template <class Type>
bool TFifo<Type>::empty() const
{
  return (size == 0);
}
//=============================================================================
// 
//=======================================
template <class Type>
inline void TFifo<Type>::clear(void) 
{
  while(size)
  {
    Buf[size-- -1] = 0;
    CLEAR_WDT;
  }
  front_ind = end_ind = 0;
}
//=============================================================================
// 
//=======================================
template <class Type>
inline void TFifo<Type>::push(const Type &elem)
{
    Buf[end_ind] = elem;
    if (++end_ind == max_size)
      end_ind = 0;
    
    if ( size == max_size)
    {
      if (++front_ind == max_size)
        front_ind = 0;
    }
    else
      size++;  
}
//=============================================================================
// 
//=======================================
template <class Type>
inline Type TFifo<Type>::pop(void)
{
  CRITICAL_SEC_ON
  Type tmp = (Type)0;
  if (size > 0)
  {
    tmp = Buf[front_ind];
    if (++front_ind == max_size)
        front_ind = 0;
    size--;
  }
  else
  {
    front_ind = end_ind = 0;
  }
  CRITICAL_SEC_OFF
  return tmp;
}
//=============================================================================
////////////////////////////////////////////////////////////////////////
// КЛАСС Tlist
////////////////////////////////////////////////////////////////////////
/**********************************************************************************
 конструктор по умолчанию
**********************/
template <class elemType>
Tlist<elemType>::Tlist():_at_front(0),_at_end(0),_at_interator(0),_size(0),interator(0)
{    
}
/**********************************************************************************
Копирующий конструктор
**********************/
template <class elemType>
inline Tlist<elemType>::Tlist(const Tlist<elemType> &rhs):_at_front(0),_at_end(0),_size(0)
{
    this->insert_all(rhs);
}
/***********************************************************************************/
template <class elemType>
inline Tlist<elemType>::~Tlist()
{
   clear(); 
}

/*********************************************************************************/
template <class elemType>
void Tlist<elemType>::insert_all ( const Tlist<elemType> &rhs )
{
   Tlist_item<elemType> *pt = rhs._at_front;
   while ( pt ) 
   {
     CLEAR_WDT;
      this->push_back( pt->value() );
      pt = pt->next();
   }
}
/*********************************************************************************/
template <class elemType>
inline int Tlist<elemType>::size() 
{
  return this->_size; 
}
/*********************************************************************************/
template <class elemType>
inline void Tlist<elemType>::bump_up_size() 
{ 
  ++(this->_size); 
}
/*********************************************************************************/
template <class elemType>
bool Tlist<elemType>::empty() 
{ 
  if ( _size == 0)
    return true;
  else
    return false;
}
/*********************************************************************************/
template <class elemType> 
inline elemType Tlist<elemType>::operator[]( int ix ) const
{
  if( ix >= _size)
    return _at_end->value();
  
  Tlist_item<elemType> *tmp;
  tmp = _at_front;
  HOLD_WDT;
  for(int i=0;i<ix;i++)
  {     
    tmp = tmp->next();
  }
  CLEAR_WDT;
  return  tmp->value();
}
/*********************************************************************************/
template <class elemType> 
inline Tlist<elemType>& Tlist<elemType>::operator=( const Tlist<elemType> &newlist ) 
{
  this->clear();
  HOLD_WDT;
  for( int i=0; i < newlist._size; i++)
  { 
    this->push_back( newlist[i] );
  }
  CLEAR_WDT;
  return *this;
}
/*********************************************************************************/
template <class elemType>   
inline void Tlist<elemType>::set_interator(int value)
{
  interator = value;
  if ( interator > (_size-1) )
  {  interator = 0; }
  
  Tlist_item<elemType> *tmp = _at_front;
  HOLD_WDT;
  for(int i=0; i<interator; i++)
  {  
    tmp = tmp->next();
  }
  CLEAR_WDT;
  _at_interator = tmp;
}  
/*********************************************************************************/
template <class elemType> 
inline elemType Tlist<elemType>::next() 
{
  if(_size==0)
    return 0;
  
  Tlist_item<elemType> *tmp;
  tmp = _at_front;
 
  HOLD_WDT;
  for(int i=0; i<interator; i++)
  {  
    tmp = tmp->next();
  }
  CLEAR_WDT;
  if ( ++interator > (_size-1) )
  {  interator = 0; }
  return  tmp->value();
}
/*********************************************************************************/
template <class elemType> 
inline elemType Tlist<elemType>::foreach_next() 
{
  if(_size==0)
    return 0;
  
  if ( interator++ > (_size-1) )
  {  
    interator = 0; 
    _at_interator =  _at_front;
    return 0;
  }
  Tlist_item<elemType> *tmp = _at_interator;
  _at_interator = _at_interator->next();
  return  tmp->value();
}
/*********************************************************************************/
template <class elemType>
inline void Tlist<elemType>::bump_down_size() 
{ 
  --(this->_size); 
}
/*********************************************************************************/
/*template <class elemType>
inline void Tlist<elemType>::insert(Tlist_item<elemType> *ptr, elemType value)
{
      if ( ptr == 0 )//рассмотрим передачу нулевого указателя как запрос на вставку элемента перед первым в списке
         push_front( value );
      else 
      {
         bump_up_size();
         new Tlist_item<elemType>( value, ptr );
      }
} */
/*********************************************************************************/
template <class elemType>
inline void Tlist<elemType>::push_front( elemType value )
{
  CRITICAL_SEC_ON
    Tlist_item<elemType> *ptr;
    /*try
    {
      ptr = new Tlist_item<elemType>( value );
    }
    catch (bad_alloc& ba)
    { 
      CRITICAL_SEC_OFF
      return;
    }*/
      ptr = new Tlist_item<elemType>( value );
      if ( !_at_front )
        _at_front = _at_end = ptr;
      else 
      {
        ptr->next( _at_front );
        _at_front = ptr;
       }
      bump_up_size();
  CRITICAL_SEC_OFF
}
/*********************************************************************************/
template <class elemType>
inline void Tlist<elemType>::push_back( elemType value )
{
  CRITICAL_SEC_ON
  Tlist_item<elemType> *ptr;
    /*try
    {
         ptr = new Tlist_item<elemType>( value );    
    }
    catch (bad_alloc& ba)
    {
      CRITICAL_SEC_OFF
      return;
    }*/
    ptr = new Tlist_item<elemType>( value );    
    if ( _at_end == 0)
    {   _at_end = _at_front = ptr;}
    else 
    {
        _at_end->next(ptr);
        _at_end = ptr;
    }
    bump_up_size();
  CRITICAL_SEC_OFF
}
/**********************************************************************************
find() ищет значение в списке. Если элемент с указанным значением найден, 
возвращается его адрес, иначе find() возвращает 0
***************************************************/ 
template <class elemType>
Tlist_item<elemType>* Tlist<elemType>::find( elemType value )
{
   Tlist_item<elemType> *ptr = _at_front;
   while ( ptr )
   {
     CLEAR_WDT;
      if ( ptr->value() == value )
      break;
      ptr = ptr->next();
   }
   return ptr;
}
/***************************************************/ 
template <class elemType>
elemType* Tlist<elemType>::front()
{
   return _at_front->ptr();
}
/***************************************************/ 
template <class elemType>
elemType* Tlist<elemType>::back()
{
   return _at_end->ptr();
}
/*********************************************************************************/
template <class elemType>
inline void Tlist<elemType>::pop_front()
{
  CRITICAL_SEC_ON
   Tlist_item<elemType> *ptr; 
   if ( _at_front ) 
   {
      ptr = _at_front;
      if (_size == 1)
        _at_front = _at_end = 0;
      else   
        _at_front = _at_front->next();
      bump_down_size() ;
      delete ptr;
   }
  CRITICAL_SEC_OFF
}
/*********************************************************************************/
/*template <class elemType>
inline void Tlist<elemType>::pop_back()
{
  CRITICAL_SEC_ON
   if ( _at_end ) 
   {
      Tlist_item<elemType> *ptr = _at_end;
      if (_size == 1)
        _at_front = _at_end = 0;
      else   
        _at_end = _at_front->prew();
      bump_down_size() ;
      delete ptr;
   }
  CRITICAL_SEC_OFF
}*/
/**********************************************************************************
clear() вызывает pop_front() до тех пор, пока все элементы не будут удалены:
***********************************************************************************/ 
template <class elemType>
void Tlist<elemType>::clear()
{
  while ( _at_front )
  {
    CLEAR_WDT;
    pop_front();
  }
  _size = 0;
  _at_front = _at_end = 0;
}
/********************************************************************************
Общая функция remove() также использует pop_front() для обработки специального
случая, когда удаляемый элемент (элементы) находится в начале списка. Для удаления
из середины списка используется итерация. У элемента, предшествующего удаляемому, 
необходимо модифицировать указатель _next 
УАЛЕНИЕ ПО-ЗНАЧЕНИЮ, ВОЗВРАЩАЕТ КОЛ-ВО УДАЛЕННЫХ ЭЛЕМЕНТОВ:  
******************************************/
template <class elemType>
int Tlist<elemType>::remove( elemType value )
{
  //CRITICAL_SEC_ON
 
   Tlist_item<elemType>   *plist = _at_front;
   int                    elem_cnt = 0;
   
    while ( plist && ( plist->value() == value) )//если в начале списка
    {
      CLEAR_WDT;
      plist = plist->next();
      pop_front();
      ++elem_cnt;
    }
    
    if ( ! plist )  //если список пуст, то возврат кол-ва удаленных
      return elem_cnt;
    
    Tlist_item<elemType> *prev = plist;
    plist = plist->next();
    while ( plist ) 
    {
      CLEAR_WDT;
      if ( plist->value() == value ) 
      {
        prev->next( plist->next() );
        delete plist;
        ++elem_cnt;
        bump_down_size();
        plist = prev->next();
        if ( ! plist ) 
        {
          _at_end = prev;
          return elem_cnt;
        }
       }
       else 
       {
          prev = plist;
          plist = plist->next();
       }
    }
    return elem_cnt;
}
/******************************************/
/*template <class elemType>
void Tlist<elemType>::erase( int index )
{
   
   
   if(index == 0)
   {
     pop_front();
   }
   else
   {
   //  Tlist_item<elemType>  plist = _at_front;
     
   }
    
}*/
/*********************************************************************************/
template <class elemType>
void Tlist<elemType>::concat( const Tlist &i1 )
{
   Tlist_item<elemType> *ptr = i1._at_front;
   while ( ptr ) 
   {
      CLEAR_WDT;
      push_back( ptr->value() );
      ptr = ptr->next();
   }
}
/*********************************************************************************/
template <class elemType>
void Tlist<elemType>::reverse()
{
   Tlist_item<elemType> *ptr = _at_front;
   Tlist_item<elemType> *prev = 0;
   _at_front = _at_end;
   _at_end = ptr;
   while ( ptr != _at_front )
  {
     CLEAR_WDT;
      Tlist_item<elemType> *tmp = ptr->next();
      ptr->next( prev );
      prev = ptr;
      ptr = tmp;
   }
  _at_front->next( prev );
}
/*********************************************************************************/
////////////////////////////////////////////////////////////////////////
// КЛАСС Tlist_item
////////////////////////////////////////////////////////////////////////
/**********************************************************************************
если передан  item - то создаём новый элемент сразу после item(item->next=новый эл-т)
*/
template <class elemType>  
inline Tlist_item<elemType>::Tlist_item( elemType &value ):_value(value),_next(0)//,_back(0)
{ 
    /*  if ( item == 0 )
        _next = 0;
      else 
      {
        _next = item->_next;
        //item->_next = this;
      } */
}
/*********************************************************************************/
template <class elemType>  
inline elemType Tlist_item<elemType>::value()
{
  return _value;
}
/*********************************************************************************/
template <class elemType>
inline void Tlist_item<elemType>::value(elemType &new_value) 
{
  _value = new_value; 
}
/*********************************************************************************/
template <class elemType>
inline Tlist_item<elemType>* Tlist_item<elemType>::next()
{
    return _next;
}
/*********************************************************************************/
template <class elemType>
inline void Tlist_item<elemType>::next(Tlist_item<elemType>* ptr)
{
  _next = ptr;
}
/*********************************************************************************/
template <class elemType>  
inline elemType *Tlist_item<elemType>::ptr()
{
  return &_value;
}
/*********************************************************************************/
