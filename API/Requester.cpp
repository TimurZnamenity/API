#include "Requester.hpp"
#include "Global.hpp"


using namespace PROJECT;


const QString Requester::KEY_QNETWORK_REPLY_ERROR = "QNetworkReplyError";
const QString Requester::KEY_CONTENT_NOT_FOUND = "ContentNotFoundError";


// для отладки в WireShark, так как при SSL все зашифровано
#ifdef DEBUG
//    #define DEBUG_WITHOUT_SSL
#endif

/* ---------------------------------------------------------------------------------------------- */
Requester::Requester(QObject* parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
}


/* ---------------------------------------------------------------------------------------------- */
void Requester::initRequester(const QString& host, QSslConfiguration* value)
{
    this->host = host;
    sslConfig  = value;

    if (sslConfig == nullptr)
        PROJECT_QDEBUG << "Requester: there is no sslConfig!!!";

    pathTemplate = "%1/address/%2";
}

/* ---------------------------------------------------------------------------------------------- */
QNetworkRequest Requester::createGetRequest(const QString& apiStr)
{
    QNetworkRequest request;
    QString url = pathTemplate.arg(host).arg(apiStr);

    request.setUrl(QUrl(url));
    request.setRawHeader( "Accept",     "application/json" );
    // Вживляем свою информацию про программу и версию
    request.setRawHeader( "User-Agent", userAgent() );
    // Вживляем токен
    if(!token.isEmpty())
        request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());
    if (sslConfig != nullptr)
        request.setSslConfiguration(*sslConfig);

    return request;
}

/* ---------------------------------------------------------------------------------------------- */
QNetworkRequest Requester::createPostRequest(const QString& apiStr)
{
    QNetworkRequest request;
    QString url = pathTemplate.arg(host).arg(apiStr);

    request.setUrl(QUrl(url));
    request.setRawHeader( "Accept",       "application/json" );
    request.setRawHeader( "Content-Type", "application/x-www-form-urlencoded" );
    // Вживляем свою информацию про программу и версию
    request.setRawHeader( "User-Agent",   userAgent() );
    // Вживляем токен
    if(!token.isEmpty())
        request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());
    if (sslConfig != nullptr)
        request.setSslConfiguration(*sslConfig);

    return request;
}

/* ---------------------------------------------------------------------------------------------- */
void Requester::sendRequest( QNetworkRequest& request,
                             const Requester::handleFunc& funcSuccess,
                             const Requester::handleFunc& funcError,
                             Requester::Type  type,
                             const QUrlQuery& data )
{
    QNetworkReply* reply;
    switch (type) {
        case Type::POST_REQUEST:
        {
            QByteArray postDataByteArray = data.toString(QUrl::FullyEncoded).toUtf8();
            reply = m_manager->post(request, postDataByteArray);
            break;
        }
        case Type::GET_REQUEST:
        {
            reply = m_manager->get(request);
            break;
        }
        case Type::DELETE_REQUEST:
        {
            if (data.isEmpty())
                reply = m_manager->deleteResource(request);
            else
                // тут тоже надо переписать на QUrlQuery
                reply = sendCustomRequest(m_manager, request, "DELETE", QVariantMap());
            break;
        }
        case Type::PATCH_REQUEST:
        {
            // тут тоже надо переписать на QUrlQuery
            reply = sendCustomRequest(m_manager, request, "PATCH", QVariantMap());
            break;
        }
    }

    connect(reply, &QNetworkReply::finished, this,
            [this, funcSuccess, funcError, reply]() {
        QJsonObject obj = parseReply(reply);

        if (onFinishRequest(reply)) {
            if (funcSuccess != nullptr)
                funcSuccess(obj);
        } else {
            if (funcError != nullptr) {
                handleQtNetworkErrors(reply, obj);
                funcError(obj);
            }
        }
        reply->close();
        reply->deleteLater();
    } );
}


/* ---------------------------------------------------------------------------------------------- */
void Requester::sendMultipageGetRequest(const QString&    apiStr,
                                        const handleFunc& funcSuccess,
                                        const handleFunc& funcError,
                                        const finishFunc& funcFinish)
{
    QNetworkRequest request = createGetRequest(apiStr);
    auto reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, this,
            [apiStr, this, funcSuccess, funcError, funcFinish, reply]() {
        QJsonObject obj = parseReply(reply);
        if (onFinishRequest(reply)) {
            if (funcSuccess != nullptr)
                funcSuccess(obj);
            if ( !isLastPage(obj) ) {
                QString nextApiStr = createNextApiStr( apiStr, nextPage(obj) );
                sendMultipageGetRequest(nextApiStr,
                                        funcSuccess,
                                        funcError,
                                        funcFinish
                                        );
            } else {
                if (funcFinish != nullptr)
                    funcFinish();
            }
        } else {
            handleQtNetworkErrors(reply, obj);
            if (funcError != nullptr)
                funcError(obj);
        }
        reply->close();
        reply->deleteLater();
    });
}

