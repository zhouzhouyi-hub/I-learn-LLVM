#include <utility>

struct Object {
  int i;
};
void UseObject(Object &) {
 
}

void UseObject(Object &&) {
 
}

template <typename T>
void NotForwardToUseObject(T x) {
  UseObject(x);
}

int main() {
  Object object;
  //NotForwardToUseObject(object);
  NotForwardToUseObject(std::move(object));
}
