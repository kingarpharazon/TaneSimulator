#ifndef SIMITEM_H
#define SIMITEM_H

#include <QString>

class SimItem
{
public:
    SimItem();

    void setDataFile(QString dataFile);
    void setParameters(QString parameters);
    void setOutputFile(QString outputFile);

    const QString& dataFile() const;
    const QString& parameters() const;
    const QString& outputFile() const;

private:
    QString _dataFile;
    QString _parameters;
    QString _outputFile;
};

#endif // SIMITEM_H
