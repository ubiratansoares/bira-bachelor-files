program exemplo;
var
	x, y: integer;

	procedure p (var s: integer);
	var
		z: integer;
	begin
		if s = 1 then
			y := 1
		else
		begin
			z := s - 1;
			p(z);
			y := y * s
		end
	end;

begin
	x := 4;
	p(x);
	write(x,y)
end.
