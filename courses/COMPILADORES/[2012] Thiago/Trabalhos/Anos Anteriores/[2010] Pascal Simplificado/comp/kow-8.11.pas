program ex8;
var
	x, y: integer;

procedure p;
var
	z: integer;

	procedure q;
	var
		s: integer;
	begin
		s:=z-1;
		x:=x-1;
		if s>1 then
			p
		else
			y := 1;
		y := y * s
	end;
begin
	z := x;
	x := x - 1;
	if z > 1 then
		q
	else
		y :=1;
	y:= y * z
end;

begin
	read(x);
	p;
	write(y)
end.
