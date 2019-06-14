#include <iostream>

int main()
{
    std::cout << "C++17 needed" << std::endl;
    return 0;
}

// #include <taskflow/taskflow.hpp>
//
// int main(){
//
//     tf::Taskflow tf;
//
//     auto [A, B, C, D] = tf.emplace(
//             [] () { std::cout << "TaskA\n"; },               //  task
//             dependency graph
//             [] () { std::cout << "TaskB\n"; },               //
//             [] () { std::cout << "TaskC\n"; },               // +---+
//             [] () { std::cout << "TaskD\n"; }                //    +---->| B
//             |-----+
//             );                                                 //    |
//             +---+     |
//     //  +---+           +-v-+
//     A.precede(B);  // A runs before B                  //  | A |           |
//     D |
//     A.precede(C);  // A runs before C                  //  +---+ +-^-+
//     B.precede(D);  // B runs before D                  //    |     +---+ |
//     C.precede(D);  // C runs before D                  //    +---->| C
//     |-----+
//     //          +---+
//     tf::Executor().run(tf);  // create an executor to run the taskflow
//
//     return 0;
// }
