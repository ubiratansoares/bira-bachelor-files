program ex7;
var
	x, y: integer;

	procedure p;
	var
		z: integer;
	begin
		z := x;
		x := x - 1;
		if z > 1 then
			p
		else
			y := 1;
		y := y * z
	end;

	procedure q;
	var
		s, t: integer;
	begin
		s := x;
		t := x - 1;
		x := t;
		if s = 0 then
			y := 1
		else if ((s div 2)*2) = s then
			q
		else
			p;
		y:= y * s
	end;

begin
	read(x);
	q;
	write(y)
end.
