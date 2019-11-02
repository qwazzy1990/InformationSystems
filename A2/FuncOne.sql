CREATE OR REPLACE FUNCTION queryOne(val integer) RETURNS TABLE(rn int, 
    rname varchar(300), 
    phone varchar(30), 
    room integer, 
    email varchar(30),
    prid int,
    reportid int,
    pi boolean) AS $table1$
BEGIN
    RETURN QUERY 
    SELECT * FROM Researchers INNER JOIN Reports on researcher_number = val AND report_id = report_number;
END; $table1$
LANGUAGE PLPGSQL;