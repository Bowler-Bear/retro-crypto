#ifndef LOJBAN_MONERO_MNEMONIC_TESTS_H
#define LOJBAN_MONERO_MNEMONIC_TESTS_H

#define BOOST_TEST_MODULE lojban monero mnemonic tests

#include <boost/test/included/unit_test.hpp>

extern "C"
{
#include "legacy_monero_mnemonic.h"
}

BOOST_AUTO_TEST_SUITE( lojban_monero_mnemonics )

BOOST_AUTO_TEST_CASE( clear_old_mnemonic )
{
	uint8_t seed[32] = { 0x00 };
	const char* mnemonic = legacy_monero_mnemonic_from_seed(seed, 32, MoneroLojban);
	BOOST_TEST( mnemonic != nullptr );
	BOOST_TEST( mnemonic[0] == 'b' );
	BOOST_TEST( mnemonic[1] == 'a' );
	BOOST_TEST( mnemonic[2] == 'c' );
	clear_legacy_monero_mnemonic();
	BOOST_TEST( mnemonic[0] == 0x00 );
}

BOOST_AUTO_TEST_CASE( zero_seed_to_zero_mnemonic )
{
	uint8_t seed[32] = { 0x00 };
	const char* mnemonic = legacy_monero_mnemonic_from_seed(seed, 32, MoneroLojban);
	BOOST_TEST( mnemonic != nullptr );
	BOOST_TEST( std::string(mnemonic) == "backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi" );
	clear_legacy_monero_mnemonic();
	BOOST_TEST( mnemonic[0] == 0x00 );
}

BOOST_AUTO_TEST_CASE( zero_mnemonic_to_zero_seed )
{
	uint8_t seed[32] = { 0xee };
	uint8_t success = legacy_monero_mnemonic_to_seed("backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi backi", seed, MoneroLojban);
	BOOST_TEST( success != 0 );
	for(int i = 0; i < 32; i++)
	{
		BOOST_TEST( seed[i] == 0x00 );
	}
}

static const char* validMnemonics[] = {
	"backi bacru badna badri bajra bakfu bakni bakri baktu balji balni balre balvi bambu bancu bandu banfi bangu banli banro banxa banzu bapli barda bakni",
	//longest possible mnemonic
	"zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a zdakemkulgu'a",
	//no checksum word
	"backi bacru badna badri bajra bakfu bakni bakri baktu balji balni balre balvi bambu bancu bandu banfi bangu banli banro banxa banzu bapli barda"
};

BOOST_AUTO_TEST_CASE( check_valid_mnemonics )
{
	for (int i = 0; i < sizeof(validMnemonics)/sizeof(char*); i++)
	{
		uint8_t valid = legacy_monero_mnemonic_check(validMnemonics[i], MoneroLojban);
		BOOST_TEST( valid == 1 );
	}
}

BOOST_AUTO_TEST_CASE( valid_mnemonics_to_seeds_and_back )
{
	for (int i = 0; i < sizeof(validMnemonics)/sizeof(char*); i++)
	{
		uint8_t seed[32] = { 0x00 };
		uint8_t success = legacy_monero_mnemonic_to_seed(validMnemonics[i], seed, MoneroLojban);
		BOOST_TEST( success != 0 );
		const char* regeneratedMnemonic = legacy_monero_mnemonic_from_seed(seed, 32, MoneroLojban);
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
		"invalid bacru badna badri bajra bakfu bakni bakri baktu balji balni balre balvi bambu bancu bandu banfi bangu banli banro banxa banzu bapli barda bakni",
		//less than 24 words
		"backi bacru badna badri bajra bakfu bakni bakri baktu balji balni balre balvi bambu bancu bandu banfi bangu banli banro banxa banzu bapli",
		//more than 25 words
		"backi bacru badna badri bajra bakfu bakni bakri baktu balji balni balre balvi bambu bancu bandu banfi bangu banli banro banxa banzu bapli barda bakni bakni"
        };
	for (int i = 0; i < sizeof(invalidMnemonics)/sizeof(char*); i++)
	{
		uint8_t seed[32] = { 0x00 };
		uint8_t success = legacy_monero_mnemonic_to_seed(invalidMnemonics[i], seed, MoneroLojban);
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
		const char* mnemonic = legacy_monero_mnemonic_from_seed(seed, i, MoneroLojban);
		BOOST_TEST( mnemonic == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( invalid_word_indices )
{
	int32_t indices[] = { -2, -1, 1627, 1628 };
	for(int i = 0; i < 4; i++)
	{
		const char* word = get_monero_mnemonic_word_from_list(indices[i], MoneroLojban);
		BOOST_TEST( word == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( find_null_pointer_word )
{
	const int32_t index = monero_mnemonic_find_word_index_allowing_partial_word(nullptr, MoneroLojban, true);
	BOOST_TEST( index == -1 );
}

BOOST_AUTO_TEST_CASE( find_word_under_4_chars )
{
	int32_t index = monero_mnemonic_find_word_index_allowing_partial_word("", MoneroLojban, true);
	BOOST_TEST( index == -1 );
	index = monero_mnemonic_find_word_index_allowing_partial_word("b", MoneroLojban, true);
	BOOST_TEST( index == -1 );
	index = monero_mnemonic_find_word_index_allowing_partial_word("ba", MoneroLojban, true);
	BOOST_TEST( index == -1 );
	index = monero_mnemonic_find_word_index_allowing_partial_word("bac", MoneroLojban, true);
	BOOST_TEST( index == -1 );
}

BOOST_AUTO_TEST_CASE( find_invalid_words )
{
	const char* words[] = { "abba", "abbd", "inz", "invalid", "jxbjyb", "timer" };
	for(int i = 0; i < sizeof(words)/sizeof(char*); i++)
	{
		const int32_t index = monero_mnemonic_find_word_index_allowing_partial_word(words[i], MoneroLojban, true);
		BOOST_TEST( index == -1 );
	}
}

BOOST_AUTO_TEST_CASE( find_all_words_then_match_index )
{
	for(int i = 0; i < 1626; i++)
	{
		const char* word = get_monero_mnemonic_word_from_list(i, MoneroLojban);
		BOOST_REQUIRE( word != nullptr );
		const int32_t returnedIndex = monero_mnemonic_find_word_index(word, MoneroLojban);
		BOOST_REQUIRE( i == returnedIndex );
	}
}

BOOST_AUTO_TEST_CASE( find_all_partial_words_then_match_index )
{
	for(int i = 0; i < 1626; i++)
	{
		const char* word = get_monero_mnemonic_word_from_list(i, MoneroLojban);
		BOOST_REQUIRE( word != nullptr );
		char partialWord[MONERO_LOJBAN_PREFIX_LENGTH] = { 0 };
		memcpy(partialWord, word, MONERO_LOJBAN_PREFIX_LENGTH);
		const int32_t returnedIndex = monero_mnemonic_find_word_index_allowing_partial_word(partialWord, MoneroLojban, true);
		BOOST_TEST( i == returnedIndex );
	}
}

BOOST_AUTO_TEST_SUITE_END()

#endif
