#ifndef TRANSFORMSTACK_H
#define TRANSFORMSTACK_H
#include <stack>
#include "Matrix4.h"


class TransformStack
{
public:
    TransformStack();

    void  push();

    void pop();

    Matrix4& get();
    
    void transform(Matrix4);

    void reset();
    
private:
    std::stack<Matrix4> C;
};

#endif // TRANSFORMSTACK_H
