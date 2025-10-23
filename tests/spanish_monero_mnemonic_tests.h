#ifndef SPANISH_MONERO_MNEMONIC_TESTS_H
#define SPANISH_MONERO_MNEMONIC_TESTS_H

#define BOOST_TEST_MODULE spanish monero mnemonic tests

#include <boost/test/included/unit_test.hpp>

extern "C"
{
#include "legacy_monero_mnemonic.h"
}

BOOST_AUTO_TEST_SUITE( spanish_monero_mnemonics )

BOOST_AUTO_TEST_CASE( clear_old_mnemonic )
{
	uint8_t seed[32] = { 0x00 };
	const char* mnemonic = legacy_monero_mnemonic_from_seed(seed, 32, MoneroSpanish);
	BOOST_TEST( mnemonic != nullptr );
	BOOST_TEST( (uint8_t)mnemonic[0] == 0xc3 );
	BOOST_TEST( (uint8_t)mnemonic[1] == 0xa1 );
	BOOST_TEST( mnemonic[2] == 'b' );
	clear_legacy_monero_mnemonic();
	BOOST_TEST( mnemonic[0] == 0x00 );
}

BOOST_AUTO_TEST_CASE( zero_seed_to_zero_mnemonic )
{
	uint8_t seed[32] = { 0x00 };
	const char* mnemonic = legacy_monero_mnemonic_from_seed(seed, 32, MoneroSpanish);
	BOOST_TEST( mnemonic != nullptr );
	BOOST_TEST( std::string(mnemonic) == "ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco" );
	clear_legacy_monero_mnemonic();
	BOOST_TEST( mnemonic[0] == 0x00 );
}

BOOST_AUTO_TEST_CASE( zero_mnemonic_to_zero_seed )
{
	uint8_t seed[32] = { 0xee };
	uint8_t success = legacy_monero_mnemonic_to_seed("ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco ábaco", seed, MoneroSpanish);
	BOOST_TEST( success != 0 );
	for(int i = 0; i < 32; i++)
	{
		BOOST_TEST( seed[i] == 0x00 );
	}
}

static const char* validMnemonics[] = {
	"ábaco abdomen abeja abierto abogado abono aborto abrazo abrir abuelo abuso acabar academia acceso acción aceite acelga acento aceptar ácido aclarar acné acoger acoso acento",
	//longest possible mnemonic
	"petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo petróleo",
	//no checksum word
	"ábaco abdomen abeja abierto abogado abono aborto abrazo abrir abuelo abuso acabar academia acceso acción aceite acelga acento aceptar ácido aclarar acné acoger acoso acento"
};

BOOST_AUTO_TEST_CASE( check_valid_mnemonics )
{
	for (int i = 0; i < sizeof(validMnemonics)/sizeof(char*); i++)
	{
		uint8_t valid = legacy_monero_mnemonic_check(validMnemonics[i], MoneroSpanish);
		BOOST_TEST( valid == 1 );
	}
}

BOOST_AUTO_TEST_CASE( valid_mnemonics_to_seeds_and_back )
{
	for (int i = 0; i < sizeof(validMnemonics)/sizeof(char*); i++)
	{
		uint8_t seed[32] = { 0x00 };
		uint8_t success = legacy_monero_mnemonic_to_seed(validMnemonics[i], seed, MoneroSpanish);
		BOOST_TEST( success != 0 );
		const char* regeneratedMnemonic = legacy_monero_mnemonic_from_seed(seed, 32, MoneroSpanish);
		BOOST_TEST( regeneratedMnemonic != nullptr );
		BOOST_TEST( memcmp(validMnemonics[i], regeneratedMnemonic, strlen(validMnemonics[i])) == 0 );
		clear_legacy_monero_mnemonic();
		BOOST_TEST( regeneratedMnemonic[0] == 0x00 );
	}
}

