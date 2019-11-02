CREATE OR REPLACE FUNCTION queryOne(val integer) RETURNS TABLE(rn int, 
    tit varchar(300),
    topic int) AS $table1$
BEGIN
    RETURN QUERY 
    SELECT report_number, title, topic_id FROM Researchers INNER JOIN Reports on researcher_number = val AND report_id = report_number;
END; $table1$
LANGUAGE PLPGSQL;