/* ---------------------------------------------------------------------------------------------- */
QString Requester::getToken() const
{
    return token;
}

/* ---------------------------------------------------------------------------------------------- */
void Requester::setToken(const QString& value)
{
    token = value;
}

/* ---------------------------------------------------------------------------------------------- */
QByteArray Requester::variantMapToJson(QVariantMap data)
{
    QJsonDocument postDataDoc = QJsonDocument::fromVariant(data);
    QByteArray postDataByteArray = postDataDoc.toJson();

    return postDataByteArray;
}

/* ---------------------------------------------------------------------------------------------- */
QNetworkRequest Requester::createCustomRequest(const QString& apiStr)
{
    QNetworkRequest request;
    QString url = pathTemplate.arg(host).arg(apiStr);

    request.setUrl(QUrl(url));
    request.setRawHeader("Content-Type","application/json");
    if(!token.isEmpty())
        request.setRawHeader("Authorization",QString("token %1").arg(token).toUtf8());
    if (sslConfig != nullptr)
        request.setSslConfiguration(*sslConfig);

    return request;
}

/* ---------------------------------------------------------------------------------------------- */
QNetworkReply* Requester::sendCustomRequest(QNetworkAccessManager* manager,
                                            QNetworkRequest&   request,
                                            const QString&     type,
                                            const QVariantMap& data)
{
    request.setRawHeader("HTTP", type.toUtf8());
    QByteArray postDataByteArray = variantMapToJson(data);
    QBuffer *buff = new QBuffer;
    buff->setData(postDataByteArray);
    buff->open(QIODevice::ReadOnly);
    QNetworkReply* reply =  manager->sendCustomRequest(request, type.toUtf8(), buff);
    buff->setParent(reply);
    return reply;
}

/* ---------------------------------------------------------------------------------------------- */
QJsonObject Requester::parseReply(QNetworkReply* reply)
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QJsonParseError parseError;
    auto replyText = reply->readAll();

    // если пришло пустое тело в ответ - такие ответы приходят на аналитику
    if( replyText.isEmpty() )
        // возвращаем пустой объект
        return jsonObj;

    jsonDoc = QJsonDocument::fromJson(replyText, &parseError);
    if(parseError.error != QJsonParseError::NoError){
        qDebug() << replyText;
        qWarning() << "Json parse error: " << parseError.errorString();
    }else{
        if(jsonDoc.isObject())
            jsonObj  = jsonDoc.object();
        else if (jsonDoc.isArray())
            jsonObj["non_field_errors"] = jsonDoc.array();
    }
    return jsonObj;
}

/* ---------------------------------------------------------------------------------------------- */
bool Requester::onFinishRequest(QNetworkReply* reply)
{
    auto replyError = reply->error();
    if (replyError == QNetworkReply::NoError ) {
        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if ((code >=200) && (code < 300)) {
            return true;
        }
    }
    return false;
}

/* ---------------------------------------------------------------------------------------------- */
void Requester::handleQtNetworkErrors(QNetworkReply* reply, QJsonObject& obj)
{
    auto replyError = reply->error();
    if (!(replyError == QNetworkReply::NoError ||
          replyError == QNetworkReply::ContentNotFoundError ||
          replyError == QNetworkReply::ContentAccessDenied ||
          replyError == QNetworkReply::ProtocolInvalidOperationError
          ) ) {
        qDebug() << reply->error();
        obj[KEY_QNETWORK_REPLY_ERROR] = reply->errorString();
    } else if (replyError == QNetworkReply::ContentNotFoundError)
        obj[KEY_CONTENT_NOT_FOUND] = reply->errorString();
}

/* ---------------------------------------------------------------------------------------------- */
bool Requester::isLastPage(QJsonObject& obj)
{
    return obj["last_page"] == obj["current_page"];
}

/* ---------------------------------------------------------------------------------------------- */
QString Requester::nextPage(QJsonObject& obj)
{
    return QString("%1").arg( obj["current_page"].toInt() + 1 );
}

/* ---------------------------------------------------------------------------------------------- */
QString Requester::createNextApiStr(const QString& apiStr, const QString& nextPage)
{
    static const QString PAGE_KEY = "&page=";

    // "&page=" находится в конце строки
    // отрезаем "&page="
    // если даже и не было "&page=" в запросе, то вернется строка целиком
    QStringList list = apiStr.split( PAGE_KEY );

    // снова добавляем номер страницы в запрос
    return list.at(0) + PAGE_KEY + nextPage;
}

/* ---------------------------------------------------------------------------------------------- */
QByteArray Requester::userAgent() const
{
    QString ret { QString("Version/%1").arg(PROJECT_VERSION) };
    return ret.toLatin1();
}
