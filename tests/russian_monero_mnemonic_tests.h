#ifndef RUSSIAN_MONERO_MNEMONIC_TESTS_H
#define RUSSIAN_MONERO_MNEMONIC_TESTS_H

#define BOOST_TEST_MODULE russian monero mnemonic tests

#include <boost/test/included/unit_test.hpp>

extern "C"
{
#include "legacy_monero_mnemonic.h"
}

BOOST_AUTO_TEST_SUITE( russian_monero_mnemonics )

BOOST_AUTO_TEST_CASE( clear_old_mnemonic )
{
	uint8_t seed[32] = { 0x00 };
	const char* mnemonic = legacy_monero_mnemonic_from_seed(seed, 32, MoneroRussian);
	BOOST_TEST( mnemonic != nullptr );
	BOOST_TEST( (uint8_t)mnemonic[0] == 0xd0 );
	BOOST_TEST( (uint8_t)mnemonic[1] == 0xb0 );
	BOOST_TEST( (uint8_t)mnemonic[2] == 0xd0 );
	BOOST_TEST( (uint8_t)mnemonic[3] == 0xb1 );
	clear_legacy_monero_mnemonic();
	BOOST_TEST( mnemonic[0] == 0x00 );
}

BOOST_AUTO_TEST_CASE( zero_seed_to_zero_mnemonic )
{
	uint8_t seed[32] = { 0x00 };
	const char* mnemonic = legacy_monero_mnemonic_from_seed(seed, 32, MoneroRussian);
	BOOST_TEST( mnemonic != nullptr );
	BOOST_TEST( std::string(mnemonic) == "абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур" );
	clear_legacy_monero_mnemonic();
	BOOST_TEST( mnemonic[0] == 0x00 );
}

BOOST_AUTO_TEST_CASE( zero_mnemonic_to_zero_seed )
{
	uint8_t seed[32] = { 0xee };
	uint8_t success = legacy_monero_mnemonic_to_seed("абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур абажур", seed, MoneroRussian);
	BOOST_TEST( success != 0 );
	for(int i = 0; i < 32; i++)
	{
		BOOST_TEST( seed[i] == 0x00 );
	}
}

static const char* validMnemonics[] = {
	"абажур абзац абонент абрикос абсурд авангард август авиация авоська автор агат агент агитатор агнец агония агрегат адвокат адмирал адрес ажиотаж азарт азбука азот аист абзац",
	"катер подумать вокзал часть ирония изгонять фаворит впускать поток лаять день ведущий кудрявый арабский дятел чулок шагать сгорать готовый детский рогатый директор класс клиент кудрявый",
	"дукат кухня асфальт зажечь папка куча плыть зять лента зной членство интерес раунд кекс бусы фото утро точка мудрый хозяин решение убогий арбуз бикини раунд",
	"проблема хозяин аккорд никель сегмент козел кодекс скучный пиковый дизайн дощатый гаишник гипотеза сустав озарение товарищ текст хриплый штык суждение аккорд шкура рыба свобода аккорд",
	//longest possible mnemonic
	"авангард авангард авангард авангард авангард авангард авангард авангард авангард авангард авангард авангард авангард авангард авангард авангард авангард авангард авангард авангард авангард авангард авангард авангард авангард",
	//no checksum word
	"абажур абзац абонент абрикос абсурд авангард август авиация авоська автор агат агент агитатор агнец агония агрегат адвокат адмирал адрес ажиотаж азарт азбука азот аист"
};

BOOST_AUTO_TEST_CASE( check_valid_mnemonics )
{
	for (int i = 0; i < sizeof(validMnemonics)/sizeof(char*); i++)
	{
		uint8_t valid = legacy_monero_mnemonic_check(validMnemonics[i], MoneroRussian);
		BOOST_TEST( valid == 1 );
	}
}

