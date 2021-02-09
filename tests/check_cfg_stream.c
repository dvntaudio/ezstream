#include <check.h>
#include <limits.h>
#include <netdb.h>

#include "cfg_private.h"
#include "log.h"

#include "check_cfg.h"

static void	_sl_cb(cfg_stream_t, void *);

Suite * cfg_suite(void);
void	setup_checked(void);
void	teardown_checked(void);

cfg_stream_list_t	streams;

static void
_sl_cb(cfg_stream_t str, void *arg)
{
	int	*count = (int *)arg;

	ck_assert_ptr_ne(cfg_stream_get_name(str), NULL);
	(*count)++;
}

START_TEST(test_stream_list_get)
{
	cfg_stream_t	str, str2;
	int		count = 0;

	ck_assert_ptr_eq(cfg_stream_list_get(streams, NULL), NULL);
	ck_assert_ptr_eq(cfg_stream_list_get(streams, ""), NULL);

	str = cfg_stream_list_get(streams, "TeSt");
	str2 = cfg_stream_list_get(streams, "test");
	ck_assert_ptr_eq(str, str2);

	(void)cfg_stream_list_get(streams, "test2");
	ck_assert_uint_eq(cfg_stream_list_nentries(streams), 2);
	cfg_stream_list_foreach(streams, _sl_cb, &count);
	ck_assert_int_eq(count, 2);
}
END_TEST

START_TEST(test_stream_str2fmt)
{
	enum cfg_stream_format	fmt;

	ck_assert_int_eq(cfg_stream_str2fmt(CFG_SFMT_OGG, &fmt), 0);
	ck_assert_int_eq(fmt, CFG_STREAM_OGG);
	ck_assert_int_eq(cfg_stream_str2fmt(CFG_SFMT_MP3, &fmt), 0);
	ck_assert_int_eq(fmt, CFG_STREAM_MP3);
	ck_assert_int_eq(cfg_stream_str2fmt(CFG_SFMT_WEBM, &fmt), 0);
	ck_assert_int_eq(fmt, CFG_STREAM_WEBM);
	ck_assert_int_eq(cfg_stream_str2fmt(CFG_SFMT_MATROSKA, &fmt), 0);
	ck_assert_int_eq(fmt, CFG_STREAM_MATROSKA);
	ck_assert_int_eq(cfg_stream_str2fmt("<something else>", &fmt), -1);
}
END_TEST

START_TEST(test_stream_fmt2str)
{
	ck_assert_str_eq(cfg_stream_fmt2str(CFG_STREAM_OGG),
	    CFG_SFMT_OGG);
	ck_assert_str_eq(cfg_stream_fmt2str(CFG_STREAM_MP3),
	    CFG_SFMT_MP3);
	ck_assert_str_eq(cfg_stream_fmt2str(CFG_STREAM_WEBM),
	    CFG_SFMT_WEBM);
	ck_assert_str_eq(cfg_stream_fmt2str(CFG_STREAM_MATROSKA),
	    CFG_SFMT_MATROSKA);
	ck_assert_ptr_eq(cfg_stream_fmt2str(CFG_STREAM_INVALID), NULL);
}
END_TEST

START_TEST(test_stream_name)
{
}
END_TEST

START_TEST(test_stream_mountpoint)
{
	TEST_XSTRDUP_T(cfg_stream_t, cfg_stream_list_get, streams,
	    cfg_stream_set_mountpoint, cfg_stream_get_mountpoint);
}
END_TEST

START_TEST(test_stream_intake)
{
	TEST_XSTRDUP_T(cfg_stream_t, cfg_stream_list_get, streams,
	    cfg_stream_set_intake, cfg_stream_get_intake);
}
END_TEST

START_TEST(test_stream_server)
{
	TEST_XSTRDUP_T(cfg_stream_t, cfg_stream_list_get, streams,
	    cfg_stream_set_server, cfg_stream_get_server);
}
END_TEST

START_TEST(test_stream_public)
{
	TEST_BOOLEAN_T(cfg_stream_t, cfg_stream_list_get, streams,
	    cfg_stream_set_public, cfg_stream_get_public);
}
END_TEST

START_TEST(test_stream_format)
{
	cfg_stream_t	 str = cfg_stream_list_get(streams, "test_stream_format");
	const char	*errstr2;

	TEST_EMPTYSTR_T(cfg_stream_t, cfg_stream_list_get, streams,
	    cfg_stream_set_format);

	ck_assert_int_eq(cfg_stream_set_format(str, streams,
	    "<something else>", NULL), -1);
	ck_assert_int_eq(cfg_stream_set_format(str, streams,
	    "<something else>", &errstr2), -1);
	ck_assert_str_eq(errstr2, "unsupported stream format");

	ck_assert_int_eq(cfg_stream_set_format(str, streams, CFG_SFMT_OGG,
	    NULL), 0);
	ck_assert_int_eq(cfg_stream_get_format(str), CFG_STREAM_OGG);
	ck_assert_str_eq(cfg_stream_get_format_str(str),
	    cfg_stream_fmt2str(CFG_STREAM_OGG));
}
END_TEST

START_TEST(test_stream_encoder)
{
	cfg_stream_t	 str = cfg_stream_list_get(streams, "test_stream_encoder");

	ck_assert_ptr_eq(cfg_stream_get_encoder(str), NULL);
	ck_assert_int_eq(cfg_stream_set_encoder(str, NULL, "test", NULL), 0);
	ck_assert_str_eq(cfg_stream_get_encoder(str), "test");
	ck_assert_int_eq(cfg_stream_set_encoder(str, NULL, NULL, NULL), 0);
	ck_assert_ptr_eq(cfg_stream_get_encoder(str), NULL);

	TEST_XSTRDUP_T_NULL(cfg_stream_t, cfg_stream_list_get, streams,
	    cfg_stream_set_encoder, cfg_stream_get_encoder);
}
END_TEST

