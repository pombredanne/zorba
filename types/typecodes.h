/* -*- mode: c++; indent-tabs-mode: nil -*-
 *
 *  $Id: TypeCodes.h,v 1.1 2006/10/09 07:07:59 Paul Pedersen Exp $
 *
 *	Copyright 2006-2007 FLWOR Foundation.
 *  Authors: John Cowan, Paul Pedersen
 */

// TODO: change defines to static const declarations?
// TODO: add emptySequence type?
// TODO: what about anyAtomicType?

#ifndef XQP_TYPECODES_H
#define XQP_TYPECODES_H

#include <stdint.h>

namespace xqp {

// The following defines are really internal to this file.

// Masks and shifts for the bitfields in a type code.
// The representations are at the little end
#define REF_MASK (1<<29)
#define REF_SHIFT 29
#define ARITY_MASK (1<<28|1<<27)
#define ARITY_SHIFT 27
#define NODE_MASK (1<<26|1<<25|1<<24|1<<23)
#define NODE_SHIFT 23
#define TYPE_MASK (1<<22|1<<21|1<<20|1<<19|1<<18)
#define TYPE_SHIFT 18
#define SUB_MASK (1<<17|1<<16)
#define SUB_SHIFT 16
#define SUB2_MASK (1<<15|1<<14)
#define SUB2_SHIFT 14
#define SUB3_MASK (1<<13|1<<12)
#define SUB3_SHIFT 12
#define SUB4_MASK (1<<11|1<<10)
#define SUB4_SHIFT 10
#define SUB5_MASK (1<<9|1<<8)
#define SUB5_SHIFT 8
#define SUB6_MASK (1<<7|1<<6)
#define SUB6_SHIFT 6
#define REP_MASK (1<<5|1<<4|1<<3|1<<2|1<<1|1<<0)
#define REP_SHIFT 0
#define FULLTYPE_MASK (TYPE_MASK|SUB_MASK|SUB2_MASK|SUB3_MASK|SUB4_MASK|SUB5_MASK|SUB6_MASK)

// REF specifies whether this value is a reference or an actual value
#define NOT_REF (0 << REF_SHIFT)
#define IS_REF (1 << REF_SHIFT)

// ARITY specifies whether this value is an item or a sequence.
// Data items use ITEM_ARITY or SEQUENCE_ARITY.
// The other codes are used only to specify argument and result types of functions.
#define ITEM_ARITY (0 << ARITY_SHIFT)
#define SEQUENCE_ARITY (1 << ARITY_SHIFT)
#define NONEMPTY_SEQ_ARITY (2 << ARITY_SHIFT)
#define OPT_ITEM_ARITY (3 << ARITY_SHIFT)

// NODE specifies the node type.
// NOT_NODE is a dummy for values,
// ANY_NODE is a dummy for sequences of any type of node (but not a value).
#define NOT_NODE (0 << NODE_SHIFT)
#define DOCUMENT_NODE (1 << NODE_SHIFT)
#define ELEMENT_NODE (2 << NODE_SHIFT)
#define ATTRIBUTE_NODE (3 << NODE_SHIFT)
#define NAMESPACE_NODE (4 << NODE_SHIFT)
#define PI_NODE (5 << NODE_SHIFT)
#define COMMENT_NODE (6 << NODE_SHIFT)
#define TEXT_NODE (7 << NODE_SHIFT)
#define ANY_NODE (8 << NODE_SHIFT)

// TYPE specifies the XML Schema type.
// UNTYPED is used for elements, UNTYPED_ATOMIC for other node types.
// COMPLEX_TYPE is reserved for later use with complex types.
#define UNTYPED_TYPE (0 << TYPE_SHIFT)
#define UNTYPED_ATOMIC_TYPE (1 << TYPE_SHIFT)
#define STRING_TYPE (2 << TYPE_SHIFT)
#define DECIMAL_TYPE (3 << TYPE_SHIFT)
#define BOOLEAN_TYPE (4 << TYPE_SHIFT)
#define FLOAT_TYPE (5 << TYPE_SHIFT)
#define DOUBLE_TYPE (6 << TYPE_SHIFT)
#define DURATION_TYPE (7 << TYPE_SHIFT)
#define DATE_TIME_TYPE (8 << TYPE_SHIFT)
#define TIME_TYPE (9 << TYPE_SHIFT)
#define DATE_TYPE (10 << TYPE_SHIFT)
#define G_YEAR_MONTH_TYPE (11 << TYPE_SHIFT)
#define G_YEAR_TYPE (12 << TYPE_SHIFT)
#define G_MONTH_DAY_TYPE (13 << TYPE_SHIFT)
#define G_DAY_TYPE (14 << TYPE_SHIFT)
#define G_MONTH_TYPE (15 << TYPE_SHIFT)
#define HEX_BINARY_TYPE (16 << TYPE_SHIFT)
#define BASE_64_BINARY_TYPE (17 << TYPE_SHIFT)
#define ANY_URI_TYPE (18 << TYPE_SHIFT)
#define QNAME_TYPE (19 << TYPE_SHIFT)
#define NOTATION_TYPE (20 << TYPE_SHIFT)
#define COMPLEX_TYPE (21 << TYPE_SHIFT)

// SUB2-SUB5 specify derived XML Schema types.
#define NORMALIZED_STRING_SUB (STRING_TYPE | 1 << SUB_SHIFT)
#define TOKEN_SUB2 (NORMALIZED_STRING_SUB | 1 << SUB2_SHIFT)
#define LANGUAGE_SUB3 (TOKEN_SUB2 | 1 << SUB3_SHIFT)
#define NMTOKEN_SUB3 (TOKEN_SUB2 | 2 << SUB3_SHIFT)
#define NAME_SUB3 (TOKEN_SUB2 | 3 << SUB3_SHIFT)
#define NCNAME_SUB4 (NAME_SUB3 | 1 << SUB4_SHIFT)
#define ID_SUB5 (NCNAME_SUB4 | 1 << SUB5_SHIFT)
#define IDREF_SUB5 (NCNAME_SUB4 | 2 << SUB5_SHIFT)
#define ENTITY_SUB5 (NCNAME_SUB4 | 3 << SUB5_SHIFT)

#define INTEGER_SUB (DECIMAL_TYPE | 1 << SUB_SHIFT)
#define NON_POSITIVE_INTEGER_SUB2 (INTEGER_SUB | 1 << SUB2_SHIFT)
#define NEGATIVE_INTEGER_SUB3 (NON_POSITIVE_INTEGER_SUB2 | 1 << SUB3_SHIFT)
#define LONG_SUB2 (INTEGER_SUB | 2 << SUB2_SHIFT)
#define INT_SUB3 (LONG_SUB2 | 1 << SUB3_SHIFT)
#define SHORT_SUB4 (INT_SUB3 | 1 << SUB4_SHIFT)
#define BYTE_SUB5 (SHORT_SUB4 | 1 << SUB5_SHIFT)
#define NON_NEGATIVE_INTEGER_SUB2 (INTEGER_SUB | 3 << SUB2_SHIFT)
#define UNSIGNED_LONG_SUB3 (NON_NEGATIVE_INTEGER_SUB2 | 1 << SUB3_SHIFT)
#define UNSIGNED_INT_SUB4 (UNSIGNED_LONG_SUB3 | 1 << SUB4_SHIFT)
#define UNSIGNED_SHORT_SUB5 (UNSIGNED_INT_SUB4 | 1 << SUB5_SHIFT)
#define UNSIGNED_BYTE_SUB6 (UNSIGNED_SHORT_SUB5 | 1 << SUB6_SHIFT)
#define POSITIVE_INTEGER_SUB3 (NON_NEGATIVE_INTEGER_SUB2 | 2 << SUB3_SHIFT)

#define DAY_TIME_DURATION_SUB2 (DURATION_TYPE | 1 << SUB2_SHIFT)
#define YEAR_MONTH_DURATION_SUB2 (DURATION_TYPE | 2 << SUB2_SHIFT)

// REP specifies the representation (C++ class)
// More reps may be added in future.
// Initially DECIMAL_REP is not used.
// NO_REP means this type has no representation
// BARE_REP means this type does not need a special rep class but is just a type+length.
#define NO_REP (0 << REP_SHIFT)
#define DOCUMENT_REP (1 << REP_SHIFT)
#define ELEMENT_REP (2 << REP_SHIFT)
#define ATTRIBUTE_REP (3 << REP_SHIFT)
#define NAMESPACE_REP (4 << REP_SHIFT)
#define PI_REP (5 << REP_SHIFT)
#define COMMENT_REP (6 << REP_SHIFT)
#define TEXT_REP (7 << REP_SHIFT)
#define STRING_REP (8 << REP_SHIFT)
#define DECIMAL_REP (9 << REP_SHIFT)
#define DOUBLE_REP (10 << REP_SHIFT)
#define BOOLEAN_REP (11 << REP_SHIFT)
#define TIME_REP (12 << REP_SHIFT)
#define BINARY_REP (13 << REP_SHIFT)
#define QNAME_REP (14 << REP_SHIFT)
#define SEQUENCE_REP (15 << REP_SHIFT)
#define REFERENCE_REP (16 << REP_SHIFT)
#define COMPLEX_TYPE_REP (17 << REP_SHIFT)
#define BARE_REP (18 << REP_SHIFT)

// Okay, at last the bit-twiddling is done.  Now we just make an enum of type codes
// that assembles the correct values together.
// Use fix_typecodes to regenerate the Ref, Seq, SeqRef, Plus, and Opt definitions.

enum TypeCode {
	// Node items.
	documentNode = ITEM_ARITY + DOCUMENT_NODE + DOCUMENT_REP + UNTYPED_TYPE,
	documentNodeRef = IS_REF + ITEM_ARITY + DOCUMENT_NODE + REFERENCE_REP + UNTYPED_TYPE,
	documentNodeSeq = SEQUENCE_ARITY + DOCUMENT_NODE + SEQUENCE_REP + UNTYPED_TYPE,
	documentNodeSeqRef = IS_REF + SEQUENCE_ARITY + DOCUMENT_NODE + REFERENCE_REP + UNTYPED_TYPE,
	documentNodePlus = NONEMPTY_SEQ_ARITY + DOCUMENT_NODE + NO_REP + UNTYPED_TYPE,
	documentNodeOpt = OPT_ITEM_ARITY + DOCUMENT_NODE + NO_REP + UNTYPED_TYPE,
	elementNode = ITEM_ARITY + ELEMENT_NODE + ELEMENT_REP + UNTYPED_TYPE,
	elementNodeRef = IS_REF + ITEM_ARITY + ELEMENT_NODE + REFERENCE_REP + UNTYPED_TYPE,
	elementNodeSeq = SEQUENCE_ARITY + ELEMENT_NODE + SEQUENCE_REP + UNTYPED_TYPE,
	elementNodeSeqRef = IS_REF + SEQUENCE_ARITY + ELEMENT_NODE + REFERENCE_REP + UNTYPED_TYPE,
	elementNodePlus = NONEMPTY_SEQ_ARITY + ELEMENT_NODE + NO_REP + UNTYPED_TYPE,
	elementNodeOpt = OPT_ITEM_ARITY + ELEMENT_NODE + NO_REP + UNTYPED_TYPE,
	attributeNode = ITEM_ARITY + ATTRIBUTE_NODE + ATTRIBUTE_REP + UNTYPED_ATOMIC_TYPE,
	attributeNodeRef = IS_REF + ITEM_ARITY + ATTRIBUTE_NODE + REFERENCE_REP + UNTYPED_ATOMIC_TYPE,
	attributeNodeSeq = SEQUENCE_ARITY + ATTRIBUTE_NODE + SEQUENCE_REP + UNTYPED_ATOMIC_TYPE,
	attributeNodeSeqRef = IS_REF + SEQUENCE_ARITY + ATTRIBUTE_NODE + REFERENCE_REP + UNTYPED_ATOMIC_TYPE,
	attributeNodePlus = NONEMPTY_SEQ_ARITY + ATTRIBUTE_NODE + NO_REP + UNTYPED_ATOMIC_TYPE,
	attributeNodeOpt = OPT_ITEM_ARITY + ATTRIBUTE_NODE + NO_REP + UNTYPED_ATOMIC_TYPE,
	namespaceNode = ITEM_ARITY + NAMESPACE_NODE + NAMESPACE_REP + UNTYPED_TYPE,
	namespaceNodeRef = IS_REF + ITEM_ARITY + NAMESPACE_NODE + REFERENCE_REP + UNTYPED_TYPE,
	namespaceNodeSeq = SEQUENCE_ARITY + NAMESPACE_NODE + SEQUENCE_REP + UNTYPED_TYPE,
	namespaceNodeSeqRef = IS_REF + SEQUENCE_ARITY + NAMESPACE_NODE + REFERENCE_REP + UNTYPED_TYPE,
	namespaceNodePlus = NONEMPTY_SEQ_ARITY + NAMESPACE_NODE + NO_REP + UNTYPED_TYPE,
	namespaceNodeOpt = OPT_ITEM_ARITY + NAMESPACE_NODE + NO_REP + UNTYPED_TYPE,
	processingInstructionNode = ITEM_ARITY + PI_NODE + PI_REP + UNTYPED_TYPE,
	processingInstructionNodeRef = IS_REF + ITEM_ARITY + PI_NODE + REFERENCE_REP + UNTYPED_TYPE,
	processingInstructionNodeSeq = SEQUENCE_ARITY + PI_NODE + SEQUENCE_REP + UNTYPED_TYPE,
	processingInstructionNodeSeqRef = IS_REF + SEQUENCE_ARITY + PI_NODE + REFERENCE_REP + UNTYPED_TYPE,
	processingInstructionNodePlus = NONEMPTY_SEQ_ARITY + PI_NODE + NO_REP + UNTYPED_TYPE,
	processingInstructionNodeOpt = OPT_ITEM_ARITY + PI_NODE + NO_REP + UNTYPED_TYPE,
	commentNode = ITEM_ARITY + COMMENT_NODE + COMMENT_REP + UNTYPED_TYPE,
	commentNodeRef = IS_REF + ITEM_ARITY + COMMENT_NODE + REFERENCE_REP + UNTYPED_TYPE,
	commentNodeSeq = SEQUENCE_ARITY + COMMENT_NODE + SEQUENCE_REP + UNTYPED_TYPE,
	commentNodeSeqRef = IS_REF + SEQUENCE_ARITY + COMMENT_NODE + REFERENCE_REP + UNTYPED_TYPE,
	commentNodePlus = NONEMPTY_SEQ_ARITY + COMMENT_NODE + NO_REP + UNTYPED_TYPE,
	commentNodeOpt = OPT_ITEM_ARITY + COMMENT_NODE + NO_REP + UNTYPED_TYPE,
	textNode = ITEM_ARITY + TEXT_NODE + TEXT_REP + UNTYPED_TYPE,
	textNodeRef = IS_REF + ITEM_ARITY + TEXT_NODE + REFERENCE_REP + UNTYPED_TYPE,
	textNodeSeq = SEQUENCE_ARITY + TEXT_NODE + SEQUENCE_REP + UNTYPED_TYPE,
	textNodeSeqRef = IS_REF + SEQUENCE_ARITY + TEXT_NODE + REFERENCE_REP + UNTYPED_TYPE,
	textNodePlus = NONEMPTY_SEQ_ARITY + TEXT_NODE + NO_REP + UNTYPED_TYPE,
	textNodeOpt = OPT_ITEM_ARITY + TEXT_NODE + NO_REP + UNTYPED_TYPE,

