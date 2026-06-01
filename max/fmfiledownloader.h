#ifndef FMFILEDOWNLOADER_H
#define FMFILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class FMFileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FMFileDownloader(QUrl dataUrl, QObject *parent = nullptr);
    virtual ~FMFileDownloader();
    QByteArray downloadedData() const;

signals:
    void downloaded();

private slots:
    void fileDownloaded(QNetworkReply* pReply);
private:
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;
};

#endif // FMFILEDOWNLOADER_H
