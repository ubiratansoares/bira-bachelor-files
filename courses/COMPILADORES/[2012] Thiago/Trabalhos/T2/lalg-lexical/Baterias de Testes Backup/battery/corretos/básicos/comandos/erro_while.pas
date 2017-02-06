program correto;
var a, b, c:integer;
    d, e, f:boolean;

procedure proc(var a1 : integer);
var a, b, c:integer;
    d, e, f:boolean;
begin
	a:=1;
	if (a<1) then
		a:=12
end;

begin
	a:=2;
	b:=10;
	c:=11;
	a:=b+c;
	d:=true;
	e:=false;
	f:=true;
	read(a);
	write(b);
	if (d) then
	begin
		a:=20;
		b:=10*c;
		c:=a div b
	end;
	while (a>1 do
	begin
		if (b>10) then
			b:=2;
		a:=a-1
	end
end.
