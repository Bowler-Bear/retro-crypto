#include <stdbool.h>
#include <string.h>

#include "legacy_monero_mnemonic.h"
#include "monero_english_words.h"
#include "monero_french_words.h"
#include "monero_italian_words.h"
#include "monero_dutch_words.h"
#include "monero_portuguese_words.h"
#include "monero_russian_words.h"
#include "monero_japanese_words.h"
#include "monero_chinese_words.h"
#include "monero_esperanto_words.h"
#include "memzero.h"
#include "crc.h"

static char static_monero_mnemonic[MONERO_MNEMONIC_MAXIMUM_LENGTH];

const char** legacy_monero_mnemonic_get_word_list(enum MoneroLanguage language) {
    switch(language) {
    case MoneroEnglish:
        return monero_english_words;
    case MoneroSpanish:
        return 0;
    case MoneroFrench:
        return monero_french_words;
    case MoneroItalian:
        return monero_italian_words;
    case MoneroDutch:
        return monero_dutch_words;
    case MoneroPortuguese:
        return monero_portuguese_words;
    case MoneroRussian:
        return monero_russian_words;
    case MoneroJapanese:
        return monero_japanese_words;
    case MoneroChinese:
        return monero_chinese_words;
    case MoneroEsperanto:
        return monero_esperanto_words;
    case MoneroLojban:
        return 0;
    case MoneroGerman:
        return 0;
    default:
        return 0;
    }
}

uint8_t legacy_monero_mnemonic_language_prefix_length(enum MoneroLanguage language) {
    switch(language) {
    case MoneroEnglish:
        return MONERO_ENGLISH_PREFIX_LENGTH;
    case MoneroSpanish:
        return MONERO_SPANISH_PREFIX_LENGTH;
    case MoneroFrench:
        return MONERO_FRENCH_PREFIX_LENGTH;
    case MoneroItalian:
        return MONERO_ITALIAN_PREFIX_LENGTH;
    case MoneroDutch:
        return MONERO_DUTCH_PREFIX_LENGTH;
    case MoneroPortuguese:
        return MONERO_PORTUGUESE_PREFIX_LENGTH;
    case MoneroRussian:
        return MONERO_RUSSIAN_PREFIX_LENGTH;
    case MoneroJapanese:
        return MONERO_JAPANESE_PREFIX_LENGTH;
    case MoneroChinese:
        return MONERO_CHINESE_PREFIX_LENGTH;
    case MoneroEsperanto:
        return MONERO_ESPERANTO_PREFIX_LENGTH;
    case MoneroLojban:
        return MONERO_LOJBAN_PREFIX_LENGTH;
    case MoneroGerman:
        return MONERO_GERMAN_PREFIX_LENGTH;
    default:
        return 0;
    }
}

int32_t legacy_monero_mnemonic_get_checksum_index(const char** mnemonic, enum MoneroLanguage language) {
    const uint8_t unique_prefix_length = legacy_monero_mnemonic_language_prefix_length(language);
    if (unique_prefix_length <= 0) return -1;

    char trimmed_words[4*MONERO_MAXIMUM_PREFIX_LENGTH*MONERO_MNEMONIC_WORDS_COUNT] = { '\0' };

    for (uint32_t i = 0; i < MONERO_MNEMONIC_WORDS_COUNT; i++) {
        uint8_t character_count = 0;
        for (uint32_t j = 0; j < strlen(mnemonic[i]) && character_count < unique_prefix_length; j++) {
            uint8_t code_point_count = 0;
            if ((mnemonic[i][j] & 0x80) == 0) {
                code_point_count = 0;
            } else if((mnemonic[i][j] & 0xe0) == 0xc0) {
                code_point_count = 1;
            } else if((mnemonic[i][j] & 0xf0) == 0xe0) {
                code_point_count = 2;
            } else if((mnemonic[i][j] & 0xf8) == 0xf0) {
                code_point_count = 3;
            }
            for (uint32_t k = 0; k < code_point_count+1; k++) {
                if (strlen(trimmed_words) < 4*MONERO_MAXIMUM_PREFIX_LENGTH*MONERO_MNEMONIC_WORDS_COUNT)
                    trimmed_words[strlen(trimmed_words)] = mnemonic[i][j+k];
            }
            j += code_point_count;
            character_count += 1;
        }
    }
    uint32_t index = (checksum_crc32(trimmed_words, strlen(trimmed_words), 0xffffffff) ^ 0xffffffff) % MONERO_MNEMONIC_WORDS_COUNT;

    memzero(trimmed_words, sizeof(trimmed_words));
    return index;
}

