// test the stack size calculations 
// by having local variables of all sizes
//

void __attribute__((noinline)) use( volatile char * x ){
   *x = 0;
}   

template< int N >
void __attribute__((noinline)) use_stack(){
   volatile char a[ N ];
   use( a );
   //for( auto & x : a ){
   //   x = 0;
   //}   
}
   
template< int N >
struct envelope {
   static void __attribute__ ((noinline)) func(){
      use_stack< N >();
      envelope< N - 1 >::func();
   }
};

template<>
struct envelope< 0 > {
   static void func(){}
};


int main( void ){	
   envelope< 1000 >::func();
}