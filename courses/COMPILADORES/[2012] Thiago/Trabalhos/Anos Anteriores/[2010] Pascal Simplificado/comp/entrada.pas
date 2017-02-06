program entrada;

type
	naipes = (copas, ouros, espadas, paus);

var
	carta: naipes;
	x: integer;
	y: boolean;

procedure imp(a: integer; var b: boolean);
begin
	if (b) then
		write(1-a)
	else
		write(a-1)
end;

begin

	read(carta,y);

	case carta of
		copas: x := 1;
		ouros: x := 3;
		espadas: x := 7;
		paus: x := 5;
		else x := 0
	end;

	imp(x,y)

end.
