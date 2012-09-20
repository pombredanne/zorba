/*
 * Copyright 2006-2008 The FLWOR Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "stdafx.h"
#include <zorba/config.h>

#ifdef ZORBA_WITH_JSON


#include <sstream>

#include "system/globalenv.h"

#include "api/serialization/serializer.h"

#include "compiler/api/compilercb.h"

#include "runtime/json/jsoniq_functions.h"
#include "runtime/parsing_and_serializing/parsing_and_serializing.h"
#include "runtime/visitors/planiter_visitor.h"
#include "runtime/api/plan_iterator_wrapper.h"
#include "runtime/util/item_iterator.h"

#include "diagnostics/diagnostic.h"
#include "diagnostics/xquery_diagnostics.h"
#include "diagnostics/util_macros.h"

#include "zorba/internal/diagnostic.h"

#include "context/static_context.h"

#include "types/casting.h"
#include "types/typeimpl.h"
#include "types/typeops.h"
#include "types/root_typemanager.h"

#include "store/api/pul.h"
#include "store/api/item.h"
#include "store/api/item_factory.h"
#include "store/api/store.h"
#include "store/api/copymode.h"

#include <zorba/store_consts.h>

namespace zorba {

class CallParameters {
public:
  CallParameters(static_context* aSctx,
                 store::ItemFactory* aFactory,
                 const QueryLoc& aLoc)
    : theSctx(aSctx),
      theFactory(aFactory),
      theLoc(aLoc)
  {}

  static_context* theSctx;
  store::ItemFactory* theFactory;
  zstring thePrefix;
  store::Item_t theSerParams;
  XQueryDiagnostics* theDiag;
  const QueryLoc& theLoc;
};

const zstring XS_URI("http://www.w3.org/2001/XMLSchema");

/*******************************************************************************
  json:decode-from-roundtrip($items as json-item()*,
                             $options as object()) as structured-item()*
********************************************************************************/

void
parseQName(store::Item_t& aResult,
           const zstring& aQNameString,
           const zstring& aPrefix,
           store::ItemFactory* aFactory)
{
  // TODO there probably is a better solution somewhere
  if (aQNameString.substr(0, 3) == "xs:")
  {
    aFactory->createQName(aResult, XS_URI, "xs", aQNameString.substr(3));
  }
  else if (aQNameString.substr(0, 2) == "Q{")
  {
    zstring::size_type lPos = aQNameString.find('}');
    aFactory->createQName(aResult,
                          aQNameString.substr(2, lPos - 2),
                          aPrefix,
                          aQNameString.substr(lPos + 1));
  }
  else
  {
    aFactory->createQName(aResult, "", "", aQNameString);
  }
}

bool
JSONDecodeFromRoundtripIterator::decodeXDM(
  const store::Item_t& anObj,
  store::Item_t& aResult,
  CallParameters& someParams)
{
  store::Item_t lItem;

  zstring lTypeKey = someParams.thePrefix + "type";
  someParams.theFactory->createString(lItem, lTypeKey);
  store::Item_t lTypeValueItem = anObj->getObjectValue(lItem);
  if (lTypeValueItem.isNull())
  {
    return false;
  }

  zstring lValueKey = someParams.thePrefix + "value";
  someParams.theFactory->createString(lItem, lValueKey);
  store::Item_t lValueValueItem = anObj->getObjectValue(lItem);
  if (lValueValueItem.isNull())
  {
    return false;
  }

  zstring lTypeNameString;
  lTypeValueItem->getStringValue2(lTypeNameString);
  if (lTypeNameString == "node()")
  {
    store::LoadProperties lProperties;
    lProperties.setStoreDocument(false);
    if (lValueValueItem->isStreamable())
    {
      aResult = GENV.getStore().loadDocument(
            "", "", &(lValueValueItem->getStream()), lProperties);
    }
    else
    {
      zstring lXmlString;
      lValueValueItem->getStringValue2(lXmlString);
      std::istringstream lStream(lXmlString.c_str());
      aResult = GENV.getStore().loadDocument("", "", lStream, lProperties);
    }
  }
  else
  {
    store::Item_t lTypeQName;
    parseQName(lTypeQName, lTypeNameString, "", someParams.theFactory);
    if (lTypeQName->getLocalName() == "QName"
        && lTypeQName->getNamespace() == XS_URI)
    {
      zstring lPrefixKey = someParams.thePrefix + "prefix";
      someParams.theFactory->createString(lItem, lPrefixKey);
      store::Item_t lPrefixValue = anObj->getObjectValue(lItem);
      zstring lPrefixString;
      if (! lPrefixValue.isNull())
      {
        lPrefixValue->getStringValue2(lPrefixString);
      }
      zstring lValueValue;
      lValueValueItem->getStringValue2(lValueValue);
      parseQName(aResult, lValueValue, lPrefixString, someParams.theFactory);
    }
    else
    {
      TypeManager* lTypeMgr = someParams.theSctx->get_typemanager();
      xqtref_t lTargetType = lTypeMgr->create_named_type(
            lTypeQName.getp(), TypeConstants::QUANT_ONE, someParams.theLoc);
      namespace_context lTmpNsCtx(someParams.theSctx);
      GenericCast::castToAtomic(aResult,
                                lValueValueItem,
                                lTargetType.getp(),
                                lTypeMgr,
                                &lTmpNsCtx,
                                someParams.theLoc);
    }
  }
  return true;
}

