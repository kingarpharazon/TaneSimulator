#include "simitem.h"

SimItem::SimItem()
{

}

void SimItem::setDataFile(QString dataFile)
{
    _dataFile = dataFile;
}

void SimItem::setParameters(QString parameters)
{
    _parameters = parameters;
}

void SimItem::setOutputFile(QString outputFile)
{
    _outputFile = outputFile;
}

const QString& SimItem::dataFile() const
{
    return _dataFile;
}

const QString& SimItem::parameters() const
{
    return _parameters;
}

const QString& SimItem::outputFile() const
{
    return _outputFile;
}
