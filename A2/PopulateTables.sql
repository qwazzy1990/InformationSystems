INSERT INTO Topics VALUES 
    (301, 'Machine Learning'),
    (302, 'Databases'),
    (303, 'Big Data'),
    (304, 'Programming Languages');

INSERT INTO Projects Values 
    (10001, 'a unified database for structured/unstructured/semistructured data', 302),
    (10001, 'a unified database for structured/unstructured/semistructured data', 303),
    (10002, 'advanced machine learning in analyzing crop images', 301),
    (10003, 'efficient functional programming in bib data', 303),
    (10003, 'efficient functional programming in bib data', 304);



INSERT INTO Reports VALUES
    (552, 'A space efficient data warehouse', 302),
    (552, 'A space efficient data warehouse', 303),
    (554, 'Computer vision based crop growth monitoring', 301),
    (561, 'Automated data extraction in big data analytics', 302),
    (561, 'Automated data extraction in big data analytics',  303),
    (565, 'A new hashing structure for large key-value data sets',  303),
    (577, 'Yet another language for big data analysis', 303),
    (577, 'Yet another language for big data analysis', 304);

INSERT INTO FundingAgencies VALUES 
    ('NSERC', '350 Albert Street, Ottawa, ON K1A 1H5', '613-995-4273'),
    ('OMAFRA', '1 Stone Road West, Guelph, ON N1G 4Y2', '519-826-3100');

INSERT INTO Grants VALUES
    (6733, 10000, '2019-2020', 'NSERC', 10001),
    (2355, 80000, '2019-2022', 'OMAFRA', 10002),
    (3744, 15000, '2018-2019', 'NSERC', 10001),
    (6347, 20000, '2017-2020', 'NSERC', 10003),
    (9877, 35000, '2019-2022', 'OMAFRA', 10002);


INSERT INTO Researchers VALUES
    (102323, 'James', 'X52838', 'REY3203', 'james@uoguelph.ca', 10002, 554, 'false'),
    (103946, 'Jones', 'X53371', 'REY3101', 'jones@uoguelph.ca', 10001, 552, 'false'),
    (103946, 'Jones', 'X53371', 'REY3101', 'jones@uoguelph.ca', 10001, 561, 'false'),
    (117673, 'White', 'X56652', 'REY2331', 'white@uoguelph.ca', 10001, 552, 'true'),
    (117673, 'White', 'X56652', 'REY2331', 'white@uoguelph.ca', 10001, 561, 'true'),
    (117673, 'White', 'X56652', 'REY2331', 'white@uoguelph.ca', 10003, 565, 'true'),
    (117673, 'White', 'X56652', 'REY2331', 'white@uoguelph.ca', 10003, 577, 'true'),
    (134262, 'Brown', 'X53342', 'REY3228', 'brown@uoguelph.ca', 10002, 554, 'true'),
    (106469, 'Green', 'X54693', 'REY2044', 'green@uoguelph.ca', 10003, 577, 'false'), 
    (225320, 'Murry', 'X52289', 'REY2388', 'murry@uoguelph.ca', 10002, 554, 'false'), 
    (324201, 'Smith', 'X54334', 'REY3781', 'smith@uoguelph.ca', 10003, 577, 'false'),
    (849171, 'Field', 'X56381', 'REY3544', 'field@uoguelph.ca', 10001, 552, 'false'),
    (849171, 'Field', 'X56381', 'REY3544', 'field@uoguelph.ca', 10001, 565, 'false');
