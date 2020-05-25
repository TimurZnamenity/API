#ifndef PROJECT_POST_REQUEST_HPP
#define PROJECT_POST_REQUEST_HPP

#include "Global.hpp"
#include "Request.hpp"


PROJECT_NAMESPACE_BEGIN


/**
 * Класс для строительства запросов POST.
 */
class PostRequest : public Request
{
public:
    PostRequest(QObject* parent = nullptr);

    /**
     * Инициация отправки запроса POST.
     * Перед отправкой нужно подписаться на сигнал получения ответа (у класса-наследника)
     * @param data - Body запроса (form-data)
     */
    void post(QUrlQuery &data);
};

PROJECT_NAMESPACE_END

#endif // PROJECT_POST_REQUEST_HPP
