#pragma once
#include <iostream>
#include <memory>
#include <string>


class BookingContext;


class BookingState {
public:
    virtual ~BookingState() = default;

    virtual void selectRoom(BookingContext& ctx, const std::string& room) {
        std::cout << "Действие selectRoom недоступно в состоянии " << name() << "\n";
    }

    virtual void changeRoom(BookingContext& ctx, const std::string& room) {
        std::cout << "Действие changeRoom недоступно в состоянии " << name() << "\n";
    }

    virtual void confirmBooking(BookingContext& ctx) {
        std::cout << "Действие confirmBooking недоступно в состоянии " << name() << "\n";
    }

    virtual void pay(BookingContext& ctx, double amount) {
        std::cout << "Действие pay недоступно в состоянии " << name() << "\n";
    }

    virtual void cancel(BookingContext& ctx) {
        std::cout << "Действие cancel недоступно в состоянии " << name() << "\n";
    }

    virtual void restart(BookingContext& ctx) {
        std::cout << "Действие restart недоступно в состоянии " << name() << "\n";
    }

    virtual const char* name() const = 0;
};


class BookingContext {
private:
    std::shared_ptr<BookingState> state;
    std::string room;
    double price = 0.0;
    bool paid = false;

public:
    BookingContext(std::shared_ptr<BookingState> initialState)
        : state(std::move(initialState)) {}

    void setState(const std::shared_ptr<BookingState>& newState) {
        state = newState;
        std::cout << "[STATE] -> " << state->name() << "\n";
    }


    void selectRoom(const std::string& r)      { state->selectRoom(*this, r); }
    void changeRoom(const std::string& r)      { state->changeRoom(*this, r); }
    void confirmBooking()                      { state->confirmBooking(*this); }
    void pay(double amount)                    { state->pay(*this, amount); }
    void cancel()                              { state->cancel(*this); }
    void restart()                             { state->restart(*this); }


    void setRoom(const std::string& r)         { room = r; }
    const std::string& getRoom() const         { return room; }

    void setPrice(double p)                    { price = p; }
    double getPrice() const                    { return price; }

    void setPaid(bool p)                       { paid = p; }
    bool isPaid() const                        { return paid; }

    void reset() {
        room.clear();
        price = 0.0;
        paid = false;
    }
};


class IdleState : public BookingState {
public:
    const char* name() const override { return "Idle"; }

    void selectRoom(BookingContext& ctx, const std::string& room) override;
};

class RoomSelectedState : public BookingState {
public:
    const char* name() const override { return "RoomSelected"; }

    void changeRoom(BookingContext& ctx, const std::string& room) override;
    void confirmBooking(BookingContext& ctx) override;
    void cancel(BookingContext& ctx) override;
};

class BookingConfirmedState : public BookingState {
public:
    const char* name() const override { return "BookingConfirmed"; }

    void pay(BookingContext& ctx, double amount) override;
    void cancel(BookingContext& ctx) override;
};

class PaidState : public BookingState {
public:
    const char* name() const override { return "Paid"; }

    void cancel(BookingContext& ctx) override {
        std::cout << "Нельзя отменить: бронирование уже оплачено.\n";
    }
};

class BookingCancelledState : public BookingState {
public:
    const char* name() const override { return "BookingCancelled"; }

    void restart(BookingContext& ctx) override;
};


void IdleState::selectRoom(BookingContext& ctx, const std::string& room) {
    ctx.setRoom(room);

    double price = 0.0;
    if (room == "Standard")      price = 100.0;
    else if (room == "Deluxe")   price = 200.0;
    else if (room == "Suite")    price = 350.0;
    else                         price = 120.0;

    ctx.setPrice(price);
    ctx.setPaid(false);

    std::cout << "Выбран номер: " << room << ", цена: " << price << "\n";
    ctx.setState(std::make_shared<RoomSelectedState>());
}


void RoomSelectedState::changeRoom(BookingContext& ctx, const std::string& room) {
    std::cout << "Изменяем номер на: " << room << "\n";
    IdleState helper;
    helper.selectRoom(ctx, room);
}

void RoomSelectedState::confirmBooking(BookingContext& ctx) {
    if (ctx.getRoom().empty()) {
        std::cout << "Нельзя подтвердить: номер не выбран.\n";
        return;
    }
    std::cout << "Бронирование подтверждено для номера: " << ctx.getRoom()
              << ", сумма к оплате: " << ctx.getPrice() << "\n";
    ctx.setState(std::make_shared<BookingConfirmedState>());
}

void RoomSelectedState::cancel(BookingContext& ctx) {
    std::cout << "Бронирование отменено до подтверждения.\n";
    ctx.reset();
    ctx.setState(std::make_shared<BookingCancelledState>());
}

void BookingConfirmedState::pay(BookingContext& ctx, double amount) {
    std::cout << "Внесено: " << amount << ", требуется: " << ctx.getPrice() << "\n";
    if (amount < ctx.getPrice()) {
        std::cout << "Недостаточно средств. Оплата не прошла.\n";
        return;
    }
    ctx.setPaid(true);
    double change = amount - ctx.getPrice();
    if (change > 0) {
        std::cout << "Сдача: " << change << "\n";
    }
    std::cout << "Оплата прошла успешно. Номер закреплен за пользователем.\n";
    ctx.setState(std::make_shared<PaidState>());
}

void BookingConfirmedState::cancel(BookingContext& ctx) {
    std::cout << "Бронирование отменено до оплаты.\n";
    ctx.reset();
    ctx.setState(std::make_shared<BookingCancelledState>());
}

void BookingCancelledState::restart(BookingContext& ctx) {
    std::cout << "Начинаем новое бронирование.\n";
    ctx.reset();
    ctx.setState(std::make_shared<IdleState>());
}