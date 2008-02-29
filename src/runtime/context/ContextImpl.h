/**
 * @copyright
 * ========================================================================
 *  Copyright FLWOR Foundation
 * ========================================================================
 *
 * @author Sorin Nasoi (sorin.nasoi@ipdevel.ro)
 * @file runtime/context/ContextImpl.h
 *
 */

#ifndef XQP_CONTEXT_IMPL_H
#define XQP_CONTEXT_IMPL_H

#include "common/shared_types.h"
#include "runtime/base/narybase.h"

namespace xqp
{

/*______________________________________________________________________
 *
 * 16 Context Functions
 *_______________________________________________________________________*/

/*
 * 16.1 fn:position
 *-----------------------*/
  NARY_ITER(FnPositionIterator);

 /*
  * 16.2 fn:last
  *-----------------------*/
  NARY_ITER(FnLastIterator);

 /*
  * 16.3 fn:current-dateTime
  *-----------------------*/
  NARY_ITER(FnCurrentDateTimeIterator);

 /*
  * 16.4 fn:current-date
  *-----------------------*/
  NARY_ITER(FnCurrentDateIterator);

 /*
  * 16.5 fn:current-time
  *-----------------------*/
  NARY_ITER(FnCurrentTimeIterator);

 /*
  * 16.6 fn:implicit-timezone
  *-----------------------*/
  NARY_ITER(FnImplicitTimezoneIterator);

 /*
  * 16.7 fn:default-collation
  *-----------------------*/
  NARY_ITER(FnDefaultCollationIterator);

 /*
  * 16.8 fn:static-base-uri
  *-----------------------*/
  NARY_ITER(FnStaticBaseURIIterator);

  
} /* namespace xqp */
#endif /*XQP_CONTEXT_IMPL_H*/