void create_pointer_to_monero_mnemonic_blocks(char* mnemonic_blocks[MONERO_MNEMONIC_WORDS_COUNT+1]) {
    for (uint32_t i = 0; i < MONERO_MNEMONIC_WORDS_COUNT+1; i++) {
        mnemonic_blocks[i] = ((char*)(&static_monero_mnemonic)) + i*(MONERO_MAXIMUM_WORD_LENGTH+2);
    }
}

uint32_t load_into_static_monero_mnemonic(const char* words, char* mnemonic_blocks[MONERO_MNEMONIC_WORDS_COUNT+1]) {
    create_pointer_to_monero_mnemonic_blocks(mnemonic_blocks);
    if (!words) return 0;
    //strtok not available for arm-none-eabi
    /*char words_copy[MONERO_MNEMONIC_MAXIMUM_LENGTH];
    strcpy(words_copy, words);
    char* current_word = strtok(words_copy, " ");
    uint32_t word_count = 0;
    while (current_word != NULL) {
        if (strlen(current_word) <= MONERO_MAXIMUM_WORD_LENGTH) {
            strcpy(mnemonic_blocks[word_count], current_word);
            word_count++;
        }
        current_word = strtok(NULL, " ");
    }
    memzero(words_copy, sizeof(words));*/
    const char* searchPointer = words;
    uint32_t i = 0;
    while (searchPointer && *searchPointer != '\0') {
         if (*searchPointer == ' ') {
             searchPointer++;
             continue;
         }
         for (uint32_t j = 0; j <= MONERO_MAXIMUM_WORD_LENGTH; j++) {
             if (*searchPointer == ' ' || *searchPointer == '\0') {
                 break;
             }
             if (i*(MONERO_MAXIMUM_WORD_LENGTH+2)+j >= MONERO_MNEMONIC_MAXIMUM_LENGTH) {
               return 0;
             }
             mnemonic_blocks[i][j] = *searchPointer;
             searchPointer++;
         }
         i++;
    }
    return i;
}

int32_t legacy_monero_mnemonic_check_words(const char* words[MONERO_MNEMONIC_WORDS_COUNT+1], enum MoneroLanguage language) {
    int32_t checksum_index = legacy_monero_mnemonic_get_checksum_index(words, language);
    if (checksum_index < 0) return 0;
    return !strcmp(words[checksum_index], words[MONERO_MNEMONIC_WORDS_COUNT]);
}

int32_t legacy_monero_mnemonic_check(const char* mnemonic, enum MoneroLanguage language) {
    //TODO: Auto detect mnemonic language if more languages added
    char* words[MONERO_MNEMONIC_WORDS_COUNT+1];
    uint32_t word_count = load_into_static_monero_mnemonic(mnemonic, words);
    if (word_count == MONERO_MNEMONIC_WORDS_COUNT) {
        for (uint8_t i = 0; i < MONERO_MNEMONIC_WORDS_COUNT; i++) {
            if (monero_mnemonic_find_word_index(words[i], language) == -1) {
                clear_legacy_monero_mnemonic();
                return 0;
            }
        }
        clear_legacy_monero_mnemonic();
        return 1;
    } else if (word_count == MONERO_MNEMONIC_WORDS_COUNT+1) {
        int32_t checksum_match = legacy_monero_mnemonic_check_words((const char**)words, language);
        clear_legacy_monero_mnemonic();
        return checksum_match;
    } else {
        clear_legacy_monero_mnemonic();
        return 0;
    }
}