	// String values.
	xs_string = ITEM_ARITY + NOT_NODE + STRING_REP + STRING_TYPE,
	xs_stringRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + STRING_TYPE,
	xs_stringSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + STRING_TYPE,
	xs_stringSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + STRING_TYPE,
	xs_stringPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + STRING_TYPE,
	xs_stringOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + STRING_TYPE,
	xs_normalizedString = ITEM_ARITY + NOT_NODE + STRING_REP + NORMALIZED_STRING_SUB,
	xs_normalizedStringRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + NORMALIZED_STRING_SUB,
	xs_normalizedStringSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + NORMALIZED_STRING_SUB,
	xs_normalizedStringSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + NORMALIZED_STRING_SUB,
	xs_normalizedStringPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + NORMALIZED_STRING_SUB,
	xs_normalizedStringOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + NORMALIZED_STRING_SUB,
	xs_token = ITEM_ARITY + NOT_NODE + STRING_REP + TOKEN_SUB2,
	xs_tokenRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + TOKEN_SUB2,
	xs_tokenSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + TOKEN_SUB2,
	xs_tokenSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + TOKEN_SUB2,
	xs_tokenPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + TOKEN_SUB2,
	xs_tokenOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + TOKEN_SUB2,
	xs_language = ITEM_ARITY + NOT_NODE + STRING_REP + LANGUAGE_SUB3,
	xs_languageRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + LANGUAGE_SUB3,
	xs_languageSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + LANGUAGE_SUB3,
	xs_languageSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + LANGUAGE_SUB3,
	xs_languagePlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + LANGUAGE_SUB3,
	xs_languageOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + LANGUAGE_SUB3,
	xs_nmtoken = ITEM_ARITY + NOT_NODE + STRING_REP + NMTOKEN_SUB3,
	xs_nmtokenRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + NMTOKEN_SUB3,
	xs_nmtokenSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + NMTOKEN_SUB3,
	xs_nmtokenSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + NMTOKEN_SUB3,
	xs_nmtokenPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + NMTOKEN_SUB3,
	xs_nmtokenOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + NMTOKEN_SUB3,
	xs_name = ITEM_ARITY + NOT_NODE + STRING_REP + NAME_SUB3,
	xs_nameRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + NAME_SUB3,
	xs_nameSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + NAME_SUB3,
	xs_nameSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + NAME_SUB3,
	xs_namePlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + NAME_SUB3,
	xs_nameOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + NAME_SUB3,
	xs_ncName = ITEM_ARITY + NOT_NODE + STRING_REP + NCNAME_SUB4,
	xs_ncNameRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + NCNAME_SUB4,
	xs_ncNameSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + NCNAME_SUB4,
	xs_ncNameSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + NCNAME_SUB4,
	xs_ncNamePlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + NCNAME_SUB4,
	xs_ncNameOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + NCNAME_SUB4,
	xs_id = ITEM_ARITY + NOT_NODE + STRING_REP + ID_SUB5,
	xs_idRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + ID_SUB5,
	xs_idSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + ID_SUB5,
	xs_idSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + ID_SUB5,
	xs_idPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + ID_SUB5,
	xs_idOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + ID_SUB5,
	xs_idref = ITEM_ARITY + NOT_NODE + STRING_REP + IDREF_SUB5,
	xs_idrefRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + IDREF_SUB5,
	xs_idrefSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + IDREF_SUB5,
	xs_idrefSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + IDREF_SUB5,
	xs_idrefPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + IDREF_SUB5,
	xs_idrefOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + IDREF_SUB5,
	xs_entity = ITEM_ARITY + NOT_NODE + STRING_REP + ENTITY_SUB5,
	xs_entityRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + ENTITY_SUB5,
	xs_entitySeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + ENTITY_SUB5,
	xs_entitySeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + ENTITY_SUB5,
	xs_entityPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + ENTITY_SUB5,
	xs_entityOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + ENTITY_SUB5,

