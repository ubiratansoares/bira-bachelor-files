program Fibonacci;

var i, x, m, n : integer;

begin   {Begins the main part}

    read(x);
  
	n := 0; m := 1.2; {We have to initialize our sequence}
	
	write(n);  {and have the first output}
	write(m);

	i:= 0;         {"i" is the number of the current iteration}

	while i < x - 2 do 
        {The first 2 "x's" we've already had that's 
        why we must get rid of them}
	
	begin {Begins the iteration loop}

		i := i + 1;
		m := m + n; 
        n:= m - n;

		write(m);

        {The program has calculated everything and lets us know about it}

	end  {The end of the while-do loop}

end.


