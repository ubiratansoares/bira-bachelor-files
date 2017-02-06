program nome1; {exemplo 1}
var a, a1, b: integer; 

begin
    readln(a, b); 
    a1:= a + b; 

    repeat 
    begin
        writeln(a1); 
        a1:= a1-1;
    end 
    until a1<a; 

    while b < 10 do 
    begin
        b := b + 2;
        a := a - 1;
    end;

    if a <> b then writeln(a); 
end.