const char* legacy_monero_mnemonic_from_seed(const uint8_t* seed, uint32_t len, enum MoneroLanguage language) {
    if (len != MONERO_SEED_BITS/8) return 0;

    const char** word_list = legacy_monero_mnemonic_get_word_list(language);
    if (!word_list) return 0;

    char* mnemonic_blocks[MONERO_MNEMONIC_WORDS_COUNT+1];
    create_pointer_to_monero_mnemonic_blocks(mnemonic_blocks);
    for (uint32_t i = 0; i < len/4; i++) {
        uint32_t w[4];

        w[0] = SWAP32LE(*(const uint32_t*)(seed + (i * 4)));

        w[1] = w[0] % MONERO_WORDLIST_WORD_COUNT;
        w[2] = ((w[0] / MONERO_WORDLIST_WORD_COUNT) + w[1]) % MONERO_WORDLIST_WORD_COUNT;
        w[3] = (((w[0] / MONERO_WORDLIST_WORD_COUNT) / MONERO_WORDLIST_WORD_COUNT) + w[2]) % MONERO_WORDLIST_WORD_COUNT;

        strcpy(mnemonic_blocks[3*i], word_list[w[1]]);
        strcpy(mnemonic_blocks[3*i+1], word_list[w[2]]);
        strcpy(mnemonic_blocks[3*i+2], word_list[w[3]]);
        memzero(w, sizeof(w));
    }
    int8_t checksum_index = legacy_monero_mnemonic_get_checksum_index((const char**)mnemonic_blocks, language);
    if (checksum_index < 0) return 0;
    strcpy(mnemonic_blocks[MONERO_MNEMONIC_WORDS_COUNT], mnemonic_blocks[checksum_index]);
    char* smm = static_monero_mnemonic;
    for (uint32_t i = 0; i < MONERO_MNEMONIC_WORDS_COUNT+1; i++) {
        for (uint32_t j = 0; j < MONERO_MAXIMUM_WORD_LENGTH+2; j++) {
            if (mnemonic_blocks[i][j] == 0x00) {
                if (i < MONERO_MNEMONIC_WORDS_COUNT) {
                    *smm = ' ';
                    smm++;
                }
                break;
            }
            *smm = mnemonic_blocks[i][j];
            smm++;
        }
    }
    *smm = '\0';

    return static_monero_mnemonic;
}

void clear_legacy_monero_mnemonic(void) {
    memzero(static_monero_mnemonic, sizeof(static_monero_mnemonic));
}

int32_t monero_mnemonic_find_word_index_allowing_partial_word(const char* word, enum MoneroLanguage language, uint8_t allow_partial_word) {
    if (!word) return -1;

    uint8_t word_length = strlen(word);
    uint8_t minPrefixLength = legacy_monero_mnemonic_language_prefix_length(language);
    if (word_length < minPrefixLength) return -1;

    const char** word_list = legacy_monero_mnemonic_get_word_list(language);
    if (!word_list) return -1;

    if (language == MoneroPortuguese) {
        if ((allow_partial_word && strncmp("botina", word, word_length) == 0) || strcmp("botina", word) == 0) {
            return 209;
        }
        else if ((allow_partial_word && strncmp("boquiaberto", word, word_length) == 0) || strcmp("boquiaberto", word) == 0) {
            return 210;
        }
        else if ((allow_partial_word && strncmp("glaucio", word, word_length) == 0) || strcmp("glaucio", word) == 0) {
            return 628;
        }
        else if ((allow_partial_word && strncmp("grelhado", word, word_length) == 0) || strcmp("grelhado", word) == 0) {
            return 660;
        }
        else if ((allow_partial_word && strncmp("respeito", word, word_length) == 0) || strcmp("respeito", word) == 0) {
            return 1282;
        }
    } else if (language == MoneroJapanese || language == MoneroChinese) {
        for (int i = 0; i < 1626; i++) {
            if (allow_partial_word) {
                if(strncmp(word_list[i], word, word_length) == 0) {
                    return i;
                }
            } else if(strcmp(word_list[i], word) == 0) {
                return i;
            }
        }
        return -1;
    }

    uint32_t lower = 0, upper = MONERO_WORDLIST_WORD_COUNT - 1;
    int32_t comparison_result = 0;
    while (upper >= lower) {
        uint32_t tested_midpoint = (upper-lower)/2 + lower;
        if (allow_partial_word) {
            comparison_result = strncmp(word_list[tested_midpoint], word, word_length);
        } else {
            comparison_result = strcmp(word_list[tested_midpoint], word);
        }
        if (comparison_result < 0) {
            lower = tested_midpoint + 1;
        } else if (comparison_result == 0) {
            return tested_midpoint;
        } else if (comparison_result > 0) {
            upper = tested_midpoint - 1;
        }
        if (lower < 0 || lower >= MONERO_WORDLIST_WORD_COUNT || upper < 0 || upper >= MONERO_WORDLIST_WORD_COUNT)
            return -1;
    }
    return -1;
}

