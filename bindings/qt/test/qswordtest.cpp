#include "qswordtest.h"
#include <iostream>

#include "qmap.h"

void TestQSword::testQSWMgrCreate() {
    QSWMgr* mgr = getQSWMgr();

    sword::SWModule* mod = mgr->getModule(KJV);
    QCOMPARE((QString)mod->getName(), KJV);

    QSWMgr copy = QSWMgr(*mgr);
    // Nothing to assert here?
}

void TestQSword::testQSWMgrAugment() {
    QSWMgr* mgr = getQSWMgr();

    sword::SWModule* mod = mgr->getModule(WEB);
    QCOMPARE((long)mod, NULL);
    mgr->augmentModules(altPath);
    mod = mgr->getModule(WEB);
    QCOMPARE((QString)mod->getName(), WEB);
}

void TestQSword::testQSWMgrOptions() {
    QMap<QString,QString>* expectedOptions = new QMap<QString,QString>();
    expectedOptions->insert("Transliteration", "Off"); // Off
    expectedOptions->insert("Strong's Numbers", "Off"); // Off
    expectedOptions->insert("Morphological Tags", "Off"); // Off
    expectedOptions->insert("Lemmas", "Off"); // Off
    expectedOptions->insert("Textual Variants", "Primary Reading"); // Primary Reading
    expectedOptions->insert("Greek Accents", "On"); // On
    expectedOptions->insert("Headings", "Off"); // Off
    expectedOptions->insert("Arabic Vowel Points", "On"); // On
    expectedOptions->insert("Footnotes", "Off"); // Off
    expectedOptions->insert("Words of Christ in Red", "Off"); // Off

    QSWMgr* mgr = getQSWMgr();

    QStringList* options = mgr->getGlobalOptionsQ();
    for(QStringList::iterator it = options->begin(); it != options->end(); it++) {
//        std::cout << qPrintable(mgr->getGlobalOption(*it)) << std::endl;
        QCOMPARE(expectedOptions->keys().contains(*it), QBool(true));
        QCOMPARE((mgr->getGlobalOption(*it)), expectedOptions->value(*it));
    }
}

void TestQSword::testQSWMgrFilter() {
    QSWMgr* mgr = getQSWMgr();

    sword::SWModule* mod = mgr->getModule(KJV);

}

QSWMgr* TestQSword::getQSWMgr() {
    QSWMgr* mgr = new QSWMgr(confPath);

    return mgr;
}

QTEST_MAIN(TestQSword)
#include "qswordtest.moc"
