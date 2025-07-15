//
// Created by sayan on 7/10/25.
//
#include <actor_system.hpp>
using cell = TypedActor<replies_to<int>::with<void>, replies_to<int>::with<double>>;
int main() {
    ActorSystem sys{};
    cell cell_t{sys};
    return 0;
}