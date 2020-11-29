#include <exception>
#include <iostream>

class exception : virtual public std::exception {

protected:
    std::string error_message;

public:

    explicit
    exception(std::string msg) :
            error_message(std::move(msg)) {}

    ~exception() noexcept override = default;

    [[nodiscard]] const char *what() const noexcept override {
        return error_message.c_str();
    }
};