/**
 * @copyright
 * ========================================================================
 *  Copyright FLWOR Foundation
 * ========================================================================
 *
 * @author Sorin Nasoi (sorin.nasoi@ipdevel.ro)
 * @file runtime/context/ContextImpl.cpp
 *
 */
 
#include "system/globalenv.h"
#include "system/zorba.h"
#include "runtime/context/ContextImpl.h"
#include "store/api/item_factory.h"
#include "system/zorba_engine.h"
#include "context/dynamic_context.h"
#include "context/static_context.h"
#include "util/datetime/parse.h"

namespace xqp
{
/**
 *______________________________________________________________________
 *
 * 16.1 fn:position
 *
 * fn:position() as xs:integer
 *_______________________________________________________________________*/

/*begin class FnPositionIterator */
Item_t
FnPositionIterator::nextImpl(PlanState& planState) const
{
  Item_t itemArg;
  unsigned long position=0;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  position = ZORBA_FOR_CURRENT_THREAD()->get_base_dynamic_context()->context_position();
  STACK_PUSH( Zorba::getItemFactory()->createInteger(Integer::parseLong(position)), state );

  STACK_END();
}
/*end class FnPositionIterator */

/**
 *______________________________________________________________________
 *
 * 16.2 fn:last
 *
 * fn:last() as xs:integer
 *_______________________________________________________________________*/

/*begin class FnLastIterator */
Item_t
FnLastIterator::nextImpl(PlanState& planState) const
{
  Item_t itemArg;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

//     position = ZORBA_FOR_CURRENT_THREAD()->get_base_dynamic_context()->context_position();
  STACK_PUSH( Zorba::getItemFactory()->createInteger(Integer::parseLong(2)), state );

  STACK_END();
}
/*end class FnLastIterator */

/**
 *______________________________________________________________________
 *
 * 16.3 fn:current-dateTime
 *
 * fn:current-dateTime() as xs:dateTime
 *_______________________________________________________________________*/

/*begin class FnCurrentDateTimeIterator */
Item_t
FnCurrentDateTimeIterator::nextImpl(PlanState& planState) const
{
  Item_t itemRes;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  itemRes = ZORBA_FOR_CURRENT_THREAD()->get_base_dynamic_context()->get_execution_date_time();
  if( NULL != itemRes )
    STACK_PUSH( itemRes, state );

  STACK_END();
}
/*end class FnCurrentDateTimeIterator */

/**
 *______________________________________________________________________
 *
 * 16.4 fn:current-date
 *
 * fn:current-date() as xs:date
 *_______________________________________________________________________*/

/*begin class FnCurrentDateIterator */
Item_t
FnCurrentDateIterator::nextImpl(PlanState& planState) const
{
  Item_t itemRes;
  xqp_date d;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  itemRes = ZORBA_FOR_CURRENT_THREAD()->get_base_dynamic_context()->get_execution_date_time();
  if( NULL != itemRes )
  {
    d = itemRes->getDateTimeValue()->getDate();
    STACK_PUSH( Zorba::getItemFactory()->createDate(d), state );
  }

  STACK_END();
}
/*end class FnCurrentDateIterator */

/**
 *______________________________________________________________________
 *
 * 16.5 fn:current-time
 *
 * fn:current-time() as xs:time
 *_______________________________________________________________________*/

/*begin class FnCurrentTimeIterator */
Item_t
FnCurrentTimeIterator::nextImpl(PlanState& planState) const
{
  Item_t itemRes;
  xqp_time t;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  itemRes = ZORBA_FOR_CURRENT_THREAD()->get_base_dynamic_context()->get_execution_date_time();
  if( NULL != itemRes )
  {
    t = itemRes->getDateTimeValue()->getTime();
    STACK_PUSH( Zorba::getItemFactory()->createTime(t), state );
  }

  STACK_END();
}
/*end class FnCurrentTimeIterator */

/**
 *______________________________________________________________________
 *
 * 16.6 fn:implicit-timezone
 *
 * fn:implicit-timezone() as xs:dayTimeDuration
 *_______________________________________________________________________*/

/*begin class FnImplicitTimezoneIterator */
Item_t
FnImplicitTimezoneIterator::nextImpl(PlanState& planState) const
{
  long tz;
  xqp_dayTimeDuration dtd;
  DurationBase_t db;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  tz = ZORBA_FOR_CURRENT_THREAD()->get_base_dynamic_context()->get_implicit_timezone();
  db = new DayTimeDuration( tz < 0, 0, abs<int>(tz), 0 , 0, 0);
  
  STACK_PUSH( Zorba::getItemFactory()->createDuration(db), state );

  STACK_END();
}
/*end class FnImplicitTimezoneIterator */

/**
 *______________________________________________________________________
 *
 * 16.7 fn:default-collation
 *
 * fn:default-collation() as xs:string
 *_______________________________________________________________________*/

/*begin class FnDefaultCollationIterator */
Item_t
FnDefaultCollationIterator::nextImpl(PlanState& planState) const
{
  xqp_string strColUri;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  strColUri = ZORBA_FOR_CURRENT_THREAD()->get_static_context()->default_collation_uri();
  STACK_PUSH( Zorba::getItemFactory()->createString(strColUri.getStore()), state );

  STACK_END();
}
/*end class FnDefaultCollationIterator */

/**
 *______________________________________________________________________
 *
 * 16.8 fn:static-base-uri
 *
 * fn:static-base-uri() as xs:anyURI?
 *_______________________________________________________________________*/

/*begin class FnStaticBaseURIIterator */
Item_t
FnStaticBaseURIIterator::nextImpl(PlanState& planState) const
{
  xqp_string strBaseUri;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  strBaseUri = ZORBA_FOR_CURRENT_THREAD()->get_static_context()->baseuri();
  STACK_PUSH( Zorba::getItemFactory()->createString(strBaseUri.getStore()), state );

  STACK_END();
}
/*end class FnStaticBaseURIIterator */

}/* namespace xqp */