START_TEST(test_stream_stream_name)
{
	TEST_XSTRDUP_T(cfg_stream_t, cfg_stream_list_get, streams,
	    cfg_stream_set_stream_name, cfg_stream_get_stream_name);
}
END_TEST

START_TEST(test_stream_stream_url)
{
	TEST_XSTRDUP_T(cfg_stream_t, cfg_stream_list_get, streams,
	    cfg_stream_set_stream_url, cfg_stream_get_stream_url);
}
END_TEST

START_TEST(test_stream_stream_genre)
{
	TEST_XSTRDUP_T(cfg_stream_t, cfg_stream_list_get, streams,
	    cfg_stream_set_stream_genre, cfg_stream_get_stream_genre);
}
END_TEST

START_TEST(test_stream_stream_description)
{
	TEST_XSTRDUP_T(cfg_stream_t, cfg_stream_list_get, streams,
	    cfg_stream_set_stream_description,
	    cfg_stream_get_stream_description);
}
END_TEST

START_TEST(test_stream_stream_quality)
{
	TEST_XSTRDUP_T(cfg_stream_t, cfg_stream_list_get, streams,
	    cfg_stream_set_stream_quality, cfg_stream_get_stream_quality);
}
END_TEST

START_TEST(test_stream_stream_bitrate)
{
	TEST_XSTRDUP_T(cfg_stream_t, cfg_stream_list_get, streams,
	    cfg_stream_set_stream_bitrate, cfg_stream_get_stream_bitrate);
}
END_TEST

START_TEST(test_stream_stream_samplerate)
{
	TEST_XSTRDUP_T(cfg_stream_t, cfg_stream_list_get, streams,
	    cfg_stream_set_stream_samplerate, cfg_stream_get_stream_samplerate);
}
END_TEST

START_TEST(test_stream_stream_channels)
{
	TEST_XSTRDUP_T(cfg_stream_t, cfg_stream_list_get, streams,
	    cfg_stream_set_stream_channels, cfg_stream_get_stream_channels);
}
END_TEST

START_TEST(test_stream_language_tag)
{
	TEST_XSTRDUP_T(cfg_stream_t, cfg_stream_list_get, streams,
	    cfg_stream_set_language_tag, cfg_stream_get_language_tag);
}
END_TEST

START_TEST(test_stream_validate)
{
	cfg_stream_t	 str = cfg_stream_list_get(streams, "test_stream_validate");
	const char	*errstr = NULL;

	ck_assert_int_ne(cfg_stream_validate(str, NULL), 0);
	ck_assert_int_ne(cfg_stream_validate(str, &errstr), 0);
	ck_assert_ptr_ne(errstr, NULL);
	ck_assert_str_eq(errstr, "format missing or unsupported");
	ck_assert_int_eq(cfg_stream_set_format(str, streams, CFG_SFMT_OGG,
	    NULL), 0);

	ck_assert_int_eq(cfg_stream_validate(str, NULL), 0);
}
END_TEST

Suite *
cfg_suite(void)
{
	Suite	*s;
	TCase	*tc_stream;

	s = suite_create("Config");

	tc_stream = tcase_create("Stream");
	tcase_add_checked_fixture(tc_stream, setup_checked, teardown_checked);
	tcase_add_test(tc_stream, test_stream_list_get);
	tcase_add_test(tc_stream, test_stream_str2fmt);
	tcase_add_test(tc_stream, test_stream_fmt2str);
	tcase_add_test(tc_stream, test_stream_name);
	tcase_add_test(tc_stream, test_stream_mountpoint);
	tcase_add_test(tc_stream, test_stream_intake);
	tcase_add_test(tc_stream, test_stream_server);
	tcase_add_test(tc_stream, test_stream_public);
	tcase_add_test(tc_stream, test_stream_format);
	tcase_add_test(tc_stream, test_stream_encoder);
	tcase_add_test(tc_stream, test_stream_stream_name);
	tcase_add_test(tc_stream, test_stream_stream_url);
	tcase_add_test(tc_stream, test_stream_stream_genre);
	tcase_add_test(tc_stream, test_stream_stream_description);
	tcase_add_test(tc_stream, test_stream_stream_quality);
	tcase_add_test(tc_stream, test_stream_stream_bitrate);
	tcase_add_test(tc_stream, test_stream_stream_samplerate);
	tcase_add_test(tc_stream, test_stream_stream_channels);
	tcase_add_test(tc_stream, test_stream_language_tag);
	tcase_add_test(tc_stream, test_stream_validate);
	suite_add_tcase(s, tc_stream);

	return (s);
}

void
setup_checked(void)
{
	if (0 < cfg_init() ||
	    0 < cfg_set_program_name("check_cfg_stream", NULL) ||
	    0 < log_init(cfg_get_program_name()))
		ck_abort_msg("setup_checked failed");

	streams = cfg_stream_list_create();
}

void
teardown_checked(void)
{
	cfg_stream_list_destroy(&streams);

	log_exit();
	cfg_exit();
}

int
main(void)
{
	int	 num_failed;
	Suite	*s;
	SRunner *sr;

	s = cfg_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	num_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	if (num_failed)
		return (1);
	return (0);
}
