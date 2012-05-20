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
#pragma once
#ifndef ZORBA_UTILS_HASHMAP_ZSTRING_NONSERIALIZABLE
#define ZORBA_UTILS_HASHMAP_ZSTRING_NONSERIALIZABLE

#include "zorbautils/hashmap.h"

#include "zorbatypes/zstring.h"

#include "util/utf8_util.h"


namespace zorba
{

/***************************************************************************//**
  Class to privide the equality and hash functions for the HashMapZString
  class defined below.
*******************************************************************************/
class HashMapZStringCmp
{
public:
  static uint32_t hash(const zstring& str)
  {
    return utf8::hash(str);
  }

  static bool equal(const zstring& s1, const zstring& s2)
  {
    return s1 == s2;
  }
};


/*******************************************************************************
  A nonserializable hash-based map container mapping zstrings to values of
  type V. Equality is based on the zstring == operator.
*******************************************************************************/
template<class V>
class HashMapZString : public HashMap<zstring, V, HashMapZStringCmp>
{
public:
  HashMapZString()
    :
    HashMap<zstring, V, HashMapZStringCmp>()
  {
  }

public:
  HashMapZString(ulong size, bool sync)
    :
    HashMap<zstring, V, HashMapZStringCmp>(size, sync)
  {
  }

  virtual ~HashMapZString() {  }
};

}

#endif
