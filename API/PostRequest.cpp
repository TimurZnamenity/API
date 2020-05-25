#include "PostRequest.hpp"


using namespace PROJECT;

PostRequest::PostRequest(QObject *parent) : Request(parent)
{

}

/* ---------------------------------------------------------------------------------------------- */
void PostRequest::post(QUrlQuery& data)
{
    // здесь собираем API-строку
    QString apiStr = constructApiString();

    // привязываем методы к указателям при помощи лямбд
    std::function<void(const QJsonObject &)> funcSuccess = [=](const QJsonObject& obj) {
        this->parseResponse(obj);
    };
    std::function<void(const QJsonObject &)> funcError = [=](const QJsonObject& obj) {
        this->parseError(obj);
    };
    // вызываем посылку запроса
    QNetworkRequest request = m_requester->createPostRequest( apiStr );
    m_requester->sendRequest( request,
                              funcSuccess,
                              funcError,
                              Requester::Type::POST_REQUEST,
                              data
                              );
}
