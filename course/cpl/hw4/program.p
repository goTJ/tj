PROGRAM program_name
       VAR %% beginning of variable declarations, required even no var's
          x, y, z: REAL;
	  k, k: REAL;
       BEGIN
          READ(y);
          x := (y + 3.0 * 4.0) - 5.0;
          WRITE(y);
          WRITESP();
          WRITE(x);
          WRITELN();
       END
