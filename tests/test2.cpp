// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

class Foo {
  int iChilds[2];
  bool hasChilds() const { return((reinterpret_cast<int>(iChilds) > 0 || reinterpret_cast<int>(iChilds) > 0)); }
};

int main(){
  Foo f;
  return 1;
}

int h(){
  return 3;
}
