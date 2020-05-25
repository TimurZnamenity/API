#include "GetRequest.hpp"
#include "AuthUtils.hpp"
#include "Token.hpp"


using namespace PROJECT;

GetRequest::GetRequest(QObject *parent) : Request(parent)
{

}

/* ---------------------------------------------------------------------------------------------- */
void GetRequest::get()
{
    // проверяем не протух ли токен
    if( AuthUtils::areAllTokensExpired() )
    {
        // отправляем запрос на обновление
        auto res = AuthUtils::refreshExpiredTokens();
        PROJECT_QDEBUG << "Result of Token Refresh = " << res;

        // токен-файлы обновились
        // все последующие запросы будут использовать уже свежий токен
    }

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
    QNetworkRequest request = m_requester->createGetRequest( apiStr );
    m_requester->sendRequest( request, funcSuccess, funcError, Requester::Type::GET_REQUEST );
}