bool
JSONDecodeFromRoundtripIterator::decodeObject(
  const store::Item_t& anObj,
  store::Item_t& aResult,
  CallParameters& someParams)
{
  if (decodeXDM(anObj, aResult, someParams))
  {
    return true;
  }

  std::vector<store::Item_t> newNames;
  std::vector<store::Item_t> newValues;
  bool modified = false;

  store::Item_t key;
  store::Item_t value;
  store::Item_t newValue;
  store::Iterator_t it = anObj->getObjectKeys();
  it->open();
  while (it->next(key))
  {
    newNames.push_back(key);
    value = anObj->getObjectValue(key);
    const bool gotNew = decodeItem(value, newValue, someParams);
    newValues.push_back(gotNew ? newValue : value);
    modified = modified || gotNew;
  }
  it->close();
  if (modified)
  {
    someParams.theFactory->createJSONObject(aResult, newNames, newValues);
    return true;
  }
  return false;
}

bool
JSONDecodeFromRoundtripIterator::decodeArray(
  const store::Item_t& anArray,
  store::Item_t& aResult,
  CallParameters& someParams)
{
  std::vector<store::Item_t> newItems;
  bool modified = false;

  store::Item_t item, newItem;
  store::Iterator_t it = anArray->getArrayValues();
  it->open();
  while (it->next(item))
  {
    const bool gotNew = decodeItem(item, newItem, someParams);
    newItems.push_back(gotNew ? newItem : item);
    modified = modified || gotNew;
  }
  it->close();
  if (modified)
  {
    someParams.theFactory->createJSONArray(aResult, newItems);
    return true;
  }
  return false;
}

bool
JSONDecodeFromRoundtripIterator::decodeItem(
  const store::Item_t& anItem,
  store::Item_t& aResult,
  CallParameters& someParams)
{
  if (anItem->isJSONObject())
  {
    return decodeObject(anItem, aResult, someParams);
  }
  else if (anItem->isJSONArray())
  {
    return decodeArray(anItem, aResult, someParams);
  }
  else
  {
    return false;
  }
}

bool
JSONDecodeFromRoundtripIterator::nextImpl(
  store::Item_t& aResult,
  PlanState& aPlanState) const
{
  store::Item_t lInput;
  CallParameters lParams(theSctx, GENV_ITEMFACTORY, loc);
  store::Item_t lDecParams;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, aPlanState);

  lParams.thePrefix = "Q{http://jsoniq.org/roundtrip}"; // TODO move into params
  lParams.theDiag = aPlanState.theCompilerCB->theXQueryDiagnostics;

  consumeNext(lInput, theChildren.at(0), aPlanState);

  // get decoding parameters
  if (theChildren.size() == 2
      && consumeNext(lDecParams, theChildren.at(1), aPlanState))
  {
    // the signature says that the second parameter has to be exactly one object
    store::Item_t lPrefixKey;
    zstring lPrefixName = "prefix";
    lParams.theFactory->createString(lPrefixKey, lPrefixName);
    store::Item_t lPrefixValue = lDecParams->getObjectValue(lPrefixKey);
    if (! lPrefixValue.isNull())
    {
      if (lPrefixValue->getTypeCode() != store::XS_STRING)
      {
        // TODO exception
        throw lPrefixValue;
      }
      lPrefixValue->getStringValue2(lParams.thePrefix);
    }
  }

  if (! decodeItem(lInput, aResult, lParams))
  {
    aResult = lInput;
  }

  STACK_PUSH (true, state);

  STACK_END(state);
}


/*******************************************************************************
  jn:encode-for-roundtrip($items as structured-item()*,
                          $options as object()) as json-item()*
********************************************************************************/
bool
JSONEncodeForRoundtripIterator::encodeObject(
  const store::Item_t& anObj,
  store::Item_t& aResult,
  CallParameters& someParams)
{
  std::vector<store::Item_t> newNames;
  std::vector<store::Item_t> newValues;
  bool modified = false;

  store::Item_t key;
  store::Item_t value;
  store::Item_t newValue;
  store::Iterator_t it = anObj->getObjectKeys();
  it->open();
  while (it->next(key))
  {
    newNames.push_back(key);
    value = anObj->getObjectValue(key);
    const bool gotNew = encodeItem(value, newValue, someParams);
    newValues.push_back(gotNew ? newValue : value);
    modified = modified || gotNew;
  }
  it->close();
  if (modified)
  {
    someParams.theFactory->createJSONObject(aResult, newNames, newValues);
    return true;
  }
  return false;
}

