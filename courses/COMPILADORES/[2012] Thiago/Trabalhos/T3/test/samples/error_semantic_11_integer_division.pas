program nome1; {exemplo 1}
var a, a1, b: integer; 

begin
    readln(b); 
    a1:= 10.33 / 2; 

    repeat 
        writeln(a1); 
        a1:= a1-1;
    until a1<a; 

    while b < 10 do 
    begin
        b := b + 2;
        a := a - 1;
    end;

    if a <> b then writeln(a); 
end.
