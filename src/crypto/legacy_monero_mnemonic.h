#ifndef LEGACY_MONERO_MNEMONIC_H
#define LEGACY_MONERO_MNEMONIC_H

#include <stdint.h>
#include "byte_order.h"

#define MONERO_SEED_BITS 256
#define MONERO_MNEMONIC_WORDS_COUNT 24
#define MONERO_MAXIMUM_WORD_LENGTH 16
//+1 for the checksum word, +2 for the spaces and 0x00 terminations
#define MONERO_MNEMONIC_MAXIMUM_LENGTH (MONERO_MNEMONIC_WORDS_COUNT+1)*(MONERO_MAXIMUM_WORD_LENGTH+2)
#define MONERO_WORDLIST_WORD_COUNT 1626

#define MONERO_MAXIMUM_PREFIX_LENGTH 4
#define MONERO_ENGLISH_PREFIX_LENGTH 3
#define MONERO_SPANISH_PREFIX_LENGTH 4
#define MONERO_FRENCH_PREFIX_LENGTH 4
#define MONERO_ITALIAN_PREFIX_LENGTH 4
#define MONERO_DUTCH_PREFIX_LENGTH 4
#define MONERO_PORTUGUESE_PREFIX_LENGTH 4
#define MONERO_RUSSIAN_PREFIX_LENGTH 4
#define MONERO_JAPANESE_PREFIX_LENGTH 1
#define MONERO_CHINESE_PREFIX_LENGTH 1
#define MONERO_ESPERANTO_PREFIX_LENGTH 4
#define MONERO_LOJBAN_PREFIX_LENGTH 4
#define MONERO_GERMAN_PREFIX_LENGTH 4

#define IDENT32(x) ((uint32_t) (x))

#define SWAP32(x) ((((uint32_t) (x) & 0x000000ff) << 24) | \
    (((uint32_t) (x) & 0x0000ff00) << 8) | \
    (((uint32_t) (x) & 0x00ff0000) << 8) | \
    (((uint32_t) (x) & 0xff000000) >> 24))

#if BYTE_ORDER == LITTLE_ENDIAN
#define SWAP32LE IDENT32
#endif

#if BYTE_ORDER == BIG_ENDIAN
#define SWAP32LE SWAP32
#endif

enum MoneroLanguage {
    MoneroLanguageNone = 0,
    MoneroEnglish = 1,
    MoneroSpanish = 2,
    MoneroFrench = 3,
    MoneroItalian = 4,
    MoneroDutch = 5,
    MoneroPortuguese = 6,
    MoneroRussian = 7,
    MoneroJapanese = 8,
    MoneroChinese = 9,
    MoneroEsperanto = 10,
    MoneroLojban = 11,
    MoneroGerman = 12
};

const char* legacy_monero_mnemonic_from_seed(const uint8_t* seed, uint32_t length, enum MoneroLanguage language);

void clear_legacy_monero_mnemonic(void);

int32_t legacy_monero_mnemonic_check(const char* mnemonic, enum MoneroLanguage language);

int32_t monero_mnemonic_find_word_index_allowing_partial_word(const char* word, enum MoneroLanguage language, uint8_t allow_partial_word);

int32_t monero_mnemonic_find_word_index(const char* word, enum MoneroLanguage language);

const char* get_monero_mnemonic_word_from_list(const int32_t word_index, enum MoneroLanguage language);

uint8_t legacy_monero_mnemonic_to_seed(const char* mnemonic, uint8_t seed[MONERO_SEED_BITS/8], enum MoneroLanguage language);

#endif
