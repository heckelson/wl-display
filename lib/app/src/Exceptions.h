#include <exception>
#include <string>

class BaseException : public std::exception {
    std::string message;

   public:
    BaseException() { this->message = ""; }
    BaseException(const std::string& message) : BaseException() {
        this->message = message;
    }
    BaseException(const char* message) : BaseException() {
        this->message = message;
    }

    const char* what() const noexcept override {
        return this->message.c_str();
    };
};

class MyException : public BaseException {
    using BaseException::BaseException;
    using BaseException::exception;
    using BaseException::what;
};
