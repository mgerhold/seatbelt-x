#pragma once

namespace statements {
    class Println;
}

class Executor {
public:
    virtual ~Executor() = default;

    virtual void println(statements::Println const& println) = 0;
};
