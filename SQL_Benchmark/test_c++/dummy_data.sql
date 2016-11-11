select * from city group by CountryCode;
select * from country group by Code;
select * from countrylanguage group by CountryCode;
select a.CountryCode, b.Code from city as a join country as b on a.CountryCode = b.Code;
select a.CountryCode, b.CountryCode from city as a join countrylanguage as b on a.CountryCode = b.CountryCode;
select a.Code, b.CountryCode from country as a join city as b on a.Code = b.CountryCode;
select a.CountryCode, b.Code from countrylanguage as a join country as b on a.CountryCode = b.Code;