bool
JSONEncodeForRoundtripIterator::encodeArray(
  const store::Item_t& anArray,
  store::Item_t& aResult,
  CallParameters& someParams)
{
  std::vector<store::Item_t> newItems;
  bool modified = false;

  store::Item_t item, newItem;
  store::Iterator_t it = anArray->getArrayValues();
  it->open();
  while (it->next(item))
  {
    const bool gotNew = encodeItem(item, newItem, someParams);
    newItems.push_back(gotNew ? newItem : item);
    modified = modified || gotNew;
  }
  it->close();
  if (modified)
  {
    someParams.theFactory->createJSONArray(aResult, newItems);
    return true;
  }
  return false;
}

bool
JSONEncodeForRoundtripIterator::encodeAtomic(
  const store::Item_t& aValue,
  store::Item_t& aResult,
  CallParameters& someParams)
{
  store::SchemaTypeCode typeCode = aValue->getTypeCode();
  switch (typeCode) {
  case store::XS_DOUBLE:
  case store::XS_FLOAT:
    if (aValue->getBaseItem() == NULL
        && ! aValue->isNaN() && ! aValue->isPosOrNegInf())
    {
      // nothing to do - no modification necessary
      return false;
    }
    break;
  case store::XS_STRING:
  case store::XS_INTEGER:
  case store::XS_DECIMAL:
  case store::XS_BOOLEAN:
  case store::JS_NULL:
    if (aValue->getBaseItem() == NULL)
    {
      // nothing to do - no modification necessary
      return false;
    }
    break;
  default:
    break;
  }

  std::vector<store::Item_t> names(2);
  std::vector<store::Item_t> values(2);

  {
    const store::Item_t& typeName = aValue->getType();

    zstring typeKey = someParams.thePrefix + "type";
    const zstring ns = typeName->getNamespace();
    const zstring local = typeName->getLocalName();
    zstring typeValue = ns.compare(XS_URI)
        ? "Q{" + ns + "}" + local : "xs:" + local;

    someParams.theFactory->createString(names.at(0), typeKey);
    someParams.theFactory->createString(values.at(0), typeValue);
  }

  {
    zstring valueKey = someParams.thePrefix + "value";
    zstring valueValue;
    if (typeCode == store::XS_QNAME)
    {
      // QNames are a special case, as the prefix should be maintained as well
      // but it's not part of the EQName serialization
      zstring prefixValue = aValue->getPrefix();
      if (prefixValue.length() > 0)
      {
        zstring prefixKey = someParams.thePrefix + "prefix";
        store::Item_t lItem;
        someParams.theFactory->createString(lItem, prefixKey);
        names.push_back(lItem);
        someParams.theFactory->createString(lItem, prefixValue);
        values.push_back(lItem);
      }

      const zstring ns = aValue->getNamespace();
      const zstring local = aValue->getLocalName();
      valueValue = ns.empty() ? local : "Q{" + ns + "}" + local;
    }
    else
    {
      aValue->getStringValue2(valueValue);
    }
    someParams.theFactory->createString(names.at(1), valueKey);
    someParams.theFactory->createString(values.at(1), valueValue);
  }

  someParams.theFactory->createJSONObject(aResult, names, values);
  return true;
}

bool
JSONEncodeForRoundtripIterator::encodeNode(
    const store::Item_t& aNode,
    store::Item_t& aResult,
    CallParameters& someParams)
{
  std::vector<store::Item_t> names(2);
  std::vector<store::Item_t> values(2);

  {
    zstring typeKey = someParams.thePrefix + "type";
    zstring typeValue = "node()";
    someParams.theFactory->createString(names.at(0), typeKey);
    someParams.theFactory->createString(values.at(0), typeValue);
  }

  {
    zstring valueKey = someParams.thePrefix + "value";

    store::Iterator_t lItemIt = new ItemIterator(aNode);
    zorba::serializer lSerializer(someParams.theDiag);
    // TODO what do we set, if nothing is passed?
    lSerializer.setParameter("omit-xml-declaration", "yes");

    if (! someParams.theSerParams.isNull())
    {
      FnSerializeIterator::setSerializationParams(
          lSerializer,
          someParams.theSerParams,
          someParams.theSctx,
          someParams.theLoc);
    }

    // and now serialize
    std::auto_ptr<std::stringstream> lResultStream(new std::stringstream());
    lItemIt->open();
    lSerializer.serialize(lItemIt, *lResultStream.get());
    lItemIt->close();

    someParams.theFactory->createString(names.at(1), valueKey);
    someParams.theFactory->createStreamableString(
        values.at(1), *lResultStream.release(),
        FnSerializeIterator::streamReleaser, true);
  }

  someParams.theFactory->createJSONObject(aResult, names, values);
  return true;
}

