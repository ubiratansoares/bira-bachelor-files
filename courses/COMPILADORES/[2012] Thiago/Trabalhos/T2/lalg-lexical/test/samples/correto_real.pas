program correto;
var a, b, c:integer;
var g: real;
var h: real;

procedure proc(a1 : integer);
var a, b, c:integer;
begin
	a:=1;
	if a<1 then
		a:=12;
end;

begin

	a:=2;
	b:=10;
	c:=11;
	a:=b+c;
	if b>5 then
	begin
		a:=20;
		b:=10*c;
		c:=a / b;
	end;
	while a>1 do
	begin
		if b>10 then
			b:=2;
		a:=a-1;
	end;
	g:=2.12;
	h:=343454353.3222;
end.
