#pragma once

#include <stacktrace>

#include "Array.h"
#include "Object.h"

class Throwable : public virtual Object, public virtual std::exception {
    shared<String> message;
    shared<Throwable> cause;
    bool suppression;
    bool writeableST;

    std::stacktrace trace;

public:
    ~Throwable() override = default;

    Throwable();

    // Throwable(shared<String> message);
    //
    // Throwable(shared<String> message, shared<Throwable> cause);
    //
    // Throwable(shared<String> message, shared<Throwable> cause, bool enableSuppression, bool writableStackTrace);
    //
    // Throwable(shared<Throwable> cause);
    //
    // void addSuppressed(shared<Throwable> exception);
    //
    // virtual shared<Throwable> fillInStackTrace();
    //
    // virtual shared<Throwable> getCause();
    //
    // virtual shared<String> getLocalizedMessage();
    //
    // virtual shared<String> getMessage();
    //
    // virtual shared<Array<>> getStackTrace();
    //
    // shared<Array<>> getSuppressed();
    //
    // virtual shared<Throwable> initCause(shared<Throwable> cause);
    //
    // virtual void printStackTrace();
    //
    // // virtual void printStackTrace(shared<PrintStream> s);
    //
    // // virtual void printStackTrace(shared<PrintWriter> s);
    //
    // virtual void setStackTrace(shared<Array<>> stackTrace);
    //
    // std::shared_ptr<String> toString() override;
};
