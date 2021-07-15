#include "TransformStack.h"
#include <stdexcept>
using namespace std;


TransformStack::TransformStack() {
    //Init to identity matrix
    C.push(Matrix4(0,1));
}

void TransformStack::pop() {
    if(C.size()==1) {
        throw underflow_error("Cannot remove last element from matrix stack.");
    }
    C.pop();
}

void TransformStack::push() {
    //Add new matrix that is copy of top
    C.push( C.top() );
}

void TransformStack::transform(Matrix4 T)
{
    C.top() *= T;
}

Matrix4& TransformStack::get()
{
    return C.top();
}

void TransformStack::reset()
{
    //Reset to only having identity matrix
    while(C.size()) C.pop();
    C.push( Matrix4(0,1) );
}
