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
#ifndef ZORBA_FULL_TEXT_ICU_TOKENIZER_H
#define ZORBA_FULL_TEXT_ICU_TOKENIZER_H

#include <map>
#include <unicode/rbbi.h>               /* for RuleBasedBreakIterator */

#include <zorba/locale.h>

#include "tokenizer.h"
#include "util/cxx_util.h"

namespace zorba {

/**
 * An %ICU_Tokenizer is-a Tokenizer that uses the ICU (International Components
 * for Unicode) library.
 */
class ICU_Tokenizer : public Tokenizer {
public:

  /**
   * Constructs an %ICU_Tokenizer.
   *
   * @param wildcards If \c true, allows XQuery wildcard syntax characters to
   * be part of tokens.
   */
  ICU_Tokenizer( bool wildcards = false );

  // inherited
  void tokenize( char const*, size_t, locale::iso639_1::type, Callback&,
                 void* );

private:
  struct ICU_Iterators {
    RuleBasedBreakIterator *word_;
    RuleBasedBreakIterator *sent_;

    ICU_Iterators() : word_( nullptr ), sent_( nullptr ) { }

    ~ICU_Iterators() {
      delete word_;
      delete sent_;
    }
  };

  typedef std::map<locale::iso639_1::type,ICU_Iterators> lang_iters_t;

  lang_iters_t lang_iters_;
  bool const wildcards_;

  static void create_iterators( locale::iso639_1::type, ICU_Iterators& );
  ICU_Iterators& get_iterators_for( locale::iso639_1::type );
};

} // namespace zorba
#endif  /* ZORBA_FULL_TEXT_ICU_TOKENIZER_H */
/* vim:set et sw=2 ts=2: */
