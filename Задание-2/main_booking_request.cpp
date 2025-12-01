#include <iostream>
#include "BookingContext.h"

int main() {

    auto initial = std::make_shared<CreatedState>();
    BookingContext booking(initial);

    std::cout << "\n Сценарий 1: Успешное бронирование \n";
    booking.sendToClient();   
    booking.pay();            
    booking.verify();          

    std::cout << "\n Сценарий 2: Отмена по таймауту \n";
    BookingContext booking2(initial);
    booking2.sendToClient();  
    booking2.timeout(); 

    std::cout << "\n Сценарий 3: Ошибочное действие \n";
    BookingContext booking3(initial);
    booking3.verify();         

    return 0;
}