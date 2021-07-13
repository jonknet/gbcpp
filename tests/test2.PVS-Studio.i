#line 1 "E:\\Documents\\Projects\\GBCppClion\\tests\\test2.cpp"




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