	// Numeric values.
//xs_decimal = ITEM_ARITY + NOT_NODE + DECIMAL_REP + DECIMAL_TYPE,  // no decimal rep yet
//xs_decimalRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + DECIMAL_TYPE,  // no decimal rep yet
//xs_decimalSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + DECIMAL_TYPE,  // no decimal rep yet
//xs_decimalSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + DECIMAL_TYPE,  // no decimal rep yet
//xs_decimalPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + DECIMAL_TYPE,  // no decimal rep yet
//xs_decimalOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + DECIMAL_TYPE,  // no decimal rep yet
	xs_decimal = ITEM_ARITY + NOT_NODE + DOUBLE_REP + DECIMAL_TYPE,  // no decimal rep yet
	xs_decimalRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + DECIMAL_TYPE,  // no decimal rep yet
	xs_decimalSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + DECIMAL_TYPE,  // no decimal rep yet
	xs_decimalSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + DECIMAL_TYPE,  // no decimal rep yet
	xs_decimalPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + DECIMAL_TYPE,  // no decimal rep yet
	xs_decimalOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + DECIMAL_TYPE,  // no decimal rep yet
//xs_integer = ITEM_ARITY + NOT_NODE + DECIMAL_REP + INTEGER_SUB,
//xs_integerRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + INTEGER_SUB,
//xs_integerSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + INTEGER_SUB,
//xs_integerSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + INTEGER_SUB,
//xs_integerPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + INTEGER_SUB,
//xs_integerOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + INTEGER_SUB,
	xs_integer = ITEM_ARITY + NOT_NODE + DOUBLE_REP + INTEGER_SUB,
	xs_integerRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + INTEGER_SUB,
	xs_integerSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + INTEGER_SUB,
	xs_integerSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + INTEGER_SUB,
	xs_integerPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + INTEGER_SUB,
	xs_integerOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + INTEGER_SUB,
//xs_nonPositiveInteger = ITEM_ARITY + NOT_NODE + DECIMAL_REP + NON_POSITIVE_INTEGER_SUB2,  // no decimal rep yet
//xs_nonPositiveIntegerRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + NON_POSITIVE_INTEGER_SUB2,  // no decimal rep yet
//xs_nonPositiveIntegerSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + NON_POSITIVE_INTEGER_SUB2,  // no decimal rep yet
//xs_nonPositiveIntegerSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + NON_POSITIVE_INTEGER_SUB2,  // no decimal rep yet
//xs_nonPositiveIntegerPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + NON_POSITIVE_INTEGER_SUB2,  // no decimal rep yet
//xs_nonPositiveIntegerOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + NON_POSITIVE_INTEGER_SUB2,  // no decimal rep yet
	xs_nonPositiveInteger = ITEM_ARITY + NOT_NODE + DOUBLE_REP + NON_POSITIVE_INTEGER_SUB2,
	xs_nonPositiveIntegerRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + NON_POSITIVE_INTEGER_SUB2,
	xs_nonPositiveIntegerSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + NON_POSITIVE_INTEGER_SUB2,
	xs_nonPositiveIntegerSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + NON_POSITIVE_INTEGER_SUB2,
	xs_nonPositiveIntegerPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + NON_POSITIVE_INTEGER_SUB2,
	xs_nonPositiveIntegerOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + NON_POSITIVE_INTEGER_SUB2,
//xs_negativeInteger = ITEM_ARITY + NOT_NODE + DECIMAL_REP + NEGATIVE_INTEGER_SUB3,  // no decimal rep yet
//xs_negativeIntegerRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + NEGATIVE_INTEGER_SUB3,  // no decimal rep yet
//xs_negativeIntegerSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + NEGATIVE_INTEGER_SUB3,  // no decimal rep yet
//xs_negativeIntegerSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + NEGATIVE_INTEGER_SUB3,  // no decimal rep yet
//xs_negativeIntegerPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + NEGATIVE_INTEGER_SUB3,  // no decimal rep yet
//xs_negativeIntegerOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + NEGATIVE_INTEGER_SUB3,  // no decimal rep yet
	xs_negativeInteger = ITEM_ARITY + NOT_NODE + DOUBLE_REP + NEGATIVE_INTEGER_SUB3,
	xs_negativeIntegerRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + NEGATIVE_INTEGER_SUB3,
	xs_negativeIntegerSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + NEGATIVE_INTEGER_SUB3,
	xs_negativeIntegerSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + NEGATIVE_INTEGER_SUB3,
	xs_negativeIntegerPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + NEGATIVE_INTEGER_SUB3,
	xs_negativeIntegerOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + NEGATIVE_INTEGER_SUB3,
//xs_long = ITEM_ARITY + NOT_NODE + DECIMAL_REP + LONG_SUB2,  // no decimal rep yet
//xs_longRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + LONG_SUB2,  // no decimal rep yet
//xs_longSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + LONG_SUB2,  // no decimal rep yet
//xs_longSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + LONG_SUB2,  // no decimal rep yet
//xs_longPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + LONG_SUB2,  // no decimal rep yet
//xs_longOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + LONG_SUB2,  // no decimal rep yet
	xs_long = ITEM_ARITY + NOT_NODE + DOUBLE_REP + LONG_SUB2,
	xs_longRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + LONG_SUB2,
	xs_longSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + LONG_SUB2,
	xs_longSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + LONG_SUB2,
	xs_longPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + LONG_SUB2,
	xs_longOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + LONG_SUB2,
	xs_int = ITEM_ARITY + NOT_NODE + DOUBLE_REP + INT_SUB3,
	xs_intRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + INT_SUB3,
	xs_intSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + INT_SUB3,
	xs_intSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + INT_SUB3,
	xs_intPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + INT_SUB3,
	xs_intOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + INT_SUB3,
	xs_short = ITEM_ARITY + NOT_NODE + DOUBLE_REP + SHORT_SUB4,
	xs_shortRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + SHORT_SUB4,
	xs_shortSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + SHORT_SUB4,
	xs_shortSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + SHORT_SUB4,
	xs_shortPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + SHORT_SUB4,
	xs_shortOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + SHORT_SUB4,
	xs_byte = ITEM_ARITY + NOT_NODE + DECIMAL_REP + BYTE_SUB5,
	xs_byteRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + BYTE_SUB5,
	xs_byteSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + BYTE_SUB5,
	xs_byteSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + BYTE_SUB5,
	xs_bytePlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + BYTE_SUB5,
	xs_byteOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + BYTE_SUB5,
//xs_nonNegativeInteger = ITEM_ARITY + NOT_NODE + DECIMAL_REP + NON_EGATIVE_INTEGER_SUB2,
//xs_nonNegativeIntegerRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + NON_EGATIVE_INTEGER_SUB2,
//xs_nonNegativeIntegerSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + NON_EGATIVE_INTEGER_SUB2,
//xs_nonNegativeIntegerSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + NON_EGATIVE_INTEGER_SUB2,
//xs_nonNegativeIntegerPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + NON_EGATIVE_INTEGER_SUB2,
//xs_nonNegativeIntegerOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + NON_EGATIVE_INTEGER_SUB2,
	xs_nonNegativeInteger = ITEM_ARITY + NOT_NODE + DOUBLE_REP + NON_NEGATIVE_INTEGER_SUB2,  // no decimal rep yet
	xs_nonNegativeIntegerRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + NON_NEGATIVE_INTEGER_SUB2,  // no decimal rep yet
	xs_nonNegativeIntegerSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + NON_NEGATIVE_INTEGER_SUB2,  // no decimal rep yet
	xs_nonNegativeIntegerSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + NON_NEGATIVE_INTEGER_SUB2,  // no decimal rep yet
	xs_nonNegativeIntegerPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + NON_NEGATIVE_INTEGER_SUB2,  // no decimal rep yet
	xs_nonNegativeIntegerOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + NON_NEGATIVE_INTEGER_SUB2,  // no decimal rep yet
//xs_unsignedLong = ITEM_ARITY + NOT_NODE + DECIMAL_REP + UNSIGNED_LONG_SUB3,
//xs_unsignedLongRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + UNSIGNED_LONG_SUB3,
//xs_unsignedLongSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + UNSIGNED_LONG_SUB3,
//xs_unsignedLongSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + UNSIGNED_LONG_SUB3,
//xs_unsignedLongPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + UNSIGNED_LONG_SUB3,
//xs_unsignedLongOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + UNSIGNED_LONG_SUB3,
	xs_unsignedLong = ITEM_ARITY + NOT_NODE + DOUBLE_REP + UNSIGNED_LONG_SUB3,
	xs_unsignedLongRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + UNSIGNED_LONG_SUB3,
	xs_unsignedLongSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + UNSIGNED_LONG_SUB3,
	xs_unsignedLongSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + UNSIGNED_LONG_SUB3,
	xs_unsignedLongPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + UNSIGNED_LONG_SUB3,
	xs_unsignedLongOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + UNSIGNED_LONG_SUB3,
	xs_unsignedInt = ITEM_ARITY + NOT_NODE + DECIMAL_REP + UNSIGNED_INT_SUB4,
	xs_unsignedIntRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + UNSIGNED_INT_SUB4,
	xs_unsignedIntSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + UNSIGNED_INT_SUB4,
	xs_unsignedIntSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + UNSIGNED_INT_SUB4,
	xs_unsignedIntPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + UNSIGNED_INT_SUB4,
	xs_unsignedIntOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + UNSIGNED_INT_SUB4,
	xs_unsignedShort = ITEM_ARITY + NOT_NODE + DOUBLE_REP + UNSIGNED_SHORT_SUB5,
	xs_unsignedShortRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + UNSIGNED_SHORT_SUB5,
	xs_unsignedShortSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + UNSIGNED_SHORT_SUB5,
	xs_unsignedShortSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + UNSIGNED_SHORT_SUB5,
	xs_unsignedShortPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + UNSIGNED_SHORT_SUB5,
	xs_unsignedShortOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + UNSIGNED_SHORT_SUB5,
	xs_unsignedByte = ITEM_ARITY + NOT_NODE + DECIMAL_REP + UNSIGNED_BYTE_SUB6,
	xs_unsignedByteRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + UNSIGNED_BYTE_SUB6,
	xs_unsignedByteSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + UNSIGNED_BYTE_SUB6,
	xs_unsignedByteSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + UNSIGNED_BYTE_SUB6,
	xs_unsignedBytePlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + UNSIGNED_BYTE_SUB6,
	xs_unsignedByteOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + UNSIGNED_BYTE_SUB6,
//xs_positiveInteger = ITEM_ARITY + NOT_NODE + DECIMAL_REP + POSITIVE_INTEGER_SUB3,  // no decimal rep yet
//xs_positiveIntegerRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + POSITIVE_INTEGER_SUB3,  // no decimal rep yet
//xs_positiveIntegerSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + POSITIVE_INTEGER_SUB3,  // no decimal rep yet
//xs_positiveIntegerSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + POSITIVE_INTEGER_SUB3,  // no decimal rep yet
//xs_positiveIntegerPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + POSITIVE_INTEGER_SUB3,  // no decimal rep yet
//xs_positiveIntegerOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + POSITIVE_INTEGER_SUB3,  // no decimal rep yet
	xs_positiveInteger = ITEM_ARITY + NOT_NODE + DOUBLE_REP + POSITIVE_INTEGER_SUB3,
	xs_positiveIntegerRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + POSITIVE_INTEGER_SUB3,
	xs_positiveIntegerSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + POSITIVE_INTEGER_SUB3,
	xs_positiveIntegerSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + POSITIVE_INTEGER_SUB3,
	xs_positiveIntegerPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + POSITIVE_INTEGER_SUB3,
	xs_positiveIntegerOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + POSITIVE_INTEGER_SUB3,

