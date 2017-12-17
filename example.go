package main
import "fmt"

func main(){
  var x = 3*5+5;
  print(x);
  fmt.Scanf("%d", &x);
  var y = 5;
  if x<10{
    print(y);
  }else{
    print(x);
  }
  var i=0;
  for i<4{
    i++;
    print(i);
  }
  for i=0; i<3; i++{
      print(x);
  }
}
