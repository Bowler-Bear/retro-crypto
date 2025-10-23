#ifndef JAPANESE_MONERO_MNEMONIC_TESTS_H
#define JAPANESE_MONERO_MNEMONIC_TESTS_H

#define BOOST_TEST_MODULE japanese monero mnemonic tests

#include <boost/test/included/unit_test.hpp>

extern "C"
{
#include "legacy_monero_mnemonic.h"
}

BOOST_AUTO_TEST_SUITE( japanese_monero_mnemonics )

BOOST_AUTO_TEST_CASE( clear_old_mnemonic )
{
	uint8_t seed[32] = { 0x00 };
	const char* mnemonic = legacy_monero_mnemonic_from_seed(seed, 32, MoneroJapanese);
	BOOST_TEST( mnemonic != nullptr );
	BOOST_TEST( (uint8_t)mnemonic[0] == 0xe3 );
	BOOST_TEST( (uint8_t)mnemonic[1] == 0x81 );
	BOOST_TEST( (uint8_t)mnemonic[2] == 0x82 );
	BOOST_TEST( (uint8_t)mnemonic[3] == 0xe3 );
	BOOST_TEST( (uint8_t)mnemonic[4] == 0x81 );
	BOOST_TEST( (uint8_t)mnemonic[5] == 0x84 );
	clear_legacy_monero_mnemonic();
	BOOST_TEST( mnemonic[0] == 0x00 );
}

BOOST_AUTO_TEST_CASE( zero_seed_to_zero_mnemonic )
{
	uint8_t seed[32] = { 0x00 };
	const char* mnemonic = legacy_monero_mnemonic_from_seed(seed, 32, MoneroJapanese);
	BOOST_TEST( mnemonic != nullptr );
	BOOST_TEST( std::string(mnemonic) == "あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん" );
	clear_legacy_monero_mnemonic();
	BOOST_TEST( mnemonic[0] == 0x00 );
}

BOOST_AUTO_TEST_CASE( zero_mnemonic_to_zero_seed )
{
	uint8_t seed[32] = { 0xee };
	uint8_t success = legacy_monero_mnemonic_to_seed("あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん あいこくしん", seed, MoneroJapanese);
	BOOST_TEST( success != 0 );
	for(int i = 0; i < 32; i++)
	{
		BOOST_TEST( seed[i] == 0x00 );
	}
}

static const char* validMnemonics[] = {
	"あいこくしん あいさつ あいだ あおぞら あかちゃん あきる あけがた あける あこがれる あさい あさひ あしあと あじわう あずかる あずき あそぶ あたえる あたためる あたりまえ あたる あつい あつかう あっしゅく あつまり あけがた",
	"こゆび さこつ おさめる あいさつ てちょう ぎじゅつしゃ ちきん はやし そふぼ でぬかえ うすぐらい そだてる げいのうじん さんいん にんめい こしつ きょうりゅう くうぐん げひん うしろがみ そめる くめる にもつ のおづま こゆび",
	"こんや そっこう はっぽう かんけい ごうまん ごうい うんちん ざいちゅう さんせい にまめ いわう いれい くしゃみ あおぞら いもうと ついか はたん しかく つながる たんまつ ずいぶん えんとつ たまる たりょう ごうい",
	"いっせい てまえ ひさしぶり げざい うけとる あしあと こうこう いがい はっしん こうかん になう こうそく かわら あんまり だいひょう ごうい ねむたい ねさげ そつぎょう えんとつ せつでん そふぼ そこそこ たいまつばな こうかん",
	//longest possible mnemonic
	"そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう そうがんきょう",
	//no checksum word
	"あいこくしん あいさつ あいだ あおぞら あかちゃん あきる あけがた あける あこがれる あさい あさひ あしあと あじわう あずかる あずき あそぶ あたえる あたためる あたりまえ あたる あつい あつかう あっしゅく あつまり"
};

BOOST_AUTO_TEST_CASE( check_valid_mnemonics )
{
	for (int i = 0; i < sizeof(validMnemonics)/sizeof(char*); i++)
	{
		uint8_t valid = legacy_monero_mnemonic_check(validMnemonics[i], MoneroJapanese);
		BOOST_TEST( valid == 1 );
	}
}

