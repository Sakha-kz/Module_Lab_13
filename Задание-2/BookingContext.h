#pragma once
#include <iostream>
#include <memory>
#include <string>
#include "BookingState.h"

class CreatedState;
class WaitingForPaymentState;
class PaidState;
class ConfirmedState;
class CancelledState;

class BookingContext {
private:
    std::shared_ptr<BookingState> state;

public:
    BookingContext(std::shared_ptr<BookingState> initial) : state(initial) {
        showState();
    }

    void setState(const std::shared_ptr<BookingState>& newState) {
        state = newState;
        showState();
    }

    void showState() {
        std::cout << "[STATE] -> " << state->name() << "\n";
    }


    void sendToClient() { state->sendToClient(*this); }
    void pay()          { state->pay(*this); }
    void timeout()      { state->timeout(*this); }
    void verify()       { state->verify(*this); }
};



class CreatedState : public BookingState {
public:
    const char* name() const override { return "Created"; }

    void sendToClient(BookingContext& ctx) override;
};

class WaitingForPaymentState : public BookingState {
public:
    const char* name() const override { return "WaitingForPayment"; }

    void pay(BookingContext& ctx) override;
    void timeout(BookingContext& ctx) override;
};

class PaidState : public BookingState {
public:
    const char* name() const override { return "Paid"; }

    void verify(BookingContext& ctx) override;
};

class ConfirmedState : public BookingState {
public:
    const char* name() const override { return "Confirmed"; }
};

class CancelledState : public BookingState {
public:
    const char* name() const override { return "Cancelled"; }
};



void CreatedState::sendToClient(BookingContext& ctx) {
    std::cout << "Заявка отправлена клиенту для оплаты.\n";
    ctx.setState(std::make_shared<WaitingForPaymentState>());
}

void WaitingForPaymentState::pay(BookingContext& ctx) {
    std::cout << "Клиент завершил оплату.\n";
    ctx.setState(std::make_shared<PaidState>());
}

void WaitingForPaymentState::timeout(BookingContext& ctx) {
    std::cout << "Клиент не оплатил вовремя. Заявка отменена.\n";
    ctx.setState(std::make_shared<CancelledState>());
}

void PaidState::verify(BookingContext& ctx) {
    std::cout << "Проверка бронирования завершена успешно.\n";
    ctx.setState(std::make_shared<ConfirmedState>());
}