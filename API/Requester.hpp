#ifndef PROJECT_REQUESTER_H
#define PROJECT_REQUESTER_H

#include <QObject>
#include <QBuffer>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <functional>
#include "Global.hpp"


PROJECT_NAMESPACE_BEGIN


/**
 * Обработчик запросов REST API.
 */
class Requester : public QObject
{
    Q_OBJECT
/* ############################################################################################## */
public: /* Конструкторы / деструктор */
/* ############################################################################################## */

    /**
     * Дефолтный конструктор.
     */
    explicit Requester(QObject* parent = nullptr);
    ~Requester() {
        delete m_manager;
    }

/* ############################################################################################## */
public: /* Определения */
/* ############################################################################################## */

    /**
     * Синонимы для указателей на функцию.
     */
    typedef std::function<void(const QJsonObject&)> handleFunc;
    typedef std::function<void()> finishFunc;

    /**
     * Строковые константы для обработки ошибок.
     */
    static const QString KEY_QNETWORK_REPLY_ERROR;
    static const QString KEY_CONTENT_NOT_FOUND;

    /**
     * Тип запроса.
     */
    enum class Type {
        POST_REQUEST,
        GET_REQUEST,
        PATCH_REQUEST,
        DELETE_REQUEST
    };

/* ############################################################################################## */
public: /* Методы */
/* ############################################################################################## */

    /**
     * Инициализирует Requester.
     * Вызывается после конструктора
     * @param host  Адрес хоста
     * @param value SSL-конфигурация
     */
    void initRequester(const QString& host, QSslConfiguration* value);

    /**
     * Создает  запрос для его последующей отправки.
     * при помощи функции sendRequest
     * @param apiStr Тело запроса
     * @return Сформированный запрос
     */
    QNetworkRequest createGetRequest(const QString& apiStr);

    /**
     * Создает запрос POST для его последующей отправки.
     * при помощи функции sendRequest
     * @param apiStr Тело запроса
     * @return Сформированный запрос
     */
    QNetworkRequest createPostRequest(const QString& apiStr);

    /**
     * Отправка запроса.
     * @param request запрос, созданный предварительно при помощи createRequest(..).
     * @param funcSuccess указатель на функцию-обработчик ответа
     * @param funcError указатель на функцию-обработчик ошибки
     * @param type тип запроса
     * @param data используемые в запросе данные
     */
    void sendRequest(QNetworkRequest&  request,
                      const handleFunc& funcSuccess,
                      const handleFunc& funcError,
                      Type type = Type::GET_REQUEST,
                      const QUrlQuery &data = QUrlQuery() );


    /**
     * Отправка многостраничного запроса.
     * @param apiStr тело запроса
     * @param funcSuccess указатель на функцию-обработчик ответа
     * @param funcError указатель на функцию-обработчик ошибки
     * @param type тип запроса
     */
    void sendMultipageGetRequest(const QString&    apiStr,
                              const handleFunc& funcSuccess,
                              const handleFunc& funcError,
                              const finishFunc& funcFinish);

    /**
     * Возвращает Token.
     * Может использоваться для нужд авторизации
     */
    QString getToken() const;

    /**
     * Устанавливает Token.
     * Для нужд авторизации
     * @param value Токен в текстовом виде
     */
    void setToken(const QString &value);

/* ############################################################################################## */
private:
/* ############################################################################################## */

    static const QString httpTemplate;
    static const QString httpsTemplate;

    QString     host;
    QString     token;
    QSslConfiguration
                *sslConfig;

    QString     pathTemplate;

    /**
     * Преобразует data в JSON-документ.
     * @param data Данные
     * @return JSON-документ в байтовом представлении
     */
    QByteArray  variantMapToJson(QVariantMap data);

    /**
     * Создает запрос для его последующей отправки при помощи sendCustomRequest.
     * @param apiStr Тело запроса
     * @return Сформированный запрос
     */
    QNetworkRequest createCustomRequest(const QString& apiStr);

    /**
     * Посылает запрос спец вида.
     * Используется для запросов DELETE и PATCH
     * @param manager Указатель на QNetworkAccessManager
     * @param request Указатель на запрос, созданный предварительно
     * @param type Тип запроса
     * @param data Данные запроса
     * @return Ответ
     */
    QNetworkReply *sendCustomRequest(QNetworkAccessManager* m_manager,
                                     QNetworkRequest&   request,
                                     const QString&     type,
                                     const QVariantMap& data);
    /**
     * Извлекает из ответа объект Json.
     * Используется для запросов DELETE и PATCH
     * @param reply Ответ на запрос
     * @return Объект QJson
     */
    QJsonObject parseReply(QNetworkReply* reply);

    /**
     * Обрабатывает ошибки по окончании запроса.
     * @param reply Ответ на запрос
     * @return Возвращает состояние ответа - содержит или нет ошибки
     */
    bool onFinishRequest(QNetworkReply* reply);

    /**
     * Обрабатывает ошибки по окончании запроса.
     * @param reply Ответ на запрос
     * @param obj Тело ответа
     */
    void handleQtNetworkErrors(QNetworkReply* reply, QJsonObject& obj);

    /**
     * Определяет последняя ли страница пришла в ответе на мультистраничный запрос.
     */
    bool isLastPage(QJsonObject& obj);

    /**
     * Рассчитывает номер следующей страницы для мультистраничного запроса.
     */
    QString nextPage(QJsonObject &obj);

    /**
     * Формирует строку API-запроса для следующих страниц мультистраничного запроса.
     */
    QString createNextApiStr(const QString& apiStr, const QString& nextPage);

    /**
     * Менеждер доступа к сети.
     * Лучше чтобы он был один на все приложение
     */
    QNetworkAccessManager* m_manager;

    /**
      * Возвращает строку - название программы + номер версии
      */
    QByteArray userAgent() const;



};

PROJECT_NAMESPACE_END


#endif // PROJECT_REQUESTER_H
