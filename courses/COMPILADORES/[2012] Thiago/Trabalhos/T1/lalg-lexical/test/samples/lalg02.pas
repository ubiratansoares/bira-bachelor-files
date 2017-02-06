program nome2; {exemplo 2}
var a: real;
var b: integer;
procedure nomep(x: real); var a, c: integer;
begin
readln(c, a);
if a<x+c then
begin
a:= c+x;
writeln(a);
end
else c:= a+x;
end;
begin {programa principal} readln(b);
nomep(b);
end.
