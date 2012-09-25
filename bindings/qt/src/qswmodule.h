#ifndef QSWMODULE_H
#define QSWMODULE_H

#include "qstring.h"

#include "swdisp.h"
#include "swmodule.h"

class QSWModule : public sword::SWModule
{
    Q_OBJECT
public:
    QSWModule(QString &imodname="",
              QString &imoddesc="",
              sword::SWDispaly* idisp=0,
              const char *imodtype=0,
              SWTextEncoding encoding=ENC_UNKNOWN,
              SWTextDirection direction=DIRECTION_LTR,
              SWTextMarkup markup=FMT_UNKNOWN,
              QString &modlang="");

    QString* Description(QString& description);
    QString* Description() const;
    virtual QSWModule& AddEncodingFilter(QSWFilter *newfilter);
    virtual
    virtual void encodingFilter(QString& buf, const sword::SWKey* key);
    virtual void filterBuffer(FilterList* filters, QString& buf, const sword::SWKey* key);
    virtual void filterBuffer(OptionFilterList* filters, QString& buf, const sword::SWKey* key);
    virtual QString* getConfigEntry(const QString& key) const;
    virtual QString* getDescription() const;
    virtual const QString* getKeyText() const;
    virtual QString* getName() const;
    virtual const QString* getRawEntry();
    virtual const QString* getRenderHeader() const;
    virtual QString* getType() const;
    virtual bool isSearchOptimallySupported(const char *istr, int searchType, int flags, SWKey *scope);
    virtual QString* KeyText(const QString &incoming="");
    virtual QString* Lang(const QString& incoming="");
    QString* Name() const;
    QString* Name(const QString &name);
    virtual void optionFilter(QString &buf, const sword::SWKey* key);
    virtual void rawFilter(QString& buf, const sword::SWKey* key);
    
signals:
    
public slots:
    
};

#endif // QSWMODULE_H
