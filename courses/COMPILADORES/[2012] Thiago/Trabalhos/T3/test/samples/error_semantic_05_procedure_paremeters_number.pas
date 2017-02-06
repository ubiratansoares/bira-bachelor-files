program nome2; {exemplo 2}
var a: real;
var b: integer;

procedure nomep(x: real); 
var a, c: integer;
begin
    readln(c, a);
    
    if a < c then
    begin
        a := c + a;
        writeln(a);
    end
    else c := a + c;
end;

begin {programa principal} 
    readln(a);
    nomep(b,c);
end.