BOOST_AUTO_TEST_CASE( valid_mnemonics_to_seeds_and_back )
{
	for (int i = 0; i < sizeof(validMnemonics)/sizeof(char*); i++)
	{
		uint8_t seed[32] = { 0x00 };
		uint8_t success = legacy_monero_mnemonic_to_seed(validMnemonics[i], seed, MoneroRussian);
		BOOST_TEST( success != 0 );
		const char* regeneratedMnemonic = legacy_monero_mnemonic_from_seed(seed, 32, MoneroRussian);
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
		"invalid абзац абонент абрикос абсурд авангард август авиация авоська автор агат агент агитатор агнец агония агрегат адвокат адмирал адрес ажиотаж азарт азбука азот аист абзац",
		//less than 24 words
		"абажур абзац абонент абрикос абсурд авангард август авиация авоська автор агат агент агитатор агнец агония агрегат адвокат адмирал адрес ажиотаж азарт азбука азот",
		//more than 25 words
		"абажур абзац абонент абрикос абсурд авангард август авиация авоська автор агат агент агитатор агнец агония агрегат адвокат адмирал адрес ажиотаж азарт азбука азот аист абзац абзац"
        };
	for (int i = 0; i < sizeof(invalidMnemonics)/sizeof(char*); i++)
	{
		uint8_t seed[32] = { 0x00 };
		uint8_t success = legacy_monero_mnemonic_to_seed(invalidMnemonics[i], seed, MoneroRussian);
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
		const char* mnemonic = legacy_monero_mnemonic_from_seed(seed, i, MoneroRussian);
		BOOST_TEST( mnemonic == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( invalid_word_indices )
{
	int32_t indices[] = { -2, -1, 1627, 1628 };
	for(int i = 0; i < 4; i++)
	{
		const char* word = get_monero_mnemonic_word_from_list(indices[i], MoneroRussian);
		BOOST_TEST( word == nullptr );
	}
}

BOOST_AUTO_TEST_CASE( find_null_pointer_word )
{
	const int32_t index = monero_mnemonic_find_word_index_allowing_partial_word(nullptr, MoneroRussian, true);
	BOOST_TEST( index == -1 );
}

BOOST_AUTO_TEST_CASE( find_word_under_3_chars )
{
	int32_t index = monero_mnemonic_find_word_index_allowing_partial_word("", MoneroRussian, true);
	BOOST_TEST( index == -1 );
	index = monero_mnemonic_find_word_index_allowing_partial_word("a", MoneroRussian, true);
	BOOST_TEST( index == -1 );
	index = monero_mnemonic_find_word_index_allowing_partial_word("aa", MoneroRussian, true);
	BOOST_TEST( index == -1 );
}

BOOST_AUTO_TEST_CASE( find_invalid_words )
{
	const char* words[] = { "abba", "abbd", "inz", "invalid", "jxbjyb", "timer" };
	for(int i = 0; i < sizeof(words)/sizeof(char*); i++)
	{
		const int32_t index = monero_mnemonic_find_word_index_allowing_partial_word(words[i], MoneroRussian, true);
		BOOST_TEST( index == -1 );
	}
}

BOOST_AUTO_TEST_CASE( find_all_words_then_match_index )
{
	for(int i = 0; i < 1626; i++)
	{
		const char* word = get_monero_mnemonic_word_from_list(i, MoneroRussian);
		BOOST_REQUIRE( word != nullptr );
		const int32_t returnedIndex = monero_mnemonic_find_word_index(word, MoneroRussian);
		BOOST_REQUIRE( i == returnedIndex );
	}
}

BOOST_AUTO_TEST_CASE( find_all_partial_words_then_match_index )
{
	for(int i = 0; i < 1626; i++)
	{
		const char* word = get_monero_mnemonic_word_from_list(i, MoneroRussian);
		BOOST_REQUIRE( word != nullptr );
		char partialWord[2*MONERO_RUSSIAN_PREFIX_LENGTH] = { 0 };
		memcpy(partialWord, word, 2*MONERO_RUSSIAN_PREFIX_LENGTH);
		const int32_t returnedIndex = monero_mnemonic_find_word_index_allowing_partial_word(partialWord, MoneroRussian, true);
		BOOST_TEST( i == returnedIndex );
	}
}

BOOST_AUTO_TEST_SUITE_END()

#endif