int32_t monero_mnemonic_find_word_index(const char* word, enum MoneroLanguage language) {
    return monero_mnemonic_find_word_index_allowing_partial_word(word, language, 0);
}

const char* get_monero_mnemonic_word_from_list(const int32_t word_index, enum MoneroLanguage language) {
    if (word_index < 0 || word_index >= MONERO_WORDLIST_WORD_COUNT) return 0;

    const char** word_list = legacy_monero_mnemonic_get_word_list(language);
    if (!word_list) return 0;

    return word_list[word_index];
}

uint8_t legacy_monero_mnemonic_to_seed(const char* mnemonic, uint8_t seed[MONERO_SEED_BITS/8], enum MoneroLanguage language) {
    if(!mnemonic || strlen(mnemonic) > MONERO_MNEMONIC_MAXIMUM_LENGTH) return 0;
    char* words[MONERO_MNEMONIC_WORDS_COUNT+1];
    uint32_t word_count = load_into_static_monero_mnemonic(mnemonic, words);

    bool has_checksum = word_count == (MONERO_MNEMONIC_WORDS_COUNT + 1);
    if (word_count != MONERO_MNEMONIC_WORDS_COUNT && word_count != MONERO_MNEMONIC_WORDS_COUNT + 1) {
        clear_legacy_monero_mnemonic();
        return 0;
    }

    //TODO: Auto detect mnemonic language if more languages added
    if (has_checksum && !legacy_monero_mnemonic_check_words((const char**)words, language)) {
        clear_legacy_monero_mnemonic();
        return 0;
    }

    uint32_t matched_indices[MONERO_MNEMONIC_WORDS_COUNT+1];
    for (uint32_t i = 0; i < word_count; i++) {
        int32_t found_index = monero_mnemonic_find_word_index(words[i], language);
        if (found_index < 0) {
            memzero(matched_indices, sizeof(matched_indices));
            clear_legacy_monero_mnemonic();
            return 0;
        }
        matched_indices[i] = found_index;
    }
    clear_legacy_monero_mnemonic();
    uint32_t word_list_length = MONERO_WORDLIST_WORD_COUNT;
    for (unsigned int i = 0; i < MONERO_MNEMONIC_WORDS_COUNT / 3; i++) {
        uint32_t w[4];
        w[1] = matched_indices[i*3];
        w[2] = matched_indices[i*3+1];
        w[3] = matched_indices[i*3+2];

        w[0]= w[1] + word_list_length * (((word_list_length - w[1]) + w[2]) % word_list_length) +
          word_list_length * word_list_length * (((word_list_length - w[2]) + w[3]) % word_list_length);

        if (!(w[0] % word_list_length == w[1])) {
            memzero(w, sizeof(w));
            return 0;
        }

        w[0] = SWAP32LE(w[0]);
        for (uint8_t j = 0; j < sizeof(uint32_t)/sizeof(uint8_t); j++) {
            seed[4*i+j] = (w[0] >> 8*j) & 0xFF;
        }
        memzero(w, sizeof(w));
    }
    memzero(matched_indices, sizeof(matched_indices));
    return 1;
}
