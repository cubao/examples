#include "clipp.h"
#include <iostream>

using namespace clipp;
using std::cout;
using std::string;

int main(int argc, char *argv[])
{
    // variables storing the parsing result; initialized with their default
    // values
    enum class mode
    {
        make,
        find,
        help
    };
    mode selected = mode::help;
    std::vector<string> input;
    string dict, out;
    bool split = false, progr = false;

    auto dictionary = required("-dict") & value("dictionary", dict);

    auto makeMode =
        (command("make").set(selected, mode::make), values("wordfile", input),
         dictionary, option("--progress", "-p").set(progr) % "show progress");

    auto findMode =
        (command("find").set(selected, mode::find), values("infile", input),
         dictionary, (option("-o", "--output") & value("outfile", out)) %
                         "write to file instead of stdout",
         (option("-split").set(split, true) |
          option("-nosplit").set(split, false)) %
             "(do not) split output");

    auto cli =
        ((makeMode | findMode | command("help").set(selected, mode::help)),
         option("-v", "--version")
             .call([] { cout << "version 1.0\n\n"; })
             .doc("show version"));

    if (parse(argc, argv, cli)) {
        switch (selected) {
        case mode::make: /* ... */
            break;
        case mode::find: /* ... */
            break;
        case mode::help:
            cout << make_man_page(cli, "finder");
            break;
        }
    } else {
        cout << usage_lines(cli, "finder") << '\n';
    }
    return 0;
}