bool
JSONEncodeForRoundtripIterator::encodeItem(
  const store::Item_t& anItem,
  store::Item_t& aResult,
  CallParameters& someParams)
{
  if (anItem->isJSONObject())
  {
    return encodeObject(anItem, aResult, someParams);
  }
  else if (anItem->isJSONArray())
  {
    return encodeArray(anItem, aResult, someParams);
  }
  else if (anItem->isAtomic())
  {
    return encodeAtomic(anItem, aResult, someParams);
  }
  else
  {
    return encodeNode(anItem, aResult, someParams);
  }
}

bool
JSONEncodeForRoundtripIterator::nextImpl(
  store::Item_t& aResult,
  PlanState& aPlanState) const
{
  store::Item_t lInput;
  CallParameters lParams(theSctx, GENV_ITEMFACTORY, loc);
  store::Item_t lEncParams;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, aPlanState);

  lParams.thePrefix = "Q{http://jsoniq.org/roundtrip}";
  lParams.theDiag = aPlanState.theCompilerCB->theXQueryDiagnostics;

  consumeNext(lInput, theChildren.at(0), aPlanState);

  // get encoding parameters
  if (theChildren.size() == 2
      && consumeNext(lEncParams, theChildren.at(1), aPlanState))
  {
    // the signature says that the second parameter has to be exactly one object
    store::Item_t lPrefixKey;
    zstring lPrefixName = "prefix";
    lParams.theFactory->createString(lPrefixKey, lPrefixName);
    store::Item_t lPrefixValue = lEncParams->getObjectValue(lPrefixKey);
    if (! lPrefixValue.isNull())
    {
      if (lPrefixValue->getTypeCode() != store::XS_STRING)
      {
        // TODO exception
        throw lPrefixValue;
      }
      lPrefixValue->getStringValue2(lParams.thePrefix);
    }


    store::Item_t lSerParamKey;
    zstring lSerParamName = "serialization-parameters";
    lParams.theFactory->createString(lSerParamKey, lSerParamName);
    store::Item_t lSerParamValue = lEncParams->getObjectValue(lSerParamKey);
    if (! lSerParamValue.isNull())
    {
      if (! lSerParamValue->isNode())
      {
        // TODO exception
        throw lSerParamValue; 
      }
      lParams.theSerParams = lSerParamValue;
    }
  }

  if (! encodeItem(lInput, aResult, lParams))
  {
    aResult = lInput;
  }

  STACK_PUSH (true, state);

  STACK_END(state);
}


/*******************************************************************************

********************************************************************************/
void
JSONParseIteratorState::init(PlanState& aState)
{
  PlanIteratorState::init(aState);
  theAllowMultiple = true; // default
  theInputStream = 0;
  theGotOne = false;
}

void
JSONParseIteratorState::reset(PlanState& aState)
{
  PlanIteratorState::reset(aState);
  if (theInput == NULL && theInputStream)
  {
    delete theInputStream;
  }
}

JSONParseIteratorState::~JSONParseIteratorState()
{
  if (theInput == NULL && theInputStream)
  {
    delete theInputStream;
  }
}

void
JSONParseIterator::processOptions(
    const store::Item_t& aOptions,
    bool& aAllowMultiple) const
{
  store::Item_t lOptionName, lOptionValue;

  zstring s("jsoniq-multiple-top-level-items");
  GENV_ITEMFACTORY->createString(lOptionName, s);
  lOptionValue = aOptions->getObjectValue(lOptionName);

  if (lOptionValue != NULL)
  {
    store::SchemaTypeCode lType = lOptionValue->getTypeCode();
    if (!TypeOps::is_subtype(lType, store::XS_BOOLEAN))
    {
      const TypeManager* tm = theSctx->get_typemanager();
      xqtref_t lType = tm->create_value_type(lOptionValue, loc);
      RAISE_ERROR(jerr::JSDY0020, loc, 
      ERROR_PARAMS(lType->toSchemaString(), s, "xs:boolean"));
    }
    aAllowMultiple = lOptionValue->getBooleanValue();
  }
}

