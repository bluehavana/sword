#include <localemgr.h>
#include <versekey.h>

int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s <locale_name> <text>\n", *argv);
		exit(-1);
	}

	LocaleMgr lm;

	printf("%s\n", lm.translate(argv[1], argv[2]));

	VerseKey bla;
	bla = "James 1:19";

	bla.setLocale("de");
	cout << bla << endl;
	bla = "Johannes 1:1";
	cout << bla << endl;

	LocaleMgr::systemLocaleMgr.setDefaultLocaleName("de");
	VerseKey key2;
	key2.setLocale("en");
	ListKey list = key2.ParseVerseList("Luke 3:23-28",key2, true);
	cout << list << endl;


}
