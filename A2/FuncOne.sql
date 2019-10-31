CREATE FUNCTION funcOne(val integer) RETURNS void AS $$
BEGIN
    SELECT * FROM Researchers INNER JOIN Reports on researcher_number = val AND report_id = report_number;
END; $$
LANGUAGE PLPGSQL;