	// Other atomic values.
	xs_untypedValue = ITEM_ARITY + NOT_NODE + STRING_REP + UNTYPED_TYPE,
	xs_untypedValueRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + UNTYPED_TYPE,
	xs_untypedValueSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + UNTYPED_TYPE,
	xs_untypedValueSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + UNTYPED_TYPE,
	xs_untypedValuePlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + UNTYPED_TYPE,
	xs_untypedValueOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + UNTYPED_TYPE,
	xs_untypedAtomicValue = ITEM_ARITY + NOT_NODE + STRING_REP + UNTYPED_ATOMIC_TYPE,
	xs_untypedAtomicValueRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + UNTYPED_ATOMIC_TYPE,
	xs_untypedAtomicValueSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + UNTYPED_ATOMIC_TYPE,
	xs_untypedAtomicValueSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + UNTYPED_ATOMIC_TYPE,
	xs_untypedAtomicValuePlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + UNTYPED_ATOMIC_TYPE,
	xs_untypedAtomicValueOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + UNTYPED_ATOMIC_TYPE,
	xs_boolean = ITEM_ARITY + NOT_NODE + BOOLEAN_REP + BOOLEAN_TYPE,
	xs_booleanRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + BOOLEAN_TYPE,
	xs_booleanSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + BOOLEAN_TYPE,
	xs_booleanSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + BOOLEAN_TYPE,
	xs_booleanPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + BOOLEAN_TYPE,
	xs_booleanOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + BOOLEAN_TYPE,
	xs_float = ITEM_ARITY + NOT_NODE + DOUBLE_REP + FLOAT_TYPE,
	xs_floatRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + FLOAT_TYPE,
	xs_floatSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + FLOAT_TYPE,
	xs_floatSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + FLOAT_TYPE,
	xs_floatPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + FLOAT_TYPE,
	xs_floatOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + FLOAT_TYPE,
	xs_double = ITEM_ARITY + NOT_NODE + DOUBLE_REP + DOUBLE_TYPE,
	xs_doubleRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + DOUBLE_TYPE,
	xs_doubleSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + DOUBLE_TYPE,
	xs_doubleSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + DOUBLE_TYPE,
	xs_doublePlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + DOUBLE_TYPE,
	xs_doubleOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + DOUBLE_TYPE,
	xs_duration = ITEM_ARITY + NOT_NODE + TIME_REP + DURATION_TYPE,
	xs_durationRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + DURATION_TYPE,
	xs_durationSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + DURATION_TYPE,
	xs_durationSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + DURATION_TYPE,
	xs_durationPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + DURATION_TYPE,
	xs_durationOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + DURATION_TYPE,
	xs_dayTimeDuration = ITEM_ARITY + NOT_NODE + TIME_REP + DAY_TIME_DURATION_SUB2,
	xs_dayTimeDurationRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + DAY_TIME_DURATION_SUB2,
	xs_dayTimeDurationSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + DAY_TIME_DURATION_SUB2,
	xs_dayTimeDurationSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + DAY_TIME_DURATION_SUB2,
	xs_dayTimeDurationPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + DAY_TIME_DURATION_SUB2,
	xs_dayTimeDurationOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + DAY_TIME_DURATION_SUB2,
	xs_yearMonthDuration = ITEM_ARITY + NOT_NODE + TIME_REP + YEAR_MONTH_DURATION_SUB2,
	xs_yearMonthDurationRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + YEAR_MONTH_DURATION_SUB2,
	xs_yearMonthDurationSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + YEAR_MONTH_DURATION_SUB2,
	xs_yearMonthDurationSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + YEAR_MONTH_DURATION_SUB2,
	xs_yearMonthDurationPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + YEAR_MONTH_DURATION_SUB2,
	xs_yearMonthDurationOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + YEAR_MONTH_DURATION_SUB2,
	xs_dateTime = ITEM_ARITY + NOT_NODE + TIME_REP + DATE_TIME_TYPE,
	xs_dateTimeRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + DATE_TIME_TYPE,
	xs_dateTimeSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + DATE_TIME_TYPE,
	xs_dateTimeSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + DATE_TIME_TYPE,
	xs_dateTimePlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + DATE_TIME_TYPE,
	xs_dateTimeOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + DATE_TIME_TYPE,
	xs_time = ITEM_ARITY + NOT_NODE + TIME_REP + TIME_TYPE,
	xs_timeRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + TIME_TYPE,
	xs_timeSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + TIME_TYPE,
	xs_timeSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + TIME_TYPE,
	xs_timePlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + TIME_TYPE,
	xs_timeOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + TIME_TYPE,
	xs_date = ITEM_ARITY + NOT_NODE + TIME_REP + DATE_TYPE,
	xs_dateRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + DATE_TYPE,
	xs_dateSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + DATE_TYPE,
	xs_dateSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + DATE_TYPE,
	xs_datePlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + DATE_TYPE,
	xs_dateOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + DATE_TYPE,
	xs_gYearMonth = ITEM_ARITY + NOT_NODE + TIME_REP + G_YEAR_MONTH_TYPE,
	xs_gYearMonthRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + G_YEAR_MONTH_TYPE,
	xs_gYearMonthSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + G_YEAR_MONTH_TYPE,
	xs_gYearMonthSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + G_YEAR_MONTH_TYPE,
	xs_gYearMonthPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + G_YEAR_MONTH_TYPE,
	xs_gYearMonthOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + G_YEAR_MONTH_TYPE,
	xs_gYear = ITEM_ARITY + NOT_NODE + TIME_REP + G_YEAR_TYPE,
	xs_gYearRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + G_YEAR_TYPE,
	xs_gYearSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + G_YEAR_TYPE,
	xs_gYearSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + G_YEAR_TYPE,
	xs_gYearPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + G_YEAR_TYPE,
	xs_gYearOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + G_YEAR_TYPE,
	xs_gMonthDay = ITEM_ARITY + NOT_NODE + TIME_REP + G_MONTH_DAY_TYPE,
	xs_gMonthDayRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + G_MONTH_DAY_TYPE,
	xs_gMonthDaySeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + G_MONTH_DAY_TYPE,
	xs_gMonthDaySeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + G_MONTH_DAY_TYPE,
	xs_gMonthDayPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + G_MONTH_DAY_TYPE,
	xs_gMonthDayOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + G_MONTH_DAY_TYPE,
	xs_gDay = ITEM_ARITY + NOT_NODE + TIME_REP + G_DAY_TYPE,
	xs_gDayRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + G_DAY_TYPE,
	xs_gDaySeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + G_DAY_TYPE,
	xs_gDaySeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + G_DAY_TYPE,
	xs_gDayPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + G_DAY_TYPE,
	xs_gDayOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + G_DAY_TYPE,
	xs_gMonth = ITEM_ARITY + NOT_NODE + TIME_REP + G_MONTH_TYPE,
	xs_gMonthRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + G_MONTH_TYPE,
	xs_gMonthSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + G_MONTH_TYPE,
	xs_gMonthSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + G_MONTH_TYPE,
	xs_gMonthPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + G_MONTH_TYPE,
	xs_gMonthOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + G_MONTH_TYPE,
	xs_hexBinary = ITEM_ARITY + NOT_NODE + BINARY_REP + HEX_BINARY_TYPE,
	xs_hexBinaryRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + HEX_BINARY_TYPE,
	xs_hexBinarySeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + HEX_BINARY_TYPE,
	xs_hexBinarySeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + HEX_BINARY_TYPE,
	xs_hexBinaryPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + HEX_BINARY_TYPE,
	xs_hexBinaryOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + HEX_BINARY_TYPE,
	xs_base64Binary = ITEM_ARITY + NOT_NODE + BINARY_REP + BASE_64_BINARY_TYPE,
	xs_base64BinaryRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + BASE_64_BINARY_TYPE,
	xs_base64BinarySeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + BASE_64_BINARY_TYPE,
	xs_base64BinarySeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + BASE_64_BINARY_TYPE,
	xs_base64BinaryPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + BASE_64_BINARY_TYPE,
	xs_base64BinaryOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + BASE_64_BINARY_TYPE,
	xs_anyURI = ITEM_ARITY + NOT_NODE + STRING_REP + ANY_URI_TYPE,
	xs_anyURIRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + ANY_URI_TYPE,
	xs_anyURISeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + ANY_URI_TYPE,
	xs_anyURISeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + ANY_URI_TYPE,
	xs_anyURIPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + ANY_URI_TYPE,
	xs_anyURIOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + ANY_URI_TYPE,
	xs_qname = ITEM_ARITY + NOT_NODE + QNAME_REP + QNAME_TYPE,
	xs_qnameRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + QNAME_TYPE,
	xs_qnameSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + QNAME_TYPE,
	xs_qnameSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + QNAME_TYPE,
	xs_qnamePlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + QNAME_TYPE,
	xs_qnameOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + QNAME_TYPE,
	xs_notation = ITEM_ARITY + NOT_NODE + QNAME_REP + NOTATION_TYPE,
	xs_notationRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + NOTATION_TYPE,
	xs_notationSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + NOTATION_TYPE,
	xs_notationSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + NOTATION_TYPE,
	xs_notationPlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + NOTATION_TYPE,
	xs_notationOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + NOTATION_TYPE,
	xs_complexType = ITEM_ARITY + NOT_NODE + QNAME_REP + NOTATION_TYPE,
	xs_complexTypeRef = IS_REF + ITEM_ARITY + NOT_NODE + REFERENCE_REP + NOTATION_TYPE,
	xs_complexTypeSeq = SEQUENCE_ARITY + NOT_NODE + SEQUENCE_REP + NOTATION_TYPE,
	xs_complexTypeSeqRef = IS_REF + SEQUENCE_ARITY + NOT_NODE + REFERENCE_REP + NOTATION_TYPE,
	xs_complexTypePlus = NONEMPTY_SEQ_ARITY + NOT_NODE + NO_REP + NOTATION_TYPE,
	xs_complexTypeOpt = OPT_ITEM_ARITY + NOT_NODE + NO_REP + NOTATION_TYPE,
};

} /* namespace xqp */
#endif /* XQP_TYPECODES_H */

