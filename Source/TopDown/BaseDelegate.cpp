#include "TopDown.h"
#include "BaseDelegate.h" 

BaseDelegate::BaseDelegate(void) {
}

BaseDelegate::~BaseDelegate(void) {
}

class MyTest : public RTS::IBuild, public RTS::IObject {

};