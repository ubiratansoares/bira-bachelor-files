Program ex10;
Var
	k: integer;

	Procedure p(n: integer; var g: integer);
	Var
		h: integer;
	Begin
		If n < 2 then
			g := g + n
		else begin
			h:=g;
			p(n-1, h);
			g := h;
			p(n-2,g)
		End;
		write(n,g)
	End;

Begin
	k:= 0;
	p(3,k)
End.
