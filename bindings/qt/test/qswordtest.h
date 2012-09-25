#ifndef QSWORDTEST_H
#define QSWORDTEST_H

#include <QtTest/QtTest>
#include "qsword.h"

class TestQSword : public QObject
{
	Q_OBJECT
private slots:
    void testQSWMgrCreate();
    void testQSWMgrAugment();
    void testQSWMgrOptions();
    void testQSWMgrFilter();
private:
    QSWMgr* getQSWMgr();

    QString confPath = "/home/greg/.sword";
    QString KJV      = "KJV";
    QString WEB      = "WEB";
    QString altPath  = "/home/greg/swordtest";
};

#endif