bool
JSONParseIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  store::Item_t lInput;

  JSONParseIteratorState* state;
  DEFAULT_STACK_INIT(JSONParseIteratorState, state, planState);

  if (consumeNext(lInput, theChildren[0].getp(), planState))
  {
    if (theChildren.size() == 2)
    {
      store::Item_t lOptions;
      consumeNext(lOptions, theChildren[1].getp(), planState);
      processOptions(lOptions, state->theAllowMultiple);
    }

    if (lInput->isStreamable())
    {
      state->theInput = lInput;
      state->theInputStream = &lInput->getStream();
    }
    else
    {
      // will be deleted in the state
      state->theInputStream = new std::stringstream(
          lInput->getStringValue().c_str());
    }

    while (true)
    {
      try
      {
        // streamable string or non-literal string
        if (state->theInput != NULL || theRelativeLocation == QueryLoc::null)
        {
          result = GENV_STORE.parseJSON(*state->theInputStream, 0);
        }
        else
        {
          // pass the query location of the StringLiteral to the JSON
          // parser such that it can give better error locations.
          zorba::internal::diagnostic::location lLoc;
          lLoc = ERROR_LOC(theRelativeLocation);
          result = GENV_STORE.parseJSON(*state->theInputStream, &lLoc);
        }
      }
      catch (zorba::XQueryException& e)
      {
        // rethrow with JSDY0021
        XQueryException xq = XQUERY_EXCEPTION(
            jerr::JSDY0021,
            ERROR_PARAMS(e.what()),
            ERROR_LOC(loc));

        // use location of e in case of literal string
        if (!(theRelativeLocation == QueryLoc::null)) set_source(xq, e);
        throw xq;
      }

      if (result != NULL)
      {
        if (!state->theAllowMultiple && state->theGotOne)
        {
          RAISE_ERROR(jerr::JSDY0021, loc, 
          ERROR_PARAMS(ZED(JSON_UNEXPECTED_EXTRA_CONTENT)));
        }
        state->theGotOne = true;
        STACK_PUSH(true, state);
        continue;
      }
      else
      {
        break;
      }
    }
  }

  STACK_END(state);
}


/*******************************************************************************
  json:names($o as object()) as xs:string*
********************************************************************************/
bool
JSONObjectNamesIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  store::Item_t input;
  store::Item_t key;

  JSONObjectNamesIteratorState* state;
  DEFAULT_STACK_INIT(JSONObjectNamesIteratorState, state, planState);

  consumeNext(input, theChild.getp(), planState);

  state->theNames = input->getObjectKeys();
  state->theNames->open();

  while (state->theNames->next(key))
  {
    result = key;
    STACK_PUSH (true, state);
  }
  state->theNames = NULL;

  STACK_END(state);
}


/*******************************************************************************
  json:value($o as object(), $name as xs:string) as item()?
********************************************************************************/
bool
JSONObjectValueIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  store::Item_t lInput;
  store::Item_t lName;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  consumeNext(lInput, theChild0.getp(), planState);
  consumeNext(lName, theChild1.getp(), planState);

  result = lInput->getObjectValue(lName);
  
  STACK_PUSH(result != 0, state);

  STACK_END(state);
}


/*******************************************************************************
  json:project($o as object(), $names as xs:string*) as object()
********************************************************************************/
bool
JSONObjectProjectIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  store::Item_t obj;
  store::Item_t key;
  store::Iterator_t keysIte;
  store::Item_t value;
  store::Item_t name;
  std::vector<store::Item_t> names;
  csize numNames = 0;
  store::CopyMode copymode;
  std::vector<store::Item_t> newNames;
  std::vector<store::Item_t> newValues;
  csize i;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  consumeNext(obj, theChild0.getp(), planState);

  while (consumeNext(name, theChild1.getp(), planState))
  {
    ++numNames;
    names.resize(numNames);
    names[numNames - 1].transfer(name);
  }

  keysIte = obj->getObjectKeys();
  keysIte->open();

  while (keysIte->next(key))
  {
    for (i = 0; i < numNames; ++i)
    {
      if (names[i]->getStringValue() == key->getStringValue())
        break;
    }

    if (i < numNames)
    {
      value = obj->getObjectValue(key);

      if (value->isNode() || value->isJSONItem())
        value = value->copy(NULL, copymode);

      newValues.push_back(value);
      newNames.push_back(key);
    }
  }

  keysIte->close();

  GENV_ITEMFACTORY->createJSONObject(result, newNames, newValues);

  STACK_PUSH(true, state);
  STACK_END(state);
}


/*******************************************************************************
  j:size($i as array()) as xs:integer*
********************************************************************************/
bool
JSONArraySizeIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  store::Item_t lJSONItem;
  xs_integer lSize;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  consumeNext(lJSONItem, theChild.getp(), planState);

  lSize = lJSONItem->getArraySize();

  STACK_PUSH(GENV_ITEMFACTORY->createInteger(result, lSize), state);

  STACK_END(state);
}


