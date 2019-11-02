CREATE FUNCTION funcOne(val integer) RETURNS TABLE(num1 int, v2 varchar(300), num2 int) AS $tab$
BEGIN
    SELECT * FROM Researchers INNER JOIN Reports on researcher_number = val AND report_id = report_number;
END; $tab$
LANGUAGE PLPGSQL;