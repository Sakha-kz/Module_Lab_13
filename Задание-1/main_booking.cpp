#include <iostream>
#include "BookingStateMachine.h"

int main() {
    auto idle = std::make_shared<IdleState>();
    BookingContext booking(idle);
    booking.setState(idle);

    std::cout << "\n Сценарий 1: нормальное бронирование \n";
    booking.selectRoom("Deluxe");   
    booking.changeRoom("Suite");       
    booking.confirmBooking();          
    booking.pay(400.0);               

    std::cout << "\nПробуем отменить после оплаты:\n";
    booking.cancel();                 

    std::cout << "\n Сценарий 2: отмена до оплаты \n";
    booking.restart();               
    booking.selectRoom("Standard");
    booking.confirmBooking();
    booking.cancel();                

    std::cout << "\n Сценарий 3: отмена ещё на этапе выбора \n";
    booking.restart();
    booking.selectRoom("Standard");
    booking.cancel();                 

    return 0;
}