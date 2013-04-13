PROGRAM main
    VAR %% can be empty or completely missing
      i, j, k : INTEGER;
      arr : ARRAY[1..10] OF INTEGER;
    ENDVAR
  BEGIN
    FOR i := 1 TO 10 DO
      BEGIN
        arr[i] := 10 - i;
      END
    FOR i := 1 TO 9 DO
    BEGIN
      FOR j := i + 1 TO 10 DO
      BEGIN
        IF arr[i] > arr[j]
        THEN
            arr[i] <-> arr[j];
        ENDIF;
      END
    END
    FOR i := 10 DOWNTO 1 DO
    BEGIN
      WRITE(arr[10 - i + 1]);
      WRITESP();
    END
    WRITELN();
    
  END