/*******************************************************************************
  json:member($a as array(), $pos as xs:integer) as item()?
********************************************************************************/
bool
JSONArrayMemberIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  store::Item_t lInput;
  store::Item_t lPosition;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  consumeNext(lInput, theChild0.getp(), planState);
  consumeNext(lPosition, theChild1.getp(), planState);

  result = lInput->getArrayValue(lPosition->getIntegerValue());

  STACK_PUSH(result != 0, state);

  STACK_END(state);
}


/*******************************************************************************
  json:members($a as array()) as item()*
********************************************************************************/
bool
JSONArrayMembersIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  store::Item_t array;

  JSONArrayMembersIteratorState* state;
  DEFAULT_STACK_INIT(JSONArrayMembersIteratorState, state, planState);

  consumeNext(array, theChild.getp(), planState);

  state->theMembers = array->getArrayValues();
  state->theMembers->open();
  while (state->theMembers->next(result))
  {
    STACK_PUSH(true, state);
  }
  state->theMembers->close();

  STACK_END(state);
}


/*******************************************************************************
  json:flatten($a as array()) as item()*

  op-zorba:flatten-internal($a as item()*) as item()*
********************************************************************************/
void JSONArrayFlattenIteratorState::reset(PlanState& planState)
{
  PlanIteratorState::reset(planState);
  while (!theStack.empty())
  {
    theStack.pop();
  }
}


bool
JSONArrayFlattenIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  store::Item_t item;
  bool lFoundArray = false;

  JSONArrayFlattenIteratorState* state;
  DEFAULT_STACK_INIT(JSONArrayFlattenIteratorState, state, planState);

  consumeNext(item, theChild.getp(), planState);

  assert(item->isJSONArray());

  state->theStack.push(item->getArrayValues());
  state->theStack.top()->open();

  while (!state->theStack.empty())
  {
    while (state->theStack.top()->next(result))
    {
      if (result->isJSONArray())
      {
        state->theStack.push(result->getArrayValues());
        state->theStack.top()->open();
        lFoundArray = true;
        break;
      }
      
      STACK_PUSH(true, state);
    }
    
    if (lFoundArray)
    {
      lFoundArray = false;
      continue;
    }
    
    state->theStack.top()->close();
    state->theStack.pop();
  }

  STACK_END(state);
}


/*******************************************************************************
  op_zorba:json-item-accessor($i as json-item(), $s as xs:anyAtomic) as item()?
********************************************************************************/
bool
JSONItemAccessorIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  store::Item_t input;
  store::Item_t selector;

  const TypeManager* tm = theSctx->get_typemanager();

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  consumeNext(input, theChild0.getp(), planState);
  consumeNext(selector, theChild1.getp(), planState);

  if (input->isJSONArray())
  {
    store::SchemaTypeCode type = selector->getTypeCode();

    if (!TypeOps::is_subtype(type, store::XS_INTEGER))
    {
      xqtref_t type = tm->create_value_type(selector, loc);

      RAISE_ERROR(err::XPTY0004, loc, 
      ERROR_PARAMS(ZED(XPTY0004_NoTypePromotion_23),
                   type->toSchemaString(),
                   GENV_TYPESYSTEM.INTEGER_TYPE_ONE->toSchemaString()));
    }

    result = input->getArrayValue(selector->getIntegerValue());
  }
  else if (input->isJSONObject())
  {
    store::SchemaTypeCode type = selector->getTypeCode();

    if (!TypeOps::is_subtype(type, store::XS_STRING) &&
        !TypeOps::is_subtype(type, store::XS_UNTYPED_ATOMIC) &&
        !TypeOps::is_subtype(type, store::XS_ANY_URI))
    {
      xqtref_t type = tm->create_value_type(selector, loc);

      RAISE_ERROR(err::XPTY0004, loc, 
      ERROR_PARAMS(ZED(XPTY0004_NoTypePromotion_23),
                   type->toSchemaString(),
                   GENV_TYPESYSTEM.STRING_TYPE_ONE->toSchemaString()));
    }

    result = input->getObjectValue(selector);
  }
  else
  {
    ZORBA_ASSERT(false);
  }

  STACK_PUSH(result != 0, state);

  STACK_END(state);
}


/*******************************************************************************
  jn:null() as jn:null
********************************************************************************/
bool
JSONNullIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  STACK_PUSH(GENV_ITEMFACTORY->createJSONNull(result), state);

  STACK_END(state);
}


/*******************************************************************************
  jn:is-null(xs:anyAtomicType) as xs:boolean
********************************************************************************/
bool
JSONIsNullIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  PlanIteratorState* state;
  store::Item_t lItem;
  bool lIsNull;

  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  consumeNext(lItem, theChild.getp(), planState);

  lIsNull = (lItem->getTypeCode() == store::JS_NULL);

  STACK_PUSH(GENV_ITEMFACTORY->createBoolean(result, lIsNull), state);

  STACK_END(state);
}


