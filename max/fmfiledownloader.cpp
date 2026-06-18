#include "fmfiledownloader.h"

FMFileDownloader::FMFileDownloader(QUrl dataUrl, QObject *parent) : QObject(parent)
{
    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileDownloaded(QNetworkReply*)));

    QNetworkRequest request(dataUrl);
    request.setRawHeader(QByteArray("x-api-key"), QByteArray("nDEhjxMAk19FSrA3p8wcB8unqQI8v9Q9b7UrbU11"));
    m_WebCtrl.get(request);
}

FMFileDownloader::~FMFileDownloader() {}

void FMFileDownloader::fileDownloaded(QNetworkReply *pReply) {
    m_DownloadedData = pReply->readAll();
    // emit a signal
    pReply->deleteLater();
    emit downloaded();
}

QByteArray FMFileDownloader::downloadedData() const { return m_DownloadedData; }
