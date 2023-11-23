#pragma once

namespace statements {
    class Print;
    class Println;
}

class Executor {
public:
    virtual ~Executor() = default;

    virtual void print(statements::Print const& println) = 0;
    virtual void println(statements::Println const& println) = 0;
};
