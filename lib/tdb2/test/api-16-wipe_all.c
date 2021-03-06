#include <ccan/tdb2/tdb2.h>
#include <ccan/tap/tap.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "logging.h"

static bool add_records(struct tdb_context *tdb)
{
	int i;
	struct tdb_data key = { (unsigned char *)&i, sizeof(i) };
	struct tdb_data data = { (unsigned char *)&i, sizeof(i) };

	for (i = 0; i < 1000; i++) {
		if (tdb_store(tdb, key, data, TDB_REPLACE) != 0)
			return false;
	}
	return true;
}


int main(int argc, char *argv[])
{
	unsigned int i;
	struct tdb_context *tdb;
	int flags[] = { TDB_INTERNAL, TDB_DEFAULT, TDB_NOMMAP,
			TDB_INTERNAL|TDB_CONVERT, TDB_CONVERT,
			TDB_NOMMAP|TDB_CONVERT,
			TDB_INTERNAL|TDB_VERSION1, TDB_VERSION1,
			TDB_NOMMAP|TDB_VERSION1,
			TDB_INTERNAL|TDB_CONVERT|TDB_VERSION1,
			TDB_CONVERT|TDB_VERSION1,
			TDB_NOMMAP|TDB_CONVERT|TDB_VERSION1 };

	plan_tests(sizeof(flags) / sizeof(flags[0]) * 4 + 1);
	for (i = 0; i < sizeof(flags) / sizeof(flags[0]); i++) {
		tdb = tdb_open("run-16-wipe_all.tdb", flags[i],
			       O_RDWR|O_CREAT|O_TRUNC, 0600, &tap_log_attr);
		if (ok1(tdb)) {
			struct tdb_data key;
			ok1(add_records(tdb));
			ok1(tdb_wipe_all(tdb) == TDB_SUCCESS);
			ok1(tdb_firstkey(tdb, &key) == TDB_ERR_NOEXIST);
			tdb_close(tdb);
		}
	}

	ok1(tap_log_messages == 0);
	return exit_status();
}
