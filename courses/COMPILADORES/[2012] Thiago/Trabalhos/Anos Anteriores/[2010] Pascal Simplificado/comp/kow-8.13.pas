program exemplo9;
var
	x, y: integer;

procedure p (t: integer);
var
	z: integer;

begin
	if t > 1 then
		p(t-1)
	else
		y := 1;
	z := y;
	y := z * t
end;

begin
	read(x);
	p(x);
	write(x,y)
end.
