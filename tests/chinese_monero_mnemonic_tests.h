#ifndef CHINESE_MONERO_MNEMONIC_TESTS_H
#define CHINESE_MONERO_MNEMONIC_TESTS_H

#define BOOST_TEST_MODULE chinese monero mnemonic tests

#include <boost/test/included/unit_test.hpp>

extern "C"
{
#include "legacy_monero_mnemonic.h"
}

BOOST_AUTO_TEST_SUITE( chinese_monero_mnemonics )

BOOST_AUTO_TEST_CASE( clear_old_mnemonic )
{
	uint8_t seed[32] = { 0x00 };
	const char* mnemonic = legacy_monero_mnemonic_from_seed(seed, 32, MoneroChinese);
	BOOST_TEST( mnemonic != nullptr );
	BOOST_TEST( (uint8_t)mnemonic[0] == 0xe7 );
	BOOST_TEST( (uint8_t)mnemonic[1] == 0x9a );
	BOOST_TEST( (uint8_t)mnemonic[2] == 0x84 );
	BOOST_TEST( (uint8_t)mnemonic[3] == 0x20 );
	BOOST_TEST( (uint8_t)mnemonic[4] == 0xe7 );
	BOOST_TEST( (uint8_t)mnemonic[5] == 0x9a );
	BOOST_TEST( (uint8_t)mnemonic[6] == 0x84 );
	BOOST_TEST( (uint8_t)mnemonic[7] == 0x20 );
	clear_legacy_monero_mnemonic();
	BOOST_TEST( mnemonic[0] == 0x00 );
}

BOOST_AUTO_TEST_CASE( zero_seed_to_zero_mnemonic )
{
	uint8_t seed[32] = { 0x00 };
	const char* mnemonic = legacy_monero_mnemonic_from_seed(seed, 32, MoneroChinese);
	BOOST_TEST( mnemonic != nullptr );
	BOOST_TEST( std::string(mnemonic) == "的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的" );
	clear_legacy_monero_mnemonic();
	BOOST_TEST( mnemonic[0] == 0x00 );
}

BOOST_AUTO_TEST_CASE( zero_mnemonic_to_zero_seed )
{
	uint8_t seed[32] = { 0xee };
	uint8_t success = legacy_monero_mnemonic_to_seed("的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的 的", seed, MoneroChinese);
	BOOST_TEST( success != 0 );
	for(int i = 0; i < 32; i++)
	{
		BOOST_TEST( seed[i] == 0x00 );
	}
}

static const char* validMnemonics[] = {
	"的 一 是 在 不 了 有 和 人 这 中 大 为 上 个 国 我 以 要 他 时 来 用 们 是",
	//longest possible mnemonic
	"口 口 口 口 口 口 口 口 口 口 口 口 口 口 口 口 口 口 口 口 口 口 口 口 口",
	//no checksum word
	"的 一 是 在 不 了 有 和 人 这 中 大 为 上 个 国 我 以 要 他 时 来 用 们"
};

BOOST_AUTO_TEST_CASE( check_valid_mnemonics )
{
	for (int i = 0; i < sizeof(validMnemonics)/sizeof(char*); i++)
	{
		uint8_t valid = legacy_monero_mnemonic_check(validMnemonics[i], MoneroChinese);
		BOOST_TEST( valid == 1 );
	}
}

BOOST_AUTO_TEST_CASE( valid_mnemonics_to_seeds_and_back )
{
	for (int i = 0; i < sizeof(validMnemonics)/sizeof(char*); i++)
	{
		uint8_t seed[32] = { 0x00 };
		uint8_t success = legacy_monero_mnemonic_to_seed(validMnemonics[i], seed, MoneroChinese);
		BOOST_TEST( success != 0 );
		const char* regeneratedMnemonic = legacy_monero_mnemonic_from_seed(seed, 32, MoneroChinese);
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
		"invalid 一 是 在 不 了 有 和 人 这 中 大 为 上 个 国 我 以 要 他 时 来 用 们 是",
		//less than 24 words
		"的 一 是 在 不 了 有 和 人 这 中 大 为 上 个 国 我 以 要 他 时 来 用",
		//more than 25 words
		"的 一 是 在 不 了 有 和 人 这 中 大 为 上 个 国 我 以 要 他 时 来 用 们 是 是"
        };
	for (int i = 0; i < sizeof(invalidMnemonics)/sizeof(char*); i++)
	{
		uint8_t seed[32] = { 0x00 };
		uint8_t success = legacy_monero_mnemonic_to_seed(invalidMnemonics[i], seed, MoneroChinese);
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
		const char* mnemonic = legacy_monero_mnemonic_from_seed(seed, i, MoneroChinese);
		BOOST_TEST( mnemonic == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( invalid_word_indices )
{
	int32_t indices[] = { -2, -1, 1627, 1628 };
	for(int i = 0; i < 4; i++)
	{
		const char* word = get_monero_mnemonic_word_from_list(indices[i], MoneroChinese);
		BOOST_TEST( word == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( find_null_pointer_word )
{
	const int32_t index = monero_mnemonic_find_word_index_allowing_partial_word(nullptr, MoneroChinese, true);
	BOOST_TEST( index == -1 );
}

BOOST_AUTO_TEST_CASE( find_invalid_words )
{
	const char* words[] = { "abba", "abbd", "inz", "invalid", "jxbjyb", "timer" };
	for(int i = 0; i < sizeof(words)/sizeof(char*); i++)
	{
		const int32_t index = monero_mnemonic_find_word_index_allowing_partial_word(words[i], MoneroChinese, true);
		BOOST_TEST( index == -1 );
	}
}

BOOST_AUTO_TEST_CASE( find_all_words_then_match_index )
{
	for(int i = 0; i < 1626; i++)
	{
		const char* word = get_monero_mnemonic_word_from_list(i, MoneroChinese);
		BOOST_REQUIRE( word != nullptr );
		const int32_t returnedIndex = monero_mnemonic_find_word_index(word, MoneroChinese);
		BOOST_REQUIRE( i == returnedIndex );
	}
}

BOOST_AUTO_TEST_CASE( find_all_partial_words_then_match_index )
{
	for(int i = 0; i < 1626; i++)
	{
		const char* word = get_monero_mnemonic_word_from_list(i, MoneroChinese);
		BOOST_REQUIRE( word != nullptr );
		char partialWord[4*MONERO_CHINESE_PREFIX_LENGTH] = { 0 };
		memcpy(partialWord, word, 4*MONERO_CHINESE_PREFIX_LENGTH);
		const int32_t returnedIndex = monero_mnemonic_find_word_index_allowing_partial_word(partialWord, MoneroChinese, true);
		BOOST_TEST( i == returnedIndex );
	}
}
BOOST_AUTO_TEST_SUITE_END()

#endif
