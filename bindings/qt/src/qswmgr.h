#ifndef QSWMGR_H
#define QSWMGR_H

#include "qobject.h"

#include "swfiltermgr.h"
#include "swmgr.h"
#include "swmodule.h"
#include "swbuf.h"
#include "swkey.h"

class QSWMgr :  public QObject, public sword::SWMgr
{
    Q_OBJECT
public:
    QSWMgr(const QSWMgr &mgr);
    QSWMgr(QString &iConfigPath, bool autoload=true, sword::SWFilterMgr* filterMgr=0, bool multiMod=false, bool augmentHome=true);
    virtual ~QSWMgr();

    virtual void augmentModules(const QString &path, bool multiMod=false);
    void deleteModule(const QString &module);
    virtual char filterText(const QString &filterName, QString &text, const sword::SWKey *key, const sword::SWModule *module);
    virtual const QString getGlobalOption(const QString &option);
    virtual QStringList* getGlobalOptionsQ();
    sword::SWModule* getModule(const QString &modName);

signals:
    
public slots:
    
};

#endif // QSWMGR_H
