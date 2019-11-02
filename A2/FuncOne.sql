CREATE FUNCTION queryOne(val integer) RETURNS TABLE(num1 int, v2 varchar(300), num2 int) AS $$
BEGIN
    return SELECT * FROM Researchers INNER JOIN Reports on researcher_number = val AND report_id = report_number;
END; $$
LANGUAGE PLPGSQL;