int f(int arg){
  int z = arg;
  int a = 1;
  int b = 2;
  int e = 3;
  int f = 4;
  int *c = &a;
  int *d = &a;
  *c = 0;
  *d = 4;
  d = &b;
  c = &b;
  c = &e;
  c = &f;
  int y = 1 / *c;
  int x = 1 / *d;

  int ** dub;
  *dub = c;
  **dub = 0;
  return x;
}