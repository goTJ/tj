PROGRAM main
  FUNCTION fab(x : INTEGER): INTEGER;
    BEGIN
    	IF (x = 0) OR (x = 1)
	THEN
	   fab := 1;
	ELSE
	   BEGIN
	     fab := fab(x-1) + fab(x-2);
    	   END
	ENDIF;
    END
  
  BEGIN
    BEGIN
      VAR
        w, x, z: INTEGER;
      ENDVAR
      READ(w);
      FOR z := 0 TO w DO
      BEGIN
      	x := fab(z);
      	WRITE(x);
	WRITESP();
      END
      WRITELN();
    END
  END
