#ifndef PROJECT_GET_REQUEST_HPP
#define PROJECT_GET_REQUEST_HPP

#include "Global.hpp"
#include "Request.hpp"


PROJECT_NAMESPACE_BEGIN


/**
 * Класс для строительства запросов GET.
 */
class GetRequest : public Request
{
public:
    GetRequest(QObject* parent = nullptr);

    /**
     * Инициация отправки запроса GET.
     * Перед отправкой нужно подписаться на сигнал обновления данных (у класса-наследника)
     */
    void get();
};

PROJECT_NAMESPACE_END

#endif // PROJECT_GET_REQUEST_HPP