BOOST_AUTO_TEST_CASE( invalid_mnemonics )
{
	const char* invalidMnemonics[] = {
		//'invalid' not a word in the word list
		"invalid abdomen abeja abierto abogado abono aborto abrazo abrir abuelo abuso acabar academia acceso acción aceite acelga acento aceptar ácido aclarar acné acoger acoso acento",
		//less than 24 words
		"ábaco abdomen abeja abierto abogado abono aborto abrazo abrir abuelo abuso acabar academia acceso acción aceite acelga acento aceptar ácido aclarar acné acoger",
		//more than 25 words
		" ábaco abdomen abeja abierto abogado abono aborto abrazo abrir abuelo abuso acabar academia acceso acción aceite acelga acento aceptar ácido aclarar acné acoger acoso acento acento"
        };
	for (int i = 0; i < sizeof(invalidMnemonics)/sizeof(char*); i++)
	{
		uint8_t seed[32] = { 0x00 };
		uint8_t success = legacy_monero_mnemonic_to_seed(invalidMnemonics[i], seed, MoneroSpanish);
		BOOST_TEST( success == 0 );
	}
}

BOOST_AUTO_TEST_CASE( bad_seed_sizes )
{
	const uint8_t seed[32] = { 0x00 };
	for (int i = 0; i < 65; i++)
	{
		if (i == 32)
			continue;
		const char* mnemonic = legacy_monero_mnemonic_from_seed(seed, i, MoneroSpanish);
		BOOST_TEST( mnemonic == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( invalid_word_indices )
{
	int32_t indices[] = { -2, -1, 1627, 1628 };
	for(int i = 0; i < 4; i++)
	{
		const char* word = get_monero_mnemonic_word_from_list(indices[i], MoneroSpanish);
		BOOST_TEST( word == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( find_null_pointer_word )
{
	const int32_t index = monero_mnemonic_find_word_index_allowing_partial_word(nullptr, MoneroSpanish, true);
	BOOST_TEST( index == -1 );
}

BOOST_AUTO_TEST_CASE( find_word_under_3_chars )
{
	int32_t index = monero_mnemonic_find_word_index_allowing_partial_word("", MoneroSpanish, true);
	BOOST_TEST( index == -1 );
	index = monero_mnemonic_find_word_index_allowing_partial_word("a", MoneroSpanish, true);
	BOOST_TEST( index == -1 );
	index = monero_mnemonic_find_word_index_allowing_partial_word("aa", MoneroSpanish, true);
	BOOST_TEST( index == -1 );
}

BOOST_AUTO_TEST_CASE( find_invalid_words )
{
	const char* words[] = { "abba", "abbd", "inz", "invalid", "jxbjyb", "timer" };
	for(int i = 0; i < sizeof(words)/sizeof(char*); i++)
	{
		const int32_t index = monero_mnemonic_find_word_index_allowing_partial_word(words[i], MoneroSpanish, true);
		BOOST_TEST( index == -1 );
	}
}

BOOST_AUTO_TEST_CASE( find_all_words_then_match_index )
{
	for(int i = 0; i < 1626; i++)
	{
		const char* word = get_monero_mnemonic_word_from_list(i, MoneroSpanish);
		BOOST_REQUIRE( word != nullptr );
		const int32_t returnedIndex = monero_mnemonic_find_word_index(word, MoneroSpanish);
		BOOST_TEST( i == returnedIndex );
	}
}

BOOST_AUTO_TEST_CASE( find_all_partial_words_then_match_index )
{
	for(int i = 0; i < 1626; i++)
	{
		const char* word = get_monero_mnemonic_word_from_list(i, MoneroSpanish);
		BOOST_REQUIRE( word != nullptr );
		char partialWord[2*MONERO_SPANISH_PREFIX_LENGTH] = { 0 };
		uint8_t characters = 0;
		while(*word != 0 && characters < MONERO_SPANISH_PREFIX_LENGTH)
		{
			partialWord[strlen(partialWord)] = *word;
			if ((*word & 0xe0) != 0xc0)
			{
				characters++;
			}
			word++;
		}
		const int32_t returnedIndex = monero_mnemonic_find_word_index_allowing_partial_word(partialWord, MoneroSpanish, true);
		BOOST_TEST( i == returnedIndex );
	}
}

BOOST_AUTO_TEST_SUITE_END()

#endif
