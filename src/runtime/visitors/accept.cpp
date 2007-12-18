/**
 * For simplicity, this file contains the accept implementation of all PlanIterators.
 * The majority of accept's can be implemented with a macro.
 **/

#include "runtime/visitors/planitervisitor.h"

#include "runtime/core/constructors.h"
#include "runtime/core/item_iterator.h"
#include "runtime/core/path_iterators.h"
#include "runtime/core/nodeid_iterators.h"
#include "runtime/core/sequencetypes.h"
#include "runtime/booleans/BooleanImpl.h"
#include "runtime/strings/StringsImpl.h"
#include "runtime/numerics/NumericsImpl.h"
#include "runtime/accessors/AccessorsImpl.h"
#include "runtime/sequences/SequencesImpl.h"
#include "runtime/dateTime/DurationsDatesTimes.h"

#define NOARY_ACCEPT(type) \
  void type::accept(PlanIterVisitor& v) const { \
    v.beginVisit(*this); \
    v.endVisit(*this); \
  }
  
#define UNARY_ACCEPT(type) \
  void type::accept(PlanIterVisitor& v) const { \
    v.beginVisit(*this); \
    theChild->accept(v); \
    v.endVisit(*this); \
  }
  
#define BINARY_ACCEPT(type) \
  void type::accept(PlanIterVisitor& v) const { \
    v.beginVisit(*this); \
    theChild0->accept(v); \
    theChild1->accept(v); \
    v.endVisit(*this); \
  }
  
#define NARY_ACCEPT(type) \
  void type::accept(PlanIterVisitor& v) const { \
    v.beginVisit(*this); \
    std::vector<PlanIter_t>::const_iterator iter = theChildren.begin(); \
    for ( ; iter != theChildren.end(); ++iter ) { \
      ( *iter )->accept ( v ); \
    } \
    v.endVisit(*this); \
  }
    
    

namespace xqp {
  
  // noary iterators (iterators without children)
  NOARY_ACCEPT(RefIterator);
  NOARY_ACCEPT(ZorNumGen);
  NOARY_ACCEPT(EmptyIterator);
  NOARY_ACCEPT(SingletonIterator);
  NOARY_ACCEPT(var_iterator);
  
  // unary iterators
  UNARY_ACCEPT(NodeDistinctIterator);
  UNARY_ACCEPT(NodeSortIterator);
  UNARY_ACCEPT(KindTestIterator);
  UNARY_ACCEPT(NameTestIterator);
  UNARY_ACCEPT(SelfAxisIterator);
  UNARY_ACCEPT(AttributeAxisIterator);
  UNARY_ACCEPT(ParentAxisIterator);
  UNARY_ACCEPT(AncestorAxisIterator);
  UNARY_ACCEPT(AncestorSelfAxisIterator);
  UNARY_ACCEPT(RSiblingAxisIterator);
  UNARY_ACCEPT(LSiblingAxisIterator);
  UNARY_ACCEPT(ChildAxisIterator);
  UNARY_ACCEPT(DescendantAxisIterator);
  UNARY_ACCEPT(DescendantSelfAxisIterator);
  UNARY_ACCEPT(PrecedingAxisIterator);
  UNARY_ACCEPT(FollowingAxisIterator);
  UNARY_ACCEPT(FnBooleanIterator);
  UNARY_ACCEPT(InstanceOfIterator);
  UNARY_ACCEPT(CodepointsToStringIterator);
  UNARY_ACCEPT(StringToCodepointsIterator);
  UNARY_ACCEPT(StringLengthIterator);
  UNARY_ACCEPT(NormalizeSpaceIterator);
  UNARY_ACCEPT(UpperCaseIterator);
  UNARY_ACCEPT(LowerCaseIterator);
  UNARY_ACCEPT(EncodeForUriIterator);
  UNARY_ACCEPT(IriToUriIterator);
  UNARY_ACCEPT(EscapeHtmlUriIterator);
  UNARY_ACCEPT(ElementContentIterator);
  UNARY_ACCEPT(FnDataIterator);
  UNARY_ACCEPT(FnAbsIterator);
  UNARY_ACCEPT(FnCeilingIterator);
  UNARY_ACCEPT(FnFloorIterator);
  UNARY_ACCEPT(FnRoundIterator);
  UNARY_ACCEPT(FnRootIterator);
  UNARY_ACCEPT(EnclosedIterator);
  UNARY_ACCEPT(FnExistsIterator);
  UNARY_ACCEPT(FnEmptyIterator);
  UNARY_ACCEPT(FnDistinctValuesIterator);
  UNARY_ACCEPT(FnZeroOrOneIterator);
  UNARY_ACCEPT(FnOneOrMoreIterator);
  UNARY_ACCEPT(FnExactlyOneIterator);
  UNARY_ACCEPT(FnCountIterator);
  UNARY_ACCEPT(FnAvgIterator);
  UNARY_ACCEPT(FnDocIterator);
  UNARY_ACCEPT(OpNumericUnaryIterator);
  UNARY_ACCEPT(TextIterator);
  UNARY_ACCEPT(CommentIterator);
  UNARY_ACCEPT(DocumentIterator);
  UNARY_ACCEPT(DocumentContentIterator);
  UNARY_ACCEPT(DocFilterIterator);
  UNARY_ACCEPT(CastIterator);
  
  // binary iterators
  BINARY_ACCEPT(CodepointEqualIterator);
  BINARY_ACCEPT(StringJoinIterator);
  BINARY_ACCEPT(LogicIterator);
  BINARY_ACCEPT(CompareIterator);
  BINARY_ACCEPT(FnIndexOfIterator);
  BINARY_ACCEPT(FnRemoveIterator);
  BINARY_ACCEPT(OpToIterator);
  BINARY_ACCEPT(FnRoundHalfToEvenIterator);
  BINARY_ACCEPT(OpDurationEqualIterator);
  BINARY_ACCEPT(AttributeIterator);
  
  // nary iterators
  NARY_ACCEPT(NormalizeUnicodeIterator);
  NARY_ACCEPT(CompareStrIterator);
  NARY_ACCEPT(ConcatStrIterator);
  NARY_ACCEPT(SubstringIterator);
  NARY_ACCEPT(TranslateIterator);
  NARY_ACCEPT(ContainsIterator);
  NARY_ACCEPT(StartsWithIterator);
  NARY_ACCEPT(EndsWithIterator);
  NARY_ACCEPT(SubstringBeforeIterator);
  NARY_ACCEPT(SubstringAfterIterator);
  NARY_ACCEPT(FnConcatIterator);
  NARY_ACCEPT(FnInsertBeforeIterator);
  NARY_ACCEPT(FnSubsequenceIterator);
  NARY_ACCEPT(FnMaxIterator);
  NARY_ACCEPT(FnMinIterator);
  NARY_ACCEPT(FnSumIterator);
  
  // special iterators
  void ElementIterator::accept(PlanIterVisitor& v) const
  {
    v.beginVisit(*this);

    if (theQNameIter != 0)
      theQNameIter->accept(v);
    
    if (theAttributesIter != 0)
      theAttributesIter->accept(v);

    if (theChildrenIter != 0)
      theChildrenIter->accept(v);
    
    if (theNamespacesIter != 0)
      theNamespacesIter->accept(v);
    
    v.endVisit(*this);
  }
  
  void IfThenElseIterator::accept(PlanIterVisitor& v) const {
    v.beginVisit(*this);
    theCondIter->accept ( v );
    theThenIter->accept ( v );
    theElseIter->accept ( v );
    v.endVisit(*this);
  }
} /* namespace xqp */