/*******************************************************************************
  updating function op-zorba:object-insert(
      $o as object(),
      $c as object())
********************************************************************************/
bool JSONObjectInsertIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  store::Item_t target;
  store::Item_t content;
  store::PUL_t pul;
  store::CopyMode copymode;
  PlanIteratorState* state;

  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  consumeNext(target, theChildren[0].getp(), planState);

  consumeNext(content, theChildren[1].getp(), planState);

  copymode.set(true,
               theSctx->construction_mode() == StaticContextConsts::cons_preserve,
               theSctx->preserve_mode() == StaticContextConsts::preserve_ns,
               theSctx->inherit_mode() == StaticContextConsts::inherit_ns);
  if (content->isNode() || content->isJSONItem())
  {
    content = content->copy(NULL, copymode);
  }

  pul = GENV_ITEMFACTORY->createPendingUpdateList();

  pul->addJSONObjectInsert(&loc, target, content);

  result.transfer(pul);

  STACK_PUSH(true, state);

  STACK_END(state);
}


/*******************************************************************************
  updating function op-zorba:array-insert(
      $o as array(),
      $pos as xs:integer, 
      $values as item()*, 
********************************************************************************/
bool JSONArrayInsertIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  store::Item_t array;
  store::Item_t member;
  store::Item_t pos;
  std::vector<store::Item_t> members;
  store::PUL_t pul;
  store::CopyMode copymode;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  consumeNext(array, theChildren[0].getp(), planState);
  consumeNext(pos, theChildren[1].getp(), planState);

  copymode.set(true, 
               theSctx->construction_mode() == StaticContextConsts::cons_preserve,
               theSctx->preserve_mode() == StaticContextConsts::preserve_ns,
               theSctx->inherit_mode() == StaticContextConsts::inherit_ns);

  while (consumeNext(member, theChildren[2].getp(), planState))
  {
    if (member->isNode() || member->isJSONItem())
    {
      member = member->copy(NULL, copymode);
    }

    members.resize(members.size() + 1);
    members.back().transfer(member);
  }

  pul = GENV_ITEMFACTORY->createPendingUpdateList();

  pul->addJSONArrayInsert(&loc, array, pos, members);

  result.transfer(pul);

  STACK_PUSH(true, state);

  STACK_END (state);
}


/*******************************************************************************
  updating function op-zorba:array-append(
      $o as array(),
      $values as item()*) 
********************************************************************************/
bool JSONArrayAppendIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  store::Item_t array;
  store::Item_t member;
  std::vector<store::Item_t> members;
  store::PUL_t pul;
  store::CopyMode copymode;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  consumeNext(array, theChildren[0].getp(), planState);

  copymode.set(true, 
               theSctx->construction_mode() == StaticContextConsts::cons_preserve,
               theSctx->preserve_mode() == StaticContextConsts::preserve_ns,
               theSctx->inherit_mode() == StaticContextConsts::inherit_ns);

  while (consumeNext(member, theChildren[1].getp(), planState))
  {
    if (member->isNode() || member->isJSONItem())
    {
      member = member->copy(NULL, copymode);
    }

    members.resize(members.size() + 1);
    members.back().transfer(member);
  }

  pul = GENV_ITEMFACTORY->createPendingUpdateList();

  pul->addJSONArrayAppend(&loc, array, members);

  result.transfer(pul);

  STACK_PUSH(true, state);

  STACK_END (state);
}


/*******************************************************************************
  updating function op-zorba:json-delete(
      $target as json-item(),
      $selector as xs:anyAtomicType) 
********************************************************************************/
bool JSONDeleteIterator::nextImpl(
    store::Item_t& result, 
    PlanState& planState) const
{
  store::Item_t target;
  store::Item_t selector;
  store::PUL_t pul;

  const TypeManager* tm = theSctx->get_typemanager();

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  consumeNext(target, theChildren[0].getp(), planState);
  consumeNext(selector, theChildren[1].getp(), planState);

  pul = GENV_ITEMFACTORY->createPendingUpdateList();

  if (target->isJSONObject())
  {
    store::SchemaTypeCode type = selector->getTypeCode();

    if (type != store::XS_STRING && type != store::XS_ANY_URI)
    {
      xqtref_t type = tm->create_value_type(selector, loc);

      RAISE_ERROR(jerr::JNUP0007, loc, 
      ERROR_PARAMS(ZED(JNUP0007_Object), type->toSchemaString()));
    }

    pul->addJSONObjectDelete(&loc, target, selector);
  }
  else if (target->isJSONArray())
  {
    store::SchemaTypeCode type = selector->getTypeCode();

    if (type != store::XS_INTEGER)
    {
      xqtref_t type = tm->create_value_type(selector, loc);

      RAISE_ERROR(jerr::JNUP0007, loc, 
      ERROR_PARAMS(ZED(JNUP0007_Array), type->toSchemaString()));
    }

    pul->addJSONArrayDelete(&loc, target, selector);
  }

  result.transfer(pul);

  STACK_PUSH(true, state);

  STACK_END (state);
}


