program nome2; {exemplo 2}
var a: real;
var b: integer;

procedure nomep(x: real); 
var a, c: integer;
begin
    readln(c, a);
    x := x + 2.0;

    if a < c then
    begin
        a := c + a;
        writeln(a);
    end
    else c := a + c;
end;

procedure nomep();
begin
    nomep(10.00);
end;

begin {programa principal} 
    readln(b);
    nomep(a);
end.
