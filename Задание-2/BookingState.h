#pragma once
#include <iostream>
#include <memory>

class BookingContext;

class BookingState {
public:
    virtual ~BookingState() = default;

    virtual void sendToClient(BookingContext& ctx) {
        deny("sendToClient");
    }
    virtual void pay(BookingContext& ctx) {
        deny("pay");
    }
    virtual void timeout(BookingContext& ctx) {
        deny("timeout");
    }
    virtual void verify(BookingContext& ctx) {
        deny("verify");
    }

    virtual const char* name() const = 0;

protected:
    void deny(const std::string& action) {
        std::cout << "Действие '" << action
                  << "' недоступно в состоянии '" << name() << "'\n";
    }
};