/*******************************************************************************
  updating function op-zorba:replace-value(
      $target as json-item(),
      $selector as xs:anyAtomicType, 
      $newValue as item()) 
********************************************************************************/
JSONReplaceValueIterator::JSONReplaceValueIterator(
    static_context* sctx,
    const QueryLoc& loc,
    std::vector<PlanIter_t>& args,
    bool copyInput)
  :
  NaryBaseIterator<JSONReplaceValueIterator, PlanIteratorState>(sctx, loc, args)
{
  if (theChildren[2]->isConstructor())
  {
    theCopyInput = false;
  }
  else
  {
    theCopyInput = copyInput;
  }
}


bool JSONReplaceValueIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  store::Item_t target;
  store::Item_t selector;
  store::Item_t newValue;
  store::PUL_t pul;
  store::CopyMode copymode;

  const TypeManager* tm = theSctx->get_typemanager();

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  consumeNext(target, theChildren[0].getp(), planState);
  consumeNext(selector, theChildren[1].getp(), planState);
  consumeNext(newValue, theChildren[2].getp(), planState);

  if (theCopyInput && (newValue->isNode() || newValue->isJSONItem()))
  {
    copymode.set(true, 
      theSctx->construction_mode() == StaticContextConsts::cons_preserve,
      theSctx->preserve_mode() == StaticContextConsts::preserve_ns,
      theSctx->inherit_mode() == StaticContextConsts::inherit_ns);

    newValue = newValue->copy(NULL, copymode);
  }

  pul = GENV_ITEMFACTORY->createPendingUpdateList();

  if (target->isJSONObject())
  {
    store::SchemaTypeCode type = selector->getTypeCode();

    if (type != store::XS_STRING && type != store::XS_ANY_URI)
    {
      xqtref_t type = tm->create_value_type(selector, loc);

      RAISE_ERROR(jerr::JNUP0007, loc, 
      ERROR_PARAMS(ZED(JNUP0007_Object), type->toSchemaString()));
    }

    pul->addJSONObjectReplaceValue(&loc, target, selector, newValue);
  }
  else if (target->isJSONArray())
  {
    store::SchemaTypeCode type = selector->getTypeCode();

    if (type != store::XS_INTEGER)
    {
      xqtref_t type = tm->create_value_type(selector, loc);

      RAISE_ERROR(jerr::JNUP0007, loc, 
      ERROR_PARAMS(ZED(JNUP0007_Array), type->toSchemaString()));
    }

    pul->addJSONArrayReplaceValue(&loc, target, selector, newValue);
  }

  result.transfer(pul);

  STACK_PUSH(true, state);

  STACK_END(state);
}


/*******************************************************************************
  updating function op-zorba:object-rename(
      $o as object(),
      $name as xs:string, 
      $newName as xs:string) 
********************************************************************************/
bool JSONRenameIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  store::Item_t target;
  store::Item_t name;
  store::Item_t newName;
  store::PUL_t pul;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  consumeNext(target, theChildren[0].getp(), planState);
  consumeNext(name, theChildren[1].getp(), planState);
  consumeNext(newName, theChildren[2].getp(), planState);

  pul = GENV_ITEMFACTORY->createPendingUpdateList();

  pul->addJSONObjectRename(&loc, target, name, newName);

  result.transfer(pul);

  STACK_PUSH(true, state);

  STACK_END(state);
}


/*******************************************************************************

********************************************************************************/
bool JSONBoxIterator::nextImpl(
  store::Item_t& result,
  PlanState& planState) const
{
  store::Item_t value1;
  store::Item_t value2;
  store::Item_t value;

  PlanIteratorState* state;
  DEFAULT_STACK_INIT(PlanIteratorState, state, planState);

  if (!consumeNext(value1, theChild, planState))
  {
    GENV_STORE.getItemFactory()->createJSONNull(result);
  }
  else if (!consumeNext(value2, theChild, planState))
  {
    result.transfer(value1);
  }
  else
  {
    store::CopyMode copymode;
    copymode.set(false, true, true, true);

    store::Iterator_t wrapper = new PlanIteratorWrapper(theChild, planState);

    GENV_STORE.getItemFactory()->
    createJSONArray(result, value1, value2, wrapper, copymode);
  }

  STACK_PUSH(true, state);
  STACK_END(state);
}


} /* namespace zorba */
/* vim:set et sw=2 ts=2: */

#endif /* ZORBA_WITH_JSON */