BOOST_AUTO_TEST_CASE( valid_mnemonics_to_seeds_and_back )
{
	for (int i = 0; i < sizeof(validMnemonics)/sizeof(char*); i++)
	{
		uint8_t seed[32] = { 0x00 };
		uint8_t success = legacy_monero_mnemonic_to_seed(validMnemonics[i], seed, MoneroJapanese);
		BOOST_TEST( success != 0 );
		const char* regeneratedMnemonic = legacy_monero_mnemonic_from_seed(seed, 32, MoneroJapanese);
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
		"invalid あいさつ あいだ あおぞら あかちゃん あきる あけがた あける あこがれる あさい あさひ あしあと あじわう あずかる あずき あそぶ あたえる あたためる あたりまえ あたる あつい あつかう あっしゅく あつまり あけがた",
		//less than 24 words
		"あいこくしん あいさつ あいだ あおぞら あかちゃん あきる あけがた あける あこがれる あさい あさひ あしあと あじわう あずかる あずき あそぶ あたえる あたためる あたりまえ あたる あつい あつかう あっしゅく",
		//more than 25 words
		"あいこくしん あいさつ あいだ あおぞら あかちゃん あきる あけがた あける あこがれる あさい あさひ あしあと あじわう あずかる あずき あそぶ あたえる あたためる あたりまえ あたる あつい あつかう あっしゅく あつまり あけがた あけがた"
        };
	for (int i = 0; i < sizeof(invalidMnemonics)/sizeof(char*); i++)
	{
		uint8_t seed[32] = { 0x00 };
		uint8_t success = legacy_monero_mnemonic_to_seed(invalidMnemonics[i], seed, MoneroJapanese);
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
		const char* mnemonic = legacy_monero_mnemonic_from_seed(seed, i, MoneroJapanese);
		BOOST_TEST( mnemonic == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( invalid_word_indices )
{
	int32_t indices[] = { -2, -1, 1627, 1628 };
	for(int i = 0; i < 4; i++)
	{
		const char* word = get_monero_mnemonic_word_from_list(indices[i], MoneroJapanese);
		BOOST_TEST( word == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( find_null_pointer_word )
{
	const int32_t index = monero_mnemonic_find_word_index_allowing_partial_word(nullptr, MoneroJapanese, true);
	BOOST_TEST( index == -1 );
}

BOOST_AUTO_TEST_CASE( find_word_under_3_chars )
{
	int32_t index = monero_mnemonic_find_word_index_allowing_partial_word("", MoneroJapanese, true);
	BOOST_TEST( index == -1 );
	index = monero_mnemonic_find_word_index_allowing_partial_word("a", MoneroJapanese, true);
	BOOST_TEST( index == -1 );
	index = monero_mnemonic_find_word_index_allowing_partial_word("aa", MoneroJapanese, true);
	BOOST_TEST( index == -1 );
	index = monero_mnemonic_find_word_index_allowing_partial_word("aba", MoneroJapanese, true);
	BOOST_TEST( index == -1 );
}

BOOST_AUTO_TEST_CASE( find_invalid_words )
{
	const char* words[] = { "abba", "abbd", "inz", "invalid", "jxbjyb", "timer" };
	for(int i = 0; i < sizeof(words)/sizeof(char*); i++)
	{
		const int32_t index = monero_mnemonic_find_word_index_allowing_partial_word(words[i], MoneroJapanese, true);
		BOOST_TEST( index == -1 );
	}
}

BOOST_AUTO_TEST_CASE( find_all_words_then_match_index )
{
	for(int i = 0; i < 1626; i++)
	{
		const char* word = get_monero_mnemonic_word_from_list(i, MoneroJapanese);
		BOOST_REQUIRE( word != nullptr );
		const int32_t returnedIndex = monero_mnemonic_find_word_index(word, MoneroJapanese);
		BOOST_REQUIRE( i == returnedIndex );
	}
}

BOOST_AUTO_TEST_CASE( find_all_partial_words_then_match_index )
{
	for(int i = 0; i < 1626; i++)
	{
		const char* word = get_monero_mnemonic_word_from_list(i, MoneroJapanese);
		BOOST_REQUIRE( word != nullptr );
		char partialWord[3*MONERO_JAPANESE_PREFIX_LENGTH] = { 0 };
		memcpy(partialWord, word, 3*MONERO_JAPANESE_PREFIX_LENGTH);
		const int32_t returnedIndex = monero_mnemonic_find_word_index_allowing_partial_word(partialWord, MoneroJapanese, true);
		BOOST_TEST( i == returnedIndex );
	}
}

BOOST_AUTO_TEST_SUITE_END()

#endif
